# include <iostream>
# include <list>

using namespace std;
class event{
	public:
		int aId;
		int argument; //size
};
class pm{
	public:
		int blockSize;
		int blockIndex;
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
		list <int> LRU;
};
buddy buddy_all(buddy B, event E ){
	//choose which block to go.
	int toGoI = 0;
	while(toGoI >= 0){
		if(B.pmL[toGoI].empty == true && E.argument <= B.pmL[toGoI].blockSize){
			break;
		}
		else{
			toGoI += 1;
		}
	}
	for(int i9 = toGoI + 1; i9<B.pmLSize ; i9++){
		if(E.argument <= B.pmL[i9].blockSize && B.pmL[toGoI].blockSize > B.pmL[i9].blockSize && B.pmL[toGoI].empty == true){ 
			//if this block is more better,
			toGoI = B.pmL[i9].blockIndex;
		}
	}
	cout<<"choosen block : "<<toGoI<<endl;
	//slice that block until it is smaller than event.argument
	while ( E.argument <= B.pmL[toGoI].blockSize){
		if(E.argument > B.pmL[toGoI].blockSize / 2){// put in this block
			
			B.pmL[toGoI].aId = E.aId;
			B.pmL[toGoI].empty = false;
			cout<<"now togoi"<<toGoI<<endl;
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
			tempP2.bI = B.pmL[toGoI].bI + "1";
			tempP2.empty = true;
			B.pmL[toGoI+1] = tempP2;
			tempP2.blockIndex = toGoI;
			tempP2.bI = B.pmL[toGoI].bI + "0";
			B.pmL[toGoI] = tempP2;
			B.pmLSize += 1;
		}
	}

	return B;
}
buddy buddy_rel(buddy B, event E){
	//release by aId
	cout<<"===============release line==================="<<endl;
	int releaseIndex;
	for(int i14 = 0; i14 < B.pmLSize; i14++){
		if(B.pmL[i14].aId == E.aId){
			releaseIndex = B.pmL[i14].blockIndex;
		}
	}
	cout<<releaseIndex<<endl;
	//release that block
	B.pmL[releaseIndex].aId = -1;
	B.pmL[releaseIndex].empty = true;
	while(releaseIndex >=0){
		bool isSummed = false;
		if(B.pmLSize > 1){
			if(releaseIndex == 0){// the left most is released
				if(B.pmL[releaseIndex].blockSize == B.pmL[releaseIndex+1].blockSize && B.pmL[1].empty == true){
					B.pmL[0].bI = B.pmL[0].bI.substr(0, B.pmL[0].bI.length()-1);
					cout<<"dfd"<<endl;
					cout<<B.pmL[0].bI<<endl;
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
				if(B.pmL[releaseIndex].blockSize == B.pmL[releaseIndex - 1].blockSize && B.pmL[releaseIndex -1].empty == true && B.pmL[releaseIndex].bI.at(B.pmL[releaseIndex].bI.length()-2) == B.pmL[releaseIndex-1].bI.at(B.pmL[releaseIndex-1].bI.length()-2)){
					for(int i16= releaseIndex + 1 ; i16 < B.pmLSize-1;  i16++){
						B.pmL[i16] = B.pmL[i16+1];
						B.pmL[i16].blockIndex-=1;
					}
					B.pmL[releaseIndex].bI = B.pmL[releaseIndex].bI.substr(0, B.pmL[releaseIndex].bI.length() -1);
					B.pmL[releaseIndex].blockSize = B.pmL[releaseIndex].blockSize * 2;
					B.pmLSize -= 1;
					isSummed = true;
				}					
				else if(B.pmL[releaseIndex].blockSize == B.pmL[releaseIndex + 1].blockSize && B.pmL[releaseIndex +1].empty == true && B.pmL[releaseIndex].bI.at(B.pmL[releaseIndex].bI.length()-2) == B.pmL[releaseIndex+1].bI.at(B.pmL[releaseIndex+1].bI.length()-2)){
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
		//if there was only block allocated, there is nothing to do.
	}
	cout<<"===============release line==================="<<endl;
	return B;
}
int main(int argc, char *argv[]){
//initialize when we make buddy system.
	buddy B(16);	
	
	event A, C;
	A.aId = 0;
	C.aId = 1;
	A.argument = 5;
	C.argument = 3;
	for (int i12 = 0 ; i12 < 16 ; i12++){
		B.pmL[i12].aId = -1; // aId is -1 if there is any process allocated.
		B.pmL[i12].empty = true;
	}

	B = buddy_all(B,C);
	B = buddy_all(B,A);
	B = buddy_rel(B,C);
	for (int i7 = 0; i7<B.pmLSize; i7++){
		for(int i8 = 0; i8 < B.pmL[i7].blockSize ; i8++){
			if(B.pmL[i7].aId< 0 ){
				cout<<"-"<<" ";
			}
			else{
				cout<<B.pmL[i7].aId<<" ";
			}
		}
	}
	cout<<endl;
	for (int i13 = 0 ; i13 < B.pmLSize ; i13++){
		cout<<B.pmL[i13].bI<<endl;
	}
}
