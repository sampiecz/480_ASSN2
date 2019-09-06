#
# PROGRAM:    Assign 2 
# PROGRAMMER: Samuel Piecz
# LOGON ID:   Z1732715
# DATE DUE:   9/17/19
#

# Compiler variables
CCFLAGS = -ansi -Wall -std=c++11

# Rule to link object code files to create executable file
Assignment2: Assignment2.o
	g++ $(CCFLAGS) -o Assignment2 Assignment2.o

# Rule to compile source code file to object code
Assignment2.o: Assignment2.cxx
	g++ $(CCFLAGS) -c Assignment2.cxx

# Pseudo-target to remove object code and executable files
clean:
	-rm *.o Assignment2
