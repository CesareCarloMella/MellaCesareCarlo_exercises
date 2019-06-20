CC = g++
CFLAGS = -Wall -O3 --std=c++11

es03_1.exe : es03_1.o random.o prezzo.o MediaBlocchi.o
	$(CC) random.o prezzo.o MediaBlocchi.o es03_1.o -o es03_1.exe
MediaBlocchi.o : MediaBlocchi.h
	$(CC) -c MediaBlocchi.cpp -o MediaBlocchi.o $(CFLAGS)
es03_1.o : es03_1.cpp
	$(CC) -c es03_1.cpp -o es03_1.o $(CFLAGS)
	
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

prezzo.o : prezzo.cpp prezzo.h random.h
	$(CC) -c prezzo.cpp -o prezzo.o $(CFLAGS)
clean :
	rm *.o es03_1.exe 
