#include <SoftwareSerial.h>
SoftwareSerial Sim800l(3, 2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Sim800l.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Sim800l.available()){
    Serial.write(Sim800l.read());
  }

  
  while(Serial.available()){
    Sim800l.write(Serial.read());
  }
}
