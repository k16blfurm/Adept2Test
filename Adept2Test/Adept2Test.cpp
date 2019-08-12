/************************************************************************/
/*																		*/
/*  DeppDemo.cpp  --  DEPP DEMO Main Program							*/
/*																		*/
/************************************************************************/
/*  Author:	Aaron Odell													*/
/*  Copyright:	2010 Digilent, Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*		DEPP Demo demonstrates how to transfer data to and from			*/
/*		a Digilent FPGA board using the DEPP module of the Adept		*/
/*		SDK.															*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	03/02/2010(AaronO): created											*/
/*																		*/
/************************************************************************/

#define	_CRT_SECURE_NO_WARNINGS

/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */

#if defined(WIN32)

	/* Include Windows specific headers here.
	*/
#include <windows.h>

#else

	/* Include Unix specific headers here.
	*/

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dpcdecl.h" 
#include "depp.h"
#include "dmgr.h"

	/* ------------------------------------------------------------ */
	/*					Local Type and Constant Definitions			*/
	/* ------------------------------------------------------------ */

const int cchSzLen = 1024;
const int cbBlockSize = 1000;

/* ------------------------------------------------------------ */
/*					Global Variables							*/
/* ------------------------------------------------------------ */

BOOL			fGetReg;
BOOL			fPutReg;
BOOL			fGetRegRepeat;
BOOL			fPutRegRepeat;
BOOL			fDvc;
BOOL			fFile;
BOOL			fCount;
BOOL			fByte;

char szDvc[64] = "CmodS6";


char			szAction[cchSzLen];
char			szRegister[cchSzLen];
//char			szDvc[cchSzLen];
char			szFile[cchSzLen];
char			szCount[cchSzLen];
char			szByte[cchSzLen];

HIF				hif = hifInvalid;

FILE* fhin = NULL;
FILE* fhout = NULL;


/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Forward Declarations						*/
/* ------------------------------------------------------------ */

BOOL		FParseParam(int cszArg, char* rgszArg[]);
void		ShowUsage(char* sz);
BOOL		FInit();
void		ErrorExit();

void		DoDvcTbl();
void		DoPutReg();
BYTE		DoGetReg(BYTE idReg[]);
void		DoPutRegRepeat();
void		DoGetRegRepeat();

void		StrcpyS(char* szDst, size_t cchDst, const char* szSrc);

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/***	main
**
**	Synopsis
**		int main(cszArg, rgszArg)
**
**	Input:
**		cszArg		- count of command line arguments
**		rgszArg		- array of command line arguments
**
**	Output:
**		none
**
**	Errors:
**		Exits with exit code 0 if successful, else non-zero
**
**	Description:
**		main function of DEPP Demo application.
*/

int main(int cszArg, char* rgszArg[]) {

	//if (!FParseParam(cszArg, rgszArg)) {
	//	ShowUsage(rgszArg[0]);
	//	return 1;
	//}

	// DMGR API Call: DmgrOpen
	if (!DmgrOpen(&hif, szDvc)) {
		printf("DmgrOpen failed (check the device name you provided)\n");
		return 0;
	}

	// DEPP API call: DeppEnable
	if (!DeppEnable(hif)) {
		printf("DeppEnable failed\n");
		return 0;
	}

	BYTE request_bit = {00000000};
	BYTE * bit_storage = 0;
	DWORD value = 0;
	char * choice = NULL;


	// This is for getting the registers forever
	while (true)
	{
		printf("Please enter the values of bits you'd like to find!");
		
		scanf("%c", choice);

		if (choice == 'q')
		{
			return 0;
		}

		if (choice != 'q')
		{
			request_bit = BYTE(choice);
		}


		if (fGetReg) {
			DeppGetReg(hif, request_bit, bit_storage, value);						/* Get single byte from register */
		}

	}

	return 0;

}


/* ------------------------------------------------------------ */
/***	FParseParam
**
**	Parameters:
**		cszArg		- number of command line arguments
**		rgszArg		- array of command line argument strings
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if not parse errors, fFalse if command line
**		errors detected.
**
**	Description:
**		Parse the command line parameters and set global variables
**		based on what is found.
*/

