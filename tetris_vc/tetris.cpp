#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <SFML/Graphics.hpp>
#include "tetris.h"
#define OFFSET_X 8
#define OFFSET_Y 288

using namespace std;
using namespace sf;

string ToString(int x)
{
	string ret;
	ret = "";
	if (x == 0)
		ret += '0';
	while (x)
	{
		ret += (char)('0' + x % 10);
		x /= 10;
	}
	reverse(ret.begin(), ret.end());
	return ret;
}

void Tetris::InitData()
{
	shapes.clear();
	nameP1 = "Player1";
	nameP2 = "Player2";
	line_between = 23;
	mTexture.loadFromFile("Resources/Textures/board_texture.jpg");

	if (difficulty == 0)
	{
		cnt_shapes = 7;
	}
	else
	{
		ifstream f;
		f.open("Resources/Shapes/cnt_shapes.txt");
		f >> cnt_shapes;
		f.close();
	}

	for (int now_shape = 1; now_shape <= cnt_shapes; ++now_shape)
	{
		shapes.push_back(MyShape("Resources/Shapes/shape" + ToString(now_shape) + ".txt"));
		//cout << "Resources/Shapes/shape" + ToString(now_shape) + ".txt";
		//cout << "\n";
	}

	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 50; ++j)
			mat[i][j] = MySquare(0, Color::White);

	for (int i = 0; i <= 2; ++i)
		for (int j = 0; j <= 46; ++j)
			mat[i][j] = MySquare(1, Color::White);
	for (int i = 13; i <= 15; ++i)
		for (int j = 0; j <= 46; ++j)
			mat[i][j] = MySquare(1, Color::White);

	for (int i = 0; i <= 15; ++i)
		for (int j = 0; j <= 2; ++j)
			mat[i][j] = MySquare(1, Color::White);
	for (int i = 0; i <= 15; ++i)
		for (int j = 43; j <= 45; ++j)
			mat[i][j] = MySquare(1, Color::White);

	score1 = score2 = overallscore1 = overallscore2 = 0;
	speed1 = speed2 = speed_init;
	distance1 = distance2 = 0;
}

void Tetris::InitData(const int & choice)
{
	difficulty = choice;
	InitData();
}


Tetris::Tetris()
{
	difficulty = 0;
	InitData();
	overallscore1 = overallscore2 = 0;
}

Tetris::Tetris(const char * namePlayer1, const char * namePlayer2)
{
	difficulty = 0;
	InitData();
	nameP1 = namePlayer1;
	nameP2 = namePlayer2;
	overallscore1 = overallscore2 = 0;
}

void Tetris::DrawGrid(RenderWindow * window)
{
	RectangleShape r;
	Color c(170, 170, 170, 255);
	r.setSize(Vector2f(1016, 267));
	r.setFillColor(c);
	r.setPosition(Vector2f(0, 281));
	window->draw(r);

	c = Color(215, 215, 215, 255);
	r.setSize(Vector2f(1000, 251));
	r.setFillColor(c);
	r.setPosition(Vector2f(8, 289));
	window->draw(r);
	
	r.setSize(Vector2f(2, 251));
	c = Color(12, 87, 9, 255);
	r.setFillColor(c);
	for (int i = 0; i <= 40; ++i)
	{
		r.setPosition((float)8 + i * 25, (float)289);
		window->draw(r);
	}

	r.setSize(Vector2f(1001, 2));
	for (int i = 0; i <= 10; ++ i)
	{
		r.setPosition((float)8, (float)288 + i * 25);
		window->draw(r);
	}
}

void Tetris::DrawTable(RenderWindow * window)
{
	for (int i = 10; i >= 1; --i)
	{
		for (int j = 40; j >= 1; --j)
		{
			int ii, jj;
			ii = i + 3 - 1;
			jj = j + 3 - 1;
			if (mat[ii][jj].x == 0)
				continue;
			RectangleShape square;
			square.setSize(Vector2f(23, 23));
			square.setPosition((float)OFFSET_X + 2 + (j - 1) * 25, (float)OFFSET_Y + 2 + (i - 1) * 25);
			square.setFillColor(mat[ii][jj].color);
			square.setOutlineThickness(2);
			square.setOutlineColor(Color::Black);
			window->draw(square);
		}
	}
}

