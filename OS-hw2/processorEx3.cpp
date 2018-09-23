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
static int *glob_var;
int main(void)
{
    	pid_t child_pid, wpid;
    	int status = 0;
    	int i;
    	int a[4] = {1, 2, 1,0};
	int segment_id;
	char *share_memory;
	const int size = 4096;
	
	segment_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);
	share_memory = (char *)shmat(segment_id, NULL, 0);
	char test[10];
	sprintf(test, "%s%s", "hehehe","ho!"); 
	cout<<test<<endl;
	sprintf(share_memory, "%s",test);
	cout<<share_memory<<endl;


    	cout<<"parent_pid = "<< getpid()<<endl;
    	for (i = 0; i < 4; i++)
    	{
		cout<<"i = "<<i<<endl;
		if ((child_pid = fork()) == 0)
		{
		    	cout<<"In child process (pid = "<< getpid()<<endl;
			
			//char ntoc[(((sizeof i) * CHAR_BIT) + 2) / 3 + 2];
			char ntoc[2];			
			sprintf(ntoc, "%d%d", i, i);
			cout<<"ntoc herer!!!!!!"<<ntoc<<endl;
			//cout<<sizeof(share_memory+i)<<endl;
			//cout<<sizeof(char)<<endl;
			sprintf(share_memory+i*2,"%s", ntoc);
			//sprintf(share_memory+i, ",");
			//cout<<getpid()<<endl;
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
	//cout<<"["<<getpid()<<"]"<<share_memory<<" "<<"["<<getpid()<<"]"<<share_memory+1<<" "<<"["<<getpid()<<"]"<<share_memory+2<<" "<<"["<<getpid()<<"]"<<share_memory+3<<endl;
	cout<<share_memory<<endl;
	//cout<<*(share_memory+0)<<" "<<*(share_memory+2)<<" "<<*(share_memory+4)<<" "<<*(share_memory+6)<<endl;	
	//cout<<*(share_memory+6)<<endl;	
	shmdt(share_memory);
	shmctl(segment_id, IPC_RMID, NULL);
    	return 0;
}
