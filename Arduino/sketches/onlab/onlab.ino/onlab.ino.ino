/* ******** Includes ******** */
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <BH1750.h>
#include <DHT.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <SoftwareSerial.h>

/* ******** Constants ******** */
// #define DEBUG
#define AT_MODE
#define DHTPIN 2
#define DHTTYPE DHT11
#define LED_PIN 7
#define ON 1
#define OFF 0
#define TX 9
#define RX 8
#define KEY 10

/* ******** Global variables ******** */
int flag_meres = 0;
uint32_t counter = 0;
uint32_t meres_sorszam = 1;
int seconds = 0;
int receivedByte;

/* ******** Global class objects ******** */
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
BH1750 lightMeter;
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial bluetooth(RX, TX);

/* ******** TIMER1 interrupt handler function ******** */
// Interrupts every second
ISR(TIMER1_COMPA_vect)
{
  seconds++;
}

/* ******** TIMER1 init function ******** */
void setTimer1(void)
{
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
  
  // set compare match register to desired timer count:
  OCR1A = 15624; // 15624
  
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  
  // enable global interrupts:
  sei();
}

/* ******** setup function ******** */
void setup(void) 
{
  setTimer1();
  Serial.begin(9600, SERIAL_8N1);
  Serial.println("Onallo laboratorium");
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  pinMode(KEY, OUTPUT);
  #ifdef AT_MODE
    digitalWrite(KEY, HIGH);
  #else
    digitalWrite(KEY, LOW);
  #endif

  #ifdef DEBUG
    bluetooth.begin(38400);
  #else
    bluetooth.begin(9600);
  #endif
  
  Wire.begin();
  lightMeter.begin();
  if(!bmp.begin())
  {
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  dht.begin();
}

void dht11_read(void)
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    // Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(h);
  Serial.print('#');
  Serial.print(t);
  Serial.print('#');
  Serial.print(hic);
  Serial.println('%');
  
  bluetooth.print(h);
  bluetooth.print('#');
  bluetooth.print(t);
  bluetooth.print('#');
  bluetooth.print(hic);
  bluetooth.print('%');
/*
  Serial.print(h); // százalékos érték!!!
  Serial.print(" ");
  Serial.print(t);
  Serial.print(" ");
  Serial.print(hic);
  Serial.println(" ");
  */
}

void loop(void) 
{
  flag_meres = 1;
  delay(10*1000);
  /*
  if(bluetooth.available() > 0)
  {
    receivedByte = bluetooth.read();
    Serial.write(receivedByte); Serial.print(" ");
    Serial.println(receivedByte, DEC);
    // Serial.flush();
  }
  if(Serial.available() > 0)
  {
    bluetooth.write(Serial.read());
    digitalWrite(LED_PIN, HIGH);
    delay(10);
    digitalWrite(LED_PIN, LOW);
  }
  if(receivedByte == '1')
  {
    digitalWrite(LED_PIN, HIGH);
    Serial.flush();
  }
  else if(receivedByte == '0')
  {
    digitalWrite(LED_PIN, LOW);
    Serial.flush();
  }
  else if(receivedByte == 47)
  {
    flag_meres = 1;
    receivedByte = 0;
  }
  */
  if(flag_meres)
  {
    float temp;
    digitalWrite(LED_PIN, HIGH);
    //Serial.print(meres_sorszam);
    //Serial.print(" ");
    sensors_event_t event;
    bmp.getEvent(&event);
    uint16_t lux = lightMeter.readLightLevel();
    //Serial.print(lux);
    //Serial.print(" ");
    if (event.pressure)
    {
      //Serial.print(event.pressure); Serial.print(" ");
      bmp.getTemperature(&temp);
      //Serial.print(temp); Serial.print(" ");
      //bluetooth.print(temp);
    }
    else
    {
      //Serial.println("BMP180 sikertelen meres");
    }
    //dht11_read();
    Serial.print('#');
    Serial.print(meres_sorszam);
    Serial.print('#');
    Serial.print(temp);
    Serial.print('#');
    Serial.print(lux);
    Serial.print('#');
    bluetooth.print('#');
    bluetooth.print(meres_sorszam);
    bluetooth.print('#');
    bluetooth.print(temp);
    bluetooth.print('#');
    bluetooth.print(lux);
    bluetooth.print('#');
    dht11_read();
    meres_sorszam++;
    seconds = 0;
    digitalWrite(LED_PIN, LOW);
    flag_meres = 0;
  }
  /*
  if(seconds >= 10)
  {
    Serial.print(meres_sorszam);
    Serial.print(" ");
    sensors_event_t event;
    bmp.getEvent(&event);
    uint16_t lux = lightMeter.readLightLevel();
    Serial.print(lux);
    Serial.print(" ");
    if (event.pressure)
    {
      Serial.print(event.pressure); Serial.print(" ");
      float temp;
      bmp.getTemperature(&temp);
      Serial.print(temp); Serial.print(" ");
    }
    else
    {
      Serial.println("BMP180 sikertelen meres");
    }
    dht11_read();
    meres_sorszam++;
    seconds = 0;
  }
  */
  
}
