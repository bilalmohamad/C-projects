CC = gcc
CFLAGS = -Wall -std=c99 -g

#The default to build the executable
plot: plot.o poly.o svg.o
    
#Builds the plot.o file
plot.o: plot.c poly.h svg.h
    
#Builds the poly.o file
poly.o: poly.c poly.h

#Builds the svg.o file
svg.o: svg.c svg.h poly.h

#Rule used for cleaning the directory of files
clean:
	rm -f plot.o poly.o svg.o
	rm -f plot
	rm -f output.html