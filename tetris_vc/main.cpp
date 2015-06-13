#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include "tetris.h"
#define OFFSET_X 8
#define OFFSET_Y 288

using namespace sf;
using namespace std;

enum GAMESTATE
{
	finished_game,
	paused_game,
	running_game
};

Tetris T("Player1", "Player2");

bool viz[12][42];
GAMESTATE state_of_the_game;

int winner;

void ReadMat()
{
	ifstream f("Resources/Shapes/test.txt");
	for (int i = 3; i <= 12; ++i)
	{
		for (int j = 3; j <= 42; ++j)
		{
			f >> T.mat[i][j].x;
			if (T.mat[i][j].x == 1)
			{
				T.mat[i][j].color = Color::Magenta;
			}
		}
	}
	f.close();
}

int main()
{
	srand((unsigned int)time(NULL));
	sf::RenderWindow window(sf::VideoMode(1016, 600), "Tetris!");
	state_of_the_game = finished_game;
	int runningshape1 = 0;
	int runningshape2 = 0;

	double dt1 = 1, dt2 = 1;

	int bricks_fallen1 = 0, bricks_fallen2 = 0;
	int choice = 0;


	//ReadMat();

	while (window.isOpen())
	{
		switch (state_of_the_game)
		{
			case finished_game:
			{
				runningshape1 = runningshape2 = 0;
				bricks_fallen1 = bricks_fallen2 = 0;
				dt1 = dt2 = 1;
				window.clear();
				T.DrawFinishedGame(winner, &window, choice);
				window.display();

				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						window.close();
					if (event.type == Event::KeyPressed)
					{
						switch (event.key.code)
						{
							case Keyboard::Y:
							{
								state_of_the_game = running_game;
								T.Init(choice);
								break;
							}
							case Keyboard::N:
							{
								window.close();
								break;
							}
							case Keyboard::A:
							{
								choice = 0;
								break;
							}
							case Keyboard::B:
							{
								choice = 1;
								break;
							}
						}
					}
				}

				break;
			}
			case paused_game:
			{
				window.clear();
				T.DrawGrid(&window);
				T.DrawTable(&window);

				T.DrawPause(&window);
				T.DrawScore(&window);

				T.DrawBetweenLine(&window);
				window.display();

				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						window.close();
					if (event.type == Event::KeyPressed)
					{
						switch (event.key.code)
						{
							case Keyboard::Space:
							{
								state_of_the_game = running_game;
								break;
							}
						}
					}
				}

				break;
			}
			case running_game:
			{
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						window.close();
					if (event.type == Event::KeyPressed)
					{
						switch (event.key.code)
						{
							case Keyboard::W:
							{
								T.Move(UP, T.shape1, T.x1, T.y1);
								break;
							}
							case Keyboard::S:
							{
								T.Move(DOWN, T.shape1, T.x1, T.y1);
								break;
							}
							case Keyboard::D:
							{
								T.speed1 *= 2;
								break;
							}
							case Keyboard::R:
							{
								T.RotateLeft(1, T.shape1, T.x1, T.y1);
								break;
							}
							case Keyboard::T:
							{
								T.RotateRight(1, T.shape1, T.x1, T.y1);
								break;
							}

							case Keyboard::Up:
							{
								T.Move(UP, T.shape2, T.x2, T.y2);
								break;
							}
							case Keyboard::Down:
							{
								T.Move(DOWN, T.shape2, T.x2, T.y2);
								break;
							}
							case Keyboard::Left:
							{
								T.speed2 *= 2;
								break;
							}
							case Keyboard::K:
							{
								T.RotateLeft(2, T.shape2, T.x2, T.y2);
								break;
							}
							case Keyboard::L:
							{
								T.RotateRight(2, T.shape2, T.x2, T.y2);
								break;
							}

							case Keyboard::Space:
							{
								state_of_the_game = paused_game;
								break;
							}
						}
					}
				}
				if (runningshape1 == 0)
				{
					runningshape1 = 1;
					bool succes = T.CreateShape1();
					++bricks_fallen1;
					if (!succes)
					{
						state_of_the_game = finished_game;
						winner = 2;
						++T.overallscore2;
						break;
					}
					T.distance1 = 0;
				}
				else
				{
					T.distance1 += T.speed1 * dt1;
					if (T.distance1 > T.max_distance)
					{
						T.distance1 = 0;
						bool succes = T.Move(RIGHT, T.shape1, T.x1, T.y1);
						if (!succes)
						{
							T.speed1 = T.speed_init;
							runningshape1 = 0;
						
							int opponent_loses;
							bool succes_check = T.Check1(opponent_loses);
							if (succes_check)
							{
								T.speed1 = T.speed_init;
								runningshape1 = 0;
								if (opponent_loses == 1)
								{
									state_of_the_game = finished_game;
									winner = 1;
									++T.overallscore1;
									break;
								}
							}
						}
					}
				}

				if (runningshape2 == 0)
				{
					runningshape2 = 1;
					bool succes = T.CreateShape2();
					++bricks_fallen2;
					if (!succes)
					{
						state_of_the_game = finished_game;
						winner = 1;
						++T.overallscore1;
					}
					T.distance2 = 0;
				}
				else
				{
					T.distance2 += T.speed2 * dt2;
					if (T.distance2 > T.max_distance)
					{
						T.distance2 = 0;
						bool succes = T.Move(LEFT, T.shape2, T.x2, T.y2);
						if (!succes)
						{
							T.speed2 = T.speed_init;
							runningshape2 = 0;
						
							int opponent_loses;
							bool succes_check = T.Check2(opponent_loses);
							if (succes_check)
							{
								T.speed2 = T.speed_init;
								runningshape2 = 0;
								if (opponent_loses == 1)
								{
									state_of_the_game = finished_game;
									winner = 2;
									++T.overallscore2;
									break;
								}
							}
						}
					}
				}

				if (bricks_fallen1 % 10 == 0)
				{
					dt1 = dt1 * 2;
					++bricks_fallen1;
				}
				if (bricks_fallen2 % 10 == 0)
				{
					dt2 = dt2 * 2;
					++bricks_fallen2;
				}

				window.clear();
				T.DrawGrid(&window);
				T.DrawTable(&window);
				T.DrawScore(&window);

				T.DrawBetweenLine(&window);
				window.display();
				break;
			}
		}
	}

	return 0;
}