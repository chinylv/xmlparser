CC = g++

CFLAGS = -Wall -I/usr/include/libxml2
LFLAGS = -lxml2

objects = mkglist.o

all: mkglist

%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

mkglist : $(objects)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

.PHONY: clean all

clean :
	-@rm -f *.o mkglist 2>&1 > /dev/null
