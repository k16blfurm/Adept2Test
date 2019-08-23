# Changed makefile for DeppDemo

CC = g++
INC = /usr/include/digilent/adept
LIBDIR = /usr/lib64/digilent/adept
TARGETS = DeppDemo
CFLAGS = -I $(INC) -L $(LIBDIR) -ldepp -ldmgr

all: $(TARGETS)

DeppDemo:
	$(CC) -o DeppDemo DeppDemo.cpp $(CFLAGS)
	

.PHONY: clean

clean:
	rm -f $(TARGETS)

