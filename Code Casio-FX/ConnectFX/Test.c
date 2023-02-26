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
#include "D:\DOCUMENTS\Raphael\casio\code\SDK\projects\ConnectFX\syscall.h"

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

int window = 0;             //current window displayed
int IsExam = 0;             //Is led blinking
int n;                      //selected BT device
int i = 0;                  //numer of devices
int IsTyping = 0;           //Is in typing mode
int IsSleeping = 0;         //Is the ESP32 in deep sleep
int MsgLength = 6;          //Cusror place for text input
char MsgBuffer[1024];       //Msg enter in text input 
int isLowercase = 0;
char ReservedBufferBis[1024];


void OpenBTList(){                          //Create the window to connect to a BT device

    unsigned char *iresult;   
    IsTyping = 1;
    memset(MsgBuffer, 0, 1024);
    Bdisp_AllClr_VRAM();                    //Clear the Vram
    
    locate(1,1);                                     //Put text to screen
    Print((unsigned char*)"F1:create BT");
    locate(1,2);
    Print((unsigned char*)"Name of the BT device");
    locate(1,4);
    Print((unsigned char*)"->");
    Cursor_SetFlashOn(1);
    MsgLength = 3;

                             //Put the Fkey bitmap on screen
    GetFKeyIconPointer(266, &iresult);
    DisplayFKeyIcon(0, iresult);       //F1


    GetFKeyIconPointer(1061, &iresult);
    DisplayFKeyIcon(1, iresult);      //F2

 
    GetFKeyIconPointer(0x02B1, &iresult);
    DisplayFKeyIcon(5, iresult);      //F6

    return;
}

void CreateBTMenu(){                                //Create the window to create a BT AP

    unsigned char* iresult5;
    MsgLength = 3;
    memset(MsgBuffer, 0, 1024);
    Bdisp_AllClr_VRAM();                            //clear VRAM
    locate(1,1);
    Print((unsigned char*)"Create new BT");       
    locate(1,2);
    Print((unsigned char*)"Name :");      
    locate(1,4);
    Print((unsigned char*)"->");                  
    Cursor_SetFlashOn(1);
    IsTyping = 1;

    GetFKeyIconPointer(0x02B1, &iresult5);
    DisplayFKeyIcon(5, iresult5);      //F6

    GetFKeyIconPointer(1061, &iresult5);
    DisplayFKeyIcon(1, iresult5);      //F2

    GetFKeyIconPointer(1017, &iresult5);
    DisplayFKeyIcon(0, iresult5);      //F1
    return;
}

void MainMenu(){


    unsigned char *iresult;
    Bdisp_AllClr_VRAM();

    //Put Fkey bmps on screen
    GetFKeyIconPointer(0x0165, &iresult);
    DisplayFKeyIcon(0, iresult);       //F1
    
    GetFKeyIconPointer(1113, &iresult);
    DisplayFKeyIcon(1, iresult);      //F2
        
    GetFKeyIconPointer(38, &iresult);
    DisplayFKeyIcon(2, iresult);      //F3

    GetFKeyIconPointer(0x0165, &iresult);
    DisplayFKeyIcon(3, iresult);      //F4

    GetFKeyIconPointer(992, &iresult);
    DisplayFKeyIcon(4, iresult);      //F5
    
    locate(1,1);
    Print((unsigned char*)"F1:open serial ESP32");
    locate(1,2);
    Print((unsigned char*)"F2:exam mode");
    locate(1,3);
    Print((unsigned char*)"F3:power off ESP32");
    locate(1,4);
    Print((unsigned char*)"F4:Connect BT");
    locate(1,5);
    Print((unsigned char*)"F5:Restart ESP32");
    locate(1,6);
    Print((unsigned char*)"ESP32 disconnected");
    locate(1,7);
    Print((unsigned char*)"Exam mode off    ");
    Keyboard_PutKeycode(-1, -1, 0x30);
    return;

}

