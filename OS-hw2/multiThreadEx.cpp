#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <chrono>
using namespace std;
using namespace chrono;
unsigned long int sum=0, sum1, sum2;

void *runner(void *param);
int main(int argc, char *argv[]){
	system_clock::time_point start = system_clock::now();
	pthread_t tid, tid2;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int i0=0, i1=1;
	pthread_create(&tid, &attr, runner, (void*)&i0 );
	pthread_create(&tid2, &attr, runner, (void*)&i1 );
	
	pthread_join(tid, NULL);
	pthread_join(tid2, NULL);
	sum = sum1 + sum2;
	cout<<"sum = "<<sum<<endl;
	system_clock::time_point end = system_clock::now();
	microseconds microSec = duration_cast<microseconds>(end - start);
	cout<<microSec.count()<<endl;	
	return 0;	
}
void *runner(void *param){
	int i, upper = 500000000;
	int reali = *((int *)param);
	cout<<reali<<endl;

	
	if(reali==0){
		for (i = 1; i <= 250000000; i++){
			sum1 += i;
			if(sum1 == 12500002500000)
				sum1 = 0;
		}
	}
	else{
		for (i = 250000001; i <= upper; i++){
			sum2 += i;
			if(sum2 == 12500002500000)
				sum2 = 0;
		}
	}
	pthread_exit(0);
}

