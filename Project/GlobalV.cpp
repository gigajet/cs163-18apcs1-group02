#include "Global.h"
#include <variant>
#include <algorithm>
bool isDelim(char c, string delimeter)
{
	for (string::iterator it = delimeter.begin(); it != delimeter.end(); ++it)
	{
		if (c == *it)
			return true;
	}
	return false;
}
bool isOperation(string s, vector<string> operations)
{
	for (int i = 0; i < operations.size(); ++i)
	{
		if (s.compare(operations[i]) == 0) // if s.compare()==0 => two string is the same
			return true;
	}
	return false;
}
Expression RefineToken(string Query)
{
	vector<string> operations = { "and","or","-","intitle","exact","~" };
	string delimeter = " ,.;:";
	vector<string> expression;
	string temp;
	int index = 0;
	string::iterator it = Query.begin();
	// abc and xyz or def intitle:you ~haha 123. => {"abc","and","xyz","or","def","intitle","you","~","haha","123}
	while (it != Query.end())
	{

		if (isDelim(*it, delimeter) == false)
		{
			bool isFirstDoubleQuotation = false;
			if (*it == '~' && temp.empty() == true)
			{
				temp.push_back('~');
				expression.emplace_back(temp);
				temp.clear();
			}
			else if (*it == '-' && temp.empty() == true)
			{
				temp.push_back('-');
				expression.emplace_back(temp);
				temp.clear();
			}
			else if (*it == '"')
			{
				if (isFirstDoubleQuotation == false && temp.empty() == true)
				{
					isFirstDoubleQuotation = true;
					temp.push_back('"');
				}
				else if (isFirstDoubleQuotation == false && temp.empty() == false)
				{
					temp.push_back('"');
					expression.emplace_back(temp);
					temp.clear();
				}
			}
			else temp.push_back(tolower(*it)); // normal case,*it is not operation,just add *it to temp

		}
		else  // case *it is delim
		{
			if (!temp.empty())
			{
				expression.emplace_back(temp);
				temp.clear();
			}
		}
		++it;
		//add the last word to expression
		if (it == Query.end())
		{

			if (temp.empty() == false) // avoid adding delimeter to expression if it is in the end of line
				expression.emplace_back(temp);
		}
	}
	// add "or" if there is no operation between two words
	//{"abc", "and", "xyz", "or", "def", "intitle", "you", "~", "haha", "123"} => {"abc","and","xyz","or","def","intitle","you","~","haha","or","123"}
	vector<string> finalExpression;
	for (int i = 0; i < expression.size(); ++i)
	{
		finalExpression.emplace_back(expression[i]);
		if (i < expression.size() - 1 && isOperation(expression[i + 1], operations) == false && isOperation(expression[i], operations) == false)
			finalExpression.emplace_back("or");// case no operation between two word 
	}
	return finalExpression;
}