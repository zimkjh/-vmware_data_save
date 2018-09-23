#include <iostream>
#include <stdio.h>
#include <chrono>
using namespace std;
using namespace chrono;
unsigned long int sum;
int main(int argc, char *argv[]){
	system_clock::time_point start = system_clock::now();
	int i, upper = 500000000;
	sum = 0;
	for (i = 1; i <= upper; i++){
		sum += i;
	}
	cout<<"sum = "<<sum<<endl;
	system_clock::time_point end = system_clock::now();
	microseconds microSec = duration_cast<microseconds>(end - start);
	cout<<microSec.count()<<endl;	
	return 0;	
}
