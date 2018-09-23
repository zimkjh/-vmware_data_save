#include<iostream>
using namespace std;
int main() {
	int cases ;
	cin>>cases;
   	int* reward1 = new int[101];
	int* reward2 = new int[65]; 
	reward1[0] = 0;
	reward1[1] = 500;
	reward1[2] = 300;
	reward1[3] = 300;
	reward1[4] = 200;
	reward1[5] = 200;
	reward1[6] = 200;
	reward1[7] = 50;
	reward1[8] = 50;
	reward1[9] = 50;
	reward1[10] = 50;
	reward1[11] = 30;
	reward1[12] = 30;
	reward1[13] = 30;
	reward1[14] = 30;
	reward1[15] = 30;
	reward1[16] = 10;
	reward1[17] = 10;
	reward1[18] = 10;
	reward1[19] = 10;
	reward1[20] = 10;
	reward1[21] = 10;
	for (int i2 = 22 ; i2<101 ; i2++){
		reward1[i2] = 0;
	}
	reward2[0] = 0;
	reward2[1] = 512;
	reward2[2] = 256;
	reward2[3] = 256;
	reward2[4] = 128;
	reward2[5] = 128;
	reward2[6] = 128;
	reward2[7] = 128;
	reward2[8] = 64;
	reward2[9] = 64;
	reward2[10] = 64;
	reward2[11] = 64;
	reward2[12] = 64;
	reward2[13] = 64;
	reward2[14] = 64;
	reward2[15] = 64;
	reward2[16] = 32;
	reward2[17] = 32;
	reward2[18] = 32;
	reward2[19] = 32;
	reward2[20] = 32;
	reward2[21] = 32;
	reward2[22] = 32;
	reward2[23] = 32;
	reward2[24] = 32;
	reward2[25] = 32;
	reward2[26] = 32;
	reward2[27] = 32;
	reward2[28] = 32;
	reward2[29] = 32;
	reward2[30] = 32;
	reward2[31] = 32;
	for (int i3 = 32 ; i3<65 ; i3++){
		reward1[i3] = 0;
	}
	for(int i=0; i<cases ; i++){
		int round1, round2;
		cin >> round1 >>round2;
		cout<< (reward1[round1] + reward2[round2])*10000<<endl;
	}}
