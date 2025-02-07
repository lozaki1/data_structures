CXX = g++
CXXFLAGS = -Wall -g

swarm.o: swarm.cpp swarm.h
	$(CXX) $(CXXFLAGS) -c swarm.cpp

proj2: swarm.o mytest.cpp
	$(CXX) $(CXXFLAGS) swarm.o mytest.cpp -o proj2

run:
	./proj2

clean:
	rm *~
	rm *.o