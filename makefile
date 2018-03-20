Program: Program.o Table.o Philosopher.o
	g++ -pthread Program.o Table.o Philosopher.o -o Program -lncurses

Program.o: Program.cpp
	g++ -pthread -c Program.cpp

Philosopher.o: Philosopher.cpp Philosopher.h
	g++ -pthread -c Philosopher.cpp

Table.o: Table.cpp Table.h
	g++ -pthread -c Table.cpp
