CC = g++

all:aes

aes:main.o aes.o
	$(CC) *.o -o aes

main.o:main.c
	$(CC) -c $<

aes.o:aes.c
	$(CC) -c $<

clean:
	rm *.o aes