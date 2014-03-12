Description
===========

xmlparser uses libxml2 &amp; GNU getopt.

User writes information in xml files, and xmlparser parses the xml files and produces the switch rules and group information.

This program is for group information.

Installation
============

Please write the email list data into an input file, separated by commas.
The input file could contain multiple lines, every line break is regarded as a separator(comma).

For mklist
1. make
2. ./mkglist -h
3. use make clean to clean up

Usage
=====

Usage: ./mkglist [-f file] | -h

The details about options & parameters:
   -f   file       A must. Specify the input file.
   -h              Not must. Find help on how to use this command.

You can sepcify the output file via pipeline.
Like ./mkglist -f users.xml >> output.txt

For further information, please contact me at chinylv@gmail.com.
