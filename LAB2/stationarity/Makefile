CC = g++
CFLAGS = -Wall -O3 --std=c++11

es02_2.exe : es02_2.o random.o walk.o
	$(CC) random.o es02_2.o walk.o -o es02_2.exe
es02_2.o : es02_2.cpp
	$(CC) -c es02_2.cpp -o es02_2.o $(CFLAGS)	
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
walk.o : walk.cpp walk.h random.h
	$(CC) -c walk.cpp -o walk.o $(CFLAGS)
clean :
	rm *.o es02_2.exe 
