#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <BH1750.h>
#include <DHT.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define DHTPIN 2
#define DHTTYPE DHT11

int flag_meres = 0;
uint32_t counter = 0;
uint32_t meres_sorszam = 1;
int seconds = 0;
   
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
BH1750 lightMeter;
DHT dht(DHTPIN, DHTTYPE);

// #define DEBUG

ISR(TIMER1_COMPA_vect)
{
  seconds++;
}

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

void setup(void) 
{
  setTimer1();
  Serial.begin(9600);
  Serial.println("Onallo laboratorium");
  
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

  Serial.print(h); // százalékos érték!!!
  Serial.print(" ");
  Serial.print(t);
  Serial.print(" ");
  Serial.print(hic);
  Serial.println(" ");
}

void loop(void) 
{
  if(seconds >= 60)
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
}
