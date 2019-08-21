# Ubuntu 18.04 Installation and Usage
This directory contains all of the necessary files for running Digilent Adept 2 on Ubuntu 18.04. This description contains detailed
instructions on how to install Adept 2 on your Linux machine and how to use the given files to be able to write and read to the registers
on the Cmod S6 FPGA using Digilent's Asynchronous Parallel Interface (DEPP) and the command line.

## Prerequisites
The first thing that you will need to do is download the approproate Adept 2 Runtime, Utilities, and SDK for Linux. I would highly
recommend downloading the deb files for Ubuntu and avoiding the zip files for the Runtime and Utilities as this will make the
installation and setup significantly easier. The rpm version is compatible with the Red Hat distro while the deb version is compatible
with distros such as Ubuntu or Debian.
Here is the link to download it: [Digilent Adept 2] (https://reference.digilentinc.com/reference/software/adept/start)

## Installing
Now that everything has been downloaded, you need to install it. Go to your Downloads folder and search for your two deb files for the
Runtime and Utilities. Simply click on each of those files, click the install button, and it's installed! I previously tried using the
zip file and extracting its contents manually, but I ran into some annoying linking errors for the libraries and header files. However,
with the deb file, it just works like its supposed to without any trouble. Finally, you have to extract the contents of the SDK that you
downloaded. The SDK is what contans the sample DEPP project that we will running to work with the registers on the FPGA.

## Checking if Adept 2 is Working
To test if you have Adept installed correctly, you need to first plug in the Cmod S6 FPGA ito your machine using your USB cable.
Then, open your terminal and type this in:

```
djtgcfg init -d CmodS6
```
and you should get an output that looks like this:

```
Initializing scan chain...
Found Device ID: 24000093

Found 1 device(s):
   Device 0: XC6SLX4
```
Where the device ID and device 0 varies depending on what board you have plugged in. In our case, it should correspond to the Cmod S6
FPGA device. If this works, then Adpet should be successfully installed.

## Compiling the DEPP Demo
Before you can us the DEPP Demom you need to compile it first. Open up your terminal and navigate into your SDK directroy.
From there, navigate to:

```
/samples/depp/DeppDemo/
```
This will put you into the directory of the Depp Demo. Here, you will find a Makefile which you can use to compile the DeppDemo.cpp
file. Enter in:

```
nano Makefile
```
This will let you edit the Makefile. If you are on a 64-bit machine, you will need to edit the line:

```
LIBDIR = /usr/lib/digilent/adept
```
to

```
LIBDIR = /usr/lib64/digilent/adept
```
This is the only tweak that you should need to make since the libraries are stored under the lib64 directory and the default lib
directory is only used for 32-bit machines. Also, you will need to take note of these two paths in the Makefile:

```
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib64/digilent/adept
```
You need to make sure that libraries and header files that you downloaded correspond to these paths. If they don't, then you will
either need to modify these paths in the Makefile or move your files within these paths. If you used the deb files, then you shouldn't
need to worry about the LIBDIR path. In my case, I only needed to move my header files into the INC path as they originally resided in
the inc directory within the SDK. You can either move this folder, or make a copy of it to place it where it needs to go. Also, you will
need to move this file into the INC path using the command line with sudo since you cannot move it using the File GUI.

Once your files are in the right place, you can now run:

```
make DeppDemo
```
This will compile the DEPP Demo. You may need to install some necessary packages such as make and gcc in order to run this command if
Ubuntu tells you to do so. This is especially likely if you are compiling this on a fresh Ubuntu install with only Adept installed.
If you recieve any compiling errors, you will need to fix them, and type in:

```
make clean
```
before you can re-run:

```
make DeppDemo
```
You can also use the command:

```
cat Makefile
```
to display the contents of the Makefile directly into the command line. This could be useful if you are interested in compiling the
other demos included with the SDK and want to see what each Makefile contains.

## Programming the FPGA
Before we can run the demo, we will also need to reprogram the FPGA with the included VHDL file and appropriate UCF file so that we can
actually comminicate with the registers. We will need to use the Xilinx ISE and Impact to do this. You can find the VHDL file under:

```
/samples/depp/DeppDemo/logic
```
within the SDK. You don't need to modify anything within this file to get it to work.

You also need to download the Cmod S6 master UCF file from:
[Digilent CmodS6] https://reference.digilentinc.com/reference/programmable-logic/cmod-s6/start()

You will then need to modify the UCF file into this:
```
#FPGA_GCLK
NET "CLK" LOC = "N8";

#DEPP Signals
NET "EppWAIT" LOC = "B6";
NET "EppASTB" LOC = "A6";
NET "EppDSTB" LOC = "B7";
NET "EppWRITE" LOC = "A7";
NET "DB<0>" LOC = "B9";
NET "DB<1>" LOC = "A9";
NET "DB<2>" LOC = "B10";
NET "DB<3>" LOC = "A10";
NET "DB<4>" LOC = "B11";
NET "DB<5>" LOC = "A11";
NET "DB<6>" LOC = "B12";
NET "DB<7>" LOC = "A12";

#IO PORTs

#A
NET "Led<0>" LOC = "P5";
NET "Led<1>" LOC = "N5";
NET "Led<2>" LOC = "N6";
NET "Led<3>" LOC = "P7";
NET "Led<4>" LOC = "P12";
NET "Led<5>" LOC = "N12";
NET "Led<6>" LOC = "L14";
NET "Led<7>" LOC = "L13";
#B
NET "sw<0>" LOC = "K14";
NET "sw<1>" LOC = "K13";
NET "sw<2>" LOC = "J14";
NET "sw<3>" LOC = "J13";
NET "sw<4>" LOC = "H14";
NET "sw<5>" LOC = "H13";
NET "sw<6>" LOC = "F14";
NET "sw<7>" LOC = "F13";

#C
NET "btn<0>" LOC = "G14";
NET "btn<1>" LOC = "G13";
NET "btn<2>" LOC = "E14";
NET "btn<3>" LOC = "E13";
```
Once this is done, you can generate the programming file in Xilinx ISE, and use Impact to reprogram the board using the .bit file.

You can also use Adept to reprogram the board. However, once the board loses power, the programming gets erased. Impact actually
rewrites the PROM on the board, so the board stays programmed without power.

If you want to use Adept to Program the board, you can type in:

```
djtgcfg prog -d -i -f
```
This will run the Adept programmer.

You can basically reprogram the board with this single command though:

```
djtgcfg prog -d CmodS6 -i 0 -f program.bit
```
in which you would replace program.bit with your appropriate .bit file.

If this succeeded, it should display:

```
Programming device. Do not touch your board. This may take a few minutes...
Programming succeeded.
```

## Running the DEPP Demo
Finally, we can now run the DEPP Demo. To do this, type in:

```
./DeppDemo
```
This will launch the Depp Demo project and display these instructions:

```
Digilent DEPP Demo
Usage: ./DeppDemo <action> <register> -d <device name> [options]

       Actions:
       -g                               Get register byte
       -p                               Put register byte
       -l                               Stream file into register
       -s                               Stream register into file
       
       Options:
       -f <filename>                    Specify file name
       -c <# bytes>                     Number of bytes to read/write
       -b <byte>                        Value to load into register
```

However, I will show the most important commands for our Cmod S6 board.

For our project, we only need to read bits, but you can also write them. You can read a single bit to stream by writing:

```
DeppDemo -s 0 -d CmodS6 -f bit.txt
```
This command will stream out the register (-s) 0 from the device (-d) Cmod S6, into file bit.txt. However, you might notice that the
bit.txt file is blank. So, to fix this, we can run a command like:

```
./DeppDemo -s 0 -d CmodS6 -c 10 -f bit.txt
```
This utilizes the -c parameter to set the number of bytes to read/write, and will write to the bit.txt file properly.

However, the bit.txt file will be in Hex format. So, to be able to read it, we can run the command:

```
xxd bit.txt
```
which will display the contents of the hex file in the command line in binary, and looks like this:

```
00000000: 0000 0000 0000 0000 0000
```
We are now able to successfully read and write to the registers of the Cmod S6 FPGA using Adept 2 running on Ubuntu 18.04!

# Streaming Out Bytes
The file CMOD_Depp.cpp has created and added to this directory which streams the output to a byte and saves it to a file using ostream.
This is an improved version of the original DeppDemo.cpp file that can be compiled and used instead.

I will continue to update this post as more revisions and changes are made to this Linux_Depp directory or Adept2Test repository.
