#include "Trie.h"
#include "Global.h"
#include <chrono>

using namespace std;


#define TESTING_PHASE

#ifdef TESTING_PHASE
using namespace std::chrono;
#endif

void test()
{
	string s = "D:\\Study\\HCMUS\\CS\\ProjectCS163\\cs163-18apcs1-group02-master\\cs163-18apcs1-group02-master\\Project\\";
	string path = "Search Engine-Data\\";
	Global* g = Global::GetInstance();

	g->ReadData(path);

	//string teststr = "live";

	//set<int> res = g->trie.Search(teststr, false);

	//for (int i : res) cout << g->fileName[i] << endl;

	g->trie.Destructor();
	return;
}

void testRPN() {
	//Case 1
	Expression e;
	cout << "Case 1" << endl;
	e.push_back("i");
	e.push_back("or");
	e.push_back("love");
	e.push_back("or");
	e.push_back("you");
	e.push_back("and");
	e.push_back("\"Ho Chi Minh\"");
	e = ConvertToRPN(e);
	for (auto token : e)
		cout << token << " ";
	cout << endl;
	CalculateRPN(e);
	cout << endl;

	//Case 2
	cout << "Case 2" << endl;
	e.clear();
	e.push_back("-");
	e.push_back("china");
	e.push_back("and");
	e.push_back("vietnam");
	e = ConvertToRPN(e);
	for (auto token : e)
		cout << token << " ";
	cout << endl;
	CalculateRPN(e);
	cout << endl;

	//Case 3
	cout << "Case 3" << endl;
	e.clear();
	e.push_back("\"dangcongsan\""); 
	e.push_back("and");
	e.push_back("vietnam");
	e.push_back("and");
	e.push_back("quangvinh");
	e.push_back("intitle");
	e.push_back("muonnam");
	e = ConvertToRPN(e);
	for (auto token : e)
		cout << token << " ";
	cout << endl;
	CalculateRPN(e);
	cout << endl;
}
void testRefineToken()
{
	string query = "\"dangcongsan\" and vietnam and quangvinh intitle:muonam";
	Expression e = RefineToken(query);
	e = ConvertToRPN(e);
	for (auto token : e)
		cout << token << " ";
	cout << endl;
	CalculateRPN(e);
	cout << endl;
}
void testAho() {
	//quick hack instead of actually reading file. Waste RAM during debugging? NO!
	int Count = 0;
	{
		string path = "d:\\Minh\\clz\\data\\Search Engine-Data\\";
		string listFile = path + "___index.txt";
		ifstream fin;
		fin.open(listFile);
		string filename;
		while (getline(fin, filename))
		{
			filename = path + filename;
			Global::GetInstance()->fileName.push_back(filename);
			Count++;
		}
		fin.close();
	}
	set<int> s;
	for (int i = 0; i < Count; ++i) s.insert(i);
	vector<Token> tokenList;

	tokenList.push_back("\"De*Q*Su\"");

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	AhoCorasick(s, tokenList);

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(t2 - t1).count();
	cout << "Aho corasick run in: " << duration;
}

int main()
{
#ifdef TESTING_PHASE
	//test();
	testRPN();
	//testAho();
	testRefineToken(); //bug in calculateRPN case intitle
	//testSearch();
#else
	//Do real thing
#endif
	return 0;
}