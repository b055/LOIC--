Tutorial3: LOIC++.o Flooder.o HTTPFlooder.o XXPFlooder.o 
	g++ -o run LOIC++.o Flooder.o HTTPFlooder.o XXPFlooder.o -std=c++0x -lpthread 

# now define how each object file is a target and list dependencies and how
#  to build that object file if any dependencies change
	
Flooder.o: Flooder.cpp Flooder.h
	g++ -c Flooder.cpp -std=c++0x -lpthread


HTTPFlooder.o: Flooder.o HTTPFlooder.h HTTPFlooder.cpp
	g++ -c HTTPFlooder.cpp -std=c++0x -lpthread

XXPFlooder.o:	Flooder.o XXPFlooder.h XXPFlooder.cpp
	g++ -c XXPFlooder.cpp -std=c++0x -lpthread
	
LOIC++.o: LOIC++.cpp
	g++ -c LOIC++.cpp

clean:
	rm  run *.o *.gch


# that's all

