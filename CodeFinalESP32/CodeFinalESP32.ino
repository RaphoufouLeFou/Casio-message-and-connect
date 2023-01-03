#include <BluetoothSerial.h>
#include <map>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

String BTlist[5];
int count = 0;
String buffer;
BluetoothSerial SerialBT;
bool connected;
String name;

#define BT_DISCOVER_TIME  10000

int ledState = LOW;             // ledState used to set the LED
bool IsExam = false;
unsigned long previousMillis = 0; 
unsigned long interval = 1000UL;

void setup() {

  
  Serial.begin(9600);
  pinMode(19, OUTPUT);
  //SerialBT.begin("ESP32test"); //Bluetooth device name
  //Serial.println("\nThe device started, now you can pair it with bluetooth!");
}

void loop() {
  
  if (Serial.available()) {
    buffer = Serial.readString();
    Serial.println(buffer);
    if(buffer == "&GetBT&"){
      GetBTlist();
    }else if(buffer == "&NewBT&"){
      CreateBT();
    }else if(buffer == "&ConnectBT&"){
      ConnectBT();
    }else if(buffer == "&GetName&"){
      Serial.write(name);
    }else if(buffer == "&ExmOn&"){
      ConnectBT();
    }else if(buffer == "&ExmOff&"){
      ConnectBT();
    }else{
      SerialBT.write(Serial.read());
    }
    
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  
  if ((millis() - previousMillis > interval ) && (IsExam)) 
  {
    previousMillis += interval;  

    if (ledState == LOW)
    {
      ledState = HIGH;
    }
    else
    {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(19, ledState);
    
  }else if (!IsExam){
    
    digitalWrite(19, LOW);
  }
}

void CreateBT(){
  Serial.write("ok");
  while(!Serial.available());
  name = Serial.readString();
  SerialBT.begin(name);
}

void GetBTlist(){

}
void ConnectBT(){
  Serial.write("ok");
  while(!Serial.available());
  name = Serial.readString();
  SerialBT.begin("", true); 
  connected = SerialBT.connect(name);
  
  if(connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }
  SerialBT.connect();
}
