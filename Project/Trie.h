#pragma once
#ifndef TRIE_H_
#define TRIE_H_

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

using namespace std;

struct TrieNode
{
	map<char, TrieNode*> child;
	int prefixes;
	bool isEndOfWord;
	set<int> fileIndexTitle;
	set<int> fileIndex;
	TrieNode()
	{
		prefixes = 0;
		isEndOfWord = false;
		//isInTitle = false;
	}
};

class Trie
{
private:
	TrieNode* root = NULL;
	void Insert(TrieNode* &root, string word, bool isInTitle, int fileIndex);
	int CountPrefix(TrieNode* root, string word);
	set<int> Search(TrieNode* root, string word, bool isInTitle);
public:
	Trie();
	void Insert(string word, bool isInTitle, int fileIndex);
	int CountPrefix(string word);
	set<int> Search(string word, bool isInTitle);
	//void ReadData(string path);
};


#endif // !TRIE_H_

