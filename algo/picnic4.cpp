#include<iostream>
#include<string>
#include<list>
using namespace std;
int result = 0;
void ifPossible(list<int>& picked, int *groups, bool *checkL){
	list<int> picked2 = picked;
	bool possible = true;
	
	while(!picked2.empty()){
		if(checkL[groups[picked2.front()*2]]==false && checkL[groups[picked2.front()*2+1]] == false){
			checkL[groups[picked2.front()*2]] = true;
			checkL[groups[picked2.front()*2+1]] = true;
		}
		picked2.pop_front();
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
void pick(int n, list<int>& picked, int toPick, int *groups, bool *checkL){
	// n : number of groups
	// picked : picked groups
	// toPick : number to pick from groups
	if(toPick == 0){ // base case : if there is no left elements
		ifPossible(picked, groups, checkL);
		return;
	}
	// choose the smallest one
	int smallest = picked.empty() ? 0 : picked.back()+1;
	// select one element
	for(int next = smallest ; next<n; ++next){
		picked.push_back(next);
		pick(n, picked, toPick-1, groups, checkL);
		picked.pop_back();
	}

}
int main() {
	
	int cases;
  	cin >> cases;
  	for(int i1 = 0; i1<cases ; i1++) { // cases
		result = 0;
		int human, group;
		cin>>human>>group;
		//int groups[group*2];
		int *groups = new int[group*2];
    		for(int i2 = 0; i2 < group ; i2++){
			cin>>groups[i2*2]>>groups[i2*2+1];
		}
		list<int> picked;
		bool *checkL = new bool[picked.size()*2];
		for(int i1 = 0; i1<int(picked.size())*2 ; i1++){
			checkL[i1] = false;
		}
		pick(group, picked, human/2, groups, checkL);
		cout<<result<<endl;		
  	}
}
