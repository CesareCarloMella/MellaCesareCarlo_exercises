CC = g++
CFLAGS = -Wall -O3 --std=c++11

compila : es01_1.exe ChiTest.exe

es01_1.exe : es01_1.o random.o MediaBlocchi.o
	$(CC) random.o es01_1.o MediaBlocchi.o -o es01_1.exe
ChiTest.exe : ChiTest.o random.o
	$(CC) random.o ChiTest.o -o ChiTest.exe
	
MediaBlocchi.o : MediaBlocchi.h MediaBlocchi.cpp
	$(CC) -c MediaBlocchi.cpp -o MediaBlocchi.o $(CFLAGS)
es01_1.o : es01_1.cpp
	$(CC) -c es01_1.cpp -o es01_1.o $(CFLAGS)
ChiTest.o: ChiTest.cpp
	$(CC) -c ChiTest.cpp -o ChiTest.o $(CFLAGS)	
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
	
clean :
	rm *.o *.exe