void Tetris::DrawBetweenLine(RenderWindow * window)
{
	RectangleShape r;
	//r.setFillColor(Color(136, 255, 0, 255));
	r.setFillColor(Color::Red);
	r.setSize(Vector2f(2, 267));
	r.setPosition(Vector2f((float)8 + 25 * (line_between-3), (float)281));
	window->draw(r);
}

void Tetris::DrawFinishedGame(const int winner, RenderWindow * window, const int & choice)
{
	Font font;
	font.loadFromFile("Resources/Fonts/arial.ttf");
	Text text;
	text.setCharacterSize(50);
	text.setFont(font);

	/*FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(1016 / 2.0f, 600 / 2.0f));
*/
	FloatRect textRect;
	
	if (winner != 0)
	{
		text.setString("The winner is player" + ToString(winner) + "!");
		textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		text.setPosition(sf::Vector2f(1016 / 2.0f, 600 / 2.0f - 170));
		text.setColor(Color::Red);
		window->draw(text);
	}


	text.setString("Score: " + ToString(overallscore1) + " - " + ToString(overallscore2));
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f );
	text.setPosition(sf::Vector2f(1016 / 2.0f, 600 / 2.0f - 110));
	text.setColor(Color::Yellow);
	window->draw(text);


	text.setString("New Game? Y/N");
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f );
	text.setPosition(sf::Vector2f(1016 / 2.0f, 600 / 2.0f - 50));;
	text.setColor(Color::White);
	window->draw(text);

	text.setString("Normal - Press A");
	text.setCharacterSize(22);
	if (choice == 0)
	{
		text.setColor(Color::Red);
	}
	else
	{
		text.setColor(Color::White);
	}
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(1016 / 2.0f, 600 / 2.0f + 40));
	window->draw(text);

	text.setString("Hard - Press B");
	text.setCharacterSize(22);
	if (choice == 1)
	{
		text.setColor(Color::Red);
	}
	else
	{
		text.setColor(Color::White);
	}
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(1016 / 2.0f, 600 / 2.0f + 70));
	window->draw(text);

	text.setString("Instructions:");
	text.setCharacterSize(22);
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(1016 / 2.0f, 600 / 2.0f + 100));;
	text.setColor(Color::White);
	window->draw(text);

	text.setString("Pause/Resume - Space");
	text.setCharacterSize(22);
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(1016 / 2.0f, 600 / 2.0f + 130));
	text.setColor(Color::White);
	window->draw(text);

	text.setString("Player1:\nDouble Speed - D\nMove Up - W\nMove Down - S\nRotate Left - R\nRotate Right - T");
	text.setCharacterSize(22);
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(1016 / 2.0f - 400, 600 / 2.0f + 180));
	text.setColor(Color::White);
	window->draw(text);

	text.setString("Player2:\nDouble Speed - Left Arrow\nMove Up - Up Arrow\nMove Down - Down Arrow\nRotate Left - K\nRotate Right - L");
	text.setCharacterSize(22);
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(1016 / 2.0f + 360, 600 / 2.0f + 180));
	text.setColor(Color::White);
	window->draw(text);
}

void Tetris::DrawPause(RenderWindow * window)
{
	Font font;
	font.loadFromFile("Resources/Fonts/arial.ttf");
	Text text;
	text.setCharacterSize(50);
	text.setFont(font);

	FloatRect textRect;
	text.setString("Paused");
	text.setCharacterSize(50);
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(1016 / 2.0f, 600 / 2.0f - 200));
	text.setColor(Color::White);
	window->draw(text);
}

