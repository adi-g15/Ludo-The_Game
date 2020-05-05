game : main.o includes.o
	g++ main.o includes.o -o game

main.o: main.cpp
	g++ -c main.cpp

includes.o: util/*.cpp
	g++ -c util/*.cpp

clean:
	rm *.o *.out