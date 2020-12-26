CC = gcc
CFLAGS = -Wall -std=c99 -g

#The default to build the executable
fwsim: fwsim.o command.o packet.o policy.o
    
#Builds the fwsim.o file
fwsim.o: fwsim.c packet.h command.h policy.h
    
#Builds the packet.o file
packet.o: packet.c packet.h command.h

#Builds the policy.o file
policy.o: policy.c policy.h command.h

#Builds the command.o file
command.o: command.c command.h

#Rule used for cleaning the directory of files
clean:
	rm -f fwsim.o policy.o packet.o command.o
	rm -f fwsim