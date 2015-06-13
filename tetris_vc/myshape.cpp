#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "tetris.h"
#include "myshape.h"
#include "mysquare.h"

using namespace std;
using namespace sf;

MyShape::MyShape()
{
	for (int i = 0; i <= 4; ++i)
		for (int j = 0; j <= 4; ++j)
			a[i][j] = MySquare(0, Color::White);
}

MyShape::MyShape(const string file)
{
	ifstream f(file);
	for (int i = 0; i <= 4; ++i)
	{
		for (int j = 0; j <= 4; ++j)
		{
			a[i][j].x = 0;
		}
	}
	for (int i = 1; i <= 4; ++i)
	{
		for (int j = 1; j <= 4; ++j)
		{
			f >> a[i][j].x;
		}
	}
	int r, g, b, alfa;
	f >> r >> g >> b >> alfa;
	f.close();
	Color color(r, g, b, alfa);
	for (int i = 1; i <= 4; ++i)
		for (int j = 1; j <= 4; ++j)
		{
			if (a[i][j].x == 1)
				a[i][j].color = color;
		}
}

void MyShape::RotateLeft()
{
	MySquare b[5][5];
	for (int i = 1; i <= 4; ++i)
		for (int j = 1; j <= 4; ++j)
			b[4 - j + 1][i] = a[i][j];
	for (int i = 1; i <= 4; ++i)
		for (int j = 1; j <= 4; ++j)
			a[i][j] = b[i][j];
}

void MyShape::RotateRight()
{
	MySquare b[5][5];
	for (int i = 1; i <= 4; ++i)
		for (int j = 1; j <= 4; ++j)
			b[j][4-i+1] = a[i][j];
	for (int i = 1; i <= 4; ++i)
		for (int j = 1; j <= 4; ++j)
			a[i][j] = b[i][j];
}
