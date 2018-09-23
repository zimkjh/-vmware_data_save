# include <iostream>

using namespace std;
int fixsize(int size){
	int temp1 = 1;
	while (size > temp1){
		temp1 *= 2;
	}
	return temp1;
}
int main(){
	cout<<fixsize(38)<<endl;
}
