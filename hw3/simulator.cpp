# include <iostream>
# include <fstream>
# include <queue>
# include <list>
using namespace std;
int num, timeqt, vmsize, pmsize, pagesize, bonusqt, bonustime;

class vm{
	public : 
		int aId;
		int valid;
};
class pt{
	public:

		vm *L;
		pt(int tempsize){
			L = new vm[tempsize];
		} 
};
vm **ptL;
class task{
	public:
		int opcode;
		int argument;
		int line;
};
class event{
	public:
		int time;
		string code;
		int ioPid;
		int pid;
		int leftTime;
		list <task> leftTask;
		int wakeUpTime;
		int nowaId;
		int vmaId;
		int vmvalid;
};
list <event> runL;
list <event> sleepL;
list <event> ioL;
bool byLRU = false;
bool byAccess = false;
bool onlyVM ;
FILE* mem = NULL;
class ins{ // instructions
	public:
		int pid;
		int aId;
		int argument;
};
class pm{
	public:
		int blockSize;
		int blockIndex;
		int pid;
		int aId;
		bool empty;
		string bI; // binary Index
};
class buddy{
	public:
		int size;
		pm *pmL;
		
		int pmLSize;
		buddy(int temp){
			pm tempP1;
			tempP1.blockSize = temp;
			tempP1.blockIndex = 0;
			tempP1.bI = "0";
			pmL = new pm [temp];
			pmL[0] = tempP1;
			size = temp;	
			pmLSize = 1;
		}
		list <ins> LRU;
};

buddy buddy_rel(buddy B, ins E , int vmSize, bool byLRU, int nowCycle, bool onlyVM){
	if(!onlyVM){
		fprintf(mem, "%d\t%d\t%d\t2\n", nowCycle, E.pid, E.aId);
	}
	//release by aId
	int releaseIndex = -1;
	for(int i14 = 0; i14 < B.pmLSize; i14++){
		if(B.pmL[i14].aId == E.aId && B.pmL[i14].pid == E.pid){
			releaseIndex = B.pmL[i14].blockIndex;
		}
	}
	//release that block if it exist.
	if(releaseIndex >= 0){
		B.pmL[releaseIndex].aId = -1;
		B.pmL[releaseIndex].pid = -1;
		B.pmL[releaseIndex].empty = true;
		while(releaseIndex >=0){
			bool isSummed = false;
			if(B.pmLSize > 1){
				if(releaseIndex == 0){// the left most is released
					if(B.pmL[releaseIndex].blockSize == B.pmL[releaseIndex+1].blockSize && B.pmL[1].empty == true){
						B.pmL[0].bI = B.pmL[0].bI.substr(0, B.pmL[0].bI.length()-1);
						B.pmL[0].blockSize = B.pmL[0].blockSize * 2;
						for(int i15= 1 ; i15 < B.pmLSize-1;  i15++){
							B.pmL[i15] = B.pmL[i15+1];
							B.pmL[i15].blockIndex-=1;
						}	
						B.pmLSize -= 1;
						isSummed = true;
					}
				}
				else if(releaseIndex == B.pmLSize-1){ // the right most is released
					if(B.pmL[releaseIndex].blockSize == B.pmL[releaseIndex - 1].blockSize && B.pmL[releaseIndex - 1].empty == true){
						B.pmL[releaseIndex-1].bI = B.pmL[releaseIndex-1].bI.substr(0, B.pmL[releaseIndex-1].bI.length() -1);
						B.pmL[releaseIndex-1].blockSize = B.pmL[releaseIndex-1].blockSize * 2;
						B.pmLSize -= 1;
						isSummed = true;
					}
				}
				else { // if is in the middle
					if(B.pmL[releaseIndex].blockSize == B.pmL[releaseIndex - 1].blockSize && B.pmL[releaseIndex -1].empty == true && B.pmL[releaseIndex].bI.at(B.pmL[releaseIndex].bI.length()-2) == B.pmL[releaseIndex-1].bI.at(B.pmL[releaseIndex-1].bI.length()-2)){//if left side block size is same
						for(int i16= releaseIndex  ; i16 < B.pmLSize-1;  i16++){
							B.pmL[i16] = B.pmL[i16+1];
							B.pmL[i16].blockIndex-=1;
						}
						B.pmL[releaseIndex-1].bI = B.pmL[releaseIndex-1].bI.substr(0, B.pmL[releaseIndex-1].bI.length() -1);
						B.pmL[releaseIndex-1].blockSize = B.pmL[releaseIndex-1].blockSize * 2;
						B.pmLSize -= 1;
						isSummed = true;
					}					
					if(B.pmL[releaseIndex].blockSize == B.pmL[releaseIndex + 1].blockSize && B.pmL[releaseIndex +1].empty == true && B.pmL[releaseIndex].bI.at(B.pmL[releaseIndex].bI.length()-2) == B.pmL[releaseIndex+1].bI.at(B.pmL[releaseIndex+1].bI.length()-2)){ // if right side block size is same
						for(int i17= releaseIndex + 1 ; i17 < B.pmLSize-1;  i17++){
							B.pmL[i17] = B.pmL[i17+1];
							B.pmL[i17].blockIndex-=1;
						}
						B.pmL[releaseIndex].bI = B.pmL[releaseIndex].bI.substr(0, B.pmL[releaseIndex].bI.length() -1);
						B.pmL[releaseIndex].blockSize = B.pmL[releaseIndex].blockSize * 2;
						B.pmLSize -= 1;
						isSummed = true;
					}
				}
			}
			if(isSummed == false){
				break;
			}
			//if there was one block allocated, there is nothing to do.
		}
	}
	for(int i27 = 0; i27 < vmSize; i27++){
		if(ptL[E.pid][i27].aId == E.aId){
			ptL[E.pid][i27].valid = 0;
			if(byLRU == false){
				
				ptL[E.pid][i27].aId = -1;
			}
			
		}
	}
	return B;
}

