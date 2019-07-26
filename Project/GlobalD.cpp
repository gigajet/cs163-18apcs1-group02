#include "Global.h"
#include <cstdio>

using namespace std;

#define TESTING_PHASE1

double calScore(double n, double f, double N, double dl, double avg)
{
	double IDF = log((N - n + 0.5) / (n + 0.5));
	return IDF * (f * (K + 1)) / (f + K * (1 - B + B * dl / avg));
}

vector<int> Top5Result(QueryAnswer qa, Expression e)
{
	Global* global = Global::GetInstance();
	priority_queue<pair<double, int> > pq;
	map<int, double> res;
	vector<int> Top5;

	for (auto index : qa)
	{
		for (auto i : e)
		{
			if (Precedence(i) == 0)
			{
				double freq = (double)global->oki.tf[i][index];
				double nq = (double)global->trie.GetN(i);
				double dl = (double)global->oki.lengthTable[index];
				double avg = global->oki.getAVGLength();

				res[index] += calScore(nq, freq, global->TotalDoc, dl, avg);
			}
		}
		pq.push(make_pair(res[index], index));
	}

	int i = 0;
	while (!pq.empty() && i < 5)
	{
#ifdef TESTING_PHASE1
		cout << endl << pq.top().second << " ";
		printf("%.2lf", pq.top().first);
#endif // TESTING_PHASE

		Top5.push_back(pq.top().second);
		pq.pop();
		i++;
	}
	return Top5;
}