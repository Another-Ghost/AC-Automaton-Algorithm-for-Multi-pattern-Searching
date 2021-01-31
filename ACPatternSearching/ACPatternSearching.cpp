

#include <iostream>

#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <ctype.h>
#include <set>
#include <windows.h>

using namespace std;

//#define ALPHABET   (26)
//typedef struct _Node {
//    vector<int> exist;
//    _Node* fail;
//    _Node* child[ALPHABET];
//}Node;

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

	
	while (!infile.eof()){
        string word;
		infile >> word;
        //word = strupr(word);
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
            letter = tolower(letter);
			wheel.push_back(letter);
		}
		wheels.push_back(wheel);
	}

	return true;
}

#define ALPHABET   (26)


typedef struct _NODE
{
	int name;               // debug
	vector<int> exist;
	_NODE* fail;
	_NODE* child[ALPHABET];
	_NODE()
	{
		name = 0;           // debug
		fail = NULL;
		memset(child, NULL, ALPHABET * sizeof(_NODE*));
	}
}NODE;

void print_matching_result(vector<char> T, int start, int len)
{
	for (int i = 0; i < start; i++)
		printf(" ");
	for (int i = 0; i < len; i++)
		printf("%c", T[start + i]);
	printf("\n");
}

set<string> output;
void InsertToOutput(vector<char> T, int start, int len) {
	string word;
	for (int i = 0; i < len; i++) {
		word = word + T[start + i];
	}
	output.insert(word);
}


void trie_insert(NODE* root, string word)
{
    NODE* tmp = root;

    for (int i = 0; i < word.size(); i++)
    {
        int c = word[i] - 'a';
        if (NULL == tmp->child[c])
            tmp->child[c] = new NODE();
        tmp = tmp->child[c];
    }
    tmp->exist.push_back(word.size());
}

void ac_build(NODE* root, vector<string> P, int n)
{
    for (int i = 0; i < n; i++)
        trie_insert(root, P[i]);

    queue<NODE*> q;
    for (int i = 0; i < ALPHABET; i++)
    {
        if (root->child[i])
        {
            root->child[i]->fail = root;
            q.push(root->child[i]);
        }
    }

    while (!q.empty())
    {
        NODE* x = q.front(); q.pop();
        for (int i = 0; i < ALPHABET; i++)
        {
            if (x->child[i])
            {
                NODE* y = x->child[i], * fafail = x->fail;
                /*
                    x--fail-→fafail         x--fail-→fafail
                      ↘ⁱ             ==>     ↘ⁱ        ↘ⁱ
                        y                      y--fail--→★
                */
                while (fafail && NULL == fafail->child[i])
                    fafail = fafail->fail;
                if (NULL == fafail)
                    y->fail = root;
                else
                    y->fail = fafail->child[i];

                if (y->fail->exist.size())
                    for (int j = 0; j < y->fail->exist.size(); j++)
                        y->exist.push_back(y->fail->exist[j]);
                q.push(y);
            }
        }
    }
}

void ac_query(NODE* root, vector<char> T)
{
    NODE* tmp = root;
    for (int i = 0; i < T.size(); i++)
    {
        int c = T[i] - 'a';
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
                print_matching_result(T, i - len + 1, len);
            }
        }
    }
}


void ac_query_wheels(NODE* root, vector<vector<char>> wheels) {
	DWORD start_time = GetTickCount();

	NODE* tmp = root;
	vector<NODE*> tmpList;
	tmpList.resize(8);
    tmpList[0] = root;

    vector<char> wheel;
	wheel.resize(8);
	long num_loop = pow(M, N);
	long count = 0;
	vector<char> wheel_pre;
	wheel_pre.resize(8);
	
	int changed_pos = 0;

	while (count < num_loop) {
		wheel.clear();
		long dividend = count;
		for (int i = 0; i < N; i++) {
			//long divisor = pow(M, (N - i + 1));
			int remainder = dividend % M;
			dividend = dividend / M;
			wheel.insert(wheel.begin(), wheels[N - i - 1][remainder]);
			//w.push_back(remainder);
			if (wheel_pre[N - i - 1] != wheels[N - i - 1][remainder]) {
				changed_pos = N - i - 1;
				wheel_pre[N - i - 1] = wheels[N - i - 1][remainder];
			}
		}
		count++;
		//wheel_pre = wheel;

		if (changed_pos == 0) {
			tmp = root;
		}
		else{
			tmp = tmpList[changed_pos - 1];
		}

		for (int i = changed_pos; i < N; i++) {
			int c = wheel[i] - 'a';
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
					//print_matching_result(wheel, i - len + 1, len);
					InsertToOutput(wheel, i - len + 1, len);

					string word;
					for (int k = 0; k < len; k++) {
						word = word + wheel[i - len + 1 + k];
					}

					output.insert(word);
				}
			}
			tmpList[i] = tmp;
		}

	}
	DWORD end_time = GetTickCount();
	cout << "The run time is:" << (end_time - start_time) << "ms!" << endl;

	for (auto word : output) {
		cout << word << endl;
	}

}


NODE* root;
void aho_corasick(vector<string> P, int n, vector<char> T)
{
    printf("**********************************************\n");
    //for (int i = 0; i < n; i++)
    //    printf("\"%s\" ", P[i]);
    //printf("\n%s\n", T);

    root = new NODE();
    ac_build(root, P, n);

    //print_automaton_info(root);         // debug
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
    aho_corasick(dictionary, dictionary.size(), wheels[0]);

  //  long num_loop = pow(M, N);
  //  long count = 0;
  //  while (count < num_loop) {
  //      vector<char> w;
  //      long dividend = count;
		//for (int i = 0; i < N; i++) {
  //          //long divisor = pow(M, (N - i + 1));
  //          int remainder = dividend % M;
  //          dividend = dividend / M;
  //          w.insert(w.begin(), wheels[N-i-1][remainder]);
  //          //w.push_back(remainder);
  //      }
  //    
  //      ac_query(root, w);
  //      count++;
  //  }
	ac_query_wheels(root, wheels);
    

    //aho_corasick(dictionary, dictionary.size(), wheels[0]);
}

