#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <algorithm>
using namespace std;

int main(int argc, char** argv){
	string s;
	vector<string> v;

	if(argc != 1){
		for(int i = 1; i< argc; i++){
			ifstream f(argv[i]);
			if(!f) {
				cerr << "file open failure" << endl;
				return EXIT_FAILURE;
			}
			while(!f.eof()){
				getline(f,s);
				v.push_back(s);
			}
			f.close();
			std::sort(v.begin(), v.end());
			vector<string>::iterator it = v.begin();
			while(it != v.end()){
				std::cout << *it << std::endl;
				++it;
			}
			v.clear();
		}
	}

	else {
		while (!cin.eof()){
			getline(cin,s);
			v.push_back(s);
		}
		
		std::sort(v.begin(), v.end());
		vector<string>::iterator it = v.begin();
		while(it != v.end()){
			std::cout << *it << std::endl;
			++it;
		}
		v.clear();
	}

  	return EXIT_SUCCESS;
}
