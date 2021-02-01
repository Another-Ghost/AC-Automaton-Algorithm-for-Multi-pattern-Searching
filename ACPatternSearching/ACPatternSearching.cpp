

#include <iostream>

#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <ctype.h>
#include <set>
#include <windows.h>
#include <algorithm>  

using namespace std;


const string FILEDIR("../files/");
const string OUTPUT_NAME("output_new.txt");
const string DICTIONARY_NAME("dictionary.txt");
const string WHEELS_NAME("wheels.txt");



/*Build dictionary from reading file*/
bool ReadDictionary(const string& file_name, vector<string> & dictionary) {
    ifstream infile(FILEDIR + file_name);
	try {
		if (infile.fail())
			throw file_name;
	}
	catch (string s){
		cout << "Failed to load "+ file_name << endl;
		exit(0);
		//return false;
	}

	while (!infile.eof()){
        string word;
		infile >> word;
        //word = strupr(word);
		dictionary.push_back(word);
	}
	return true;
}



/*Build Wheels from reading file*/
bool ReadWheels(const string& file_name, vector<vector<char>> & wheels, int & N, int & M) {
	ifstream infile(FILEDIR + file_name);
	try {
		if (infile.fail())
			throw file_name;
	}
	catch (string s) {
		cout << "Failed to load " + file_name << endl;
		exit(0);
		//return false;
	}

	infile >> N;
	infile >> M;

	for (int i = 0; i < N; i++) {
		vector<char> wheel;
		for (int j = 0; j < M; j++) {
			char letter = 0;
			infile >> letter;
            letter = tolower(letter);
			wheel.push_back(letter);
		}
		wheels.push_back(wheel);
	}

	return true;
}


#define ALPHABET   (26)
/*Node of Trie Tree*/
typedef struct _Node
{
	vector<int> exist;			//Existing words ended by this node, the value of each member means the length of that word and which could be used to find out the whole word 
	_Node* fail;				//Fail note
	_Node* child[ALPHABET];		//Child notes representing the next letters
	_Node()
	{
		fail = NULL;
		memset(child, NULL, ALPHABET * sizeof(_Node*));
	}
}Node;



//void InsertToOutput(vector<char> t_string, int start, int len) {
//	string word;
//	for (int i = 0; i < len; i++) {
//		word = word + t_string[start + i];
//	}
//	output.insert(word);
//}

/*Add word to Trie Tree*/
void TrieInsert(Node* root, string word)
{
    Node* tmp = root;

    for (int i = 0; i < word.size(); i++)
    {
        int c = word[i] - 'a';
		if (NULL == tmp->child[c]) {
			tmp->child[c] = new Node();
		}
        tmp = tmp->child[c];
    }
    tmp->exist.push_back(word.size());
}

/*Build Trie Graph*/
Node* ACBuild(vector<string> p_strings)
{
	Node* root = new Node();
	int n = p_strings.size();

    for (int i = 0; i < n; i++)
		TrieInsert(root, p_strings[i]);

    queue<Node*> node_queue; //Using BSF building Tire Graph
    for (int i = 0; i < ALPHABET; i++)
    {
        if (root->child[i])
        {
            root->child[i]->fail = root;
			node_queue.push(root->child[i]);
        }
    }

    while (!node_queue.empty())
    {
        Node* x = node_queue.front(); 
		node_queue.pop();
        for (int i = 0; i < ALPHABET; i++)
        {
            if (x->child[i])
            {
                Node* y = x->child[i], * fafail = x->fail; //fafail is fail node of y's father node(x)

                while (fafail && NULL == fafail->child[i])
                    fafail = fafail->fail;
                if (NULL == fafail) //If father node is root
                    y->fail = root;
                else
                    y->fail = fafail->child[i];

                if (y->fail->exist.size())	//If there is at least one word ended by y  
                    for (int j = 0; j < y->fail->exist.size(); j++)
                        y->exist.push_back(y->fail->exist[j]);
				node_queue.push(y);
            }
			else {
				x->child[i] = (x != root) ? x->fail->child[i] : root; //Optimization
			}
        }
    }
	return root;
}

//Find out matched pattern strings in the dictionary of target string 
void ACQuery(Node* root, vector<char> t_string, set<string>& output) {
	Node* tmp = root;
	for (int i = 0; i < t_string.size(); i++)
	{
		int c = t_string[i] - 'a';
		while (NULL == tmp->child[c] && tmp->fail)
			tmp = tmp->fail;
		if (tmp->child[c])
			tmp = tmp->child[c];
		else
			continue;
		if (tmp->exist.size())
		{
			for (int j = 0; j < tmp->exist.size(); j++)
			{
				int len = tmp->exist[j];
				//print_matching_result(w, i - len + 1, len);

				string word;
				for (int k = 0; k < len; k++) {
					word = word + t_string[i - len + 1 + k];
				}

				output.insert(word);
			}
		}
	}
}

