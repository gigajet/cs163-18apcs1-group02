#pragma once
#ifndef FRONTEND_H_INCLUDED
#define FRONTEND_H_INCLUDED

#include <iostream>
#include <cwchar>
#include <windows.h>
#include <conio.h>
#include<iomanip>
#include<string>
#include <vector>
#include <fstream>
#include<Windows.h>
#include"Global.h"
#include<conio.h>
using namespace std;
typedef string Token;
typedef vector<Token> Expression;

void gotoxy(int x, int y);
void color(int x);
void paintFrame(int x, int y, int height, int width);
//void showName(const char* title);
Token showLogo();
void colorText(int colour, int textColor, string text, int x, int y, int width);
void showResultandSearch(Token keyword);
#endif // FRONTEND_H_INCLUDED