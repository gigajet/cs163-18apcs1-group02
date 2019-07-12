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

using namespace std;

struct TrieNode
{
	map<char, TrieNode*> child;
	int prefixes;
	bool isEndOfWord;
	//ctdl de luu cac ten file
	//vector<int> fileIndex;
	TrieNode()
	{
		prefixes = 0;
		isEndOfWord = false;
	}
};

class Trie
{
private:
	TrieNode* root = NULL;
	void Insert(TrieNode* &root, string word);
	int CountPrefix(TrieNode* root, string word);
	//vector<int> Search(TrieNode* root, string word);
public:
	Trie();
	void Insert(string word);
	int CountPrefix(string word);
	//vector<int> Search(string word);
};


#endif // !TRIE_H_

