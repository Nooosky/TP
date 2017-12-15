CC = gcc
CFLAGS = -g -Wall -std=c99
LDFLAGS = -g

CXX = c++
CXXFLAGS = -Wall -O2 -g -Iinclude -Itests -Itests/include

SRC = ./src
BIN = ./bin
INC = ./include

FA = fa
AUTO = automate
GTEST=./lib/libgtest.a ./lib/libgtest_main.a
RUNTESTS_OBJ = tests/src/gtest-all.o bin/testsFa.o run_tests.o

all : $(AUTO)

$(AUTO) : $(BIN)/$(FA).o $(BIN)/$(AUTO).o
	$(CC) $(LDFLAGS) -o $(AUTO) $^ -lm

$(BIN)/$(FA).o : $(SRC)/$(FA).c $(INC)/$(FA).h
	$(CC) -c -o $@ $(CFLAGS) $<

$(BIN)/$(AUTO).o : $(SRC)/$(AUTO).c $(INC)/$(FA).h
	$(CC) -c -o $@ $(CFLAGS) $<

maketesto:
	g++ -c ./tests/testsFa.cpp -o ./bin/testsFa.o -lgtest -lpthread

test: run_tests maketesto
	./run_tests

run_tests: $(RUNTESTS_OBJ)
	$(CXX) -o $@ $(RUNTESTS_OBJ) -L. -lpthread

clean :
	rm -f ./bin/*.o

mrproper : clean
	rm -f $(AUTO)
