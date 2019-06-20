CC = g++
CFLAGS = -Wall -O3 --std=c++11

compila : Solid.exe Liquid.exe Gas.exe
	
Solid.exe : Solid.o 
	$(CC) Solid.o -o Solid.exe
Solid.o : Solid.cpp
	$(CC) -c Solid.cpp -o Solid.o $(CFLAGS)
Liquid.exe : Liquid.o 
	$(CC) Liquid.o -o Liquid.exe
Liquid.o : Liquid.cpp
	$(CC) -c Liquid.cpp -o Liquid.o $(CFLAGS)
Gas.exe : Gas.o 
	$(CC) Gas.o -o Gas.exe
Gas.o : Gas.cpp
	$(CC) -c Gas.cpp -o Gas.o $(CFLAGS)	
clean :
	rm *.o Solid.exe Liquid.exe Gas.exe
