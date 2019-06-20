CC = g++
CFLAGS = -Wall -O5 --std=c++11

GroundState.exe : GroundState.o random.o Function.o MediaBlocchi.o
	$(CC) random.o GroundState.o Function.o MediaBlocchi.o -o GroundState.exe
Function.o : Function.cpp Function.h random.h
	$(CC) -c Function.cpp -o Function.o $(CFLAGS)
GroundState.o : GroundState.cpp
	$(CC) -c GroundState.cpp -o GroundState.o $(CFLAGS)
MediaBlocchi.o : MediaBlocchi.cpp MediaBlocchi.h
	$(CC) -c MediaBlocchi.cpp -o MediaBlocchi.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o GroundState.exe 
