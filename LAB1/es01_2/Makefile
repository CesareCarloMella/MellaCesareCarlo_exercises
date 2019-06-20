CC = g++
CFLAGS = -Wall -O3 --std=c++11

compila: es01_2U.exe es01_2E.exe es01_2L.exe


es01_2U.exe : es01_2U.o random.o 
	$(CC) random.o es01_2U.o -o es01_2U.exe
es01_2E.exe : es01_2E.o random.o
	$(CC) random.o es01_2E.o -o es01_2E.exe
es01_2L.exe : es01_2L.o random.o
	$(CC) random.o es01_2L.o -o es01_2L.exe


es01_2U.o : es01_2U.cpp
	$(CC) -c es01_2U.cpp -o es01_2U.o $(CFLAGS)
es01_2E.o : es01_2E.cpp
	$(CC) -c es01_2E.cpp -o es01_2E.o $(CFLAGS)
es01_L.o : es01_2L.cpp
	$(CC) -c es01_2L.cpp -o es01_2L.o $(CFLAGS)
	
	
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
	
esegui: es01_2U.exe es01_2E.exe es01_2L.exe
	./es01_2U.exe 
	./es01_2E.exe 
	./es01_2L.exe
	
clean :
	rm *.o *.exe
