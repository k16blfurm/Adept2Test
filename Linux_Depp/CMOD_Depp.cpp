#define	_CRT_SECURE_NO_WARNINGS

#if defined(WIN32)
 /* Include Windows specific headers here.*/
 #include <windows.h>
#endif

#include <stdio.h>
#include <iostream>  
#include <cctype>
#include <fstream>
#include "dpcdecl.h" 
#include "dmgr.h"
#include "depp.h"

#define OP1ADDR         0x00
#define OP2ADDR         0x01
#define RESADDR         0x00

int main()
{
    HIF deviceHandle;
    int status = fTrue;
    char deviceName[32] = "CmodS6";
    unsigned char result;
    bool test = true;  
    bool get_reg = true;
    int reg;
    bool getting_reg = true;
    unsigned long long int stream;
    char choice;
    bool restart = false;
    // stream for saving to file
    std::ofstream file;


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
            if(isVowel(reg) || isalpha(reg))
            {    reg = ''; 
                 get_reg == true;
                std::cout<<"enter the address in again, it is invalid"<<std::endl;
            }
            if(std::to_string(reg) == 'q')
            return 0;
            else get_reg = false;
        }

        std::cout<<"type l to stop saving data"<<std::endl;

        while(getting_reg == true)
        {   // This is the repeat to a stream register
            DeppGetRegRepeat(deviceHandle, reg, stream, 10, true);
            //std::cout<<"Reg :"<<reg<<std::cout<<"stream"
            std::cin<<getting_reg;
            if(getting_reg == 'q')
                return 0;
            if(getting_reg == 'l')
                getting_reg = false;
        }

        while(restart == false)
        {
        std::cout<<"Would you like clear stream and save values or restart?"<<std::endl;
        std::cout<<"Enter r to restart, s to save";
        if(choice == 'r')
            restart = true;
        if(choice == 's')
        {
            restart = true;
            test = false;
        }
        else{
            std::cout<<"Sorry, the program couldn't understand your choice"<<std::endl;
            std::cout<<"Please enter your choice again..."<std::endl;
        }
        choice = '';
        }
    }

// Saving to a file
    file.open("registers.txt");
    file<<stream;
    file.close()

if (status)
  printf("Successfully closed device handle\n");
else
{
  status = DmgrGetLastError();
  printf("Error code: %d\n", status);
} 
    return 0;
    
}