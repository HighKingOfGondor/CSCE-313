# makefile

all: dataserver client

reqchannel.o: reqchannel.h reqchannel.cpp
	g++ -std=c++11 -c -g reqchannel.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++ -std=c++11 -g -o dataserver dataserver.cpp reqchannel.o -lpthread

client: client_MP8.cpp reqchannel.o
	g++ -std=c++11 -g -o client client_MP8.cpp reqchannel.o -lpthread

clean:
	rm -rf *.o fifo* *.DS_Store dataserver client output*
