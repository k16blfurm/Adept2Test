---
description: Just a quick guide on how to install and run it on Ubuntu 18.04
---

# Installing on Ubuntu 18.04/Reading Registers

So, we need to start with getting all of the install files from the Adept Website found here.

{% embed url="https://reference.digilentinc.com/reference/software/adept/start?redirect=1\#software\_downloads" %}

You're going to want to download all of the files available for Ubuntu and install them either through the .tar file or the .deb install

To see if the programs installed successfully, you want to try and use the Makefile to compile a GCC program for your specific board. \(In my case, the board is a CMOD S6, which works the easiest with DEPP, which I try to run below.\)

For the case of demonstration, go to the file extension of \(in your extracted digilent.adept.sdk\_2.X:

```text
/samples/depp/DeppDemo/
```

To compile the test program, you can just enter this below

```text
$ make DeppDemo
```

Note: to compile other programs, you can just enter this command in that test folder to see what commands the Makefile has.

```text
$ cat Makefile
```

Going from there, we can now run our DeppDemo project by entering this in...

```text
$ ./DeppDemo
```

This will bring up a few options from the program, but I'll show you the most important ones in use for this board.

For our project, we only need to read bits, but you can also write them. You can read a single bit to stream by writing.

```text
$ DeppDemo -s 0 -d CmodS6 -f bit.txt 
```

This command will stream out the register \(-s\) 0 from the device \(-d\) Cmod S6, into file bit.txt, which is what we want.

We can also set commands using the -c parameter and much more.

I'll be updating this post with my own code and linking it to my GitHub to read out the bytes to a constantly updating file or database.

