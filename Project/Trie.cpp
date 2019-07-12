#include "Trie.h"

void Trie::Insert(TrieNode* &root, string word)
{
	if (root == NULL)
	{
		root = new TrieNode();
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
		if (node == NULL) return;
		root = node;
	}
	return root->prefixes;
}

Trie::Trie()
{
	root = new TrieNode();
}

void Trie::Insert(string word)
{
	Insert(root, word);
}

int Trie::CountPrefix(string word)
{
	return CountPrefix(root, word);
}
