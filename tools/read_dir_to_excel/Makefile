CC=g++

dirext:dirext.o
	$(CC) $< -o dirext -lxls
dirext.o:dirext.cpp
	$(CC) -c $< -I/usr/local/include -L/usr/local/lib
clean:
	rm -f dirext.o dirext