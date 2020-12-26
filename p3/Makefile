CC = gcc
CFLAGS = -Wall -std=c99 -g

#The default to build the executable
cnp: cnp.o document.o buffer.o
    
#Builds the cnp.o file
cnp.o: cnp.c document.h buffer.h
    
#Builds the document.o file
document.o: document.c document.h

#Builds the buffer.o file
buffer.o: buffer.c buffer.h document.h

#Rule used for cleaning the directory of files
clean:
	rm -f cnp.o document.o buffer.o
	rm -f cnp