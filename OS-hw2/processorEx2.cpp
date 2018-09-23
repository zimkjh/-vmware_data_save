#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main(int argc, char *argv[]){
	pid_t pids[4], pid;
	int runProcess = 0; 
	int state;
	long firstplace = (long)getpid();
	while(runProcess < 4 && (long)getpid()<= firstplace+4) { 
		wait(NULL);
		//cout<<"herer"<<pids[0]<<endl;
		//cout<<pids[runProcess]<<endl;
		pids[runProcess] = fork();
		//cout<<pids[runProcess]<<endl;
		cout<<"[nowplace: "<< (long)getpid()<<"] "<<runProcess<<endl;
		if(pids[runProcess] < 0) {
			return -1;
		} else if(pids[runProcess] == 0 && (long)getpid()<= firstplace+4) { //use when process is created.
			cout<<"[nowplace: "<< (long)getpid()<<"] "<<("child")<<(long)getpid()<<endl;
			
			sleep(10);
		} else if((long)getpid()<= firstplace+4){
			
			//cout<<"hey!"<<endl;			
			cout<<"[nowplace: "<< (long)getpid()<<"] "<<"parent "<<(long)getpid()<<","<<"child "<<(long)pids[runProcess]<<endl;
		}
		
		if((long)getpid()<= firstplace+4){
			runProcess+=1;
			cout<<"[nowplace: "<< (long)getpid()<<"] "<<pids[0]<<" "<<pids[1]<<" "<<pids[2]<<" "<<pids[3]<<endl;
		}
	}
	return 0;
}
