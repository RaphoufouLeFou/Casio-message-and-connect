#include "BluetoothSerial.h"
#include <map>

int count = 0;
String buffer;
BluetoothSerial SerialBT;

#define RXD2 16
#define TXD2 17


bool connected;
String name = "Not linked";
String RecevedMsg;

int ledState = LOW;             // ledState used to set the LED
bool IsExam = false;
unsigned long previousMillis = 0; 
unsigned long intervalOn = 300UL;
unsigned long intervalOff = 1750UL;

unsigned long previousScanMillis = 0; 
unsigned long ScanTime = 5000UL;

unsigned long MaxOnTime = 1200000UL;
unsigned long MaxOnTimeInExam = 21600000UL;

void setup() {

  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(19, OUTPUT);
  //SerialBT.begin("RRRRR");
  
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
    }else if(buffer == "&Rst&"){
      delay(500);
      
      esp_restart();
    }else if(buffer == "&GetName&"){
      Serial2.print(name);
    }else if(buffer == "&ExmOn&"){
      IsExam = true;
    }else if(buffer == "&ExmOff&"){
      IsExam = false;
    }else if(buffer == "&Sleep&"){
      delay(500);
      esp_deep_sleep_start();
    }else if(buffer == "&GetMsg&"){
      Serial2.print(RecevedMsg);
      Serial.print(RecevedMsg);
      RecevedMsg = "&NoNew";
    }else if(buffer != "&SleepEXIT&"){
      SerialBT.print(buffer);
    }    
  }
  
  if (SerialBT.available()) {
    RecevedMsg = SerialBT.readString();
    Serial.print(RecevedMsg);
  }
  
  if ((millis() - previousMillis > intervalOff ) && (IsExam) && ledState == LOW) {
 
    previousMillis += intervalOff;  
    ledState = HIGH; 
    digitalWrite(19, !ledState);
  
  }else if ((millis() - previousMillis > intervalOn ) && (IsExam) && ledState == HIGH){
    
    previousMillis += intervalOn;  
    ledState = LOW; 
    digitalWrite(19, !ledState);
    
  }else if (!IsExam){
    digitalWrite(19, HIGH);
  }
  if ((millis() > MaxOnTime) && (!IsExam)) {
    esp_deep_sleep_start();
  }
  if ((millis() > MaxOnTimeInExam) && (IsExam)) {
    esp_deep_sleep_start();
  }
}

void CreateBT(){
  Serial2.write("ok");
  Serial.write("ok");
  while(!Serial2.available());
  name = Serial2.readString();
  SerialBT.begin(name);
  Serial2.write("ok");
  Serial.write("ok");
}

void ConnectBT(){
  Serial2.write("ok");
  Serial.write("ok");
  while(!Serial2.available());
  name = Serial2.readString();
  SerialBT.begin("client", true); 
  connected = SerialBT.connect(name);
  
  if(connected) {
    Serial2.print("&Connect");
    Serial.write("&Connect");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial2.print("&Fail"); 
      Serial.write("&Fail");
    }
  }
  SerialBT.connect();
}
