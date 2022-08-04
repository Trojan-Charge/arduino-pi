//This program controls the central Arduino Nano and how it communicates with
//the peripheral Nanos, the SIM module, and ThingSpeak
//Author: Rachel Anderson
//Date: 8/4/22

#include <SoftwareSerial.h>

SoftwareSerial simPort(25, 26); // RX, TX
SoftwareSerial port1(5, 6);
SoftwareSerial port2(7, 8);
SoftwareSerial port3(9, 10);
SoftwareSerial port4(11, 12);
SoftwareSerial port5(13, 14);
SoftwareSerial port6(15, 16);

int slotIsEmpty[] = {0, 0, 0, 0, 0, 0}; //all charger slots start as full
SoftwareSerial portArray[] = {port1, port2, port3, port4, port5, port6};

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Start each software serial port
  simPort.begin(9600);
  for(int i = 0; i < 6; i++){
    portArray[i].begin(9600);
  }
}

void loop() {
//code to borrow charger
  //check api to see if someone wants to borrow charger
  int wantsToBorrow = ReadDataFromAPI();
  if(wantsToBorrow){
    //tell peripheral nano to eject charger and return uid
    int indexToEject = -1;
    for(int i = 0; i < 6; i++){ //find the first full slot
      if(!slotIsEmpty){
        indexToEject = i;
        break;
      }
    }
    if(indexToEject != -1){ //if there is a full slot, eject that charger
      portArray[indexToEject].println("Eject");
      portArray[indexToEject].listen();
      String uid = "";
      while(portArray[indexToEject].available()){
        uid += portArray[indexToEject].read();
      }
      WriteDataToAPI(uid);  //upload uid to different channel of api
      slotIsEmpty[indexToEject] = 1;  //mark slot as empty in array
    } else {
      Serial.println("No charger to eject, all slots are empty.");
    }
  }

  
//code for charger return
  for(int i = 0; i < 6; i++){
    if(slotIsEmpty[i]){
      portArray[i].println("Ready for incoming message.");
      portArray[i].listen();
      
      //if a charger has been returned, print the UID to the serial monitor
      if(portArray[i].available()){
        Serial.print("A charger has been returned to port " + String(i+1) + " with this UID: ");
        while(portArray[i].available()){
          Serial.write(portArray[i].read());
        }
        slotIsEmpty[i] = 0; //mark slot as full in array
      }
    }
  }
}

void ShowSimData()
{
  while(simPort.available()){
    Serial.write(simPort.read());
  }
}
int ReadDataFromAPI()
{
  simPort.println("AT+CSTT=\"m2mglobal\""); //start task and setting the APN
  delay(1000);
  ShowSimData();

  simPort.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\""); //start up the connection
  delay(6000);
  ShowSimData();
 
  simPort.println("AT+CIPSEND");  //begin send data to remote server
  delay(4000);
  ShowSimData();

  String str="GET https://api.thingspeak.com/channels/1808792/fields/1.json?api_key=RPKW7O2GQTV0CRG2&results=2";  //read data from thingspeak
  Serial.println(str);
  simPort.println(str);  //begin send data to remote server
  delay(3000);
  ShowSimData();
 
  simPort.println((char)26);  //sending
  delay(5000);  //waiting for reply, important! the time is base on the condition of internet 
  simPort.println();
  ShowSimData();
 
  simPort.println("AT+CIPSHUT");  //close the connection
  delay(100);
  ShowSimData();

  return 1; //replace with thingspeak data
}
void WriteDataToAPI(String data)
{
  simPort.println("AT+CSTT=\"m2mglobal\""); //start task and setting the APN
  delay(1000);
  ShowSimData();

  simPort.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\""); //start up the connection
  delay(6000);
  ShowSimData();
 
  simPort.println("AT+CIPSEND");  //begin send data to remote server
  delay(4000);
  ShowSimData();

  String str="GET https://api.thingspeak.com/update?api_key=6045WJVT56ZL6SKA&field2=" + data;  //write data to thingspeak
  Serial.println(str);
  simPort.println(str);  //begin send data to remote server
  delay(3000);
  ShowSimData();
 
  simPort.println((char)26);  //sending
  delay(5000);  //waiting for reply, important! the time is base on the condition of internet 
  simPort.println();
  ShowSimData();
 
  simPort.println("AT+CIPSHUT");  //close the connection
  delay(100);
  ShowSimData();
}
