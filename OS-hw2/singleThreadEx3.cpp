#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <chrono>
using namespace std;
using namespace chrono;

int main(int argc, char *argv[]){
	system_clock::time_point start = system_clock::now();
	int x = 0;
    	for (int i = 1; i <= 50000; i++)
    	{
        	for (int j = 1; j <= 50000; j++)
        	{
           	 	x = (i + j) * 2 + (i / j);
        	}
    	}
    	for (int i2 = 1; i2 <= 50000; i2++)
    	{
        	for (int j2 = 1; j2 <= 50000; j2++)
        	{
           	 	x = (i2 + j2) * 2 + (i2 / j2);
        	}
    	}
	system_clock::time_point end = system_clock::now();
	microseconds microSec = duration_cast<microseconds>(end - start);
	cout<<microSec.count()<<endl;	
	return 0;	
}

