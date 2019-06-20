CC = g++
CFLAGS = -Wall -O3 --std=c++11


Conversion.exe : Conversion.o 
	$(CC) Conversion.o -o Conversion.exe
Conversion.o : Conversion.cpp
	$(CC) -c Conversion.cpp -o Conversion.o $(CFLAGS)
clean :
	rm *.o Conversion.exe
