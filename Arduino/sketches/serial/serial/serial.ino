#include <SoftwareSerial.h>

#define RX 10
#define TX 11
#define KEY 7

SoftwareSerial bluetooth_HC05(RX, TX);

void setup()
{
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  pinMode(KEY, OUTPUT);
  digitalWrite(KEY, HIGH);
  
  Serial.begin(9600);
  Serial.println("AT parancs:");
  bluetooth_HC05.begin(38400);
}

void loop()
{
  if(bluetooth_HC05.available() > 0)
  {
    Serial.write(bluetooth_HC05.read());
  }
  if(Serial.available() > 0)
  {
    bluetooth_HC05.write(Serial.read());
  }
}
