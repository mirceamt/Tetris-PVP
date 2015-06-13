#ifndef MYSHAPE_H
#define MYSHAPE_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "tetris.h"
#include "mysquare.h"

using namespace std;
using namespace sf;

class MyShape
{
public:
	MySquare a[5][5];
	MyShape();
	MyShape(const string file);
	void RotateLeft();
	void RotateRight();
};



#endif SHAPE_H