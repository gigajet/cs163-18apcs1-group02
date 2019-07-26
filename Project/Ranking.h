#pragma once
#ifndef RANKING_H_
#define RANKING_H_

#include <iostream>
#include <map>
#include <unordered_map>

using namespace std;

const double K = 1.2;
const double B = 0.75;

struct OkapiBM25
{
	map<string, unordered_map<int, int> > tf; //freq
	map<int, int> lengthTable; //document length
	void addToTF(string word, int index);
	void addToLengthTable(int index, int length);

	double getAVGLength()
	{
		double sum = 0.0;
		double size = 0;
		for (auto i : lengthTable) sum += i.second, size++;
		return (size == 0) ? 1 : (sum / size);
	}
};

#endif // !RANKING_H_
