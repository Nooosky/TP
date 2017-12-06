CC = gcc
CFLAGS = -g -Wall -std=c99
LDFLAGS = -g
SRC = ./src
BIN = ./bin
INC = ./include
FA = fa
AUTO = automate



all : $(AUTO)


$(AUTO) : $(BIN)/$(AUTO).o $(BIN)/$(FA).o
	$(CC) $(LDFLAGS) -o $(AUTO) $^

$(BIN)/$(FA).o : $(SRC)/$(FA).c $(INC)/$(FA).h
	$(CC) -c -o $@ $(CFLAGS) $<

$(BIN)/$(AUTO).o : $(SRC)/$(AUTO).c $(INC)/$(FA).h
	$(CC) -c -o $@ $(CFLAGS) $<


clean :
	rm -f *.o

mrproper : clean
	rm -f $(AUTO)
