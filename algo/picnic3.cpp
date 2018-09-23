#include<iostream>
#include<string>
#include<list>
#include <time.h>
using namespace std;
int result = 0;
int times = 0;
void ifPossible(list<int>& picked, int *groups){
	list<int> picked2 = picked;
	bool possible = true;
	bool *checkL = new bool[picked.size()*2];
	//bool checkL[picked.size()*2];
	for(int i1 = 0; i1<int(picked.size())*2 ; i1++){
		checkL[i1] = false;
	}
	list<int>::iterator iter;
	for(iter = picked.begin(); iter!= picked.end(); iter++){
		if(checkL[groups[*iter*2]] == false && checkL[groups[*iter*2 + 1]]==false){
			checkL[groups[*iter*2]] = true;
			checkL[groups[*iter*2+1]] = true;
		}
	}    


	for(int i2 = 0; i2<int(picked.size())*2 ; i2++){
		if(checkL[i2] == false){ //if just one of human is not checked, then not possible case
			possible = false;
		}
	}
	if(possible){
		result += 1;
	}
	delete checkL;
}
void pick(int n, list<int>& picked, int toPick, int *groups){
	times += 1;
	// n : number of groups
	// picked : picked groups
	// toPick : number to pick from groups
	if(toPick == 0){ // base case : if there is no left elements
		ifPossible(picked, groups);
		return;
	}
	// choose the smallest one
	int smallest = picked.empty() ? 0 : picked.back()+1;
	// select one element
	
	for(int next = smallest ; next<n; ++next){
		picked.push_back(next);
		pick(n, picked, toPick-1, groups);
		picked.pop_back();
	}

}
int main() {
	clock_t begin, end, t1, t2;
	int cases;
  	cin >> cases;
	begin = clock(); 
  	for(int i1 = 0; i1<cases ; i1++) { // cases
		times = 0;
		cout<<"case :"<< i1<<"starts!"<<endl;
		result = 0;
		int human, group;
		cin>>human>>group;
		//int groups[group*2];
		int *groups = new int[group*2];
    		for(int i2 = 0; i2 < group ; i2++){
			cin>>groups[i2*2]>>groups[i2*2+1];
		}
		list<int> picked;
		t1 = clock();
		pick(group, picked, human/2, groups);
		t2 = clock();
		cout<<result<<endl;	
		cout<<"t2 - t1 : "<<t2-t1<<endl;
		cout<<"case :"<<i1 <<"ends..."<<endl<<endl;	
  	}
	end = clock();          // 시간설정
	cout<<"times : "<< times<<endl;
	cout<<"수행시간 : "<<((end-begin))<<endl;

}
