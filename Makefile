CC=gcc
Target=libusb
INC=-I/usr/include/libusb-1.0
LIB=-lusb-1.0

libusb: main.o
	$(CC) main.o -o $(Target) $(INC) $(LIB)

main.o: main.c
	$(CC) main.c -c -o main.o $(INC) $(LIB)

clean:
	rm -f *.o
	rm -f $(Target)