buddy buddy_all(buddy B, ins E ,int vmSize, bool byAccess, int nowCycle){

	//choose which block to go.
	int toGoI = 0;
	bool noLeftSpace = false;
	bool placeToGo = false;
	while(placeToGo == false){
		toGoI = 0;
		while(toGoI >= 0){ // choose there is possible block to go
			if(B.pmL[toGoI].empty == true && E.argument <= B.pmL[toGoI].blockSize){
				placeToGo = true;
				break;
			}
			else{
				toGoI += 1;
			}
			if(toGoI >= B.pmLSize){
				noLeftSpace = true;
				break;
			}
		}
		if(placeToGo == true){
			break;
		}
		if(noLeftSpace){ //we have to release some of them using LRU
			byLRU = true;
			ins tempI3;
			tempI3.pid = B.LRU.front().pid;
			tempI3.aId = B.LRU.front().aId;
			B = buddy_rel(B, tempI3, vmsize/pagesize, byLRU, nowCycle, onlyVM);
			B.LRU.pop_front();
		}
	}
	for(int i9 = toGoI + 1; i9<B.pmLSize ; i9++){
		if(E.argument <= B.pmL[i9].blockSize && B.pmL[toGoI].blockSize > B.pmL[i9].blockSize && B.pmL[i9].empty == true){ 
			//if this block is more better,
			toGoI = B.pmL[i9].blockIndex;
		}
	}
	//slice that block until it is smaller than ins.argument
	while ( E.argument <= B.pmL[toGoI].blockSize){
		if(E.argument > B.pmL[toGoI].blockSize / 2){// put in this block
			
			B.pmL[toGoI].aId = E.aId;
			B.pmL[toGoI].pid = E.pid;
			B.pmL[toGoI].empty = false;
			break; 
		}
		else{//split!
			for (int i10 = B.pmLSize-1 ; i10>toGoI ; i10--){
				B.pmL[i10+1] = B.pmL[i10];
				B.pmL[i10+1].blockIndex += 1;
			}
			pm tempP2;
			tempP2.blockSize = B.pmL[toGoI].blockSize / 2;
			tempP2.blockIndex = toGoI + 1;
			tempP2.aId = -1;
			tempP2.pid = -1;
			tempP2.bI = B.pmL[toGoI].bI + "1";
			tempP2.empty = true;
			B.pmL[toGoI+1] = tempP2;
			tempP2.blockIndex = toGoI;
			tempP2.bI = B.pmL[toGoI].bI + "0";
			B.pmL[toGoI] = tempP2;
			B.pmLSize += 1;
		}
	}

	int ptLeft = 0;
	int i26 = 0;
	while(i26 < vmSize){
		if(ptL[E.pid][i26].aId < 0 && ptL[E.pid][i26].valid == 0){ //here is empty space!
			ptLeft = i26;
			break;
		}
		i26++;
	}
	if(!byAccess){
		for(int i27 = 0; i27 < E.argument; i27++){
		
			ptL[E.pid][ptLeft+i27].aId = E.aId;
			ptL[E.pid][ptLeft+i27].valid = 1;
		}
	}
	if(byAccess){
		for(int i33 = 0; i33 < vmSize; i33++){
			if(ptL[E.pid][i33].aId == E.aId){
				ptL[E.pid][i33].valid = 1;
			}
		}
	}
	fprintf(mem, "%d\t%d\t%d\t0\n", nowCycle, E.pid, E.aId);
	return B;
}
int main(int argc, char *argv[]){
	queue<event> eventQ;
	event tempEvent;
	// read in.txt
	FILE* in = NULL;
	in = fopen(argv[1], "r");
	fscanf(in, "%d\t%d\t%d\t%d\t%d\t%d\t%d\n", &num, &timeqt, &vmsize, &pmsize, &pagesize, &bonusqt, &bonustime);
	int processNum = num;
	// push all events in eventQ from input.txt
	for(int i2 = 0; i2<num; i2++){
		
		int time;
		char code[100];
		//fscanf(
		fscanf(in, "%d", &time);
		tempEvent.time = time;
		fscanf(in, "%s", code);
		tempEvent.code = string(code);
		if(string(code) == "INPUT"){ // IO interrupt
			int ioPid;
			fscanf(in, "%d", &ioPid);
			tempEvent.ioPid = ioPid;
			processNum-= 1;
		}
		tempEvent.nowaId = 0;
		eventQ.push(tempEvent);
	
	}
	ptL = new vm *[processNum]; // page table. 
	for(int i29 = 0; i29 < processNum; i29++){
    		ptL[i29] = new vm[vmsize/pagesize];
		for(int i30 = 0; i30 < vmsize/pagesize ; i30++){
			ptL[i29][i30].aId = -1;
			ptL[i29][i30].valid = 0;
		}
	}	
	buddy B(pmsize / pagesize);
	for(int i24 = 0; i24 < processNum; i24++){
		for(int i25 = 0 ;i25 < vmsize/pagesize ; i25++){
			ptL[i24][i25].aId = -1;
			ptL[i24][i25].valid = 0;
		}
	}
	for (int i20 = 0 ; i20 < 16 ; i20++){
		B.pmL[i20].aId = -1; // aId is -1 if there is any process allocated.
		B.pmL[i20].pid = -1;
		B.pmL[i20].empty = true;
	}

	
	// time cycle start
	int eventNum = 0;
	int nowCycle = 0;
	int nowPid = 0;
	int nowTimeQt = 0; // we have max time quantum
	int modifyCycle = 0;

	FILE* pout[processNum];
	for(int i24 = 0 ; i24 < processNum; i24++){
		pout[i24] = NULL;
		string tempfile = to_string(i24) + "_ham.txt";
		pout[i24] = fopen(tempfile.c_str(),"w");
	}	
	FILE* out = NULL;
	out = fopen("system_ex_ham.txt", "w");
	mem = fopen("memory_ex_ham.txt", "w");
	FILE* sche = NULL;
	sche = fopen("scheduler_ex_ham.txt", "w");
	while(nowCycle >= 0){
		onlyVM = false;
		// cycle distribution
		if((nowCycle-modifyCycle) % bonusqt == 0){	
			list<event>::iterator iter2;
			iter2 = runL.begin();
			for (iter2 = runL.begin(); iter2!=runL.end();iter2++){
				iter2->leftTime += bonustime;
			}
			for (iter2= sleepL.begin(); iter2!=sleepL.end();iter2++){
				iter2->leftTime += bonustime;
			}
			for (iter2= ioL.begin(); iter2!=ioL.end();iter2++){	
				iter2->leftTime += bonustime;
			}
		}
		// check sleepL
		if(!sleepL.empty()){
			int tempSleepLSize = int(sleepL.size());
			for (int i4 = 0; i4 < tempSleepLSize ; i4++){
				event tempE5;
				tempE5 = sleepL.front();
				sleepL.pop_front();
				if(tempE5.wakeUpTime == nowCycle) { // it's time to wake up!
					runL.push_back(tempE5); //go to runL
				}
				else{
					sleepL.push_back(tempE5); //go to sleepL again
				}
			}
		}
		// input - IO 
		while(!eventQ.empty()&& eventQ.front().time == nowCycle){// pass if evenQ is empty
			if(eventQ.front().code == "INPUT"){ // is IO
				eventNum += 1;
				int thisPid = eventQ.front().ioPid;
				event tempE2, tempE4;
				// find process from ioList.
				int tempIoLSize = int(ioL.size());
				for (int i3= 0; i3 < tempIoLSize; i3++){
					if(ioL.front().pid == thisPid){ // go to runL from ioL
						tempE2 = ioL.front();
						ioL.pop_front();
						runL.push_back(tempE2);
					}
					else{
						tempE4 = ioL.front();
						ioL.pop_front();
						ioL.push_back(tempE4);
					}
				}
				eventQ.pop();
			}
			else{
				break;
			}
		}
		// input - Process 
		while(!eventQ.empty()&& eventQ.front().time == nowCycle){ // pass if evenQ is empty
			if(eventQ.front().code != "INPUT"){ //NOT INPUT
				eventNum += 1;
				event tempEvent;
				tempEvent = eventQ.front();
				eventQ.pop();
				tempEvent.pid = nowPid; // set pid
				// set tasks
				FILE* codeFile = NULL;
				codeFile = fopen(tempEvent.code.c_str(), "r");
				int codeNum;
				int tempOpcode;
				int tempArgument;
				fscanf(codeFile, "%d\n", &codeNum);
				for(int i1 = 0; i1<codeNum; i1++){
					fscanf(codeFile,"%d\t%d\n",&tempOpcode,&tempArgument);
					task tempTask;
					tempTask.opcode = tempOpcode;
					tempTask.argument = tempArgument;
					tempTask.line = i1+1;
					tempEvent.leftTask.push_back(tempTask);
				}
				tempEvent.leftTime = bonustime;
				runL.push_back(tempEvent);
				nowPid += 1;
			}
			else{
				break;
			}
		}
		// choose process from scheduler if none process is running ( check there is time quantum left)
		if(!runL.empty()){ // if runQueue is empty, pass.
		


			list<event>::iterator iter6;
			iter6 = runL.begin();
			bool allEmpty = true;
			for (iter6 = runL.begin() ; iter6 != runL.end(); iter6++){
				if(iter6->leftTime >0){
					allEmpty = false;
				}
			}
			if(allEmpty==true){//if there is no process that has leftTime(in runL)
				list<event>::iterator iter7;
				iter7 = runL.begin();
				for (iter7 = runL.begin() ; iter7 != runL.end(); iter7++){
					iter7-> leftTime += bonustime;
				}
			}
			if(runL.front().leftTime == 0){ // if the first process don't have leftTime
				while(true){
					event tempE3;
					tempE3 = runL.front();
					runL.pop_front();
					runL.push_back(tempE3);
					if(runL.front().leftTime != 0){//we found appropriate process
						break;
					}
				}
			}
		}
		list<event>::iterator iter4; //check for time quantum
		iter4 = runL.begin();
		bool noLeftTime = true;
		for (iter4 = runL.begin(); iter4!=runL.end();iter4++){
			if(iter4->leftTime > 0){ //if there is any process that have leftTime, True
				noLeftTime = false;
			}
		}
		list<event>::iterator iter5;
		iter5 = runL.begin();
		if(noLeftTime&&runL.size()>0){ //it there is no left time quantum
			for (iter5 = runL.begin(); iter5!=runL.end();iter5++){
				iter5->leftTime += bonustime;
				modifyCycle = nowCycle % bonusqt;
			}
		}


		// print system.txt 
			//i want to print!
		if(eventNum == num && runL.empty() && sleepL.empty() && ioL.empty()){// event all done and lists are all empty
			break;	
		}
		//cout<<nowCycle<<" Cycle : ";
		fprintf(out, "%d Cycle: ", nowCycle);
		if(runL.size()>0){ // if something is running
			//cout<<"Process#"<<runL.front().pid<<" runing code "<<runL.front().code<<" line "<<runL.front().leftTask.front().line<<"(op "<< runL.front().leftTask.front().opcode<<", arg "<<runL.front().leftTask.front().argument<<")";
			fprintf(out,"Process#%d running code %s line %d(op %d, arg %d)\n", runL.front().pid, runL.front().code.c_str(), runL.front().leftTask.front().line, runL.front().leftTask.front().opcode, runL.front().leftTask.front().argument);
		}
		else{
			fprintf(out,"\n");
		}
		//cout<<endl;
		//cout<<"runQ : ";
		fprintf(out,"RunQueue: ");

		if(runL.size() <=1){
			//cout<<"Empty";
			fprintf(out, "Empty");
		}
		list<event>::iterator iter1;
		iter1 = runL.begin();
		
		for (++iter1 ; iter1!=runL.end();iter1++){
			//cout<<iter1->pid<<"("<<iter1->code<<") ";
			fprintf(out,"%d(%s) ", iter1->pid, iter1->code.c_str());
		}
		//cout<<endl;
		//cout<<"sleepL : "	;
		fprintf(out,"\nSleepList: ");
		if(sleepL.size() ==0){
			//cout<<"Empty";
			fprintf(out,"Empty");
		}
		for (iter1 = sleepL.begin(); iter1!=sleepL.end();iter1++){
			//cout<<iter1->pid<<"("<<iter1->code<<") ";
			fprintf(out, "%d(%s) ", iter1->pid, iter1->code.c_str());
		}
		//cout<<endl;
		//cout<<"ioL : " ;
		fprintf(out,"\nIOWait List: ");
		if(ioL.size() ==0){
			//cout<<"Empty";
			fprintf(out, "Empty");
		}
		for (iter1 = ioL.begin(); iter1!=ioL.end();iter1++){
			//cout<<iter1->pid<<"("<<iter1->code<<") ";
			fprintf(out, "%d(%s) ", iter1->pid, iter1->code.c_str());
		}
		//cout<<endl<<"|";
		fprintf(out, "\n|");
		for(int i18 = 0 ; i18 < B.pmLSize ; i18++){
			for(int i19 = 0; i19 < B.pmL[i18].blockSize ; i19++){
				if(B.pmL[i18].aId< 0 ){
					if(i19 == B.pmL[i18].blockSize -1){
						//cout<<"---";
						fprintf(out, "---");
					}
					else{
						//cout<<"---"<<" ";
						fprintf(out, "--- ");
					}
				}
				else{
					if(i19 == B.pmL[i18].blockSize -1){
						//cout<<B.pmL[i18].pid<<"#"<<B.pmL[i18].aId;
						fprintf(out, "%d#%d", B.pmL[i18].pid, B.pmL[i18].aId);
					}
					else{
						//cout<<B.pmL[i18].pid<<"#"<<B.pmL[i18].aId<<" ";
						fprintf(out, "%d#%d ", B.pmL[i18].pid, B.pmL[i18].aId);
					}
				}
			}
			//cout<<"|";
			fprintf(out, "|");
		}
		//cout<<endl<<"LRU: ";
		fprintf(out, "\nLRU:");
		
		list<ins>::iterator iter8;
		iter8 = B.LRU.begin();
		for (iter8 = B.LRU.begin(); iter8!=B.LRU.end();iter8++){
			//cout<<"("<<iter8->pid<<":"<<iter8->aId<<") ";
			fprintf(out, " (%d:%d)", iter8->pid, iter8->aId);
		}
		//cout<<endl;
		fprintf(out, "\n\n");
		
		if((nowTimeQt == 0 && !runL.empty()) ){
			fprintf(sche, "%d\t%d\t%s\n", nowCycle, runL.front().pid, runL.front().code.c_str());
		}
		// do process instruction
		bool lastTask = false;
		if(!runL.empty()){ // if runQueue is empty, pass.
			event tempE;
			list<task> tempT;
			tempE = runL.front();
			tempT = tempE.leftTask;
			task nowT = tempT.front();
			if(nowT.opcode == 5){ // now task will go to IO List
				nowTimeQt = -1;
				tempT.pop_front();
				if (tempT.empty()){//if iowait was last task..!
					nowTimeQt = 0; ///this was placed new.
					runL.pop_front();
					lastTask = true;
				}
				else{
					tempE.leftTask = tempT;
					tempE.leftTime -= 1;
					ioL.push_back(tempE);
					runL.pop_front();
				}
			}
			else if(nowT.opcode == 4){ // now task will go to sleepL
				
				nowTimeQt = -1;
				tempT.pop_front();
				if(tempT.empty()){ // if sleep was last task..!
					nowTimeQt = 0;
					runL.pop_front();
					lastTask = true;
					
				}
				else{
					tempE.leftTask = tempT;
					tempE.leftTime -= 1;
					tempE.wakeUpTime = nowCycle+nowT.argument;//set wakeUpTime;
					sleepL.push_back(tempE);
					runL.pop_front();
				}
			}
			else if(nowT.opcode == 3){// non-memory instruction
				tempT.pop_front();
				if(tempT.empty()){ // if it was last task..!
					lastTask = true;
				}
				tempE.leftTask = tempT;
				runL.pop_front();
				tempE.leftTime -= 1;
				runL.push_front(tempE);
			}
			else if(nowT.opcode == 0){// memory allocation
				byAccess = false;
				tempT.pop_front();
				if(tempT.empty()){ // if it was last task..!
					lastTask = true;
					
				}
				tempE.leftTask = tempT;
				runL.pop_front();
				tempE.leftTime -= 1;
				tempE.nowaId += 1;
				runL.push_front(tempE);

				ins tempI1; // we allocate process by pid and aid
				tempI1.pid = tempE.pid;
				tempI1.aId = tempE.nowaId-1;
				tempI1.argument = nowT.argument;
				B= buddy_all(B, tempI1 ,vmsize/pagesize, byAccess, nowCycle);
				B.LRU.push_back(tempI1);
				
			}
			else if(nowT.opcode == 1){// memory access
		
				byAccess = true;
				// nowT.argument : aId..
				tempT.pop_front();
				if(tempT.empty()){ // if it was last task..!
					lastTask = true;
				}
				tempE.leftTask = tempT;
				runL.pop_front();
				tempE.leftTime -= 1;
				runL.push_front(tempE);
					
				bool alreadyM = false;

				for(int i22 = 0; i22 < B.pmLSize; i22++){
					if(B.pmL[i22].aId == nowT.argument && B.pmL[i22].pid == tempE.pid){ 
						alreadyM = true;
					}
				}
	
				if(alreadyM ){//if there is memory in pm
					ins tempI5; //change sequence of LRU
					int tempLRUSize1 = int(B.LRU.size());
					for(int i23 = 0; i23<tempLRUSize1 ; i23++){
						if(B.LRU.front().aId == nowT.argument && B.LRU.front().pid == tempE.pid){
							tempI5 = B.LRU.front();
							B.LRU.pop_front();
						}
						else{		
							ins tempI6;
							tempI6 = B.LRU.front();
							B.LRU.pop_front();
							B.LRU.push_back(tempI6);
						}
					}
					B.LRU.push_back(tempI5);
				}
				else if(!alreadyM ){	// if there is no memory in pm
					byAccess = true;
					ins tempI4;
					tempI4.pid = tempE.pid;
					tempI4.aId = nowT.argument;
					int tempA1 = 0;
					for(int i32 = 0 ; i32 < vmsize/pagesize; i32++){
						if(ptL[tempE.pid][i32].aId == nowT.argument){
							tempA1 += 1;
						}
					}
					tempI4.argument = tempA1;
					B = buddy_all(B, tempI4, vmsize/pagesize, byAccess, nowCycle);
					
					B.LRU.push_back(tempI4);

				}
				fprintf(mem, "%d\t%d\t%d\t1\n", nowCycle, tempE.pid, nowT.argument);

			}
			else if(nowT.opcode == 2){// memory release
		
				byLRU = false;
				tempT.pop_front();
				if(tempT.empty()){ // if it was last task..!
					lastTask = true;
					
				}
				tempE.leftTask = tempT;
				runL.pop_front();
				tempE.leftTime -= 1;
				runL.push_front(tempE);
				ins tempI2; // we release process by pid and aid
				tempI2.pid = tempE.pid;
				tempI2.aId = nowT.argument;
				B = buddy_rel(B, tempI2, vmsize/pagesize, byLRU, nowCycle, onlyVM);
				int ti2 = B.LRU.size();
				for(int i35 = 0 ; i35 < ti2 ; i35++){
					ins tempL2 = B.LRU.front();
					if(tempL2.pid == tempE.pid  && tempL2.aId == nowT.argument){ // if it is (should be)released aid
						B.LRU.pop_front();
					}
					else{
						B.LRU.pop_front();
						B.LRU.push_back(tempL2);
					}
				}
			}
			nowTimeQt+=1;
			//if time quantum expired?
			if(nowTimeQt == timeqt || runL.front().leftTime == 0){//my time quantum is done or leftTime is zero, It has to be rescheduled
				nowTimeQt = 0;
				event tempE1;
				tempE1 = runL.front();
				runL.pop_front();
				runL.push_back(tempE1);
			}
			//print after instruction is done.
			fprintf(pout[tempE.pid], "%d Cycle#Instruction op %d arg %d\n", nowCycle, nowT.opcode, nowT.argument);
			fprintf(pout[tempE.pid], "AllocID");
			for(int i30 = 0; i30 < vmsize/pagesize ; i30++){
				if(i30%4 == 0){
					fprintf(pout[tempE.pid], "|");
				}
				if(ptL[tempE.pid][i30].aId < 0){
					fprintf(pout[tempE.pid], "-");
				}
				else{
					fprintf(pout[tempE.pid], "%d", ptL[tempE.pid][i30].aId);
				}
			}
			fprintf(pout[tempE.pid], "|\n");
			fprintf(pout[tempE.pid], "Valid  ");
			for (int i31 = 0; i31 < vmsize/pagesize ; i31++){
				if(i31%4 ==0){
					fprintf(pout[tempE.pid], "|");
				}
				fprintf(pout[tempE.pid], "%d", ptL[tempE.pid][i31].valid);
			}
			fprintf(pout[tempE.pid], "|\n\n");
			//erase process if last Task is none
			if(lastTask == true ){
				if(nowT.opcode != 5 && nowT.opcode !=4){
					runL.pop_front();
				}
				ins tempI7; // delete all in pm, vm, LRU
				tempI7.pid = tempE.pid;
				for(int i34 = 0; i34 < vmsize/pagesize ; i34++){
					onlyVM = false;
					if(ptL[tempE.pid][i34].aId >= 0){
						tempI7.aId = ptL[tempE.pid][i34].aId;
						if(ptL[tempE.pid][i34].valid == 0){
							onlyVM = true;
						}
						byLRU = false;
						int ti1 = B.LRU.size();
						B = buddy_rel(B, tempI7 , vmsize/pagesize, byLRU, nowCycle, onlyVM);
						for(int i35 = 0 ; i35 < ti1 ; i35++){
							ins tempL2 = B.LRU.front();
							if(tempL2.pid == tempE.pid ){
								B.LRU.pop_front();
							}
							else{
								B.LRU.pop_front();
								B.LRU.push_back(tempL2);
							}
						}
					}

				}
				nowTimeQt = 0;
			}

		}
		
		nowCycle += 1; // one cycle done
	}









}




