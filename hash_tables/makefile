CXX = g++
CXXFLAGS = -Wall -g

filesys.o: filesys.cpp filesys.h
	$(CXX) $(CXXFLAGS) -c filesys.cpp

proj4: filesys.o mytest.cpp
	$(CXX) $(CXXFLAGS) filesys.o mytest.cpp -o proj4

run:
	./proj4

clean:
	rm *~
	rm *.o