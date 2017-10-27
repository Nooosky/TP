CC = gcc
CFLAGS = -g -Wall -std=c99
LDFLAGS = -g
FA = fa
AUTO = automate



all : $(AUTO) clean

$(AUTO) : $(AUTO).o $(FA).o
	$(CC) $(LDFLAGS) -o $(AUTO) $^

$(FA).o : $(FA).c $(FA).h
	$(CC) -c -o $@ $(CFLAGS) $<

$(AUTO).o : $(AUTO).c $(FA).h
	$(CC) -c -o $@ $(CFLAGS) $<


clean :
	rm -f *.o

mrproper : clean
	rm -f $(AUTO)
