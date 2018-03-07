Program: Program.o Philosophers.o
	g++ -pthread Program.o Philosophers.o -o Program -lncurses

Program.o: Program.cpp
	g++ -pthread -c Program.cpp 

Philosophers.o: Philosophers.cpp Philosophers.h
	g++ -pthread -c Philosophers.cpp
