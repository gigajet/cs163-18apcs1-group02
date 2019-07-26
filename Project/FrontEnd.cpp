#include "FrontEnd.h"
void gotoxy(int x, int y) {
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}
void color(int x)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, x);
}
void paintFrame(int x, int y, int height, int width) {
	for (int i = 1; i < width; i++) {
		gotoxy(i + y, x);
		cout << (char)205;
		gotoxy(i + y, x + height - 1);
		cout << (char)205;
	}
	for (int i = 1; i < height - 1; i++) {
		gotoxy(y, i + x);
		cout << (char)186;
		gotoxy(width + y - 1, i + x);
		cout << (char)186;
	}
	gotoxy(1, 1);
	cout << (char)201;
	gotoxy(width, 1);
	cout << (char)187;
	gotoxy(1, height);
	cout << (char)200;
	gotoxy(width, height);
	cout << (char)188;
}
void SEARCH() {
	gotoxy(3, 3);
	color(170);
	//S
	for (int i = 0; i < 20; i++) {
		gotoxy(3 + i, 3);
		cout << " ";
		gotoxy(3 + i, 6);
		cout << " ";
		gotoxy(3 + i, 9);
		cout << " ";
	}
	for (int i = 0; i < 3; i++) {
		gotoxy(3, 3 + i);
		cout << "        ";
		gotoxy(15, 6 + i);
		cout << "        ";
	}

	//E

	gotoxy(24, 5);
	cout << "          ";
	gotoxy(24, 7);
	cout << "          ";
	gotoxy(24, 9);
	cout << "          ";
	for (int i = 0; i < 5; i++) {
		gotoxy(24, 5 + i);
		cout << "   ";
	}
	//A
	gotoxy(35, 5);
	cout << "          ";
	gotoxy(35, 7);
	cout << "          ";
	gotoxy(35, 9);
	cout << "          ";
	for (int i = 0; i < 5; i++) {
		gotoxy(42, 5 + i);
		cout << "   ";
		gotoxy(35, 7 + i / 2);
		cout << "   ";
	}
	//R
	gotoxy(46, 5);
	cout << "          ";
	gotoxy(46, 9);
	cout << "   ";
	for (int i = 0; i < 5; i++) {
		gotoxy(46, 5 + i);
		cout << "   ";
	}
	//C
	gotoxy(57, 5);
	cout << "          ";
	gotoxy(57, 9);
	cout << "          ";
	for (int i = 0; i < 5; i++) {
		gotoxy(57, 5 + i);
		cout << "   ";
	}
	//H
	gotoxy(68, 3);
	cout << "   ";
	gotoxy(68, 7);
	cout << "          ";
	gotoxy(68, 9);
	cout << "   ";
	for (int i = 0; i < 5; i++) {
		gotoxy(68, 3 + i);
		cout << "   ";
		gotoxy(68, 7 + i / 2);
		cout << "   ";
		gotoxy(75, 7 + i / 2);
		cout << "   ";
	}
}
void ENGINE()
{
	color(200);
	//E
	for (int i = 0; i < 20; i++) {
		gotoxy(3 + i, 11);
		cout << " ";
		gotoxy(3 + i, 14);
		cout << " ";
		gotoxy(3 + i, 17);
		cout << " ";
	}
	for (int i = 0; i < 6; i++) {
		gotoxy(3, 11 + i);
		cout << "      ";
	}
	//N

	for (int i = 0; i < 5; i++) {
		gotoxy(24, 13 + i);
		cout << "   ";
		gotoxy(32, 13 + i);
		cout << "   ";
		gotoxy(24 + i * 2, 13 + i);
		cout << "   ";
	}
	//G

	gotoxy(36, 13);
	cout << "          ";
	gotoxy(41, 15);
	cout << "     ";
	gotoxy(36, 17);
	cout << "          ";
	for (int i = 0; i < 5; i++) {
		gotoxy(36, 13 + i);
		cout << "   ";
		gotoxy(43, 15 + i / 2);
		cout << "   ";
	}
	//I
	for (int i = 0; i < 5; i++) {
		gotoxy(47, 13 + i);
		cout << "   ";
	}
	//N

	for (int i = 0; i < 5; i++) {
		gotoxy(51, 13 + i);
		cout << "   ";
		gotoxy(59, 13 + i);
		cout << "   ";
		gotoxy(51 + i * 2, 13 + i);
		cout << "   ";
	}
	//E

	gotoxy(63, 13);
	cout << "          ";
	gotoxy(63, 15);
	cout << "          ";
	gotoxy(63, 17);
	cout << "          ";
	for (int i = 0; i < 5; i++) {
		gotoxy(63, 13 + i);
		cout << "   ";
	}
}