void Tetris::DrawScore(RenderWindow * window)
{
	Font font;
	font.loadFromFile("Resources/Fonts/arial.ttf");
	Text text;
	text.setFont(font);

	FloatRect textRect;
	text.setString("Player1");
	text.setCharacterSize(25);
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(1016 / 2.0f - 450, 600 / 2.0f - 50 ));
	text.setColor(Color::White);
	window->draw(text);

	text.setString(ToString(score1));
	text.setCharacterSize(25);
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(1016 / 2.0f - 200, 600 / 2.0f - 52));
	text.setColor(Color::White);
	window->draw(text);

	text.setString(ToString(score2));
	text.setCharacterSize(25);
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(1016 / 2.0f + 200, 600 / 2.0f - 52));
	text.setColor(Color::White);
	window->draw(text);

	text.setString("Player2");
	text.setCharacterSize(25);
	textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(1016 / 2.0f + 450, 600 / 2.0f - 50));
	text.setColor(Color::White);
	window->draw(text);
}

bool Tetris::Move(const int & direction, const MyShape & s, int & x, int & y)
{
	int maxim = 0, minim = 50;
	MySquare aux[20][50];
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 50; ++j)
		{
			aux[i][j] = mat[i][j];
		}
	}
	int i, ii, j, jj;
	for (i = 1, ii = x; i <= 4; ++i, ++ii)
	{
		for (j = 1, jj = y; j <= 4; ++j, ++jj)
		{
			if (s.a[i][j].x == 1)
			{
				mat[ii][jj] = MySquare(0, Color::White);
			}
		}
	}
	for (i = 1, ii = x + dx[direction]; i <= 4; ++i, ++ii)
	{
		for (j = 1, jj = y + dy[direction]; j <= 4; ++j, ++jj)
		{
			mat[ii][jj].x += s.a[i][j].x;
			if (s.a[i][j].x == 1)
			{
				mat[ii][jj].color = s.a[i][j].color;
				maxim = max(maxim, jj);
				minim = min(minim, jj);
			}
			if (mat[ii][jj].x > 1)
			{
				for (int l = 0; l < 20; ++l)
					for (int c = 0; c < 50; ++c)
						mat[l][c] = aux[l][c];
				return false;
			}
		}
	}
	if (direction == RIGHT && maxim >= line_between)
	{
		for (int l = 0; l < 20; ++l)
			for (int c = 0; c < 50; ++c)
				mat[l][c] = aux[l][c];
		return false;
	}
	if (direction == LEFT && minim < line_between)
	{
		for (int l = 0; l < 20; ++l)
			for (int c = 0; c < 50; ++c)
				mat[l][c] = aux[l][c];
		return false;
	}
	x += dx[direction];
	y += dy[direction];
	return true;
}

bool Tetris::RotateLeft(const int player, MyShape & s, const int & x, const int &y)
{
	int maxim = 0, minim = 50;
	MySquare aux[20][50];
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 50; ++j)
			aux[i][j] = mat[i][j];
	int i, ii, j, jj;
	for (i = 1, ii = x; i <= 4; ++i, ++ii)
		for (j = 1, jj = y; j <= 4; ++j, ++jj)
			if (s.a[i][j].x == 1)
				mat[ii][jj] = MySquare(0, Color::White);
	s.RotateLeft();

	for (i = 1, ii = x; i <= 4; ++i, ++ii)
	{
		for (j = 1, jj = y; j <= 4; ++j, ++jj)
		{
			mat[ii][jj].x += s.a[i][j].x;
			if (s.a[i][j].x == 1)
			{
				mat[ii][jj].color = s.a[i][j].color;
				maxim = max(maxim, jj);
				minim = min(minim, jj);
			}
			if (mat[ii][jj].x > 1)
			{
				for (int l = 0; l < 20; ++l)
					for (int c = 0; c < 50; ++c)
						mat[l][c] = aux[l][c];
				s.RotateRight();
				return false;
			}
		}
	}
	if (player == 1 && maxim >= line_between)
	{
		for (int l = 0; l < 20; ++l)
			for (int c = 0; c < 50; ++c)
				mat[l][c] = aux[l][c];
		s.RotateRight();
		return false;
	}
	if (player == 2 && minim < line_between)
	{
		for (int l = 0; l < 20; ++l)
			for (int c = 0; c < 50; ++c)
				mat[l][c] = aux[l][c];
		s.RotateRight();
		return false;
	}
	return true;
}

