CC = g++
CFLAGS = -Wall -O3 --std=c++11

es09.exe : es09.o TSM.o random.o
	$(CC) es09.o TSM.o random.o -o es09.exe
es09.o : es09.cpp
	$(CC) -c es09.cpp -o es09.o $(CFLAGS)	
random.o : random.h random.cpp
	$(CC) -c random.cpp -o random.o $(CFLAGS)	
TSM.o : TSM.cpp TSM.h 
	$(CC) -c TSM.cpp -o TSM.o $(CFLAGS)
clean :
	rm *.o es09.exe
