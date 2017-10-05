g++flags_dbg = -std=c++14 -g
g++flags_rls = -std=c++14 -O2 -Wall -pedantic -pthread

test:
	g++ ${g++flags_dbg} test.cpp -o test.exe >& build.log ; less build.log

clean:
	rm *.exe

format:
	clang-format -i *.{h,c}*

all: test
