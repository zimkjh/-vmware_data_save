#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <chrono>
using namespace std;
using namespace chrono;
unsigned long int sum=0;

void *runner(void *param);
int main(int argc, char *argv[]){
	system_clock::time_point start = system_clock::now();
	int x = 0;
    	for (int i = 1; i <= 50000; i++)
    	{
        	for (int j = 1; j <= 50000; j++)
        	{
           	 	x = (i + j) * 2 + (i / j);
			sum += x;
        	}
    	}
	cout<<sum<<endl;
	system_clock::time_point end = system_clock::now();
	microseconds microSec = duration_cast<microseconds>(end - start);
	cout<<microSec.count()<<endl;	
	return 0;	
}

