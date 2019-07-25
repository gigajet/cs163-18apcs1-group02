#include "Ranking.h"

void OkapiBM25::addToTF(string word, int index)
{
	tf[word][index] += 1;
}

void OkapiBM25::addToLengthTable(int index, int length)
{
	lengthTable[index] = length;
}
