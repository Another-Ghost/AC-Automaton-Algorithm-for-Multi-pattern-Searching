

#include <iostream>

#include <vector>
#include <string>
#include <fstream>

using namespace std;


typedef struct _Node {
    vector<int> exist;
    _Node* fail;
    _Node* child[26];
}Node;

const string FILEDIR("../files/");
vector<string> dictionary;

bool ReadDictionary(const string& filename) {
    ifstream infile(FILEDIR + filename);
	try {
		if (infile.fail())
			throw filename;
	}
	catch (string s){
		cout << "Failed to load "+ filename << endl;
		//exit(1);
		return false;
	}

	string word;
	while (!infile.eof()){
		word.clear();
		infile >> word;
		dictionary.push_back(word);
	}

	return true;
}



int main()
{
	if(ReadDictionary("dictionary.txt")){
		for (int i = 0; i < dictionary.size(); i++) {
			cout << dictionary[i] << endl;
		}
	}



}

