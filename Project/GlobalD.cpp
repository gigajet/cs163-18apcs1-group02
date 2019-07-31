#include "Global.h"
#include <cstdio>

using namespace std;

//#define TESTING_PHASE1

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
		stack<Token> st;
		for (auto i : e)
		{
			if (Precedence(i) == 0)
			{
				/*double freq = (double)global->oki.tf[i][index];
				double nq = (double)global->trie.GetN(i);
				double dl = (double)global->oki.lengthTable[index];
				double avg = global->oki.getAVGLength();

				res[index] += calScore(nq, freq, global->TotalDoc, dl, avg);*/
				st.push(i);
			}
			if (Precedence(i) > 0)
			{
				if (i == searchOp)
				{
					string temp = st.top(); st.pop();

					double freq = (double)global->oki.tf[temp][index];
					double nq = (double)global->trie.GetN(temp);
					double dl = (double)global->oki.lengthTable[index];
					double avg = global->oki.getAVGLength();

					res[index] += calScore(nq, freq, global->TotalDoc, dl, avg);
				}
				if (i == exactOp)
				{
					string temp = st.top();
					st.pop();
					stringstream str(temp);
					string word;
					while (getline(str, word, ' '))
					{
						double freq = (double)global->oki.tf[word][index];
						double nq = (double)global->trie.GetN(word);
						double dl = (double)global->oki.lengthTable[index];
						double avg = global->oki.getAVGLength();

						res[index] += calScore(nq, freq, global->TotalDoc, dl, avg);
					}
				}
				if (i == "..")
				{
					Token tmp1 = st.top(); st.pop();
					Token tmp2 = st.top(); st.pop();
					if (cmpNumber(tmp2, tmp1)) swap(tmp1, tmp2);
					Global* g = Global::GetInstance();
					if (isNumber(tmp1)) {
						for (auto it = lower_bound(g->numberList[index].begin(), g->numberList[index].end(), tmp1, cmpNumber);
							it != upper_bound(g->numberList[index].begin(), g->numberList[index].end(), tmp2, cmpNumber);
							it++) {
							string temp = *it;
							double freq = (double)global->oki.tf[temp][index];
							double nq = (double)global->trie.GetN(temp);
							double dl = (double)global->oki.lengthTable[index];
							double avg = global->oki.getAVGLength();

							res[index] += calScore(nq, freq, global->TotalDoc, dl, avg);
						}
					}
					else { //Price
						for (auto it = lower_bound(g->priceList[index].begin(), g->priceList[index].end(), tmp1, cmpNumber);
							it != upper_bound(g->priceList[index].begin(), g->priceList[index].end(), tmp2, cmpNumber);
							it++) {
							//cerr << *it << endl;
							string temp = "$" + *it;
							double freq = (double)global->oki.tf[temp][index];
							double nq = (double)global->trie.GetN(temp);
							double dl = (double)global->oki.lengthTable[index];
							double avg = global->oki.getAVGLength();

							res[index] += calScore(nq, freq, global->TotalDoc, dl, avg);
						}
					}
				}
				if (i == "~")
				{
					vector<string> syn = getSynonymList(st.top()); st.pop();
					for (auto temp : syn)
					{
						double freq = (double)global->oki.tf[temp][index];
						double nq = (double)global->trie.GetN(temp);
						double dl = (double)global->oki.lengthTable[index];
						double avg = global->oki.getAVGLength();

						res[index] += calScore(nq, freq, global->TotalDoc, dl, avg);
					}
				}
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

vector<string> RefineMinusToken(string Query)
{
	//vector<string> operations = { "and","or","-","intitle","~" };
	string delimeter = " ,.;:'?/";

	bool isMinusNotation = false;
	bool isFirstDoubleNotation = false;
	vector<string> expression;
	vector<string> finalExpression;
	string temp;
	string::iterator it = Query.begin();
	while (it != Query.end())
	{
		if (*it == '"' && isFirstDoubleNotation == true)
			isFirstDoubleNotation = false;
		else if (*it == '"' && isFirstDoubleNotation == false)
			isFirstDoubleNotation = true;
		else if (*it != '"' && isFirstDoubleNotation == false)
		{
			if (*it == '-')
			{
				isMinusNotation = true;
			}
			else if (isDelim(*it, delimeter) == true || it == Query.end())
			{
				if (!temp.empty())
				{
					expression.emplace_back("-");
					expression.emplace_back(temp);
				}

				isMinusNotation = false;
				temp.clear();
			}
			else if (isMinusNotation == true)
			{
				temp.push_back(*it);
			}
		}
		++it;
		if (it == Query.end())
		{
			if (!temp.empty() && isMinusNotation == true)
			{
				expression.emplace_back("-");
				expression.push_back(temp);
			}
		}
	}
	for (int i = 0; i < expression.size(); ++i)
	{
		finalExpression.emplace_back(expression[i]);
		//if (i + 1 < expression.size())
		//	finalExpression.emplace_back("&");
		if ((i % 2 == 1) && (i + 1) < expression.size())
			finalExpression.emplace_back("&");
	}
	return finalExpression;
}
