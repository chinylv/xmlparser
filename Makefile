CC = g++

CFLAGS = -Wall -lxml2 -I /usr/include/libxml2

objects = xml_parser.o

default: rule

%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

rule : $(objects)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

.PHONY: clean

clean :
	rm -f *.o rule
