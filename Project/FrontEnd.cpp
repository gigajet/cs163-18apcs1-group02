﻿#include "FrontEnd.h"
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
	Token keyword; getline(cin,keyword,'\n'); return keyword;
}
void SearchOption(char ch,Token& keyword) {
	if (ch == 8)
	{
		gotoxy(14 + keyword.size(), 2);
		if (keyword.size() != 0) {
			keyword.pop_back();
			gotoxy(14 + keyword.size() + 1, 2); cout << '\b' << " " << '\b';
		}
	}
	else//thêm ký tự vào từ khóa
	{
		keyword.push_back(ch);
		gotoxy(14 + keyword.size() - 1, 2); cout << ch;
	}
}
void DetailFile(int fileindex,vector<string> path)//hiện file chi tiết
{
	//cin.ignore(1000, '\n');
	system("cls");
	string fileout;
	ifstream fin;
	fin.open(path[fileindex]);
	gotoxy(2, 1);
	while (!fin.eof())
	{
		getline(fin, fileout, ' ');
		cout << fileout << " ";
	}
	fin.close();
	gotoxy(0, 0); cout << "Want to leave? Press 0:";
	char a = _getch();
	while (a != '0') {
		gotoxy(24,0);
		a = _getch();
	}
	fflush(stdin);
	system("cls");
}
void Review5file(vector<string> path) {
	for (int i = 0; i < path.size(); i++)
	{
		gotoxy(5, 5 + 10 * i);
		ifstream fin;
		fin.open(path[i]);
		//In title 
		string out;
		getline(fin, out, '\n');
		color(9);
		cout << out ;
		//In 50 từ kể từ title
		int count = 0;
		gotoxy(5, 5 + 10 * i + 3); color(7); cout << "...";
		while (!fin.eof() && count <= 50)
		{
			++count;
			getline(fin, out, ' ');
			for (int i = 0; i < out.size(); i++)
			{
				if (out[i] == '\n') {
					out[i] = '.';
					out.insert(out.begin()+i,' ');
				}
			}
			cout << out << " ";
		}
		cout << "... " ;
		color(11); cout <<'\n'<< "Source: " << path[i];
		fin.close();
	}
}
bool showResultandSearchdemo(Token &keyword, vector<string> path,vector<string>& history) {
	//Show result
	fflush(stdin);
	system("cls");
	//Cập nhật lịch sử
	if (history.size() < 5) history.push_back(keyword);
	for (int i = history.size()-1; i>=1; i--)
		history[i] = history[i-1];
	history[0] = keyword;
	
	//Trường hợp ko tìm thấy từ khóa
	if (path.size() == 0)
	{
		color(10);
		gotoxy(5, 3); cout << "Press tab to search more keywords";
		gotoxy(5, 5);
		cout << "Oops. There're no result for: "; color(11); cout << keyword;
		gotoxy(5, 6);
		color(10);
		cout << "You might want to check your spelling or try something else";
		//find another word
	}
	//Review 5 file
	else {
		Review5file(path);
		//Search new keyword
		color(10);
		gotoxy(5, 3); cout << "Press tab to search more keywords";
		gotoxy(5, 2);
		cout << "Go to file detail: ";
	}
	color(7);
	while (1) {
		if (_kbhit())
		{
			char ch = _getch();
			while (ch != 9 && ch != 27) {
				if (ch >= 48 && ch <= 47+path.size()) {//di chuyển đến 1 file để đọc chi tiết
					int fileindex = int(ch) - 48;
						DetailFile(fileindex, path);
						Review5file(path);
						color(10);
					gotoxy(5, 3); cout << "Press tab to search more keywords";
					gotoxy(5, 2); color(10);
					cout << "Go to file detail: ";
					color(7);
				}
				ch = _getch();
			}
			if (ch==9) {//Giao diện phần tìm kiếm tiếp theo
				system("cls");
				for (int i = 0; i < history.size(); i++)
				{
					gotoxy(14, 4 + i);
					cout << history[i];
				}
				gotoxy(5, 2); color(10);
				cout << "Search >>"; color(7); cout << keyword;
				ch = _getch();
				while (ch != 13&&ch!=27) {
					SearchOption(ch, keyword);
					ch = _getch();
				}
				if (ch == 13)
					return true;//Thoát ra để tìm kiếm tiếp tục
			}
			if(ch==27)//Thoát
			{
				system("cls");
				cout << "Good bye" << endl;
				return false;
			}
		}
	}
}
void showResultandSearch(Token keyword) {
	//Giao diện Search
	Global* g = Global::GetInstance();
	vector<string> path, history;
	//fflush(stdin);
	
	set<int> res = g->trie.Search(keyword, false);

	vector<int>a = Top5Result(res, { keyword });

	for (int i : a) path.push_back(g->fileName[i]);

	bool keepsearching = showResultandSearchdemo(keyword, path,history);

	while (keepsearching)
	{
		system("cls");

		path.clear();

		res = g->trie.Search(keyword, false);

		vector<int>b = Top5Result(res, {keyword});

		for (int i : b) path.push_back(g->fileName[i]);

		keepsearching = showResultandSearchdemo(keyword, path ,history);
	}
	
}
