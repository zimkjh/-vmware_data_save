#include <iostream>
#include <pthread.h>
#include <cstdio>
#include <chrono>
#include <cmath>
#include <unistd.h>

using namespace std;
using namespace chrono;

int main(int argc, char *argv[])
{
      
   pthread_t tid;
   
   for(int i=0; i<30; i++)
{
   system_clock::time_point start = system_clock::now();
   pthread_create(&tid, NULL,NULL, argv[1]);
   system_clock::time_point end = system_clock::now();
   microseconds timerun = duration_cast<microseconds>(end - start);
   int a = (int)timerun.count();
   cout << a << endl;
}
   
}
