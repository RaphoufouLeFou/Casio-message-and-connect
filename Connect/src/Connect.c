#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/system.h>
#include <fxcg/serial.h>
#include <CONVERT_syscalls.h>
#include <stdio.h>
#include <string.h>

int window = 0;             //current window displayed
int IsExam = 0;             //Is led blinking
int n;                      //selected BT device
int i = 0;                  //numer of devices
int IsTyping = 0;           //Is in typing mode
char *DetailedList[5][1024];//list of devices
int MsgLength = 6;          //Cusror place for text input
char MsgBuffer[1024];       //Msg enter in text input 

void GetBTLsit(unsigned char *List){    //Get BT device list

    Serial_Write("&GetBT&", 10);    //send signal to the ESP32 to get the avalable device list 
    char *ReservedBuffer[2048];
    int recSize;
    int CountExcess = 0;
    while(Serial_Read(ReservedBuffer, 1024, recSize) == 1){     //waiting for a response
        CountExcess++;
        if(CountExcess >= 1000000){                             //TimeOut
            break;
        }
    }
    Serial_Read(ReservedBuffer, 1024, recSize);                 //Read the list receve from the ESP32
    if(CountExcess < 1000000){
        List = ReservedBuffer;
    }else{
        List = "TimeOut";                                       //TimeOut
    }

    return;
}

void CreateBT(char Name){           //create BT acces point

    Serial_Write("&NewBT&", 8);     //send signal to the ESP32 to create BT AP
    char *ReservedBuffer[2048];
    int recSize;
    int CountExcess = 0;
    while(Serial_Read(ReservedBuffer, 1024, recSize) == 1){     //waiting for a response
        CountExcess++;
        if(CountExcess >= 10000){                               //TimeOut
            break;
        }
    }
    Serial_Write(Name, 21);         //Send the name of BT AP to the ESP32
    return 1;
}

int ConnectBT(char Name){           //Connect ESP32 to a BT device

    Serial_Write("&ConnectBT&", 14);    //send signal to the ESP32 to connect
    char *ReservedBuffer[2048];
    int recSize;
    int CountExcess = 0;
    while(Serial_Read(ReservedBuffer, 1024, recSize) == 1){     //waiting for a response
        CountExcess++;
        if(CountExcess >= 100000){                              //TimeOut
            break;
        }
    }

    Serial_Write(Name, 256);              //send the name of the device to connect to the ESP32
    char *ReservedBufferBis[2048];
    int recSizeBis;
    CountExcess = 0;
    while(Serial_Read(ReservedBufferBis, 1024, recSizeBis) == 1){   //waiting for a response
        CountExcess++;
        if(CountExcess >= 10000000){                                //TimeOut
            break;
        }
    }

    Serial_Read(ReservedBufferBis, 1024, recSizeBis);   //Read the response from the ESP32
    if(ReservedBufferBis == "&Connect"){            //connected
        return 1;
    }else if(ReservedBufferBis == "&Fail"){         //TimeOut
        return 0;
    }

    return;
}

void OpenBTList(){                          //Create the window to connect to a BT device
    IsTyping = 0;
    n = 0;
    Bdisp_AllClr_VRAM();                    //Clear the Vram
    DefineStatusAreaFlags(3, SAF_BATTERY, 0, 0);
    locate_OS(1,1);                                     //Put text to screen
    Print_OS("F1:create BT", 0, 0);
    locate_OS(1,2);
    Print_OS("BT avalables :", 0, 0);

    int iresult;                                //Put the Fkey bitmap on screen
    GetFKeyPtr(0x0524, &iresult);
    FKey_Display(0, iresult);       //F1

    int iresult5;
    GetFKeyPtr(0x02B1, &iresult5);
    FKey_Display(5, iresult5);      //F6


    char *List[2048];                           //Get the list of devices
    GetBTLsit(List);
    char delim[] = "#";                         //set the delimiter of Device in the list

    char *ptr = strtok(List, delim);            
    i = 0;

    while(ptr != NULL)                          //separates the devices in the list
	{
		DetailedList[i];
		ptr = strtok(NULL, delim);
        i++;
	}
    for (int j = 0; j < i; j++){                //Display the devices on screen
        
        locate_OS(1,(j+3));
        unsigned char VaTeFaireFoutre[1024];
        itoa((j+1), VaTeFaireFoutre);
        strcat(VaTeFaireFoutre, ":");
        strcat(VaTeFaireFoutre, DetailedList[j]);
        Print_OS(VaTeFaireFoutre, 0, 0);
    }
    return;
}

