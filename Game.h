#pragma once
#include <vector>
#include <string>
#include <map>

struct Card {
	std::string rank;
	std::string display;

	bool operator < (const Card& that) const
	{
		return display < that.display;
	}
};

class Game
{
public:
	Game(void);
	~Game(void);
	void Play();
private:
	static int randomize(int i);
	Card assignCard(std::vector<Card>& hand);
	std::string fish(int player, std::string guess, std::vector<Card>& hand);
	void showHands();
	bool score(std::vector<Card>& hand, std::string rank, int player);
	bool playerInGame(int player);
	void finalScore();
	void turn(int player);
	int getPlayerCount();

	int playerCount;
	std::vector<int> outOfGame;
	std::vector<std::string> ranks;
	std::vector<Card> deck;
	std::map <int, std::vector<Card>> playerHands;
	std::map<std::string, int> pileWinners;
	std::map<int, int> playerScores;

};


