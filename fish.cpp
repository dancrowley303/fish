// fish.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Game.h"
#include <ctime>

#include "vld.h"

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

int _tmain(int argc, _TCHAR* argv[])
{
	std::srand((unsigned int)std::time(0));

	Game game;
	game.Play();

	return 0;
}

