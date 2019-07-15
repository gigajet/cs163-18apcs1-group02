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

#endif // !GLOBAL_H_
