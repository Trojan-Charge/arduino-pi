void setup() {
  // put your setup code here, to run once:
  pinMode(6, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<255;i++){
    digitalWrite(6, HIGH); // sets the digital pin 13 on
    Serial.println(i);
    delay(200);            // waits for a second
    digitalWrite(6, LOW);  // sets the digital pin 13 off
    delay(200);
  }
              // waits for a second
}
