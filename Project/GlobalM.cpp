#include "Global.h"
#include <variant>
#include <algorithm>

#define TESTING_PHASE

using namespace std;
//Return 0 if just a token, positive integer if operator. The higher integer is, the more precedence.
int Precedence(Token token) {
	if (token == "or") return 1;
	if (token == "and") return 2;
	if (token == "-") return 3;
	if (token == "search") return 4;
	if (token == "exact") return 4;
	if (token == "..") return 5;
	//if (token == "#") return 6; //Tạm thời hashtag xem như cụm bình thường
	if (token == "intitle") return 6;
	if (token == "~") return 7;
	return 0;
}

//I love you and him -> I search love search him search and
//Shunting-yard algorithm
Expression ConvertToRPN(Expression e) {
	Expression ans;
	stack<Token> op; //operator Stack.
	for (auto token : e) {

		//Giả sử là query được chuyển hết thành chữ thường, cắt chuỗi các thứ rồi.
		//token = RefineToken(token);

		int pred = Precedence(token);
		if (pred == 0) {//not an operator 
			ans.push_back(token);

			if (token[0] == '\"') {
				if (op.empty() || (op.top()!="intitle") && (op.top()!="~"))
					ans.push_back("exact");
			}
			else {
				if (op.empty() || (op.top() != "intitle") && (op.top() != "~"))
					ans.push_back("search");
			}
		}
		else {
			while (!op.empty()
					&& Precedence(op.top()) >= pred) {
				ans.push_back(op.top());
				op.pop();
			}
			op.push(token);
		}
	}
	while (!op.empty()) {
		ans.push_back(op.top());
		op.pop();
	}
	return ans;
}

//TODO Edit calling function CalculateRPN
QueryAnswer CalculateRPN(Expression rpn) {
	stack<variant<Token, QueryAnswer, int> > st;
	bool invalidExpression = 0;
	variant<Token, QueryAnswer, int> tmp1(0), tmp2(0);
	try {
		for (auto token : rpn)
			if (Precedence(token) > 0) { //an operator
				if (token == "or") {
					if (st.size() < 2) throw 1;
					tmp1 = st.top(); st.pop();
					tmp2 = st.top(); st.pop();
					QueryAnswer k1 = get<QueryAnswer>(tmp1), k2 = get<QueryAnswer>(tmp2);
					//st.push(Or(k1, k2));

					cerr << "Or call." << endl;
					st.push(QueryAnswer());
				}
				if (token == "and") {
					if (st.size() < 2) throw 1;
					tmp1 = st.top(); st.pop();
					tmp2 = st.top(); st.pop();
					QueryAnswer k1 = get<QueryAnswer>(tmp1), k2 = get<QueryAnswer>(tmp2);
					//st.push(Or(k1, k2));

					cerr << "And call." << endl;
					st.push(k1);
				}
				if (token == "-") {
					if (st.size() < 1) throw 1;
					tmp1 = st.top(); st.pop();
					QueryAnswer k1 = get<QueryAnswer>(tmp1);
					//st.push(Exclude(k1));

					cerr << "Exclude call." << endl;
					st.push(QueryAnswer());
				}
				if (token == "search") {
					if (st.size() < 1) throw 1;
					tmp1 = st.top(); st.pop();
					Token k1 = get<Token>(tmp1);
					//st.push(Search(k1));

					cerr << "Search call for: " << k1 << endl;
					st.push(QueryAnswer());
				}
				if (token == "exact") {
					if (st.size() < 1) throw 1;
					tmp1 = st.top(); st.pop();
					Token k1 = get<Token>(tmp1);
					//st.push(Exact(k1));

					cerr << "Exact call for: " << k1 << endl;
					st.push(QueryAnswer());
				}
				if (token == "intitle") {
					if (st.size() < 1) throw 1;
					tmp1 = st.top(); st.pop();
					Token k1 = get<Token>(tmp1); //problem with this line
					//st.push(Exact(k1));

					cerr << "Intitle call for: " << k1 << endl;
					st.push(QueryAnswer());
				}
				//if (token == "..") ;
				//if (token == "~") ;
			}
			else { //a token
				st.push(token);
			}

		if (st.size() < 1) throw 1;
		//QueryAnswer ans = get<QueryAnswer>(st.top());
		QueryAnswer ans = QueryAnswer();
		return ans;
	}
	catch (int e) {
		if (e == 1) cerr << "Invalid expression." << endl;
		return QueryAnswer();
	}
}

