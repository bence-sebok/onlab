#include <SoftwareSerial.h>

// GPIO-k
#define TX 9
#define RX 8
#define KEY 10

SoftwareSerial bluetooth(RX, TX);

void setup()
{
  // AT módba lépés (tápfeszültség ráadása előtt!)
  pinMode(KEY, OUTPUT);
  digitalWrite(KEY, HIGH);
  
  Serial.begin(9600);
  bluetooth.begin(38400);

  Serial.println("Enter AT command here: ");
}

void loop()
{
  // Visszacsatolt soros portok
  if (bluetooth.available() > 0)
  {
    Serial.write(bluetooth.read());
  }
  if (Serial.available() > 0)
  {
    bluetooth.write(Serial.read());
  }
}
