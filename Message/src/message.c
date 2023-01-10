#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/system.h>
#include <fxcg/serial.h>
#include <CONVERT_syscalls.h>
#include <stdio.h>
#include <string.h>

char DeviceName[2048];

void GetDeviceName(){
    char *Name;
    Name = &DeviceName;
    Serial_Write("&GetName&", 10);     //send signal to the ESP32 to create BT AP
    int recSize;
    int CountExcess = 0;
    while(Serial_Read(Name, 1024, recSize) == 1){     //waiting for a response
        CountExcess++;
        if(CountExcess >= 10000000){                               //TimeOut
            break;
        }
    }
    if ((strcmp(Name, "") == 0) || (strcmp(Name, " ") == 0) || (CountExcess >= 10000000)){
        strcpy(Name, " ERR");
    }
    return;
}

char GetNewMessage(void){

    const char* Msg[2048];
    Serial_Write("&GetMsg&", 9);     //send signal to the ESP32 to get new messages
    int recSize;
    int CountExcess = 0;
    while(Serial_Read(Msg, 1024, recSize) == 1){     //waiting for a response
        CountExcess++;
        if(CountExcess >= 10000000){                               //TimeOut
            break;
        }
    }
    if (strcmp(Msg, "") == 0){
        return "";
    }else{
        return Msg;
    }
}

void main(void) {
    int key;
    int IsExam = 0;
    int MsgLength = 3;
    char MsgBuffer[256];
    char ReservedBuffer[1024];
    int recSize;
    Bdisp_AllClr_VRAM();
    Bdisp_EnableColor(1);
    int iresult;
    GetFKeyPtr(0x022D, &iresult);
    FKey_Display(5, iresult);

    GetFKeyPtr(0x022C, &iresult);
    FKey_Display(0, iresult);
    locate_OS(1,1);
    char msg[] = "Bt name :";
    GetDeviceName();
    char res[2062];
    sprintf(res, "%s%s", msg, DeviceName);
    Print_OS(res , 0, 0);
    locate_OS(1,4);
    Print_OS("->", 0, 0);
    Cursor_SetFlashOn("|");
    int Nber = 1;

    while (1) {
        strcpy(ReservedBuffer, "");
        if(Serial_Read(ReservedBuffer, 1024, recSize) == 0){
            
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
        /*if (key == KEY_CTRL_F4) {

            GetFKeyPtr(Nber, &iresult);
            FKey_Display(0, iresult);


            GetFKeyPtr(Nber+1, &iresult);
            FKey_Display(1, iresult);

            GetFKeyPtr(Nber+2, &iresult);
            FKey_Display(2, iresult);

  
            GetFKeyPtr(Nber+3, &iresult);
            FKey_Display(3, iresult);


            GetFKeyPtr(Nber+4, &iresult);
            FKey_Display(4, iresult);


            GetFKeyPtr(Nber+5, &iresult);
            FKey_Display(5, iresult);

            Nber+=6;
            char buff[12];
            itoa(Nber, buff);
            locate_OS(1,5);
            Print_OS(buff, 0, 0);
        }*/
        if (key == KEY_CTRL_F6) {
            const char* Msg = "";
            strcpy(Msg, GetNewMessage());
            if(strcmp(Msg, "") == 0){
                locate_OS(1,7);
                Print_OS("No new messages      ", 0, 0);
            }
            else {
                locate_OS(1, 6);
                Print_OS(">>", 0, 0);
                locate_OS(3, 6);
                strcat(Msg, "                ");
                Print_OS(Msg, 0, 0);
                locate_OS(3, 4);
            }
        }

        if((key >= 65 && key <= 90) || (key >= 48 && key <= 57) || key == 32){
            
            locate_OS(MsgLength,4);
            unsigned char buffer[12];
            sprintf(buffer, "%c", key);
            Print_OS(buffer, 0, 0);
            strncat(MsgBuffer, &buffer, 1);
            MsgLength++;
        }
        else if(key == KEY_CTRL_DEL){
            if(MsgLength > 3){
                MsgLength--;
                locate_OS(MsgLength,4);
                Print_OS(" ", 0, 0);
                strcpy(&MsgBuffer[strlen(MsgBuffer)-1], &MsgBuffer[strlen(MsgBuffer)]);
                locate_OS(MsgLength,4);
            }
        }
        if(key == KEY_CTRL_EXE || key == KEY_CTRL_F1){
            if(strlen(MsgBuffer) == 0){
                AUX_DisplayErrorMessage(20);
            }else{
                Serial_ClearTX();
                int debug = Serial_Write(MsgBuffer, strlen(MsgBuffer));
                locate_OS(1,7);
                char buff[12];
                itoa(debug, buff);
                strcat(buff, " Msg sent !             ");
                Print_OS(buff , 0, 0); 
                locate_OS(MsgLength,4);
            }
        }

    }

    return;
}