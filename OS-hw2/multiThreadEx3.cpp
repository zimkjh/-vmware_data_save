#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <chrono>
using namespace std;
using namespace chrono;

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
	system_clock::time_point end = system_clock::now();
	microseconds microSec = duration_cast<microseconds>(end - start);
	cout<<microSec.count()<<endl;	
	return 0;	
}
void *runner(void *param){

	int x = 0;
	int reali = *((int *)param);	
	if(reali==0){
	    	for (int i = 1; i <= 50000; i++)
		{
			for (int j = 1; j <= 50000; j++)
			{
		 		x = (i + j) * 2 + (i / j);
			}
	    	}
	}


	if(reali==1){
	    	for (int i2 = 1; i2 <= 50000; i2++)
		{
			for (int j2 = 1; j2 <= 50000; j2++)
			{
		 		x = (i2 + j2) * 2 + (i2 / j2);
			}
	    	}
	}


	pthread_exit(0);



}

