#include <iostream>
#include <climits>
#include <stdio.h>
using namespace std;

int main(){
	int n = 1234;
	char ntoc[(((sizeof n) * CHAR_BIT) + 2) / 3 + 2];

	sprintf(ntoc, "%d", n);
	cout<<ntoc<<endl;	
	return 0;	
}
