// fish.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Game.h"
#include <ctime>

int _tmain(int argc, _TCHAR* argv[])
{
	std::srand((unsigned int)std::time(0));

	Game game;
	game.Play();

	return 0;
}