vector<QueryAnswer> AhoCorasick(set<int> fileList, vector<Token> tokenList) {
	const int AsteriskMatchLength = 10;

	vector<string> processedToken;

	//Single exact without asterisk : additionalCheck = -1
	/*
	t1*t2*...*tk : additionalCheck[t2]=t1, meaning match raised in Trie if and only if 
	the last previous token match is within AsteriskMatchLength(10) character about the current token.
	*/
	vector<int> additionalCheck;

	//Return corresponding answer for each token.
	vector<QueryAnswer> ans;
	for (auto i = 0; i < tokenList.size(); ++i)
		ans.push_back(QueryAnswer());

	//processed Token is actually what id in the token list?
	//We need to know in order to update corresponding QueryAnswer
	//originalTokenListId=-1 means that it is a sub-token of a wildcard exact match
	vector<int> originalTokenListId;

	int nMaxState = 1; //one for the root.

	//1. process the token
	int nToken = 0;
	for (auto i = 0; i < tokenList.size(); ++i) {
		string tok = tokenList[i];
		if (tok[0] == '\"') {
			tok.erase(0, 1);
			tok.pop_back(); //assume last is "

			//Trim beginning and trailing asterisks and NO spaces
			//while (tok != "" && (tok[0] == '*' || tok[0]==' ')) 
				//tok.erase(0, 1);
			//while (tok != "" && (tok.back() == '*' || tok.back() == ' '))
				//tok.erase(0, 1);

			while (tok != "" && (tok[0] == '*') )
				tok.erase(0, 1);
			while (tok != "" && (tok.back() == '*'))
				tok.erase(0, 1);

			//Split into sub-tokens with * as delimeter
			string cur(""); bool firstToken = true;
			for (char c : tok) {
				if (c != '*') {
					cur += c;
				}
				else {
					if (!firstToken)
						additionalCheck.push_back((int)processedToken.size() - 1);
					else
						additionalCheck.push_back(-1);
					processedToken.push_back(cur);
					nMaxState += (int)cur.length();
					cur.clear();
					originalTokenListId.push_back(-1);
					firstToken = false;
				}
			}

			//Last sub-token
			if (cur == "") continue;
			if (firstToken)
				additionalCheck.push_back(-1);
			else
				additionalCheck.push_back((int)processedToken.size() - 1);
			processedToken.push_back(cur);
			nMaxState += (int)cur.length();
			firstToken = false;
			originalTokenListId.push_back(i);
		}
		else { //not an exact
			processedToken.push_back(tok);
			additionalCheck.push_back(-1);
			nMaxState += (int)tok.length();
			originalTokenListId.push_back(i);
		}
	}

	//2. Build FSM a.k.a Augmented Trie
	map<char, int> *g = new map<char, int>[nMaxState]; //TRIE implement as array.
	
	//for non-output nodes i, out[i]=-1, otherwise index in processedToken vector.
	int *out = new int[nMaxState]; fill(out, out + nMaxState, -1);

	//if there is next node to be outputted, outNext[i]=index of that node, otherwise -1.
	int *outNext = new int[nMaxState]; fill(outNext, outNext + nMaxState, -1);

	//2.1 Add tokens into trie
	int nState = 0;
	for (int i = 0; i < (int)processedToken.size(); ++i) {
		string tok = processedToken[i];
		int curState = 0;
		for (auto ch : tok) {
			if (g[curState].find(ch) == g[curState].end()) {
				nState++;
				g[curState][ch] = nState;
				curState = nState;
			}
			else curState = g[curState][ch];
		}
		//partial output construction
		out[curState] = i;
	}
	nState++; //0..nState+1;
	//2.2 Build fail function: Algorithm 3, Aho
	int *fail = new int[nMaxState]; fill(fail, fail + nMaxState, -1);
	{
		queue<int> q;
		for (auto p : g[0]) {
			fail[p.second] = 0;
			q.push(p.second);
		}
		while (!q.empty()) {
			int r = q.front(); q.pop();
			for (auto p : g[r]) {
				int s = p.second; char a = p.first;
				q.push(s);
				int state = fail[r];
				while (state!=0 && g[state].find(a) == g[state].end())
					state = fail[state];
				fail[s] = (g[state].find(a) == g[state].end() ? 0 : g[state][a]);
				outNext[s] = (out[fail[s]] != -1 || outNext[fail[s]] != -1) ? fail[s] : -1;
			}
		}
	}

	//Consider: Use f' instead of f to speed up.

	//3. Scan through the text and find match!
	//See also: Algorithm 1, Aho

	const unsigned int maxBufferSize = 512; //Flush left-HALF when buffer is too large.

	//When did last match of a word?
	int *lastMatchOffset = new int[processedToken.size()];

	for (int fileIndex : fileList) {

		string fileName = Global::GetInstance()->fileName[fileIndex];
		string buffer("");
		int lineNo = 1;
		int lineOffset = 1;
		int offSet = 0;
		fill(lastMatchOffset, lastMatchOffset + (int)processedToken.size(), -AsteriskMatchLength);

		int state = 0;

		char c;
		ifstream fin; fin.open(fileName);
		if (!fin.is_open()) {
			//cerr << "Dit me may! File bi loi. GlobalM.cpp, Aho Corasick part 3." << endl;
			//cerr << "File: " << fileName << endl;
			continue;
		}
		while (fin.get(c)) {
			buffer += c;
			if (c == '\n') {
				lineNo++;
				lineOffset = 0;
				//If '\n' is counted as a character, then do below line.
				//offSet++;
				continue;
			}
			offSet++; lineOffset++;

			while (state != 0 && g[state].find(c) == g[state].end())
				state = fail[state];
			state = (g[state].find(c) == g[state].end() ? 0 : g[state][c]);

			//A match is found.
			if (out[state] != -1 || outNext[state]!=-1) {
				//Get list of matched id of processedToken
				vector<int> matchList;
				for (int st = state; st != -1; st = outNext[st])
					if (out[st] != -1)
						matchList.push_back(out[st]);
			

				//TODO AHO Add condition when match: token before is space 
				for (auto id : matchList) {
					//Is it really a match?
					if (additionalCheck[id] != -1) {
						int ad = additionalCheck[id];
						if (offSet - lastMatchOffset[ad] > AsteriskMatchLength)
							continue;
					}
					//ADDITIONAL: must be the beginning of a word
					{
						int len = (int)processedToken[id].length();
						if (offSet - len >= 0) {
							if (buffer[(int)buffer.length() - len - 1] != ' '
								&& buffer[(int)buffer.length() - len - 1] != '\n')
								continue;
						}
					}
					
					lastMatchOffset[id] = offSet;
					//Check if real token, not a sub-token
					if (originalTokenListId[id] != -1) {
						ans[originalTokenListId[id]].insert(fileIndex);
#ifdef TESTING_PHASE
						cerr << "Fileid " << fileIndex << " Ln " << lineNo << " Col " << lineOffset << endl;
#endif
					}
						

				}
			}

			if (buffer.size() > maxBufferSize)
				buffer.erase(0, maxBufferSize / 2);
		}

		fin.close();
	}

	delete[] lastMatchOffset;
	delete[] fail;
	delete[] outNext;
	delete[] out;
	delete[] g;

	return ans;
}