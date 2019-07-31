#pragma once
#ifndef GLOBAL_H_
#define GLOBAL_H_

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
#include "Trie.h"
#include <stack>
#include "Ranking.h"
#include <cmath>

using namespace std;

class Global
{
private:
	static Global* instance;
	Global();
public:
	int TotalDoc = 0;
	OkapiBM25 oki;
	Trie trie;
	vector<vector<string> > numberList; //numberList[i]: list of number in file i of fileName
	vector<vector<string> > priceList; //priceList[i]: list of prices, "$" discarded
	vector<string> fileName;
	void ReadData(string path);
	static Global* GetInstance();
};
/*
set<int> And(set<int> prevRes, string keyword); //1
set<int> Or(set<int> prevRes, string keyword); //2
set<int> Exclude(set<int> prevRes, string keyword); //3
set<int> InTitle(set<int> prevRes, string keyword); //4

set<int> HashTag(set<int> prevRes, string keyword); //8
set<int> ExactMatch(set<int> prevRes, string keyword); //9
*/

/*DEFINITION CANDIDATE 3*/
//For consistent data type.
typedef set<int> QueryAnswer;
typedef string Token;
typedef vector<Token> Expression;

QueryAnswer And(QueryAnswer a, QueryAnswer b);
QueryAnswer Or(QueryAnswer a, QueryAnswer b);
QueryAnswer Exclude(QueryAnswer a);

QueryAnswer Search(Token keyword);
QueryAnswer Exact(Token keyword);
QueryAnswer InTitle(Token keyword);
QueryAnswer PriceRange(Token a, Token b);
QueryAnswer NumberRange(Token a, Token b);

bool isNumber(string token);
bool isPrice(string token);
Token NumberCommaForm(Token numToken);
Token PriceCommaForm(Token priceToken);
bool hasNumberPrefix(string token);
bool hasPricePrefix(string token);
bool cmpNumber(string a, string b);


const string exactOp = "/",
searchOp = "+";

/*
  Job:
  0. If the name "RefineToken" is bad, change it.
  1. Convert all to lowercase
  2. "I love -you" -> "i","or","love","or","-","you"
  3. "#surevkl intitle:Dota2Vn -cOtICK -> "#surevkl","or","intitle","dota2vn","and","-","cotick"
  //And or Or when no operator between? Think, think twice and contact the one doing RPN thing.
*/
Expression RefineToken(string Query);
vector<string> RefineAddToken(string Query);
QueryAnswer getSynonymSet(Token token);
Expression getSynonymList(string query);
int Precedence(Token token);
Expression ConvertToRPN(Expression e);
QueryAnswer CalculateRPN(Expression rpn);

vector<QueryAnswer> AhoCorasick(set<int> fileList, vector<Token> tokenList);

double calScore(double n, double f, double N, double dl, double avg);

vector<int> Top5Result(QueryAnswer qa, Expression e);

set<int> GetHighlightInfo(int globalIndex, Expression rpn, string filename);

string getNumber(string word);

bool isDelim(char c, string delimeter);

vector<string> RefineMinusToken(string Query);

#endif // !GLOBAL_H_
