CXX = g++
CXXFLAGS = -Wall -g

mqueue.o: mqueue.cpp mqueue.h
	$(CXX) $(CXXFLAGS) -c mqueue.cpp

proj3: mqueue.o mytest.cpp
	$(CXX) $(CXXFLAGS) mqueue.o mytest.cpp -o proj3

run:
	./proj3

clean:
	rm *~
	rm *.o