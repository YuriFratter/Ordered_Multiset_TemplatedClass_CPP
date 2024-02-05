#MODE = -DNDEBUG
CCFLAGS = -Wall -pedantic
main.exe: main.o elementNotFound.o
	g++ $(MODE) main.o elementNotFound.o -o main.exe

main.o: main.cpp
	g++ $(MODE) $(CCFLAGS) -c main.cpp -o main.o

elementNotFound.o: elementNotFound.cpp 
	g++ $(MODE) $(CCFLAGS) -c elementNotFound.cpp -o elementNotFound.o

.PHONY: clean
clean:
	rm -rf *.o *.exe