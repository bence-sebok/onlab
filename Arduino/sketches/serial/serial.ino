#include <SoftwareSerial.h>

#define LED_PIN 7
#define ON 1
#define OFF 0
#define TX 9
#define RX 8

int state = OFF;
int receivedByte;

SoftwareSerial bluetooth(RX, TX);

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  
  bluetooth.begin(38400);
  Serial.begin(9600, SERIAL_8N1);
}

void loop()
{
  if (bluetooth.available() > 0)
  {
    receivedByte = bluetooth.read();
    Serial.println(receivedByte, DEC);
    Serial.flush();
    // Serial.println(bluetooth.available(), DEC);
  }
  if(receivedByte == 120)
  {
    digitalWrite(LED_PIN, HIGH);
    Serial.flush();
  }
  else if(receivedByte == 0)
  {
    digitalWrite(LED_PIN, LOW);
    Serial.flush();
  }
  else if(receivedByte == 248)
  {
    Serial.println("meres");
    Serial.flush();
  }
  if(Serial.available() > 0)
  {
    digitalWrite(LED_PIN, HIGH);
    bluetooth.write(Serial.read());
    delay(10);
    digitalWrite(LED_PIN, LOW);
  }
}