void CreateBTMenu(){                                //Create the window to create a BT AP
    Bdisp_AllClr_VRAM();                            //clear VRAM
    locate_OS(1,1);
    Print_OS("Name:", 0, 0);                        
    Cursor_SetFlashOn("|");
    IsTyping = 1;

    int iresult5;
    GetFKeyPtr(0x02B1, &iresult5);
    FKey_Display(5, iresult5);      //F6

}
void MainMenu(){

    Bdisp_AllClr_VRAM();
    DefineStatusAreaFlags(3, SAF_BATTERY, 0, 0);
    //Put Fkey bmps on screen
    int iresult;
    GetFKeyPtr(0x0524, &iresult);
    FKey_Display(0, iresult);       //F1
    
    int iresult1;
    GetFKeyPtr(0x04F7, &iresult1);
    FKey_Display(1, iresult1);      //F2
        
    int iresult3;
    GetFKeyPtr(0x0165, &iresult3);
    FKey_Display(3, iresult3);      //F4
    


//Base text print to screen

    locate_OS(1,1);
    Print_OS("F1:open serial ESP32", 0, 0);
    locate_OS(1,2);
    Print_OS("F2:exam mode", 0, 0);
    locate_OS(1,3);
    Print_OS("F3:test mode", 0, 0);
    locate_OS(1,4);
    Print_OS("F4:open calc list", 0, 0);
    locate_OS(1,6);
    Print_OS("ESP32 disconnected", 0, 0);
    locate_OS(1,7);
    Print_OS("Exam mode off    ", 0, 0);

    return;
}

