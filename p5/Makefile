CC = gcc
CFLAGS = -Wall -std=c99 -g

#The default to build the executable
hash: hash.o md5.o hmac-md5.o buffer.o

#Builds the hash.o file
hash.o: hash.c md5.h hmac-md5.h buffer.h

#Builds the buffer.o file
buffer.o: buffer.c buffer.h

#Builds the md5.o file
md5.o: md5.c md5.h buffer.h

#Builds the hmac-md5.o file
hmac-md5.o: hmac-md5.c hmac-md5.h buffer.h md5.h

#Builds the testdriver.o file
testdriver: testdriver.c md5.c buffer.c
	gcc -Wall -std=c99 -g -DTESTABLE testdriver.c md5.c buffer.c -o testdriver

#Rule used for cleaning the directory of files
clean:
	rm -f hash.o md5.o hmac-md5.o buffer.o
	rm -f hash
	rm -f testdriver