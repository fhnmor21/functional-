g++flags = -std=c++14 
#-O2 -Wall -pedantic -pthread

curry_1:
	g++ ${g++flags} curry_1.cpp -o curry_1.exe

curry_2:
	g++ ${g++flags} curry_2.cpp -o curry_2.exe

curry_3:
	g++ ${g++flags} curry_3.cpp -o curry_3.exe

clean:
	rm *.exe

all: curry_1 curry_2 curry_3