CC = gcc
CFLAGS = -Wall -g
TARGET = lib.o

.SUFFIXES : .c .o

all : $(TARGET)

clean : 
	rm $(TARGET)

.c.o : $<
	$(CC) -c $(CFLAGS) $<
