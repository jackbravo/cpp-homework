all: shrobj1.so shrobj2.so dltest

shrobj1.o:	shrobj1.cpp objdef.h
	g++ -fpic -c shrobj1.cpp

shrobj2.o:	shrobj2.cpp objdef.h
	g++ -Wall -fpic -c shrobj2.cpp

shrobj1.so:     shrobj1.o
	g++ -o shrobj1.so -shared shrobj1.o 

shrobj2.so:     shrobj2.o
	g++ -o shrobj2.so -shared shrobj2.o 

dltest:	dltest.cpp objdef.h
	g++ -o dltest -ldl dltest.cpp

clean:
	rm -v *.o *.so dltest 
