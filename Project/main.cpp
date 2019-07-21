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
int main()
{
#ifdef TESTING_PHASE
	test();
#else
	//Do real thing
	
#endif
	return 0;
}