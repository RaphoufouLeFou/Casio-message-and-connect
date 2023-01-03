#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/system.h>
#include <fxcg/serial.h>
#include <CONVERT_syscalls.h>
#include <stdio.h>
#include <string.h>


char GetDeviceName(void){
    unsigned char *name[2048];
    Serial_Write("&GetName&", 8);     //send signal to the ESP32 to create BT AP
    int recSize;
    int CountExcess = 0;
    while(Serial_Read(name, 1024, recSize) == 1){     //waiting for a response
        CountExcess++;
        if(CountExcess >= 10000){                               //TimeOut
            break;
        }
    }
    if (name == NULL){
        return "Not connected";
    }else{
        return name;
    }
}

char GetNewMessage(void){

    unsigned char *Msg[2048];
    Serial_Write("&GetMsg&", 8);     //send signal to the ESP32 to get new messages
    int recSize;
    int CountExcess = 0;
    while(Serial_Read(Msg, 1024, recSize) == 1){     //waiting for a response
        CountExcess++;
        if(CountExcess >= 10000){                               //TimeOut
            break;
        }
    }
    if (Msg == NULL){
        return "";
    }else{
        return Msg;
    }
}

void main(void) {
    int key;
    int IsExam = 0;
    int MsgLength = 3;
    char MsgBuffer[1024];
    char ReservedBuffer[1024];
    int recSize;
    Bdisp_AllClr_VRAM();
    Bdisp_EnableColor(1);
    int iresult;
    GetFKeyPtr(0x0003, &iresult);
    FKey_Display(0, iresult);
    locate_OS(1,1);
    char msg = "Connected to:";
    strcat(msg, GetDeviceName);
    Print_OS("Connected to:", 0, 0);
    locate_OS(1,4);
    Print_OS("->", 0, 0);
    Cursor_SetFlashOn("|");

    while (1) {

        if(Serial_Read(ReservedBuffer, 1024, recSize) == 0){
            Serial_Read(ReservedBuffer, 1024, recSize);
            locate_OS(1,7);
            Print_OS("Msg receve !   ", 0, 0);
            locate_OS(1,6);
            Print_OS(">>", 0, 0);
            Print_OS(ReservedBuffer, 0, 0);
            locate_OS(MsgLength,4);
        }

        GetKey(&key);

        if (key == KEY_CTRL_EXIT) {
            //Restart();
            //TestMode(1);
            break;
        }
        if (key == KEY_CTRL_F6) {
            unsigned char *Msg[2048];
            if(Msg == ""){
                locate_OS(1,7);
                Print_OS("No new messages      ", 0, 0);
            }
            else {
                locate_OS(1, 5);
                Print_OS(">>", 0, 0);
                locate_OS(3, 5);
                strcat(Msg, GetNewMessage());
                strcat(Msg, "                ");
                Print_OS(Msg, 0, 0);
            }
        }

        if((key >= 65 && key <= 90) || (key >= 48 && key <= 57) || key == 32){
            
            locate_OS(MsgLength,3);
            unsigned char buffer[12];
            sprintf(buffer, "%c", key);
            Print_OS(buffer, 0, 0);
            strncat(MsgBuffer, &buffer, 1);
            MsgLength++;
        }
        else if(key == KEY_CTRL_DEL){
            if(MsgLength > 3){
                MsgLength--;
                locate_OS(MsgLength,3);
                Print_OS(" ", 0, 0);
                strcpy(&MsgBuffer[strlen(MsgBuffer)-1], &MsgBuffer[strlen(MsgBuffer)]);
                locate_OS(MsgLength,3);
            }
        }
        if(key == KEY_CTRL_EXE){
            if(strlen(MsgBuffer) == 0){
                AUX_DisplayErrorMessage(20);
            }else{
                Serial_Write(MsgBuffer, strlen(MsgBuffer));
                locate_OS(1,7);
                Print_OS("Msg sent !             ", 0, 0);
                locate_OS(MsgLength,3);
            }
        }

    }

    return;
}