CPPFLAGS=-std=gnu++98 -pedantic -Wall -Werror -ggdb3
circles: main.o circle.o point.o
	g++ -o circles main.o circle.o point.o
main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp
circle.o: circle.cpp circle.h
	g++ $(CPPFLAGS) -c circle.cpp
point.o: point.cpp point.h
	g++ $(CPPFLAGS) -c point.cpp

.PHONY: clean
clean:
	rm -f *.o *~ circles
