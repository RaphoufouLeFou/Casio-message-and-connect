/*****************************************************************/
/*                                                               */
/*   CASIO fx-9860G SDK Library                                  */
/*                                                               */
/*   File name : Test.c                                          */
/*                                                               */
/*   Copyright (c) 2006 CASIO COMPUTER CO., LTD.                 */
/*                                                               */
/*****************************************************************/
#include "D:\CASIO\casio\OS\FX\include\fxlib.h"
#include <stdio.h>
#include <string.h>
#include "D:\DOCUMENTS\Raphael\casio\code\SDK\projects\MessageFX\syscall.h"

//****************************************************************************
//  AddIn_main (Sample program main function)
//
//  param   :   isAppli   : 1 = This application is launched by MAIN MENU.
//                        : 0 = This application is launched by a strip in eACT application.
//
//              OptionNum : Strip number (0~3)
//                         (This parameter is only used when isAppli parameter is 0.)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************



char DeviceName[1024];
char Msg[1024] = "No New messages";
int isLowercase = 0;

void main() {
    int key;
    int IsExam = 0;
    int MsgLength = 3;
    char MsgBuffer[256];
    char ReservedBuffer[1024];
    short* recSize;
    int iresult;
    char msg[] = "Bt name :";
    char res[2062];
    int Nber = 1;
    char *Name = &DeviceName;
    short *recSize2;
    int CountExcess = 0;

    Serial_ClearTransmitBuffer();
    Serial_WriteBytes("&GetName&", 10);     //send signal to the ESP32 to create BT AP
    while(Serial_ReadBytes(Name, 1024, recSize2) == 1){     //waiting for a response
        CountExcess++;
        if(CountExcess >= 5){                               //TimeOut
            break;
        }
        Sleep(1000);
    }
    if ((strcmp(Name, "") == 0) || (strcmp(Name, " ") == 0) || (CountExcess >= 5)){
        strcpy(Name, " ERR");
    }
    
    
    locate(1,1);
    sprintf(res, "%s%s", msg, DeviceName);
    Print(res);
    locate(1,4);
    Print("->");
    //Cursor_SetFlashOn("|");


    while (1) {
        strcpy(ReservedBuffer, "");

        if(Serial_ReadBytes(ReservedBuffer, 1024, recSize) == 0){
            
            locate(1,7);
            Print("Msg receve !   ");
            locate(1,6);
            Print(">>");
            Print(ReservedBuffer);
            locate(MsgLength,4);
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
            locate(1,5);
            Print(buff);
        }*/
        if (key == KEY_CTRL_F6) {
            
            char msgg[] = ">>";
            char buffer[2048];
            char *Name;
            short * recSize;
            int CountExcess = 0;

            memset(Msg, '\0', strlen(Msg));
            Name = &Msg;
            Serial_WriteBytes("&GetMsg&", 9);     //send signal to the ESP32 to get new messages

            while(Serial_ReadBytes(Name, 1024, recSize) == 1){     //waiting for a response
                CountExcess++;
                if(CountExcess >= 5){                               //TimeOut
                    break;
                }
                Sleep(1000);
            }
            Serial_ReadBytes(Name, 1024, recSize);
            Sleep(10);
            
            sprintf(buffer, "%s%s", msgg, Msg);
            if(strcmp(Msg, "&NoNew") == 0){
                locate(1,7);
                Print("No new messages              ");
                locate(MsgLength,4);
            }
            else {
                locate(1, 6);
                Print("                             ");
                locate(1, 6);
                Print(buffer);
                locate(MsgLength,4);
            }

            Sleep(15);
        }

        if (key == KEY_CTRL_F2) {

            int iresult3;

            if(isLowercase == 1){
                isLowercase = 0;
            }else if(isLowercase == 0){
                isLowercase = 1;
            }
        }

        if((key >= 65 && key <= 90) || (key >= 48 && key <= 57) || key == 32){
            
            unsigned char buffer[12];

            locate(MsgLength,4);        
            if(isLowercase == 0){
                sprintf(buffer, "%c", key);         //If the input is upper case, get the normal input
            }else if(key >= 65 && key <= 90){
                sprintf(buffer, "%c", (key+32));    //if the input is a number and upper case, get the input + 32 to be in lower case
            }else{
                sprintf(buffer, "%c", key);
            }
            Print(buffer);                 //Print the result
            strncat(MsgBuffer, &buffer, 1);
            MsgLength++;

        }
        else if(key == KEY_CTRL_DEL){
            if(MsgLength > 3){
                MsgLength--;
                locate(MsgLength,4);
                Print(" ");
                strcpy(&MsgBuffer[strlen(MsgBuffer)-1], &MsgBuffer[strlen(MsgBuffer)]);
                locate(MsgLength,4);
            }
        }
        if(key == KEY_CTRL_EXE || key == KEY_CTRL_F1){
            if(strlen(MsgBuffer) == 0){
                //AUX_DisplayErrorMessage(20);
                ;
            }else{

                int debug;
                Serial_ClearTransmitBuffer();
                debug = Serial_WriteBytes(MsgBuffer, strlen(MsgBuffer));
                Sleep(10);
                memset(MsgBuffer, '\0', strlen(MsgBuffer));
                locate(3,4);
                Print("                                      " ); 
                MsgLength = 3;
                locate(1,7);
                Print("Msg sent !             " ); 
                locate(MsgLength,4);
            }
        }
    }
    return;
}


int AddIn_main(int isAppli, unsigned short OptionNum)
{
    main();
    
}

//****************************************************************************
//**************                                              ****************
//**************                 Notice!                      ****************
//**************                                              ****************
//**************  Please do not change the following source.  ****************
//**************                                              ****************
//****************************************************************************


#pragma section _BR_Size
unsigned long BR_Size;
#pragma section


#pragma section _TOP

//****************************************************************************
//  InitializeSystem
//
//  param   :   isAppli   : 1 = Application / 0 = eActivity
//              OptionNum : Option Number (only eActivity)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section
