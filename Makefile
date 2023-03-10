PROGRAM=trash
CC=gcc

$(PROGRAM): main.o
	$(CC) -o $(PROGRAM) main.o

clean:
	rm $(PROGRAM) main.o
