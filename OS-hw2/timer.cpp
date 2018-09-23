#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <chrono>
using namespace std;
using namespace chrono;
void BigLoop()
{
	int x = 0;
    for (int i = 1; i <= 50000; i++)
    {
        for (int j = 1; j <= 50000; j++)
        {
            x = (i + j) * 2 + (i / j);
        }
    }
    for (int i = 1; i <= 50000; i++)
    {
        for (int j = 1; j <= 50000; j++)
        {
            x = (i + j) * 2 + (i / j);
        }
    }
}
int main(){
	system_clock::time_point start = system_clock::now();
 
	BigLoop();
 
    	system_clock::time_point end = system_clock::now();
 
   	// 초 단위 (소수점으로 표현)
    	microseconds microSec = duration_cast<microseconds>(end - start);
	cout<<microSec.count()<<endl;	
	return 0;	
}
