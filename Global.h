#pragma once
#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <map>
#include <vector>
#include <cmath>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
#include "Trie.h"

using namespace std;

class Global
{
private:
	static Global* instance;
	Global();
public:
	Trie trie;
	vector<string> fileName;
	void ReadData(string path);
	static Global* GetInstance();

};

//mustInTitle=false -> search both Title and Content, not just the content.

set<int> And(set<int> prevRes, string keyword, bool mustInTitle); //1
set<int> Or(set<int> prevRes, string keyword, bool mustInTitle); //2
set<int> Exclude(set<int> prevRes, string keyword, bool mustInTitle); //3

set<int> HashTag(set<int> prevRes, string keyword); //8
set<int> ExactMatch(set<int> prevRes, string keyword, bool mustInTitle); //9


#endif // !GLOBAL_H_
