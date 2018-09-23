#include<iostream>
#include <cmath>
using namespace std;
int main() {
	int num, size;
	cin>> num >> size;
	int* numbers = new int[num];
	
	for(int i1 = 0 ;i1<num ; i1++){
		cin>>numbers[i1];
	}
	cout.precision(12);
	double* stds = new double[num-size+1];
	//double* avgs = new double[num-size+1];
	//for(int i6=0; i6<num-size+1 ; i6++){
		
	for(int i2 = 0; i2<num-size+1 ; i2++){
		double variance;
		double sum=0;
		double avgsum=0;
		double avg ;	
		long double std;
		for(int i3 = 0; i3<size ; i3++){ // calculate average - avgsum
			avgsum += numbers[i2+i3];
		}
		avg = avgsum / size;
		for(int i4 = 0; i4<size ; i4++){
			sum+= pow(numbers[i2+i4]-avg,2);  
		}
		variance = sum/size; //calculate variance
		std= sqrt(variance);
		stds[i2] = std;
	}
	long double min = stds[0];
	for(int i5 = 0; i5<num-size+1 ; i5++){
		cout<<stds[i5]<<endl;
		if(min>stds[i5]){
            		min=stds[i5];
		}
	}
	cout<<min<<endl;      		
	
}
