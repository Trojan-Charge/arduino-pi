#include <ESP8266WiFi.h>

const char* ssid = "simon";
const char* password = "92381299";

int ledPin = 1;

void setup() {
  // put your setup code here, to run once:

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  digitalWrite(ledPin, HIGH);

  Serial.println("WiFi Connected Success!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
