g++flags_dbg = -std=c++14 -g
g++flags_rls = -std=c++14 -O2 -Wall -pedantic -pthread

test_dbg:
	g++ ${g++flags_dbg} test.cpp -o test.exe

test_rls:
	g++ ${g++flags_rls} test.cpp -o test.exe

clean:
	rm *.exe

all: test
