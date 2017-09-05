g++flags = -std=c++14 -g
#-O2 -Wall -pedantic -pthread

test:
	g++ ${g++flags} test.cpp -o test.exe

clean:
	rm *.exe

all: test
