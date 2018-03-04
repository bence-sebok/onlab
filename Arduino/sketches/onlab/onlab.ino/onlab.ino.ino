#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <BH1750.h>

int flag_meres = 0;
uint32_t counter = 0;
   
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
BH1750 lightMeter;

// TIMER0 interrupt 2 kHz
ISR(TIMER0_COMPA_vect)
{
  //Serial.print(counter); 
  //Serial.println("");
  counter++;
}

void setup(void) 
{
  cli();
  //set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS02) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  sei();
  
  Serial.begin(9600);
  Serial.println("Onallo laboratorium");
  
  Wire.begin();
  lightMeter.begin();
  if(!bmp.begin())
  {
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
}

void loop(void) 
{
  sensors_event_t event;
  bmp.getEvent(&event);
  if(counter >= 7500)
  {
    if (event.pressure)
    {
      Serial.print(event.pressure); Serial.print(" ");
      float temp;
      bmp.getTemperature(&temp);
      Serial.print(temp); Serial.print(" ");
    }
    else
    {
      Serial.println("BMP180 sikertele meres");
    }
    delay(50);
  
    uint16_t lux = lightMeter.readLightLevel();
    Serial.print(lux);
    Serial.println("");
    delay(50);
    counter = 0;
  }
}