void main() {

    
    unsigned int key;
    int isPressed = 0;
    MainMenu();
   
    while(1){

        isPressed = 0;
        GetKey(&key);

        if(window == 0){
            if (key == KEY_CTRL_EXIT) {     //Stop the program if the key is EXIT

                const char* pd = "turn off ESP32 !";
                locate(3,3);
                DisplayMessageBox(6, pd);

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

            if (key == KEY_CTRL_F5) {       
                Serial_ClearTransmitBuffer();
                Serial_WriteBytes("&Rst&", 6);
                Sleep(10);
            }

            if(Serial_IsOpen() == 1){       //check if the serial is open
                unsigned char* iresult;
                GetFKeyIconPointer(267, &iresult);
                DisplayFKeyIcon(0, iresult);       //F1
                locate(1,6);
                Print((unsigned char*)"ESP32 connected    ");
            }else{                          //This execute if the serial is closed
                unsigned char* iresult;
                GetFKeyIconPointer(0x0165, &iresult);
                DisplayFKeyIcon(0, iresult);       //F1
                locate(1,6);
                Print((unsigned char*)"ESP32 disconnected");
            }

            if(key == KEY_CTRL_F2){         //Send signal to ESP32 to blink the exam led if F2 is pressed
                if(IsExam == 0){
                    Serial_ClearTransmitBuffer();
                    Serial_WriteBytes("&ExmOn&", 8);
                    Sleep(10);
                    locate(1,7);
                    Print((unsigned char*)"Exam mode on     ");
                    IsExam = 1;
                }
                else if(IsExam == 1){       //Send signal to ESP32 to stop blinking the exam led
                    Serial_ClearTransmitBuffer();
                    Serial_WriteBytes("&ExmOff&", 9);
                    Sleep(10);
                    locate(1,7);
                    Print((unsigned char*)"Exam mode off    ");
                    IsExam = 0;
                }
            }

            if(key == KEY_CTRL_F3){         //Open test mode if F3 is pressed
                
                if(IsSleeping == 0){
                    
                    locate(1,3);
                    Print((unsigned char*)"F3:power on ESP32  ");
                    IsExam = 0; 
                    IsSleeping=1;
                    Serial_WriteBytes("&Sleep&", 8);
                    Sleep(10);

                }else if(IsSleeping == 1){
                    locate(1,3);
                    Print((unsigned char*)"F3:power off ESP32  ");
                    IsSleeping=0;
                    Serial_ClearTransmitBuffer();
                    Serial_WriteBytes("&SleepEXIT&", 12);
                    Sleep(1000);
                }
            }

            if(IsSleeping == 0 && key != KEY_CTRL_F3){
                locate(1,3);
                Print((unsigned char*)"F3:power off ESP32  ");
            }else if(IsSleeping == 1 && ( key == KEY_CTRL_F2 || key == KEY_CTRL_F1)){
                locate(1,3);
                Print((unsigned char*)"F3:power off ESP32  ");
                IsSleeping=0;
                Serial_ClearTransmitBuffer();
                Serial_WriteBytes("&SleepEXIT&", 12);
                Sleep(1000);
            }

            if(key == KEY_CTRL_F4){         //Open BT list if F4 is pressed
                
                OpenBTList();
                window = 1;
            }
        }

        if(window == 1){
            if (key == KEY_CTRL_F2) {

                unsigned char* iresult3;
                if(isLowercase == 1){
                    GetFKeyIconPointer(1060, &iresult3);
                    DisplayFKeyIcon(1, iresult3);
                    isLowercase = 0;
                }else if(isLowercase == 0){
                    
                    GetFKeyIconPointer(1061, &iresult3);
                    DisplayFKeyIcon(1, iresult3);
                    isLowercase = 1;
                }
            }
            if(((key >= 65 && key <= 90) || (key >= 48 && key <= 57)) && IsTyping == 1){

                
                char buffer[12];
                locate(MsgLength,4);
                if(isLowercase == 0){
                    sprintf(buffer, "%c", key);         //If the input is upper case, get the normal input
                }else if(key >= 65 && key <= 90){
                    sprintf(buffer, "%c", (key+32));    //if the input is a number and upper case, get the input + 32 to be in lower case
                }else{
                    sprintf(buffer, "%c", key);
                }
                Print(buffer);
                strncat(MsgBuffer, &buffer, 1);
                MsgLength++;
            }
            else if((key == KEY_CTRL_DEL) && IsTyping == 1){
                if(MsgLength > 3){
                    MsgLength--;
                    locate(MsgLength,4);
                    Print((unsigned char*)" ");
                    strcpy(&MsgBuffer[strlen(MsgBuffer)-1], &MsgBuffer[strlen(MsgBuffer)]);
                    locate(MsgLength,4);
                }
            }
            if((key == KEY_CTRL_EXE) && IsTyping == 1){
                IsTyping = 0;
                if(strlen(MsgBuffer) == 0){
                    DisplayErrorMessage(20);
                }else{
                    
                    int IsConnected = 0;

                    char *ReservedBuffer[1024];
                    short *recSize;
                    int CountExcess = 0;

                    char *ReservedBufferBisPointer = &ReservedBufferBis;
                    short* recSizeBis;


                    Cursor_SetFlashOff();


                    //******************************************** ConnectBT *********************************************//


                    Serial_ClearTransmitBuffer();
                    Serial_WriteBytes("&ConnectBT&", 12);    //send signal to the ESP32 to connect
                    Sleep(10);
                    while(Serial_ReadBytes(ReservedBuffer, 1024, recSize) == 1){     //waiting for a response
                        CountExcess++;
                        if(CountExcess >= 5){                              //TimeOut
                            break;
                        }
                        Sleep(1000);
                    }

                    Serial_ClearTransmitBuffer();
                    Serial_WriteBytes(MsgBuffer, strlen(MsgBuffer)+1);              //send the name of the device to connect to the ESP32

                    Sleep(10);
                    CountExcess = 0;
                    while(Serial_ReadBytes(ReservedBufferBisPointer, 1024, recSizeBis) == 1){   //waiting for a response
                        CountExcess++;
                        if(CountExcess >= 12){                                //TimeOut
                            break;
                        }
                        Sleep(1000);
                    }


                    //******************************************************************************************************//


                    locate(1,7);

                    if(strcmp(ReservedBufferBis, "&Connect") == 0){            //connected
                        Print((unsigned char*)"Connected !            ");
                    }else if(strcmp(ReservedBufferBis, "&Fail") == 0){         //TimeOut
                        Print((unsigned char*)"BT not found           ");
                    }else{
                        Print((unsigned char*)"Communication error    ");
                    }

                    MsgLength = 5;
                }
            }
            if(key == KEY_CTRL_F1){
                isPressed = 1;
                window = 2;
                CreateBTMenu();
            }
            if(key == KEY_CTRL_F6 || key == KEY_CTRL_EXIT){
                Bdisp_AllClr_VRAM();
                window = 0;
                Cursor_SetFlashOff();
                MainMenu();
            }

        }

        if(window == 2){
            if(key == KEY_CTRL_F6 || key == KEY_CTRL_EXIT){
                Bdisp_AllClr_VRAM();
                Cursor_SetFlashOff();
                window = 1;
                OpenBTList();
            } 

            if (key == KEY_CTRL_F2) {

                unsigned char* iresult3;
                if(isLowercase == 1){
                    GetFKeyIconPointer(1060, &iresult3);
                    DisplayFKeyIcon(1, iresult3);
                    isLowercase = 0;
                }else if(isLowercase == 0){
                    
                    GetFKeyIconPointer(1061, &iresult3);
                    DisplayFKeyIcon(1, iresult3);
                    isLowercase = 1;
                }
            }
            
            else if(((key >= 65 && key <= 90) || (key >= 48 && key <= 57)) && IsTyping == 1){

                
                unsigned char buffer[12];
                locate(MsgLength,4);
                if(isLowercase == 0){
                    sprintf(buffer, "%c", key);         //If the input is upper case, get the normal input
                }else if(key >= 65 && key <= 90){
                    sprintf(buffer, "%c", (key+32));    //if the input is a number and upper case, get the input + 32 to be in lower case
                }else{
                    sprintf(buffer, "%c", key);
                }
                Print(buffer);
                strncat(MsgBuffer, &buffer, 1);
                MsgLength++;
            }
            else if((key == KEY_CTRL_DEL) && IsTyping == 1){

                if(MsgLength > 3){
                    MsgLength--;
                    locate(MsgLength,4);
                    Print((unsigned char*)" ");
                    strcpy(&MsgBuffer[strlen(MsgBuffer)-1], &MsgBuffer[strlen(MsgBuffer)]);
                    locate(MsgLength,4);
                }
            }
            if(((key == KEY_CTRL_EXE) || (isPressed*-1+1) * (key == KEY_CTRL_F1)) && IsTyping == 1){

                IsTyping = 0;
                if(strlen(MsgBuffer) == 0){
                    DisplayErrorMessage(20);
                    
                }else if(strlen(MsgBuffer) > 25){
                    DisplayErrorMessage(33);
                    
                }else{
                   

                    unsigned char *ReservedBuffer;
                    short *recSize;
                    int CountExcess = 0;
                    Cursor_SetFlashOff();

                    //******************************* Create BT **********************************//

                    Serial_ClearTransmitBuffer();
                    Serial_WriteBytes((unsigned char*)"&NewBT&", 8);     //send signal to the ESP32 to create BT AP
                    Sleep(10);
                    while(Serial_ReadBytes(ReservedBuffer, 1024, recSize) == 1){     //waiting for a response
                        CountExcess++;
                        if(CountExcess >= 5){                               //TimeOut
                            break;
                        }
                        Sleep(1000);
                    }
                    Sleep(10);
                    Serial_ClearTransmitBuffer();
                    Serial_WriteBytes(MsgBuffer, strlen(MsgBuffer));         //Send the name of BT AP to the ESP32
                    Sleep(10);

                    //******************************************************************************//

                    locate(1,7);
                    Print((unsigned char*)"Created !            ");
                    MsgLength = 3;
                }
            }
        }
    }

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
