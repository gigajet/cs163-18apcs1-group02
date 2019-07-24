#include "Trie.h"

void Trie::Insert(TrieNode* &root, string word, bool isInTitle, int fil eIndex)
{
	if (root == NULL)
	{
		root = new TrieNode();
		//if (isInTitle) root->fileIndexTitle.insert(fileIndex);
		//root->fileIndex.insert(fileIndex);
		//return;
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
		//if (isInTitle) node->fileIndexTitle.insert(fileIndex);
		//node->fileIndex.insert(fileIndex);

		current = node;
		current->prefixes++;
	}
	current->isEndOfWord = true;
	if (isInTitle) current->fileIndexTitle.insert(fileIndex);
	current->fileIndex.insert(fileIndex);

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
	set<int> result;
	if (root == NULL) return result;
	for (auto i : word)
	{
		TrieNode* node = root->child[i];
		if (node == NULL) return result;
		root = node;
	}
	
	queue<TrieNode*> q;
	q.push(root);

	while (!q.empty())
	{
		TrieNode* u = q.front();
		q.pop();
		if (u->isEndOfWord)
		{
			
			if (isInTitle)
				result.insert(u->fileIndexTitle.begin(), u->fileIndexTitle.end());
			else
				result.insert(u->fileIndex.begin(), u->fileIndex.end());
		}
		for (auto i : u->child)
		{		
			q.push(i.second);
		}
	}
	return result;
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

void Trie::Destructor()
{
	Destructor(root);
}

void Trie::Destructor(TrieNode* &root)
{
	if (root == NULL) return;
	for (auto i : root->child)
		Destructor(i.second);
	delete root;
	root = NULL;
}


