#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <chrono>
using namespace std;
using namespace chrono;
unsigned long int sum=0, sum1, sum2;

void *runner(void *param);
int main(int argc, char *argv[]){
	
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int i0=0, i1=1;
	
	for (int i; i<30; i++){
		system_clock::time_point start = system_clock::now();
		pthread_create(&tid, &attr, runner, argv[1] );
		system_clock::time_point end = system_clock::now();
		microseconds microSec = duration_cast<microseconds>(end - start);
	
		cout<<i<<" times "<<microSec.count()<<endl;	
		pthread_join(tid, NULL);
	}
	return 0;	
}
void *runner(void *param){
	
	pthread_exit(0);
}

