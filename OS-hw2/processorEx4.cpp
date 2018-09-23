#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sstream>
#include <climits>

using namespace std;
int main(void)
{
    	pid_t child_pid, wpid;
    	int status = 0;
    	int i;
    	int a[4] = {1, 2, 1,0};
	


    	cout<<"parent_pid = "<< getpid()<<endl;
    	for (i = 0; i < 4; i++)
    	{
		cout<<"i = "<<i<<endl;
		if ((child_pid = fork()) == 0)
		{
		    	cout<<"In child process (pid = "<< getpid()<<")"<<endl;
		    	if (a[i] < 2)
		    	{
		        	cout<<"Should be accept"<<endl;
		        	exit(1);
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
    	return 0;
}
