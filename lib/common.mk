CC = gcc
CFLAGS = -Wall -g
OBJS = $(TARGET_OBJS) ../lib/lib.o
LIB_DEPS = ../lib/lib.h

all : $(TARGET)

$(TARGET) : $(OBJS) Makefile $(LIB_DEPS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

clean : 
	rm $(TARGET) $(OBJS)

%.o : %.c Makefile $(LIB_DEPS)
	$(CC) $(CFLAGS) -c -o $@ $*.c
