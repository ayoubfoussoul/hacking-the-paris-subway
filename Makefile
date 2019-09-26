Entry.o: Entry.cpp
	c++ -g -std=c++11 -stdlib=libc++ Entry.cpp -c

RawEntry.o: RawEntry.cpp
	c++ -g -std=c++11 -stdlib=libc++ RawEntry.cpp -c

Graph.o: Graph.cpp
	c++ -g -std=c++11 -stdlib=libc++ Graph.cpp -c

Dataset.o: Dataset.cpp
	c++ -g -std=c++11 -stdlib=libc++ Dataset.cpp -c

Network.o: Network.cpp
	c++ -g -std=c++11 -stdlib=libc++ Network.cpp -c

tests.o: tests.cpp
	c++ -g -std=c++11 -stdlib=libc++ tests.cpp -c

main: main.cpp Entry.o RawEntry.o Graph.o Dataset.o Network.o tests.o
	c++ -g -std=c++11 -stdlib=libc++ main.cpp Entry.o RawEntry.o Graph.o Dataset.o Network.o tests.o -o main

clean:
	rm -f *.o
