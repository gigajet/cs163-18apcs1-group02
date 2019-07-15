#include "Trie.h"

void Trie::Insert(TrieNode* &root, string word, bool isInTitle, int fileIndex)
{
	if (root == NULL)
	{
		root = new TrieNode();
		if (isInTitle) root->fileIndexTitle.insert(fileIndex);
		root->fileIndex.insert(fileIndex);
		return;
	}
	TrieNode* current = root;
	for (char i : word)
	{
		TrieNode* node = current->child[i];
		if (node == NULL)
		{
			node = new TrieNode();
			current->child[i] = node;
		}
		//update node information
		if (isInTitle) node->fileIndexTitle.insert(fileIndex);
		node->fileIndex.insert(fileIndex);

		current = node;
		current->prefixes++;
	}
	current->isEndOfWord = true;
}

int Trie::CountPrefix(TrieNode* root, string word)
{
	for (char i : word)
	{
		TrieNode* node = root->child[i];
		if (node == NULL) return 0;
		root = node;
	}
	return root->prefixes;
}

set<int> Trie::Search(TrieNode* root, string word, bool isInTitle)
{
	for (auto i : word)
	{
		TrieNode* node = root->child[i];
		if (node == NULL) return set<int>();
		root = node;
	}
	return (isInTitle) ? root->fileIndexTitle : root->fileIndex;
}

Trie::Trie()
{
	root = new TrieNode();
}

void Trie::Insert(string word, bool isInTitle, int fileIndex)
{
	Insert(root, word, isInTitle, fileIndex);
}

int Trie::CountPrefix(string word)
{
	return CountPrefix(root, word);
}

set<int> Trie::Search(string word, bool isInTitle)
{
	return Search(root, word, isInTitle);
}


