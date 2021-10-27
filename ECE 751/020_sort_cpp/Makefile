CCFLAGS=--std=gnu++98 -pedantic -Wall -Werror -ggdb3
sortLines: sortLines.o
	g++ -o sortLines $(CCFLAGS) sortLines.o

sortLines.o: code.cpp
	g++ -c $(CCFLAGS) code.cpp -o sortLines.o

clean:
	rm -f *.o  *~ sortLines
