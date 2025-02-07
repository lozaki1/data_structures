CXX = g++
CXXFLAGS = -Wall -g

snakes.o: snakes.cpp snakes.h
	$(CXX) $(CXXFLAGS) -c snakes.cpp

proj1: snakes.o mytest.cpp
	$(CXX) $(CXXFLAGS) snakes.o mytest.cpp -o proj1

run:
	./proj1

clean:
	rm *~
	rm *.o