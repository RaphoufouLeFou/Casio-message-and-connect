#include "BluetoothSerial.h"
#include <map>

int count = 0;
String buffer;
BluetoothSerial SerialBT;

#define RXD2 16
#define TXD2 17

bool connected;
String name;
String RecevedMsg;

int ledState = LOW;             // ledState used to set the LED
bool IsExam = false;
unsigned long previousMillis = 0; 
unsigned long interval = 1000UL;

unsigned long previousScanMillis = 0; 
unsigned long ScanTime = 5000UL;
void setup() {

  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(19, OUTPUT);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4,0); //1 = High, 0 = Low
}

void loop() {
  
  if (Serial2.available()) {
    buffer = Serial2.readString();
    Serial.print(buffer);
    if(buffer == "&NewBT&"){
      CreateBT();
    }else if(buffer == "&ConnectBT&"){
      ConnectBT();
    }else if(buffer == "&GetName&"){
      Serial2.print(name);
    }else if(buffer == "&ExmOn&"){
      IsExam = true;
    }else if(buffer == "&ExmOff&"){
      IsExam = false;
    }else if(buffer == "&Sleep&"){
      delay(1000);
      esp_deep_sleep_start();
    }else if(buffer == "&GetMsg&"){
      Serial2.print(RecevedMsg);
      RecevedMsg = "";
    }else if(buffer != "&SleepEXIT&"){
      SerialBT.print(buffer);
    }    
  }
  
  if (SerialBT.available()) {
    RecevedMsg = SerialBT.readString();
  }
  
  if ((millis() - previousMillis > interval ) && (IsExam)) {
    
    previousMillis += interval;  
    
    if (ledState == LOW){
      ledState = HIGH;
    }else{
      ledState = LOW;
    }
    
    digitalWrite(19, ledState);
  
  }else if (!IsExam){
    digitalWrite(19, LOW);
  }
}

void CreateBT(){
  Serial2.write("ok");
  Serial.write("ok");
  while(!Serial2.available());
  name = Serial2.readString();
  SerialBT.begin(name);
}

void ConnectBT(){
  Serial2.write("ok");
  Serial.write("ok");
  while(!Serial2.available());
  name = Serial2.readString();
  SerialBT.begin("", true); 
  connected = SerialBT.connect(name);
  
  if(connected) {
    Serial2.print("&Connect");
    Serial.write("ok");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial2.print("&Fail"); 
      Serial.write("ok");
    }
  }
  SerialBT.connect();
}
