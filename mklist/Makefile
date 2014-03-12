CC = g++

CFLAGS = -Wall -I/usr/include/libxml2
LFLAGS = -lxml2

objects = mklist.o

default: mklist

%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

mklist : $(objects)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

.PHONY: clean

clean :
	-@rm -f *.o mklist 2>&1 > /dev/null
