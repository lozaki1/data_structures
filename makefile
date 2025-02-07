CXX = g++
CXXFLAGS = -Wall -g

solitaire.o: solitaire.cpp solitaire.h
	$(CXX) $(CXXFLAGS) -c solitaire.cpp

proj0: solitaire.o mytest.cpp
	$(CXX) $(CXXFLAGS) solitaire.o mytest.cpp -o proj0

run:
	./proj0

clean:
	rm *~
	rm *.o
