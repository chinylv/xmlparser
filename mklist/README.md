Description
===========

xmlparser uses libxml2 &amp; GNU getopt.

User writes information in xml files, and xmlparser parses the xml files and produces the switch rules and group information.

This program is for switch rules.

Installation
============

For mklist
1. make
2. ./mklist -h
3. use make clean to clean up

Usage
=====

Usage: ./mklist [-f file -a action [-p port] [-t protocol]] | -h

The details about options & parameters:

   -f   file       A must. Specify the input xml file.

   -a   action     A must. Specify permit or deny.

   -p   port       Not must. Specify the port number, 0~65535.

   -t   protocol   Not must. Specify the tcp or udp protocol.

   -h              Not must. Find help on how to use this command.

You can sepcify the output file via pipeline. Like ./mklist -f users.xml -p 20 -t tcp -a permit >> switches.rules

For further information, please contact me at chinylv@gmail.com.