bool Tetris::RotateRight(const int player, MyShape & s, const int & x, const int &y)
{
	int maxim = 0, minim = 50;
	MySquare aux[20][50];
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 50; ++j)
			aux[i][j] = mat[i][j];
	int i, ii, j, jj;
	for (i = 1, ii = x; i <= 4; ++i, ++ii)
		for (j = 1, jj = y; j <= 4; ++j, ++jj)
			if (s.a[i][j].x == 1)
				mat[ii][jj] = MySquare(0, Color::White);
	s.RotateRight();

	for (i = 1, ii = x; i <= 4; ++i, ++ii)
	{
		for (j = 1, jj = y; j <= 4; ++j, ++jj)
		{
			mat[ii][jj].x += s.a[i][j].x;
			if (s.a[i][j].x == 1)
			{
				mat[ii][jj].color = s.a[i][j].color;
		
				maxim = max(maxim, jj);
				minim = min(minim, jj);
			}
			if (mat[ii][jj].x > 1)
			{
				for (int l = 0; l < 20; ++l)
					for (int c = 0; c < 50; ++c)
						mat[l][c] = aux[l][c];
				s.RotateLeft();
				return false;
			}
		}
	}
	if (player == 1 && maxim >= line_between)
	{
		for (int l = 0; l < 20; ++l)
			for (int c = 0; c < 50; ++c)
				mat[l][c] = aux[l][c];
		s.RotateLeft();
		return false;
	}
	if (player == 2 && minim < line_between)
	{
		for (int l = 0; l < 20; ++l)
			for (int c = 0; c < 50; ++c)
				mat[l][c] = aux[l][c];
		s.RotateLeft();
		return false;
	}
	return true;
}

bool Tetris::CreateShape1()
{
	speed1 = speed_init;
	MySquare aux[20][50];
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 50; ++j)
			aux[i][j] = mat[i][j];

	shape1 = shapes[rand() % cnt_shapes];
	int rotations = rand() % 4;
	for (int i = 1; i <= rotations; ++i)
		shape1.RotateLeft();
	x1 = 6; y1 = 0;
	int i, ii, j, jj;
	for (int offset = 0; offset <= 3; ++offset)
	{
		bool ok = true;
		for (i = 1, ii = x1; i <= 4; ++i, ++ii)
		{
			for (j = 1, jj = y1 + offset; j <= 4; ++j, ++jj)
			{
				mat[ii][jj].x += shape1.a[i][j].x;
				if (mat[ii][jj].x > 1)
					ok = false;
			}
		}
		if (ok)
		{
			for (i = 1, ii = x1; i <= 4; ++i, ++ii)
			{
				for (j = 1, jj = y1 + offset; j <= 4; ++j, ++jj)
				{
					if (shape1.a[i][j].x == 1)
						mat[ii][jj].color = shape1.a[i][j].color;
				}
			}

			y1 += offset;
			return true;
		}
		else
		{
			for (i = 1, ii = x1; i <= 4; ++i, ++ii)
			{
				for (j = 1, jj = y1 + offset; j <= 4; ++j, ++jj)
				{
					mat[ii][jj].x -= shape1.a[i][j].x;
				}
			}
		}
	}
	return false;

}

