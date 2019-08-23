#define	_CRT_SECURE_NO_WARNINGS

#if defined(WIN32)
 /* Include Windows specific headers here.*/
 #include <windows.h>
#else
	//Linux stuffs
#endif

#include <stdio.h>
#include <iostream>  
#include <string>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fstream>
#include "dpcdecl.h" 
#include "dmgr.h"
#include "depp.h"

#define OP1ADDR         0x00
#define OP2ADDR         0x01
#define RESADDR         0x00

//function declaration

int is_valid(char letter);

int main()
{
    HIF deviceHandle;
    int status = fTrue;
    char deviceName[32] = "CmodS6";
    unsigned char result;
    bool test = true;  
    bool get_reg = true;
    int reg;
    char getting_reg;
    bool FinishGettingReg = false; // this is for checking if getting the register is finished
    BYTE* stream;
    char choice;
    bool restart = false;
    char value;
    int finished_reg; // this is for checking if the user is done or not
    // stream for saving to file
    //std::ofstream file;
    FILE * pFile;


    // this opens the file registers, let the user make their own names in new releases
    pFile = fopen ("registers", "wb");
    //file.open("registers");


    //opening test for device
    status = DmgrOpen(&deviceHandle, deviceName);
    
    if(status)
        std::cout<<"Device Success: %s"<<deviceName<<std::endl;
    else 
    {
        status = DmgrGetLastError();
        std::cout<<"Error is "<<status<<std::endl;
    }    


    // now opening port

    while(test == true)
    {   
        // This get the registers
        while(get_reg == true)
        {
            std::cout<<"Enter in the addresses you'd like";
            std::cin>> reg;
            if(isalpha(reg))
            {    reg = 0; 
                 get_reg == true;
                std::cout<<"enter the address in again, it is invalid"<<std::endl;
            }
            value = (char)reg;
            if( value == 'q')
            return 0;
            else get_reg = false;
        }

        std::cout<<"type l to stop saving data"<<std::endl;

        while(FinishGettingReg == false)
        {   // This is the repeat to a stream register
            DeppGetRegRepeat(deviceHandle, reg, stream, 10, true);
            // this is to save to a file before going to next run
            fwrite(stream,sizeof(BYTE),10,pFile);
            //std::cout<<"Reg :"<<reg<<std::cout<<"stream"
            std::cin>>getting_reg;
            finished_reg = is_valid(getting_reg); 
            if(finished_reg == 1)
                return 0;
            if(finished_reg == 2)
                FinishGettingReg = true;
        }

        while(restart == false)
        {
        std::cout<<"Would you like clear stream and save values or restart?"<<std::endl;
        std::cout<<"Enter r to restart, s to save"<<std::endl;
        std::cin>> choice;
        
        if(choice == 'r')
            restart = true;
        if(choice == 's')
        {
            restart = true;
            test = false;
        }
        else{
            std::cout<<"Sorry, the program couldn't understand your choice"<<std::endl;
            std::cout<<"Please enter your choice again..."<<std::endl;
        }

        
        }
    }

// Saving to a file
    fclose(pFile);

if (status)
  printf("Successfully closed device handle\n");
else
{
  status = DmgrGetLastError();
  printf("Error code: %d\n", status);
} 
    return 0;
    
}

// This function is for checking if a char is a valid entry
int is_valid(char letter)
{
        switch(letter)
        {
            case 'q': return 1;
            case 'l': return 2;
            default: return false;

        }

}
