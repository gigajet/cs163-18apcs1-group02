#include"FrontEnd.h"
#include <chrono>
#include <filesystem>

using namespace std;
using namespace std::chrono;

//#define TESTING_PHASE

void test()
{
	string s = "C:\\Users\\Admin\\source\\repos\\cs163-18apcs1-group02-master\\Search Engine-Data\\";
	string path = "Search Engine-Data\\";
	Global* g = Global::GetInstance();

	g->ReadData(s);

	//string teststr = "live";

	//set<int> res = g->trie.Search(teststr, false);

	//for (int i : res) cout << g->fileName[i] << endl;

	//g->trie.Destructor();
	string a = showLogo();
	showResultandSearch(a);
	Global::GetInstance()->trie.Destructor();
	delete Global::GetInstance();
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
	e.push_back("\"dang cong san\""); 
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
	/*int Count = 0;
	{
		string path = "Search Engine-Data\\";
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
	}*/
	cout << Global::GetInstance()->fileName.size();
	set<int> s;
	for (int i = 0; i < Global::GetInstance()->fileName.size(); ++i) s.insert(i);
	vector<Token> tokenList;

	tokenList.push_back("\"De*uo*Su\"");

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	vector<QueryAnswer> ttt = AhoCorasick(s, tokenList);

	Top5Result(ttt[0], ConvertToRPN(tokenList));

	//for (auto i : tttt) cout << endl << i;

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(t2 - t1).count();
	cout << "Aho corasick run in: " << duration;
}

void testExactSearch() {
	if (Global::GetInstance()->fileName.size() == 0)
		test();
	QueryAnswer ans = Exact("De Nhi Quoc Su");
	for (auto i : ans)
		cout << i << " ";
	cout << endl;
	
	Global::GetInstance()->trie.Destructor();
}

#ifdef TESTING_PHASE
void DetailFile_RW(int globalIndex, Expression rpn);
#endif

//Make ___index.txt at that directory
void MakeIndex(string path) {
	vector<string> v;
	for (auto &p : filesystem::directory_iterator(path)) {
		v.push_back(p.path().filename().string());
	}
	ofstream fout; fout.open(path+"___index.txt");
	for (auto x : v) {
		fout << x << endl;
	}
	fout.close();
}

void SearchDebug() {
	do {
		string s;
		cout << "Query: "; getline(cin, s);
		if (s == "eXiT") return;
		Expression e = RefineToken(s);
		cout << "Refine token: ";
		for (auto i : e) cout << i << " "; cout << endl;
		e = ConvertToRPN(e);
		cout << "RPN Exp: ";
		for (auto i : e) cout << i << " "; cout << endl;
		QueryAnswer ans = CalculateRPN(e);
		cout << "File match: ";
		for (auto i : ans) cout << i << " " << Global::GetInstance()->fileName[i] << endl; cout << endl;
		cout << "Score: ";
		vector<int> k = Top5Result(ans, e);
		for (int x : k) cout << x << " "; cout << endl;
		cout << "======" << endl;
	} while (1);
}

void testTop5() {
	if (Global::GetInstance()->fileName.size() == 0)
		test();
	QueryAnswer ans = Search("5000");
	cout << "Search result: " << ans.size() << endl;
	vector<int> top5 = Top5Result(ans, { "vietnam" });
	cout << "Top5: ";
	for (auto x : top5) cout << x << " ";
	cout << endl;

	Global::GetInstance()->trie.Destructor();
}

int main()
{
#ifdef TESTING_PHASE
	Global *g = Global::GetInstance();
	//test();

	//cout << "Doc file run in: " << duration;

	//testRPN();
	//testAho();
	//testRefineToken(); //bug in calculateRPN case intitle
	//testSearch();
	//testExactSearch();

	/*
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	testTop5();
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(t2 - t1).count();
	cout << "DocFile+Exact+top5 run in: " << duration;

	Global::GetInstance()->trie.Destructor();
	delete Global::GetInstance();
	*/

	/*
	string kw = "40..50";
	Expression e = RefineToken(kw);
	for (auto x : e) cout << x << endl;
	*/
	
	/* //Test highlight
	g->fileName.push_back("d:\\Minh\\clz\\data\\Search Engine-Data\\Data1254.txt");
	g->numberList.push_back({});
	g->numberList[0].insert(g->numberList[0].end(), { 14, 18, 68, 79, 1992, 2010, 2013 });
	Expression e;
	e.insert(e.end(), { "14", "..", "2013"});
	e = ConvertToRPN(e);
	DetailFile_RW(0, e);
	*/

	/* //Test highlight case 2
	g->fileName.push_back("d:\\Minh\\clz\\data\\Search Engine-Data\\003.txt");
	g->numberList.push_back({});
	g->numberList[0].insert(g->numberList[0].end(), {70});
	Expression e;
	e.insert(e.end(), { "70" });
	e = ConvertToRPN(e);
	DetailFile_RW(0, e);
	*/
	
	/* //Test number
	g->fileName.push_back("d:\\Minh\\clz\\data\\Search Engine - Data\\Data1254.txt");
	g->numberList.push_back({});
	g->numberList[0].insert(g->numberList[0].end(), {14, 18, 68, 79, 1992, 2010, 2013});
	QueryAnswer ans = NumberRange("69", "78");
	for (auto i : ans) cout << i << " "; cout << endl;
	cout << "Total: " << ans.size() << " files" << endl;
	cout << endl;
	*/

#else
	//Do real thing

	//string path = "Search Engine-Data\\";
	//string path = "d:\\Minh\\clz\\data\\Search Engine-Data\\";
	string path = "d:\\Minh\\clz\\data_small\\";
	//MakeIndex(path);
	Global* g = Global::GetInstance();
	g->ReadData(path);

	//SearchDebug();
	string a = showLogo();
	showResultandSearch(a);

	/*string key = "vietNaM and live";

	Expression e = RefineToken(key);
	e = ConvertToRPN(e);
	cout << "RPN: " << endl;
	for (auto i : e) cout << i << endl;

	QueryAnswer res = CalculateRPN(e);

	for (auto i : res)
		cout << g->fileName[i] << endl;*/

	/*QueryAnswer ans = Exact("muc kien lien");
	for (int i : ans)
		cout << g->fileName[i] << endl;*/

	Global::GetInstance()->trie.Destructor();
	delete Global::GetInstance();
#endif
	return 0;
}