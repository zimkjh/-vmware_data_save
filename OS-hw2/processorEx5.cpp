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
	const char *msg[] ={"i...","love..","bum..!","sexy hehe"};
	char buffer[BUFFER_SIZE];
	int filedes[2], nRead, bi;
    	pid_t child_pid, wpid;
    	int status = 0;
    	int i;
    	int a[4] = {1, 2, 1,0};
	int parentid = getpid();
	
	if(pipe(filedes)==-1){
		cout<<"fail to call pipe()"<<endl;
		exit(1);
	}


    	cout<<"parent_pid = "<< getpid()<<endl;
	
    	for (i = 0; i < 4; i++)
    	{
		
		if ((child_pid = fork()) == 0) // new child
		{	
			close(filedes[0]);
		    	cout<<"In child process (pid = "<< getpid()<<")"<<endl;
		    	if (a[i] < 2)
		    	{
				
		        	cout<<"Should be accept"<<endl;
				
				strcpy(buffer, msg[i]);
				write(filedes[1],buffer, BUFFER_SIZE);
		        	exit(0);
		    	}
		    	else
		    	{
		        	cout<<"Should be reject"<<endl;
		        	exit(0);
		    	}
			
		    	/*NOTREACHED*/
		}
    	}

    	while ((wpid = wait(&status)) > 0)
    	{
		cout<<"Exit status of "<<(int)wpid<<" was "<<status;
		if(status > 0) 
			cout<<"accept"<<endl;
		else
			cout<<"reject"<<endl;
    	}

	if(parentid == getpid()){
		cout<<"parent!!"<<endl;
		close(filedes[1]);	
		for (i=0; i<3; i++){
			nRead = read(filedes[0], buffer, BUFFER_SIZE);
			cout<<"[child] "<<buffer<<endl;
		}
	}
    	return 0;
}