bool Tetris::CreateShape2()
{
	speed2 = speed_init;
	MySquare aux[20][50];
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 50; ++j)
			aux[i][j] = mat[i][j];

	shape2 = shapes[rand() % cnt_shapes];
	int rotations = rand() % 4;
	for (int i = 1; i <= rotations; ++i)
		shape2.RotateLeft();
	x2 = 6; y2 = 42;
	int i, ii, j, jj;
	for (int offset = 0; offset <= 3; ++offset)
	{
		bool ok = true;
		for (i = 1, ii = x2; i <= 4; ++i, ++ii)
		{
			for (j = 1, jj = y2 - offset; j <= 4; ++j, ++jj)
			{
				mat[ii][jj].x += shape2.a[i][j].x;
				if (mat[ii][jj].x > 1)
					ok = false;
			}
		}
		if (ok)
		{
			for (i = 1, ii = x2; i <= 4; ++i, ++ii)
			{
				for (j = 1, jj = y2 - offset; j <= 4; ++j, ++jj)
				{
					if (shape2.a[i][j].x == 1)
						mat[ii][jj].color = shape2.a[i][j].color;
				}
			}
			y2 -= offset;
			return true;
		}
		else
		{
			for (i = 1, ii = x2; i <= 4; ++i, ++ii)
			{
				for (j = 1, jj = y2 - offset; j <= 4; ++j, ++jj)
				{
					mat[ii][jj].x -= shape2.a[i][j].x;
				}
			}
		}
	}
	return false;
}

bool Tetris::Check1(int & opponent_loses)
{
	int cnt = 0;
	bool viz[50];
	memset(viz, 0, sizeof(viz));
	for (int c = 3; c <= line_between-1; ++c)
	{
		bool found = true;
		for (int l = 3; l <= 12; ++l)
		{
			if (mat[l][c].x == 0)
				found = false;
		}
		if (found)
		{
			viz[c] = true;
			for (int l = 3; l <= 12; ++ l)
			{
				mat[l][c] = MySquare(0, Color::White);
			}
		}
	}
	for (int c = 3; c <= line_between-1; ++c)
	{
		if (viz[c])
			++cnt;
	}
	if (cnt)
	{
		int k = 1; ////
		for (int j = 42; j >= 3; --j)
		{
			if (viz[j])
			{
				++k;
				continue; ////
			}
			for (int i = 3; i <= 12; ++i)
			{
				if (j + k > 42)
				{
					if (mat[i][j].x == 1)
					{
						opponent_loses = 1;
						return true;
					}
				}
				else
				{
					mat[i][j + k] = mat[i][j];
				}

			}
		}

		for (int i = 3; i <= 12; ++i)
			for (int j = 3; j < 3 + k; ++j)
				mat[i][j] = MySquare(0, Color::White);

		score1 += (cnt + 1) * cnt / 2;
		y2+=1; //////
		line_between += 1; /// creste cu cat vreau eu
		opponent_loses = 0;
		return true;
	}
	return false;
}

bool Tetris::Check2(int & opponent_loses)
{
	int cnt = 0;
	bool viz[50];
	memset(viz, 0, sizeof(viz));
	for (int c = line_between; c <= 42; ++c)
	{
		bool found = true;
		for (int l = 3; l <= 12; ++l)
		{
			if (mat[l][c].x == 0)
				found = false;
		}
		if (found)
		{
			viz[c] = true;
			for (int l = 3; l <= 12; ++l)
			{
				mat[l][c] = MySquare(0, Color::White);
			}
		}
	}
	for (int c = line_between; c <= 42; ++c)
	{
		if (viz[c])
			++cnt;
	}
	if (cnt)
	{
		int k = 1;
		for (int j = 3; j <= 42; ++ j)
		{
			if (viz[j])
			{
				++k;
				continue;
			}
			for (int i = 3; i <= 12; ++i)
			{
				if (j - k < 3)
				{
					if (mat[i][j].x == 1)
					{
						opponent_loses = 1;
						return true;
					}
				}
				else
				{
					mat[i][j - k] = mat[i][j];
				}

			}
		}

		for (int i = 3; i <= 12; ++i)
			for (int j = 42 - k + 1; j <= 42; ++j)
				mat[i][j] = MySquare(0, Color::White);

		score2 += (cnt + 1) * cnt / 2;
		y1 -= 1;
		line_between -= 1; /// creste cu cat vreau eu
		opponent_loses = 0;
		return true;
	}
	return false;
}

void Tetris::Init(const int & choice)
{
	int save1 = overallscore1, save2 = overallscore2;
	InitData(choice);
	overallscore1 = save1;
	overallscore2 = save2;
}