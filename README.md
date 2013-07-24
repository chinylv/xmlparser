Description
===========

xmlparser uses libxml2 &amp; GNU getopt

User writes information in xml files, and xmlparser parses the xml files and produces the switch whitelist or blacklist rules

Installation
============
1. make
2. ./rule -h
3. use make clean to clean up

Usage
=====
Usage: ./rule [-f file -a action [-p port] [-t protocol]] | -h

The details about options & parameters:
   -f   file       A must. Specify the input xml file.
   -a   action     A must. Specify permit or deny.
   -p   port       Not must. Specify the port number, 0~65535.
   -t   protocol   Not must. Specify the tcp or udp protocol.
   -h              Not must. Find help on how to use this command.

You can sepcify the output file via pipeline. 
Like ./rule -f users.xml -p 20 -t tcp -a permit >> switches.rules

For further information, please contact me at chinylv@gmail.com.
