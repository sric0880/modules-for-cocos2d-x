CC = gcc
CFLAGS = -D SQLITE_HAS_CODEC

all:sqlite3 sqlite3_test

sqlite3_test:main_test.o sqlite3secure.o
	$(CC) -o $@ $^
sqlite3:main.o sqlite3secure.o
	$(CC) -o $@ $^
sqlite3secure.o:sqlite3secure.c
	$(CC) -c $< $(CFLAGS)
main.o:main.c
	$(CC) -c $< $(CFLAGS)
main_test.o:main_test.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm *.o sqlite3 sqlite3_test