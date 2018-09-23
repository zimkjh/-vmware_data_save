#include <iostream>
#include <pthread.h>
#include <cstdio>
#include <chrono>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
using namespace chrono;

int main()
{   
   pid_t pid;
	int parentid = getpid();
   	for (int i = 0; i<30; i++){
		system_clock::time_point start = system_clock::now();
      		if ((pid = fork()) == 0 ){ //new child
         		if(getpid()-parentid-1==i){
				system_clock::time_point end = system_clock::now();
				microseconds timerun = duration_cast<microseconds>(end - start);
				cout<<i<<" times : "<< timerun.count()<<endl;
				exit(0);
         		}
      		}
		else  {
            		wait(0);
		}
      
   	}
          
   
   
}
