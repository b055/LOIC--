Tutorial3: LOIC++.cpp Flooder.o HTTPFlooder.o XXPFlooder.o 
	g++ -Wall -o run LOIC++.cpp Flooder.o HTTPFlooder.o XXPFlooder.o -lboost_thread-mt -lboost_date_time

# now define how each object file is a target and list dependencies and how
#  to build that object file if any dependencies change
	
Flooder.o: Flooder.cpp Flooder.h
	g++ -Wall -c Flooder.cpp


HTTPFlooder.o: Flooder.o HTTPFlooder.h HTTPFlooder.cpp
	g++ -Wall -c HTTPFlooder.cpp

XXPFlooder.o:	Flooder.o XXPFlooder.h XXPFlooder.cpp
	g++ -Wall -c XXPFlooder.cpp
	
clean:
	rm  run *.o *.gch


# that's all

