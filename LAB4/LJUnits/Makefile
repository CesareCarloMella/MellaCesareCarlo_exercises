CC = g++
CFLAGS = -Wall -O3 --std=c++11

es04.exe : es04.o MolDyn.o
	$(CC) es04.o MolDyn.o -o es04.exe
es04.o : es04.cpp
	$(CC) -c es04.cpp -o es04.o $(CFLAGS)	

MolDyn.o : MolDyn.cpp MolDyn.h 
	$(CC) -c MolDyn.cpp -o MolDyn.o $(CFLAGS)
clean :
	rm *.o es04.exe config/config.final config/config.prefinal 
