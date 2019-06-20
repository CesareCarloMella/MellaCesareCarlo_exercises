CC = g++
CFLAGS = -Wall -O3 --std=c++11

es02_1.exe : es02_1.o random.o MediaBlocchi.o
	$(CC) random.o es02_1.o MediaBlocchi.o -o es02_1.exe
es02_1.o : es02_1.cpp
	$(CC) -c es02_1.cpp -o es02_1.o $(CFLAGS)
MediaBlocchi.o : MediaBlocchi.cpp MediaBlocchi.h
	$(CC) -c MediaBlocchi.cpp -o MediaBlocchi.o $(CFLAGS)	
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o es02_1.exe 
