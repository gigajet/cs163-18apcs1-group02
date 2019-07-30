#include "Global.h"
#include <variant>
#include <algorithm>

#define TESTING_PHASE

using namespace std;


//Return 0 if just a token, positive integer if operator. The higher integer is, the more precedence.
int Precedence(Token token) {
	if (token == "|") return 1;
	if (token == "&") return 2;
	if (token == "-") return 3;
	if (token == searchOp) return 4;
	if (token == exactOp) return 4;
	if (token == "..") return 8; //Cập nhật ưu tiên cho số: cao nhất
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
	for (auto i = 0; i < e.size(); ++i) {
		Token token = e[i];
		//Giả sử là query được chuyển hết thành chữ thường, cắt chuỗi các thứ rồi.
		//token = RefineToken(token);

		int pred = Precedence(token);
		if (pred == 0) {//not an operator 
			if (token[0] == '\"') {
				token.erase(0, 1); token.pop_back();
				ans.push_back(token);
				if (op.empty() || (op.top() != "intitle") && (op.top() != "~"))
					ans.push_back(exactOp);
			}
			else if (isNumber(token)
				&& i + 2 < e.size() && e[i + 1] == ".." && isNumber(e[i + 2]))
			{ //Number range
				ans.push_back(token);
			}
			else if (isPrice(token)
				&& i + 2 < e.size() && e[i + 1] == ".." && isPrice(e[i + 2])) //Price range
			{
				ans.push_back(token);
			}
			else {
				ans.push_back(token);
				if (op.empty() || (op.top() != "intitle") && (op.top() != "~") && (op.top() != ".."))
					ans.push_back(searchOp);
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


QueryAnswer CalculateRPN(Expression rpn) {
	stack<variant<Token, QueryAnswer, int> > st;
	bool invalidExpression = 0;
	variant<Token, QueryAnswer, int> tmp1(0), tmp2(0);
	try {
		for (auto token : rpn)
			if (Precedence(token) > 0) { //an operator
				if (token == "|") {
					if (st.size() < 2) throw 1;
					tmp1 = st.top(); st.pop();
					tmp2 = st.top(); st.pop();
					QueryAnswer k1 = get<QueryAnswer>(tmp1), k2 = get<QueryAnswer>(tmp2);
					st.push(Or(k1, k2));

					//cerr << "Or call." << endl;
					//st.push(QueryAnswer());
				}
				if (token == "&") {
					if (st.size() < 2) throw 1;
					tmp1 = st.top(); st.pop();
					tmp2 = st.top(); st.pop();
					QueryAnswer k1 = get<QueryAnswer>(tmp1), k2 = get<QueryAnswer>(tmp2);
					st.push(And(k1, k2));

					//cerr << "And call." << endl;
					//st.push(k1);
				}
				if (token == "-") {
					if (st.size() < 1) throw 1;
					tmp1 = st.top(); st.pop();
					QueryAnswer k1 = get<QueryAnswer>(tmp1);
					st.push(Exclude(k1));

					//cerr << "Exclude call." << endl;
					//st.push(QueryAnswer());
				}
				if (token == searchOp) {
					if (st.size() < 1) throw 1;
					tmp1 = st.top(); st.pop();
					Token k1 = get<Token>(tmp1);
					st.push(Search(k1));

					//cerr << "Search call for: " << k1 << endl;
					//st.push(QueryAnswer());
				}
				if (token == exactOp) {
					if (st.size() < 1) throw 1;
					tmp1 = st.top(); st.pop();
					Token k1 = get<Token>(tmp1);
					st.push(Exact(k1));

					//cerr << "Exact call for: " << k1 << endl;
					//st.push(QueryAnswer());
				}
				if (token == "intitle") {
					if (st.size() < 1) throw 1;
					tmp1 = st.top(); st.pop();
					Token k1 = get<Token>(tmp1);
					st.push(InTitle(k1));

					//cerr << "Intitle call for: " << k1 << endl;
					//st.push(QueryAnswer());
				}
				if (token == "..") {
					if (st.size() < 2) throw 1;
					tmp1 = st.top(); st.pop();
					tmp2 = st.top(); st.pop();
					Token k1 = get<Token>(tmp1), k2 = get<Token>(tmp2);
					if (isNumber(k1))
						st.push(NumberRange(k1, k2));
					else st.push(PriceRange(k1, k2));
				}
				if (token == "~")
				{
					if (st.size() < 1) throw 1;
					tmp1 = st.top(); st.pop();
					Token k1 = get<Token>(tmp1);
					st.push(getSynonymSet(k1));
				}
			}
			else { //a token
				st.push(token);
			}

		if (st.size() < 1) throw 1;
		QueryAnswer ans = get<QueryAnswer>(st.top());
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

			while (tok != "" && (tok[0] == '*'))
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
				while (state != 0 && g[state].find(a) == g[state].end())
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
			c = tolower(c);
			offSet++; lineOffset++;

			while (state != 0 && g[state].find(c) == g[state].end())
				state = fail[state];
			state = (g[state].find(c) == g[state].end() ? 0 : g[state][c]);

			//A match is found.
			if (out[state] != -1 || outNext[state] != -1) {
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
					//FOR FIRST TOKEN IN THE EXACT ONLY.
					if (additionalCheck[id] == -1)
					{
						int len = (int)processedToken[id].length();
						if (offSet - len - 1 >= 0) {
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
						//cerr << "Fileid " << fileIndex << " Ln " << lineNo << " Col " << lineOffset << endl;
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

QueryAnswer Search(Token keyword) {
	Global *g = Global::GetInstance();
	return g->trie.Search(keyword, false);
}

QueryAnswer Exact(Token keyword) {
	set<int> lst;
	for (auto i = 0; i < Global::GetInstance()->fileName.size(); ++i)
		lst.insert(i);

	string cur("");
	string loweredKw("");
	for (char c : keyword) {
		c = tolower(c);
		if (c == ' ' || c == '\n') {
			if (cur != "" && cur != "*") {
				cout << cur;
				QueryAnswer k = Search(cur);
				//cerr << cur << "'s size: " << k.size() << endl;
				lst = And(lst, k);
				cur.clear();
			}
		}
		else cur += c;
		loweredKw += c;
	}
	//Last sub-token
	if (cur != "" && cur != "*") {
		lst = And(lst, Search(cur));
	}
	//cerr << cur;
	//cerr << "lst's size: " << lst.size() << endl;

	auto ans = AhoCorasick(lst, { "\"" + loweredKw + "\"" });
	return ans[0];
}

QueryAnswer InTitle(Token keyword) {
	Global *g = Global::GetInstance();
	return g->trie.Search(keyword, true);
}

bool isNumber(string token) {
	return all_of(token.begin(), token.end(), isdigit);
}
bool isPrice(string token) {
	if (token[0] != '$') return false;
	token.erase(0, 1); if (token.length() == 0) return false;
	return isNumber(token);
}
bool hasNumberPrefix(string token) {
	if (token.length() == 0) return false;
	return isdigit(token[0]);
}
bool hasPricePrefix(string token) {
	if (token.length() < 1) return false;
	if (token[0] != '$') return false;
	token.erase(0, 1);
	return hasNumberPrefix(token);
}

QueryAnswer PriceRange(Token a, Token b) {
	Global *g = Global::GetInstance();
	QueryAnswer ans;
	a.erase(0, 1); b.erase(0, 1);
	if (cmpNumber(b, a)) swap(a, b);
	for (int i = 0; i < (int)g->fileName.size(); ++i) {
		if (upper_bound(g->priceList[i].begin(), g->priceList[i].end(), b) >
			lower_bound(g->priceList[i].begin(), g->priceList[i].end(), a)) {
			ans.insert(i);
		}
	}
	return ans;
}
QueryAnswer NumberRange(Token a, Token b) {
	Global *g = Global::GetInstance();
	QueryAnswer ans;
	a.erase(0, 1); b.erase(0, 1);
	//if (A > B) swap(A, B);
	if (cmpNumber(b, a)) swap(a, b);
	for (int i = 0; i < (int)g->fileName.size(); ++i) {
		if (upper_bound(g->numberList[i].begin(), g->numberList[i].end(), b) >
			lower_bound(g->numberList[i].begin(), g->numberList[i].end(), a)) {
			ans.insert(i);
		}
	}
	return ans;
}


//tokenList là token mặc định. Hàm thứ hai trong kế hoạch 27/7
//trả về vị trí khi đọc bằng fin.get(char) rồi tăng biến đêm với ký tự đầu là 1. Không trả tellg()
//TODO Lỗi highlight nếu "De*De*De"
//TODO Lỗi match nếu trước đầu exact không phải khoảng trắng hoặc \n
set<int> GetHighlightPosition(string filename, vector<Token> tokenList) {

	const int AsteriskMatchLength = 10;

	vector<string> processedToken;

	//Single exact without asterisk : additionalCheck = -1
	/*
	t1*t2*...*tk : additionalCheck[t2]=t1, meaning match raised in Trie if and only if
	the last previous token match is within AsteriskMatchLength(10) character about the current token.
	*/
	vector<int> additionalCheck;

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

			while (tok != "" && (tok[0] == '*'))
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
				while (state != 0 && g[state].find(a) == g[state].end())
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

	set<int> ans;
	string buffer("");
	int lineNo = 1;
	int lineOffset = 1;
	int offSet = 0;
	fill(lastMatchOffset, lastMatchOffset + (int)processedToken.size(), -AsteriskMatchLength);

	int state = 0;

	char c;
	ifstream fin; fin.open(filename);
	if (!fin.is_open()) {
		cerr << "Dit me may! File bi loi. GlobalM.cpp, GetHighlightPosition part 3." << endl;
		cerr << "File: " << filename << endl;
	}
	while (fin.get(c)) {
		buffer += c;
		if (c == '\n') {
			lineNo++;
			lineOffset = 0;

			/*
			//If '\n' is counted as a character, then do below line.
			offSet++;
			continue;
			*/
		}
		c = tolower(c);
		offSet++; lineOffset++;

		while (state != 0 && g[state].find(c) == g[state].end())
			state = fail[state];
		state = (g[state].find(c) == g[state].end() ? 0 : g[state][c]);

		//A match is found.
		if (out[state] != -1 || outNext[state] != -1) {
			//Get list of matched id of processedToken
			vector<int> matchList;
			for (int st = state; st != -1; st = outNext[st])
				if (out[st] != -1)
					matchList.push_back(out[st]);


			for (auto id : matchList) {
				//Is it really a match?
				if (additionalCheck[id] != -1) {
					int ad = additionalCheck[id];
					if (offSet - lastMatchOffset[ad] > AsteriskMatchLength)
						continue;
				}
				//ADDITIONAL: must be the beginning of a word
				//FOR FIRST TOKEN IN THE EXACT ONLY.
				if (additionalCheck[id] == -1)
				{
					int len = (int)processedToken[id].length();
					if (offSet - len - 1 >= 0) {
						if (buffer[(int)buffer.length() - len - 1] != ' '
							&& buffer[(int)buffer.length() - len - 1] != '\n')
							continue;
					}
				}
				lastMatchOffset[id] = offSet;

				/*
				  Add to highlight list: must be last sub-token (originalTokenId != -1)
					1. If it is not an exact with wildcard (i.e additionalCheck == -1), all of them
					2. If it is exact, then traverse back to find the first token. From that, get till current
				*/

				if (originalTokenListId[id] != -1) { //Surevkl last token
					if (additionalCheck[id] == -1) { //Non-exact match or exact with non-wildcard
						int len = (int)processedToken[id].length();
						for (int i = max(0, offSet - len + 1); i <= offSet; ++i)
							ans.insert(i);
					}
					else { //Last sub-token of a wildcard search: Traverse back to first token
						int firstId = id;
						while (additionalCheck[firstId] != -1) firstId = additionalCheck[firstId];
						int len = (int)processedToken[firstId].length();
						int firstPos = max(0, lastMatchOffset[firstId] - len + 1);
						for (int i = firstPos; i <= offSet; ++i)
							ans.insert(i);
					}
				}

				//Check if real token, not a sub-token
#ifdef TESTING_PHASE
					//cerr << "Fileid " << fileIndex << " Ln " << lineNo << " Col " << lineOffset << endl;
#endif
			}
		}

		if (buffer.size() > maxBufferSize)
			buffer.erase(0, maxBufferSize / 2);
	}

	fin.close();

	delete[] lastMatchOffset;
	delete[] fail;
	delete[] outNext;
	delete[] out;
	delete[] g;

	return ans;
}

//Hàm thứ nhất trong kế hoạch 27/7. Từa tựa CalculateRPN
vector<Token> GetHighlightToken(int globalIndex, Expression rpn) {
	vector<Token> v;
	stack<Token> st;
	for (Token token : rpn) {
		if (Precedence(token) > 0) {
			if (token == searchOp) {
				Token tmp1 = st.top(); st.pop();
				v.push_back(tmp1);
				if (isNumber(tmp1))
					v.push_back(NumberCommaForm(tmp1));
				if (isPrice(tmp1))
					v.push_back(PriceCommaForm(tmp1));
			}
			if (token == exactOp) {
				v.push_back("\"" + st.top() + "\""); st.pop();
			}
			if (token == "..") {
				Token tmp1 = st.top(); st.pop();
				Token tmp2 = st.top(); st.pop();
				if (cmpNumber(tmp2, tmp1)) swap(tmp1, tmp2);
				Global *g = Global::GetInstance();
				if (isNumber(tmp1)) {
					for (auto it = lower_bound(g->numberList[globalIndex].begin(), g->numberList[globalIndex].end(), tmp1, cmpNumber);
						it != upper_bound(g->numberList[globalIndex].begin(), g->numberList[globalIndex].end(), tmp2, cmpNumber);
						it++) {
						//cerr << *it << endl;
						v.push_back(*it);
						v.push_back(NumberCommaForm(*it));
					}
				}
				else { //Price
					for (auto it = lower_bound(g->priceList[globalIndex].begin(), g->priceList[globalIndex].end(), tmp1, cmpNumber);
						it != upper_bound(g->priceList[globalIndex].begin(), g->priceList[globalIndex].end(), tmp2, cmpNumber);
						it++) {
						//cerr << *it << endl;
						v.push_back("$" + *it);
						v.push_back("$" + NumberCommaForm(*it));
					}
				}
			}
			if (token == "~")
			{
				vector<string> syn = getSynonymList(st.top()); st.pop();
				v.insert(v.end(), syn.begin(), syn.end());
			}
		}
		else { //a token
			st.push(token);
		}
	}
	return v;
}

//Vị trí khi dùng fin.get() với biến Count. Ký tự đầu file tính là 1.
set<int> GetHighlightInfo(int globalIndex, Expression rpn, string filename) {
	vector<Token> v = GetHighlightToken(globalIndex, rpn);
	return GetHighlightPosition(filename, v);
}

//"5000000" -> "5,000,000"
Token NumberCommaForm(Token numToken) {
	string ans(""); int count = 0;
	for (int i = (int)numToken.length() - 1; i >= 0; --i) {
		ans += numToken[i]; count++;
		if (count % 3 == 0) ans += ',';
	}
	while (!ans.empty() && ans.back() == ',') ans.pop_back();
	reverse(ans.begin(), ans.end());
	return ans;
}

//"$5000" -> "$5,000"
Token PriceCommaForm(Token priceToken) {
	priceToken.erase(0, 1);
	Token ans = NumberCommaForm(priceToken);
	return "$" + ans;
}

string getNumber(string word) {
	string ans("");
	for (int i = 0; i < (int)word.length(); ++i)
		if (isdigit(word[i]))
			ans += word[i];
		else break;
	while (!ans.empty() && ans[0] == '0') ans.erase(0, 1);
	if (ans.empty()) ans += '0';
	return ans;
}

bool cmpNumber(string a, string b) {
	if (a.length() < b.length()) return 1;
	if (a.length() > b.length()) return 0;
	return a < b;
}