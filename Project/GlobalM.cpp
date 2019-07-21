#include "Global.h"
#include <variant>

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
			if (token[0] == '\"')
				ans.push_back("exact");
			else ans.push_back("search");
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

//DEBUG STATE
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
					Token k1 = get<Token>(tmp1);
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