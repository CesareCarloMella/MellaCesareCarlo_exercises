CC = g++
CFLAGS = -Wall -O3 --std=c++11

es10.exe : es10.o TSM.o random.o
	$(CC) es10.o TSM.o random.o -o es10.exe
es10.o : es10.cpp
	$(CC) -c es10.cpp -o es10.o $(CFLAGS)	
random.o : random.h random.cpp
	$(CC) -c random.cpp -o random.o $(CFLAGS)	
TSM.o : TSM.cpp TSM.h 
	$(CC) -c TSM.cpp -o TSM.o $(CFLAGS)
clean :
	rm *.o es10.exe
