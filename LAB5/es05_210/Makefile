CC = g++
CFLAGS = -Wall -O5 --std=c++11

es05_2.exe : es05_2.o random.o Function.o MediaBlocchi.o
	$(CC) random.o es05_2.o Function.o MediaBlocchi.o -o es05_2.exe
Function.o : Function.cpp Function.h random.h
	$(CC) -c Function.cpp -o Function.o $(CFLAGS)
es05_2.o : es05_2.cpp
	$(CC) -c es05_2.cpp -o es05_2.o $(CFLAGS)
MediaBlocchi.o : MediaBlocchi.cpp MediaBlocchi.h
	$(CC) -c MediaBlocchi.cpp -o MediaBlocchi.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

clean :
	rm *.o es05_2.exe 
