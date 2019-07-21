#include "Trie.h"
#include "Global.h"

using namespace std;

#define TESTING_PHASE

void test()
{
	string s = "D:\\Study\\HCMUS\\CS\\ProjectCS163\\cs163-18apcs1-group02-master\\cs163-18apcs1-group02-master\\Project\\";
	string path = "Search Engine-Data\\";
	Global* g = Global::GetInstance();

	g->ReadData(path);

	string teststr = "live";

	set<int> res = g->trie.Search(teststr, false);

	for (int i : res) cout << g->fileName[i] << endl;

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
	e.push_back("or");
	e.push_back("muonnam");
	e = ConvertToRPN(e);
	for (auto token : e)
		cout << token << " ";
	cout << endl;
	CalculateRPN(e);
	cout << endl;
}

int main()
{
#ifdef TESTING_PHASE
	//test();
	testRPN();
#else
	//Do real thing
	
#endif
	return 0;
}