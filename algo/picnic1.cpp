#include<iostream>
#include<string>
using namespace std;
int goodcase(int human, int group, int *groups){
	bool checkL[human];
	for (int i4 = 0; i4<human; i4++){
		checkL[i4]= false;
	} // make default false
	
}
int main() {
	int cases;
  	cin >> cases;
  	for(int i1 = 0; i1<cases ; i1++) { // cases
		int human, group;
		cin>>human>>group;
		int groups[group*2];
    		for(int i2 = 0; i2 < group ; i2++){
			cin>>groups[i2*2]>>groups[i2*2+1];
		}
		//goodcase(human, group, groups);
		bool checkL[human];
		bool allchecked;
		int casenum = 0;
		for(int i3 = 0; i3 < group ; i3++){ // choose the first right group
			for (int i4 = 0; i4<human; i4++){// make default false
				checkL[i4]= false;
			} 
			allchecked = true ;// make allchecked true
			for(int i5 = i3; i5 < group ; i5++){
				if(checkL[groups[i5*2]] == false && checkL[groups[i5*2+1]] == false){ // two all not checked than check!
					checkL[groups[i5*2]] = true;
					checkL[groups[i5*2+1]] = true;
				}
				//if one of them is checked, then pass				
			}
			for(int i6= 0;i6<human; i6++){
				if(checkL[i6] ==false){ //if something is unchecked , allchecked is false
					allchecked = false;
				}
			}		
			if(allchecked ==true){
				casenum += 1;
			}	
					
		}
		cout<<"case : "<< i1 << " ::::: "<<casenum<<endl;		
  	}
}
