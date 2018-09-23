#include <iostream>
#include <pthread.h>
#include <cmath>
#include <stdio.h>
#include <chrono>
using namespace std;
using namespace chrono;

void *runner(void *param);
int saveOutput[100][100000];
int I[100]; //algorithm repeat num
int K[100]; // cluster num
int N[100];// dots num
float xy[100][100000][3]; // 0:T, 1:x, 2:y, 3:cluster
float centers[100][10000][2]; // 0:T, 1:x, 2:y
float dist[100], tempdist[100];
float clustersum[100][10000][3]; // 0:T, 1:Xsum, 2:Ysum, 3:num
int i1, i2, i3[100], i4[100], i5[100], i6[100], i7[100], i8[100], i9[100], i10[100], i11[100], i12, i13, i14;
int timeduration[100];
system_clock::time_point starttime[100];
system_clock::time_point endtime[100];
int main(int argc, char *argv[]){
	
	int T; // test-case num
	cin>>T;
	system_clock::time_point start = system_clock::now();
	int saveN[T];
	pthread_t threads[T];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int Tnum[T];
	for (i14=0; i14<T; i14++)
		Tnum[i14] = i14;


	for (i1=0; i1<T; i1++){ // T cases
	//ready for k-means clustering
		cin>>I[i1];
		cin >>K[i1]; 
		cin>> N[i1];
		saveN[i1] = N[i1];
		for (i2=0 ; i2<N[i1] ; i2++){
			cin >> xy[i1][i2][0] >> xy[i1][i2][1];
		}
		//make thread 
		pthread_create(&threads[i1], &attr, runner, (void*)&Tnum[i1] );
		pthread_join(threads[i1], NULL);
	}
	// output
	for (i12 = 0; i12< T; i12++){ // T cases' output
		cout << "Test Case #"<<i12<<endl;
		cout<< timeduration[i12]<<" microseconds"<<endl;
		for (i13 = 0; i13<saveN[i12]; i13++){
			cout<< saveOutput[i12][i13]<<endl;		
		}
	}

	system_clock::time_point end = system_clock::now();
	microseconds microSec = duration_cast<microseconds>(end - start);
	//cout<<"total time : "<<microSec.count()<<endl;	
	return 0;	
}
void *runner(void *param){
	int rT = *((int *)param); //rT : real T number
	starttime[rT] = system_clock::now();
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
	for (i11[rT]=0; i11[rT]<N[rT] ; i11[rT]++){//here N dots result.
		saveOutput[rT][i11[rT]] = xy[rT][i11[rT]][2];
	}


	endtime[rT] = system_clock::now();
	microseconds duration = duration_cast<microseconds>(endtime[rT] - starttime[rT]);
	timeduration[rT] = duration.count();
	pthread_exit(0);


}

