#ifndef MYSQUARE_H
#define MYSQUARE_H
#include <SFML/Graphics.hpp>

using namespace sf;

class MySquare
{
public:
	int x;
	Color color;
	MySquare()
	{
		this->x = 0;
		color = Color(255, 255, 255, 255);
	}
	MySquare(const int & _x, const Color & _color)
	{
		this->x = _x;
		this->color = _color;
	}
	MySquare(const int & _x, const int &r, const int &g, const int &b, const int &a)
	{
		this->x = _x;
		this->color = Color(r, g, b, a);
	}
};

#endif