void main(void) {

    MainMenu();

    int key;

    while(1){
        GetKey(&key);

        if(window == 0){
            if (key == KEY_CTRL_EXIT) {     //Stop the program if the key is EXIT
                break;
            }

            if (key == KEY_CTRL_F1) {       //Start serial comunication with the ESP32 at 9600 bps if F1 is pressed
                if (Serial_IsOpen() != 1) {
                    unsigned char mode[6] = {0, 5, 0, 0, 0, 0};    // 9600 bps 8n1
                    Serial_Open(mode);
                }else {
                    Serial_Close(1);
                }
            }

            if(Serial_IsOpen() == 1){       //check if the serial is open
                int iresult;
                GetFKeyPtr(0x0522, &iresult);
                FKey_Display(0, iresult);       //F1
                locate_OS(1,6);
                Print_OS("ESP32 connected    ", 0, 0);
            }else{                          //This execute if the serial is closed
                int iresult;
                GetFKeyPtr(0x0524, &iresult);
                FKey_Display(0, iresult);       //F1
                locate_OS(1,6);
                Print_OS("ESP32 disconnected", 0, 0);
            }

            if(key == KEY_CTRL_F2){         //Send signal to ESP32 to blink the exam led if F2 is pressed
                if(IsExam == 0){
                    Serial_Write("&ExmOn&", 8);
                    locate_OS(1,7);
                    Print_OS("Exam mode on     ", 0, 0);
                    IsExam = 1;
                }
                else if(IsExam == 1){       //Send signal to ESP32 to stop blinking the exam led
                    Serial_Write("&ExmOff&", 9);
                    locate_OS(1,7);
                    Print_OS("Exam mode off    ", 0, 0);
                    IsExam = 0;
                }

            }

            if(key == KEY_CTRL_F3){         //Open test mode if F3 is pressed
                TestMode(1);
            }

            if(key == KEY_CTRL_F4){         //Open BT list if F4 is pressed
                
                OpenBTList();
                window = 1;
            }
        }

        if(window == 1){

            if((key >= 49 && key <= 48+i) && IsTyping == 0){
                n = key - 48;/*
                IsTyping = 1;
                locate_OS(1,7);
                Print_OS("Pwd:               ", 0, 0);
                Cursor_SetFlashOn("|");*/
            }/*
            else if(((key >= 65 && key <= 90) || (key >= 48 && key <= 57)) && IsTyping == 1){

                locate_OS(MsgLength,7);
                unsigned char buffer[12];
                sprintf(buffer, "%c", key);
                Print_OS(buffer, 0, 0);
                strncat(MsgBuffer, &buffer, 1);
                MsgLength++;
            }
            else if((key == KEY_CTRL_DEL) && IsTyping == 1){
                if(MsgLength > 5){
                    MsgLength--;
                    locate_OS(MsgLength,7);
                    Print_OS(" ", 0, 0);
                    strcpy(&MsgBuffer[strlen(MsgBuffer)-1], &MsgBuffer[strlen(MsgBuffer)]);
                    locate_OS(MsgLength,7);
                }
            }*/
            if((key == KEY_CTRL_EXE) && IsTyping == 1){/*
                IsTyping = 0;
                if(strlen(MsgBuffer) == 0){
                    AUX_DisplayErrorMessage(20);
                }else if(strlen(MsgBuffer) < 8){
                    AUX_DisplayErrorMessage(79);
                }else{*/
                    //Cursor_SetFlashOff();
                    int IsConnected = 0;
                    IsConnected = ConnectBT(DetailedList[n]);
                    locate_OS(1,7);
                    if(IsConnected == 0){
                        Print_OS("error                  ", 0, 0);
                    }else{
                        Print_OS("Connected !            ", 0, 0);
                    }
                    MsgLength = 5;
                //}
            }
            if(key == KEY_CTRL_F1){
                window = 2;
                CreateBTMenu();
            }
            if(key == KEY_CTRL_F6){
                Bdisp_AllClr_VRAM();
                DefineStatusAreaFlags(3, SAF_BATTERY, 0, 0);
                window = 0;
                MainMenu();
            }

        }

        if(window == 2){
            if(key == KEY_CTRL_F6){
                Bdisp_AllClr_VRAM();
                DefineStatusAreaFlags(3, SAF_BATTERY, 0, 0);
                window = 1;
                OpenBTList();
            } 

            else if(((key >= 65 && key <= 90) || (key >= 48 && key <= 57)) && IsTyping == 1){

                locate_OS(MsgLength,1);
                unsigned char buffer[12];
                sprintf(buffer, "%c", key);
                Print_OS(buffer, 0, 0);
                strncat(MsgBuffer, &buffer, 1);
                MsgLength++;
            }
            else if((key == KEY_CTRL_DEL) && IsTyping == 1){
                if(MsgLength > 6){
                    MsgLength--;
                    locate_OS(MsgLength,1);
                    Print_OS(" ", 0, 0);
                    strcpy(&MsgBuffer[strlen(MsgBuffer)-1], &MsgBuffer[strlen(MsgBuffer)]);
                    locate_OS(MsgLength,1);
                }
            }
            if((key == KEY_CTRL_EXE) && IsTyping == 1){
                IsTyping = 0;
                if(strlen(MsgBuffer) == 0){
                    AUX_DisplayErrorMessage(20);
                }else if(strlen(MsgBuffer) > 20){
                    AUX_DisplayErrorMessage(16);
                }else{
                    Cursor_SetFlashOff();
                    CreateBT(MsgBuffer);
                    locate_OS(1,7);
                    Print_OS("Created !            ", 0, 0);
                    MsgLength = 6;
                }
            }
        }
    }
}


//Hex code for some bmp FKey bmp folowing by there backgrond's color :

//0x010A -> open        black
//0x010B -> close       black
//0x01A6 -> OFF         black
//0x022D -> receve      black
//0x0065 -> transmit    black
//0x01F0 -> connect     black
//0x01FE -> clear       black
//0x022C -> send        black
//0x02B1 -> back        black
//0x03E2 -> message     black
//0x04F7 -> exam        black

//0x0524 -> open        white
//0x0522 -> close       white
//0x0011 -> on          white
//0x0012 -> off         white
//0x0066 -> receve      white
//0x008D -> transmit    white
//0x0165 -> connect     white
//0x0397 -> load        white
//0x03F1 -> 3pin        white

