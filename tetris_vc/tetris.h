#ifndef TETRIS_H
#define TETRIS_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <SFML/Graphics.hpp>
#include "myshape.h"
#include "mysquare.h"
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

using namespace std;
using namespace sf;

const int dx[] = { -1, 0, 1, 0 };
const int dy[] = { 0, 1, 0, -1 };

class Tetris
{
public :

	const double speed_init = 10;
	const double max_distance = 10000.0;
	double speed1, speed2, distance1, distance2;
	int difficulty;
	int cnt_shapes;
	int score1, score2, overallscore1, overallscore2;
	vector <MyShape> shapes;
	MySquare mat[20][50];
	MyShape shape1, shape2;
	int x1, y1, x2, y2;
	int line_between;
	Texture mTexture;
	string nameP1, nameP2;
	Tetris();
	Tetris(const char * namePlayer1, const char * namePlayer2);
	void DrawGrid(RenderWindow * window);
	void DrawTable(RenderWindow * window);
	void DrawBetweenLine(RenderWindow * window);
	void DrawFinishedGame(const int winner, RenderWindow * window, const int & choice);
	void DrawPause(RenderWindow * window);
	void DrawScore(RenderWindow * window);
	bool Move(const int & direction, const MyShape & s, int & x, int & y);
	bool RotateLeft(const int player, MyShape & s, const int & x, const int & y);
	bool RotateRight(const int player, MyShape & s, const int & x, const int & y);
	bool CreateShape1();
	bool CreateShape2();
	bool Check1(int & opponent_loses);
	bool Check2(int & opponent_loses);
	void Init(const int & choice);


private:
	void InitData();
	void InitData(const int & choice);

};

#endif