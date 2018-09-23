#include <iostream>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sstream>
#include <climits>
#include <string.h>
#include <sys/types.h>
#include <string>
#include <chrono>
#define BUFFER_SIZE 16

using namespace std;
using namespace chrono;
char buffer[BUFFER_SIZE];
void runner(int rT);
int filedes[2], nRead, bi;
int I[100]; //algorithm repeat num
int K[100]; // cluster num
int N[100];// dots num
float xy[100][100000][3]; // 0:T, 1:x, 2:y, 3:cluster
float centers[100][10000][2]; // 0:T, 1:x, 2:y
float dist[100], tempdist[100];
float clustersum[100][10000][3]; // 0:T, 1:Xsum, 2:Ysum, 3:num
int i1, i2, i3[100], i4[100], i5[100], i6[100], i7[100], i8[100], i9[100], i10[100], i11[100], i12, i13, i14, i15, i16, i17, i18, i19;
system_clock::time_point starttime;
system_clock::time_point endtime;
string bufferToString(char* buffer, int bufflen)
{
	std::string ret(buffer, bufflen);

    	return ret;
}
int main(int argc, char *argv[]){
   
   
       	pid_t child_pid, wpid;
       	int status = 0;
   	int parentid = getpid();
   	if(pipe(filedes)==-1){
      		cout<<"fail to call pipe()"<<endl;
      		exit(1);
   	}
   	int static T; // test-case num
   	cin>>T;
   	system_clock::time_point start = system_clock::now();
   	int Tnum[T];
   	int NSum=0;
   	for (i14=0; i14<T; i14++)
      	Tnum[i14] = i14;


   	for (i1=0; i1<T; i1++){ // T cases
   	//ready for k-means clustering
      		cin>>I[i1];
      		cin >>K[i1]; 
      		cin>> N[i1];
      		for (i2=0 ; i2<N[i1] ; i2++){
         		cin >> xy[i1][i2][0] >> xy[i1][i2][1];
      		}
   	}
   	for (i16=0; i16<T; i16++){
      	NSum = NSum + N[i16];
   	}
   	for (i15 = 0; i15<T; i15++){
      	//fork
      		if ((child_pid = fork()) == 0 ){ //new child
         		if(getpid()-parentid-1==i15){
            			close(filedes[0]); // close pipe's reading line.
            			runner(i15); // k-means clustering
            			
				exit(0);
         		}
      		}
		else  {
            		wait(0);
		}
      
   	}
   	// get result from pipe and set output
   	close(filedes[1]); // close pipe's writing line 
   	int finalResult[T][100001];
   	int finalPosition[T];
   	for (i17 =0 ; i17<T; i17++){
      		finalPosition[i17] = 0;
   	}
   	for (i12 = 0; i12< NSum+T; i12++){ // T cases' output
      		nRead = read(filedes[0], buffer, BUFFER_SIZE);	
      		string tempT = bufferToString(buffer, BUFFER_SIZE);
      		nRead = read(filedes[0], buffer, BUFFER_SIZE);
      		string tempN = bufferToString(buffer, BUFFER_SIZE);
      		finalResult[stoi(tempT)-parentid -1][finalPosition[stoi(tempT)-parentid-1]] = stoi(tempN);
      		finalPosition[stoi(tempT)-parentid-1] = finalPosition[stoi(tempT)-parentid-1] + 1;
      
   	}
   

   	for (i18 = 0; i18<T; i18++){
      		cout << "Test Case #"<<i18<<endl;
      		cout<<finalResult[i18][N[i18]]<<" microseconds"<<endl;
      		for (i19 = 0; i19<N[i18]; i19++){
         		//cout<< finalResult[i18][i19]<<endl;      
      		}
   	}
   	system_clock::time_point end = system_clock::now();
   	microseconds microSec = duration_cast<microseconds>(end - start);
   	cout<<"whole time : "<< microSec.count()<<endl;   

   	return 0;   
}
void runner(int rT){
   	starttime = system_clock::now();
   	//initialize centers
   	for (i3[rT]=0; i3[rT] < K[rT]; i3[rT]++){
	      	centers[rT][i3[rT]][0] = xy[rT][i3[rT]][0];
	      	centers[rT][i3[rT]][1] = xy[rT][i3[rT]][1];
   	}
   	// repeat clustering I times
   	for (i4[rT]=0; i4[rT]<I[rT]; i4[rT]++){
   	// set xy's cluster
      
      		for (i9[rT]= 0 ; i9[rT]<K[rT]; i9[rT]++){
         		clustersum[rT][i9[rT]][0] = 0;
         		clustersum[rT][i9[rT]][1] = 0;
         		clustersum[rT][i9[rT]][2] = 0;      
      		}
      		for (i5[rT]= 0; i5[rT]<N[rT] ; i5[rT]++){ // N dots
         		for (i6[rT] = 0; i6[rT] < K[rT]; i6[rT]++) {// K clusters
            			if (i6[rT]==0){
               				dist[rT] = sqrt(pow((centers[rT][0][0] - xy[rT][i5[rT]][0]),2) + pow((centers[rT][0][1] - xy[rT][i5[rT]][1]), 2));
               				xy[rT][i5[rT]][2] = 0;
            			}
               	// distance from center to dot
            			else {
               				tempdist[rT] = sqrt(pow((centers[rT][i6[rT]][0] - xy[rT][i5[rT]][0]),2) + pow((centers[rT][i6[rT]][1] - xy[rT][i5[rT]][1]), 2));
               				if( tempdist[rT] < dist[rT]){
                  				xy[rT][i5[rT]][2] = i6[rT];
                  				dist[rT] = tempdist[rT];
               				}	
            			}
         
         		}
      		}
   		// set cluster's center
      		for (i7[rT] =0; i7[rT] < N[rT]; i7[rT]++){
         		for (i8[rT] = 0; i8[rT]<K[rT]; i8[rT]++){
            			if (xy[rT][i7[rT]][2] == i8[rT]) { // if it is in "i8"th cluster
               				clustersum[rT][i8[rT]][0] = clustersum[rT][i8[rT]][0] + xy[rT][i7[rT]][0];
               				clustersum[rT][i8[rT]][1] = clustersum[rT][i8[rT]][1] + xy[rT][i7[rT]][1];
               				clustersum[rT][i8[rT]][2] = clustersum[rT][i8[rT]][2] + 1;
            			}
         		}
      		}
      		for (i10[rT] = 0; i10[rT]<K[rT] ; i10[rT]++){
         		centers[rT][i10[rT]][0] = clustersum[rT][i10[rT]][0] / clustersum[rT][i10[rT]][2];
         		centers[rT][i10[rT]][1] = clustersum[rT][i10[rT]][1] / clustersum[rT][i10[rT]][2];

      		}
   	}
   	// save output
   	for (i11[rT]=0; i11[rT]<N[rT]+1 ; i11[rT]++){//here N dots result.
      		int temp = (int)getpid();
            	char const *pchar = to_string(temp).c_str();
            	strcpy(buffer, pchar); 
      		write(filedes[1],buffer, BUFFER_SIZE);
      		if(i11[rT]<N[rT]){
         		char const *pchar2 = to_string((int)xy[rT][i11[rT]][2]).c_str();
			
         		strcpy(buffer, pchar2);
         		write(filedes[1],buffer, BUFFER_SIZE);
      		}
      		else{
         		endtime = system_clock::now();
         		microseconds duration = duration_cast<microseconds>(endtime - starttime);
         		char const *pchar3 = to_string((int)duration.count()).c_str();
         		strcpy(buffer, pchar3);
         		write(filedes[1],buffer, BUFFER_SIZE);
      		}
		
  	}
}
