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


vector<string> RefineToken(string Query)
{
	vector<string> operations = { "and","or","-","intitle","~" };
	string delimeter = " ,.;:";
	vector<string> expression;
	string temp;
	int index = 0;
	string::iterator it = Query.begin();
	// abc and xyz or def intitle:you ~haha 123. => {"abc","and","xyz","or","def","intitle","you","~","haha","123}
	bool isFirstDoubleQuotation = false;
	while (it != Query.end())
	{
		if (isDelim(*it, delimeter) == false)
		{
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
				else if (isFirstDoubleQuotation == true && temp.empty() == false)
				{
					isFirstDoubleQuotation = false;
					temp.push_back('"');
					expression.emplace_back(temp);
					temp.clear();
				}
			}
			else
			{
				temp.push_back(tolower(*it)); // normal case,*it is not operation,just add *it to temp
			}
		}
		else  // case *it is delim
		{
			if (!temp.empty())
			{

				if (isFirstDoubleQuotation == false)
				{
					if (*it == '.')
					{
						if (it + 1 != Query.end() && it + 2 != Query.end())
						{
							if (*(it + 1) == '.' && *(it + 2) == '$' && temp[0] == '$')
							{
								bool flag = true;
								// check if  every character after $ in temp are digit
								for (string::iterator i = temp.begin() + 1; i != temp.end(); ++i)
									if (!isdigit(*i))
									{
										flag = false;
										break;
									}
								if (flag == true)
								{
									temp.push_back('.');
									temp.push_back('.');
								}
							}
							else if (*(it + 1) == '.' && isdigit(*(it + 2)) && isdigit(temp[0]))
							{
								temp.push_back('.');
								temp.push_back('.');
							}
							
						}

					}
					else {
						expression.emplace_back(temp);
						temp.clear();
					}
					
				}
				else temp.push_back(tolower(*it));
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
	
	int size = expression.size();
	for (int i = 0; i < size; ++i)
	{
		if (isdigit(expression[i][0]))
		{
			string s = expression[i];
			int pos = s.find("..");
			bool flag = true;
			if (pos != string::npos)
			{
				string t1;
				string t2;
				string t3;
				for (string::iterator it = s.begin(); it != s.begin() + pos; ++it)
				{
					if (isdigit(*it))
					{
						t1.push_back(*it);
					}
					else flag = false;
				}
				t2.append("..");
				for (string::iterator it = s.begin()+pos+2; it != s.end() ; ++it)
				{
					if (isdigit(*it))
					{
						t3.push_back(*it);
					}
					else flag = false;
				}
				if (flag == true)
				{
					finalExpression.push_back(t1);
					finalExpression.push_back(t2);
					finalExpression.push_back(t3);
					if (i < expression.size() - 1 && isOperation(expression[i + 1], operations) == false && isOperation(expression[i], operations) == false)
					{
						finalExpression.emplace_back("or");// case no operation between two word 
					}
				}
				else if (flag == false)
				{
					finalExpression.emplace_back(expression[i]);
					if (i < expression.size() - 1 && isOperation(expression[i + 1], operations) == false && isOperation(expression[i], operations) == false)
						finalExpression.emplace_back("or");// case no operation between two word 
				}

			}
			else
			{
				finalExpression.emplace_back(expression[i]);
				if (i < expression.size() - 1 && isOperation(expression[i + 1], operations) == false && isOperation(expression[i], operations) == false)
					finalExpression.emplace_back("or");// case no operation between two word 
			}
		}
		else if(expression[i][0]=='$')
		{
			string s = expression[i];
			int pos = s.find("..");
			bool flag = true;
			if (pos != string::npos)
			{
				
				string t1; // contain $123
				string t2; // contain ..
				string t3; // contain $234
				for (string::iterator it = s.begin();it!= s.begin()+pos; ++it)
				{
					if (isdigit(*it) == false)
					{
						if (*it != '$')
						{
							flag = false;
							break;
						}
					}
					t1.push_back(*it);
				}
				for (string::iterator it = s.begin()+pos+2; it != s.end(); ++it)
				{
					if (isdigit(*it) == false)
					{
						if (*it != '$')
						{
							flag = false;
							break;
						}
					}
					t3.push_back(*it);
				}
				t2.append("..");
				if (flag == true)
				{
					
					finalExpression.push_back(t1);
					finalExpression.push_back(t2);
					finalExpression.push_back(t3);	

					if (i < expression.size() - 1 && isOperation(expression[i + 1], operations) == false && isOperation(expression[i], operations) == false)
					{
						finalExpression.emplace_back("or");// case no operation between two word 
					}
				}
				else if (flag == false)
				{
					finalExpression.emplace_back(expression[i]);
					if (i < expression.size() - 1 && isOperation(expression[i + 1], operations)==false && isOperation(expression[i], operations) == false)
						finalExpression.emplace_back("or");// case no operation between two word 
				}
			}
			else
			{
				finalExpression.emplace_back(expression[i]);
				if (i < expression.size() - 1 && isOperation(expression[i + 1], operations)==false && isOperation(expression[i], operations) == false)
					finalExpression.emplace_back("or");// case no operation between two word 
			}
			
		}
		else
		{
			finalExpression.emplace_back(expression[i]);                                            
			if (i < expression.size() - 1 && isOperation(expression[i + 1], operations) == false  && isOperation(expression[i], operations) == false)
				finalExpression.emplace_back("or");// case no operation between two word 
		}
	}
	return finalExpression;
}
bool isCompletelyMatched(string line, string query, int pos)  // Ex: set is not completely matched with setting , set is completely matched with set
{
	if (pos == string::npos) return false;
	if (pos > 0 && pos < line.size() - 1)
	{
		if (line[pos - 1] == ',')
		{
			int temp = query.size() + pos;
			if (line[temp] == ',')
				return true;
		}
	}
	if (pos == 0)
	{
		if (line[query.size()] == ',')
			return true;
	}
	if (pos + query.size() >= line.size())
	{
		if (line[pos-1] == ',')
			return true;
	}
	return false;
}
vector<string> getSynonymList(string query)
{
	vector<string>temp;
	fstream fin;
	fin.open("synonym.txt");
	string line;
	if (fin.is_open())
	{
		while (!fin.eof())
		{
			getline(fin, line);
			if (isCompletelyMatched(line, query, line.find(query, 0)) == true)
			{
				string s;
				string::iterator it = line.begin();
				while (it != line.end())
				{

					if (*it != ',')
						s.push_back(*it);
					else if (*it == ',')
					{
						temp.push_back(s);
						s.clear();
					}
					++it;
					if (it == line.end())
					{
						if (s.empty() == false)
							temp.push_back(s);
						break;
					}
				}
				break;
			}
		}
	}
	fin.close();
	return temp;
}
QueryAnswer getSynonymSet(Token token)
{
	vector<string> synonymList = getSynonymList(token);
	QueryAnswer currentSet;
	if (synonymList.empty())
		return currentSet;
	else
	{
		for (int i = 0; i < synonymList.size(); ++i)
		{
			currentSet=Or(currentSet, Search(synonymList[i]));
		}
	}
	return currentSet;
}