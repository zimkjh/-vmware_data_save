#include <iostream>
#include <stdio.h>
#include <sstream>
#include <vector>
using namespace std;

int main(){
	string text = "10.0 8.8";
	char delim = ' ';
	stringstream f(text);


 	string s;
        while (getline(f, s, delim))
        {
               cout << s << endl;
        }


}