Token showLogo() {
	SEARCH();
	ENGINE();
	//MANAGEMENT();
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
	gotoxy(35, 19);
	color(11);
	cout << "Group 02 project"; color(7);
	gotoxy(3, 23); cout << "Search >>";
	Token keyword; //cin >> keyword; return keyword;
	//cin.ignore(100, '\n');
	getline(cin, keyword);
	return keyword;
}

void colorText(int colour, int textColor, string text, int x, int y, int width)
{
	gotoxy(x, y);
	color(colour);
	cout << (char)186;
	color(textColor);
	cout << setw(width);
	cout << text;
	color(8);
}
bool showResultandSearchdemo(Token &keyword, Expression path,Expression history) {
	//Show result
	gotoxy(5, 2); color(10);
	cout << "Search >>"; color(7); cout << keyword;
	for (int i = 0; i < history.size(); i++)
	{
		gotoxy(14, 4 + i);
		cout << history[i];
	}
	if (history.size() < 5)
		history.push_back(keyword);
	else {
		history.pop_back();
		history.push_back(keyword);
	}
	fflush(stdin);
	int numoffile = min(path.size(), 5);
	for (int i = 0; i < numoffile; i++)
	{
		gotoxy(5, 12 + 10 * i);
		ifstream fin;
		fin.open(path[i]);
		//In title 
		string out;
		getline(fin, out, '.');
		color(9);
		cout << out << ".";
		//In 100 ký tự kể từ title
		int count = 0;
		gotoxy(5, 12 + 10 * i + 3); color(7); cout << "...";
		while (!fin.eof() && count <= 50)
		{
			++count;
			getline(fin, out, ' ');
			for (int i = 0; i < out.size(); i++)
			{
				if (out[i] == '\n')
					out.erase(out.begin() + i);
			}
			cout << out << " ";
		}
		cout << "... " << endl;
		color(11); cout << "Source: " << path[i];
		fin.close();
	}
	//Search new keyword
	gotoxy(14 + keyword.size(), 2);
	color(7);
	while (1) {
		if (_kbhit())
		{
			char ch = _getch();
			while (ch != 13 && ch != 27) {
				if (ch == 8)
				{
					if (keyword.size() != 0) {
						keyword.pop_back();
						gotoxy(14 + keyword.size() + 1, 2); cout << '\b' << " " << '\b';
					}
					ch = _getch();
				}
				else
				{
					keyword.push_back(ch);
					gotoxy(14 + keyword.size() - 1, 2); cout << ch;
					ch = _getch();
				}
			}
			if (ch == 13) {
				system("cls");
				return true;
			}
			else
			{
				system("cls");
				cout << "Good bye" << endl;
				return false;
			}
		}
	}
}
void showResultandSearch(Token keyword) {
	system("cls");
	fflush(stdin);

	Expression e = RefineToken(keyword);

	for (auto i : e)
		cout << i << endl;
	cout << "========================" << endl;

	e = ConvertToRPN(e);

	for (auto i : e)
		cout << i << endl;

	system("pause");


	Expression path, history;
	Global* g = Global::GetInstance();
	history.push_back(keyword);
	//set<int> res = g->trie.Search(keyword, false);

	QueryAnswer res = CalculateRPN(e);

	vector<int>a = Top5Result(res, e);
	for (int i : a) path.push_back(g->fileName[i]);
	bool keepsearching = showResultandSearchdemo(keyword, path,history);
	while (keepsearching)
	{
		system("cls");
		Expression temp;
		/*res = g->trie.Search(keyword, false);
		vector<int>b = Top5Result(res, {keyword});*/
		//for (int i : b) temp.push_back(g->fileName[i]);

		Expression e = RefineToken(keyword);
		e = ConvertToRPN(e);

		res = CalculateRPN(e);
		vector<int> b = Top5Result(res, e);
		for (int i : b) temp.push_back(g->fileName[i]);

		if (res.size() == 0)
		{
			gotoxy(7, 7);
			cout << "Oops. There're no result for: "; color(11); cout << keyword;
			gotoxy(7, 8);
			cout << "You might want to check your spelling or try something else";
			//find another word
		}
		keepsearching = showResultandSearchdemo(keyword, temp, history);
	}
}