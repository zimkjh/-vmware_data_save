all: noparallel process thread

noparallel : noparallel.o 
	g++ -o noparallel noparallel.o
thread : thread.o 
	g++ -o thread thread.o -lpthread 
process : process.o
	g++ -o process process.o 

noparallel.o : noparallel.cpp
	g++ -c -std=c++11 noparallel.cpp

thread.o : thread.cpp 
	g++ -c -std=c++11  thread.cpp 

process.o : process.cpp
	g++ -c -std=c++11 process.cpp


clean : 
	rm -rf *o noparallel
	rm -rf *o thread
	rm -rf *o process
