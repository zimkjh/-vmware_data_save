#include <iostream>
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
#define BUFFER_SIZE 512

using namespace std;
int main(void)
{
	const char *msg[] ={"i...","love..","bum..!"};
	char buffer[BUFFER_SIZE];
	int filedes[2], nRead, i;
	pid_t pid;
	
	if(pipe(filedes)==-1){
		cout<<"fail to call pipe()"<<endl;
		exit(1);
	}
	if((pid = fork())==-1){
		cout<<"fail to call fork()"<<endl;
		exit(1);
	}
	//parent process
	if(pid>0){
		close(filedes[0]);
		for (i=0; i<3;i++){
			//strcpy(buffer, to_string((int)getpid()));
			int test = (int)getpid();
			char const *pchar = to_string(test).c_str();
			strcpy(buffer, pchar); 
			write(filedes[1],buffer, BUFFER_SIZE);
			strcpy(buffer, msg[i]);
			write(filedes[1],buffer, BUFFER_SIZE);
		}
	}
	// child process
	else if(pid==0){
		close(filedes[1]);
		for (i=2; i<6; i++){
			nRead = read(filedes[0], buffer, BUFFER_SIZE);
			if(buffer== pchar)
				cout<<"3333"<<endl;
			cout<<"[child] "<<buffer<<endl;
		}
	}    	
    	return 0;
}
