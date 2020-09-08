#
## CSCI 370: Sample Makefile for OCCI compilation
#

G = g++-4.9
CFLAGS = -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
LFLAGS = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei
all: forum



forums.o: forums.cpp
	$(G) -c $(CFLAGS) forums.cpp

forum: forums.o
	$(G) $(LFLAGS) -o forum forums.o

#exampleUnprepared.o: exampleUnprepared.cpp
#	$(G) -c $(CFLAGS) exampleUnprepared.cpp

#exampleU: exampleUnprepared.o
#	$(G) $(LFLAGS) -o exampleU exampleUnprepared.o

clean:
	rm -f *.o exampleP exampleU