/*find all possible words of two letters or more created by wheels(with outputting to file)*/
void ACQueryWheels(Node* root, vector<vector<char>> wheels, set<string>& output)
{
	int N = wheels.size();
	int M = wheels[1].size();

	string name("Out.txt");
	ofstream out_file(FILEDIR + name, fstream::out);
	try
	{
		if (out_file.fail())
			throw name;
	}
	catch (string s)
	{
		cout << "save file:[" << s << "] failed" << endl;
		exit(0);
		//return;
	}

	//DWORD start_time = GetTickCount(); //Debug

	long num_loop = pow(M, N);
	long count_loop = 0;
	while (count_loop < num_loop) {
		vector<char> t_string;
		t_string.resize(N);
		long dividend = count_loop;
		for (int i = 0; i < N; i++) {
			int remainder = dividend % M;
			dividend = dividend / M;
			t_string[N - i - 1] = wheels[N - i - 1][remainder];
		}

		ACQuery(root, t_string, output);

		count_loop++;
	}
	//DWORD end_time = GetTickCount(); //Debug
	//cout << "The run time is:" << (end_time - start_time) << "ms!" << endl; //Debug

	for (auto word : output) {
		out_file << word << endl;
	}

	out_file.close(); //Debug
	//system("pause"); //Debug

}

/*Optimized version of ACQueryWheels(without outputting to file)*/
void ACQueryWheels_Opt(Node* root, vector<vector<char>> wheels, set<string> & output) {
	int N = wheels.size();	
	int M = wheels[1].size();

	//DWORD start_time = GetTickCount(); //Debug

	Node* temp = root; 
	vector<Node*> temp_list; //Preserving the result nodes of last searching
	temp_list.resize(N); 
	temp_list[0] = root;


	long num_loop = pow(M, N); //total of loop times
	long count_loop = 0; //current loop times
	vector<char> t_string_pre; //preserving last T string
	t_string_pre.resize(N); 
	
	int lowest_changed_bit = 0; //mark of the lowest changed bit when the T string has changed along with the number of loop has increased

	while (count_loop < num_loop) {
		vector<char> t_string;
		t_string.resize(N);

		//Update t_string
		long dividend = count_loop; 
		for (int i = 0; i < N; i++) {

			int remainder = dividend % M;
			dividend = dividend / M;
			t_string[N - i - 1] = wheels[N - i - 1][remainder];
			if (t_string_pre[N - i - 1] != t_string[N - i - 1]) {
				lowest_changed_bit = N - i - 1;
				t_string_pre[N - i - 1] = t_string[N - i - 1]; 		//make wheel_pre = wheel;
			}
		}
		count_loop++;


		if (lowest_changed_bit == 0) {
			temp = root;
		}
		else{
			temp = temp_list[lowest_changed_bit - 1];
		}

		for (int i = lowest_changed_bit; i < N; i++) {	//Iteration starts from the the lowest changed bit, doing this could decrease a huge number of outputting repetitive words.
			int c = t_string[i] - 'a';
			while (NULL == temp->child[c] && temp->fail)		
				temp = temp->fail;
			if (temp->child[c])
				temp = temp->child[c];
			else
				continue;	//temp is root node now
			if (temp->exist.size())
			{
				for (int j = 0; j < temp->exist.size(); j++)
				{
					int len = temp->exist[j];

					string word;
					for (int k = 0; k < len; k++) {
						word = word + t_string[i - len + 1 + k];
					}

					output.insert(word);
				}
			}
			temp_list[i] = temp;
		}

	}

	//DWORD end_time = GetTickCount(); //Debug
	//cout << "The run time is:" << (end_time - start_time) << "ms!" << endl; //Debug
	//system("pause"); //Debug
}



int main()
{
	set<string> output; //Using set preserving output words to eliminating repetition 
	vector<string> dictionary; //The list of words(as known as Pattern string)
	vector<vector<char>> wheels; //Target strings are composed by a sequence of single letter provided by each wheel  
	int N, M; //N is the number of wheels, M is the number of letters per wheels


	ReadDictionary("dictionary.txt", dictionary);

	ReadWheels("wheels.txt", wheels, N, M);

	cout << "Loaded successfully..." << endl;
	//Debug
	//if (ReadWheels("wheels.txt", wheels, N, M)) {
	//	for (int i = 0; i < wheels.size(); i++) {
	//		for (int j = 0; j < wheels[0].size(); j++) {
	//			cout << wheels[i][j];
	//		}
	//		cout << endl;
	//	}
	//}
	Node* root_node = ACBuild(dictionary);

	cout << "Please wait for the output..." << endl;

	ACQueryWheels_Opt(root_node, wheels, output);
	//ACQueryWheels(root_node, wheels, output);
	

	//Output to console
	for (auto word : output) {
		transform(word.begin(), word.end(), word.begin(), ::toupper);
		cout << word << endl;
	}
	cout << "Found " + to_string(output.size()) + " words" << endl;

	//Output to file
	ofstream out_file(FILEDIR + OUTPUT_NAME, fstream::out);
	try
	{
		if (out_file.fail())
			throw OUTPUT_NAME;
	}
	catch (string s)
	{
		cout << "Saving file:[" << s << "] failed." << endl;
		exit(0);
		//return;
	}

	for (auto word : output) {
		transform(word.begin(), word.end(), word.begin(), ::toupper);
		out_file << word << endl;
	}
	out_file << "Found " + to_string(output.size()) + " words" << endl;

	out_file.close();

	system("pause");
}

