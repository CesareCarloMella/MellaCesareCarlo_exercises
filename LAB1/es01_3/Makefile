CC = g++
CFLAGS = -Wall -O3 --std=c++11

es01_3.exe : es01_3.o random.o ago.o MediaBlocchi.o
	$(CC) random.o es01_3.o ago.o MediaBlocchi.o -o es01_3.exe
	
MediaBlocchi.o : MediaBlocchi.cpp MediaBlocchi.h
	$(CC) -c MediaBlocchi.cpp -o MediaBlocchi.o $(CFLAGS)
es01_3.o : es01_3.cpp
	$(CC) -c es01_3.cpp -o es01_3.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
ago.o : ago.cpp random.h
	$(CC) -c ago.cpp -o ago.o $(CFLAGS)
	
clean :
	rm *.o *.exe