BOOL FParseParam(int cszArg, char* rgszArg[]) {

	int		iszArg;

	/* Initialize default flag values */
	fGetReg = fFalse;
	fPutReg = fFalse;
	fGetRegRepeat = fFalse;
	fPutRegRepeat = fFalse;
	fDvc = fFalse;
	fFile = fFalse;
	fCount = fFalse;
	fByte = fFalse;

	// Ensure sufficient paramaters. Need at least program name, action flag, register number
	if (cszArg < 3) {
		return fFalse;
	}

	/* The first parameter is the action to perform. Copy the
	** the first parameter into the action string.
	*/
	StrcpyS(szAction, cchSzLen, rgszArg[1]);

	if (strcmp(szAction, "-g") == 0) {
		fGetReg = fTrue;
	}
	else if (strcmp(szAction, "-p") == 0) {
		fPutReg = fTrue;
	}
	else if (strcmp(szAction, "-s") == 0) {
		fGetRegRepeat = fTrue;
	}
	else if (strcmp(szAction, "-l") == 0) {
		fPutRegRepeat = fTrue;
	}
	else { // unrecognized action
		return fFalse;
	}

	/* Second paramater is target register on device. Copy second
	** paramater to the register string */
	StrcpyS(szRegister, cchSzLen, rgszArg[2]);


	/* Parse the command line parameters.
	*/
	iszArg = 3;
	while (iszArg < cszArg) {

		/* Check for the -d parameter which is used to specify
		** the device name of the device to query.
		*/
		if (strcmp(rgszArg[iszArg], "-d") == 0) {
			iszArg += 1;
			if (iszArg >= cszArg) {
				return fFalse;
			}
			StrcpyS(szDvc, cchSzLen, rgszArg[iszArg++]);
			fDvc = fTrue;
		}

		/* Check for the -f parameter used to specify the
		** input/output file name.
		*/
		else if (strcmp(rgszArg[iszArg], "-f") == 0) {
			iszArg += 1;
			if (iszArg >= cszArg) {
				return fFalse;
			}
			StrcpyS(szFile, cchSnMax, rgszArg[iszArg++]);
			fFile = fTrue;
		}

		/* Check for the -c parameter used to specify the
		** number of bytes to read/write from file.
		*/
		else if (strcmp(rgszArg[iszArg], "-c") == 0) {
			iszArg += 1;
			if (iszArg >= cszArg) {
				return fFalse;
			}
			StrcpyS(szCount, cchUsrNameMax, rgszArg[iszArg++]);
			fCount = fTrue;
		}

		/* Check for the -b paramater used to specify the
		** value of a single data byte to be written to the register
		*/
		else if (strcmp(rgszArg[iszArg], "-b") == 0) {
			iszArg += 1;
			if (iszArg >= cszArg) {
				return fFalse;
			}
			StrcpyS(szByte, cchUsrNameMax, rgszArg[iszArg++]);
			fByte = fTrue;
		}

		/* Not a recognized parameter
		*/
		else {
			return fFalse;
		}
	} // End while

	/* Input combination validity checks
	*/
	if (!fDvc) {
		printf("Error: No device specified\n");
		return fFalse;
	}
	if (fPutReg && !fByte) {
		printf("Error: No byte value provided\n");
		return fFalse;
	}
	if ((fGetRegRepeat || fPutRegRepeat) && !fFile) {
		printf("Error: No filename provided\n");
		return fFalse;
	}

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	ShowUsage
**
**	Synopsis
**		VOID ShowUsage(sz)
**
**	Input:
**		szProgName	- program name as called
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		prints message to user detailing command line options
*/

void ShowUsage(char* szProgName) {

	printf("\nDigilent DEPP demo\n");
	printf("Usage: %s <action> <register> -d <device name> [options]\n", szProgName);

	printf("\n\tActions:\n");
	printf("\t-g\t\t\t\tGet register byte\n");
	printf("\t-p\t\t\t\tPut Register byte\n");
	printf("\t-l\t\t\t\tStream file into register\n");
	printf("\t-s\t\t\t\tStream register into file\n");

	printf("\n\tOptions:\n");
	printf("\t-f <filename>\t\t\tSpecify file name\n");
	printf("\t-c <# bytes>\t\t\tNumber of bytes to read/write\n");
	printf("\t-b <byte>\t\t\tValue to load into register\n");

	printf("\n\n");
}



/* ------------------------------------------------------------ */
/***	StrcpyS
**
**	Parameters:
**		szDst - pointer to the destination string
**		cchDst - size of destination string
**		szSrc - pointer to zero terminated source string
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Cross platform version of Windows function strcpy_s.
*/
void StrcpyS(char* szDst, size_t cchDst, const char* szSrc) {

#if defined (WIN32)

	strcpy_s(szDst, cchDst, szSrc);

#else

	if (0 < cchDst) {

		strncpy(szDst, szSrc, cchDst - 1);
		szDst[cchDst - 1] = '\0';
	}

#endif
}

/* ------------------------------------------------------------ */

/************************************************************************/






