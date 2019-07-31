#include "Trie.h"

void Trie::Insert(TrieNode* &root, string word, bool isInTitle, int fileIndex)
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
	TrieNode* current = root;
	if (root == NULL) return 0;
	for (char i : word)
	{
		TrieNode* node = current->child[i];
		if (node == NULL) return 0;
		current = node;
	}
	return root->prefixes;
}

set<int> Trie::Search(TrieNode* root, string word, bool isInTitle)
{
	//cout << "test" << word << endl;
	set<int> result;
	if (root == NULL) return result;
	TrieNode* current = root;
	for (auto i : word)
	{
		TrieNode* node = current->child[i];
		if (node == NULL) return result;
		current = node;
	}
	
	/*if (current->isEndOfWord)
		return (isInTitle) ? current->fileIndexTitle : current->fileIndex;
	else
		return result;*/
	queue<TrieNode*> q;
	q.push(current);

	while (!q.empty())
	{
		TrieNode* u = q.front();
		q.pop();
		//if (u == NULL) continue;
		if (u->isEndOfWord)
		{
			if (isInTitle)
				result.insert(u->fileIndexTitle.begin(), u->fileIndexTitle.end());
			else
				result.insert(u->fileIndex.begin(), u->fileIndex.end());
		}
		for (auto i : u->child)
		{
			if (i.second != NULL)
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

int Trie::GetN(string word)
{
	TrieNode* current = root;
	set<int> result;
	if (current == NULL) return 0;
	for (auto i : word)
	{
		TrieNode* node = current->child[i];
		if (node == NULL) return 0;
		current = node;
	}

	return (current->isEndOfWord) ? current->fileIndex.size() : 0;
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


