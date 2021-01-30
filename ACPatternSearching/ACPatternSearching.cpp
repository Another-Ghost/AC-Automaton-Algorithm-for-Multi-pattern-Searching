

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
		s[1];
	}

	string word;
	while (!infile.eof()){
		word.clear();
		infile >> word;
		dictionary.push_back(word);
	}

	return true;
}

vector<vector<char>> wheels;
int N, M; //n is the number of wheels, m is the number of letters per wheels
bool ReadWheels(const string& filename) {
	ifstream infile(FILEDIR + filename);
	try {
		if (infile.fail())
			throw filename;
	}
	catch (string s) {
		cout << "Failed to load " + filename << endl;
		//exit(1);
		return false;
	}
	infile >> N;
	infile >> M;
	for (int i = 0; i < N; i++) {
		vector<char> wheel;
		for (int j = 0; j < M; j++) {
			char letter = 0;
			infile >> letter;
			wheel.push_back(letter);
		}
		wheels.push_back(wheel);
	}

	return true;
}

int main()
{
	if(ReadDictionary("dictionary.txt")){
		//for (int i = 0; i < dictionary.size(); i++) {
		//	cout << dictionary[i] << endl;
		//}
	}
	
	if (ReadWheels("wheels.txt")) {
		for (int i = 0; i < wheels.size(); i++) {
			for (int j = 0; j < wheels[0].size(); j++) {
				cout << wheels[i][j];
			}
			cout << endl;
		}
	}
}

