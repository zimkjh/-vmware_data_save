#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc, char *argv[]){
	pid_t pid;
	cout<<pid<<endl;
	pid = fork(); 
	cout<<pid<<endl;
	if(pid == -1) { 
		cout<<("can't fork, erro")<<endl;
	}

	if(pid == 0) {
		int j;	
		for(j = 0; j < 10; j++) {
			cout<<("child: ")<<( j)<<endl;
			sleep(1);
		}
	} else { 
		int i;
		for(i = 0; i < 10; i++) {
			cout<<("parent: ")<< (i)<<endl;
			sleep(1);
		}
	}
	return 0;	
}
