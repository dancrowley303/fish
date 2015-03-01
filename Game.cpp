#include "StdAfx.h"
#include "Game.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <ctime>
#include <iterator>
#include <map>

Game::Game(void)
{
	static const std::string rankSet[] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
	ranks = std::vector<std::string>(rankSet, rankSet + sizeof(rankSet) / sizeof(rankSet[0]));

	//set up card set
	static const Card cardSet[] = {
		{"2", "2\3"}, {"2", "2\4"}, {"2","2\5"},{"2","2\6"},
		{"3", "3\3"}, {"3", "3\4"}, {"3","3\5"},{"3","3\6"},
		{"4", "4\3"}, {"4", "4\4"}, {"4","4\5"},{"4","4\6"},
		{"5", "5\3"}, {"5", "5\4"}, {"5","5\5"},{"5","5\6"},
		{"6", "6\3"}, {"6", "6\4"}, {"6","6\5"},{"6","6\6"},
		{"7", "7\3"}, {"7", "7\4"}, {"7","7\5"},{"7","7\6"},
		{"8", "8\3"}, {"8", "8\4"}, {"8","8\5"},{"8","8\6"},
		{"9", "9\3"}, {"9", "9\4"}, {"9","9\5"},{"9","9\6"},
		{"10", "10\3"}, {"10", "10\4"}, {"10","10\5"},{"10","10\6"},
		{"J", "J\3"}, {"J", "J\4"}, {"J","J\5"},{"J","J\6"},
		{"Q", "Q\3"}, {"Q", "Q\4"}, {"Q","Q\5"},{"Q","Q\6"},
		{"K", "K\3"}, {"K", "K\4"}, {"K","K\5"},{"K","K\6"},
		{"A", "A\3"}, {"A", "A\4"}, {"A","A\5"},{"A","A\6"}
	};

	deck = std::vector<Card>(cardSet, cardSet + sizeof(cardSet) / sizeof(cardSet[0]));

}


Game::~Game(void)
{
}

void Game::Play()
{
	//set up deck
	
	std::random_shuffle(deck.begin(), deck.end(), Game::randomize);

	//initialise game variables
	playerCount = getPlayerCount();
	int startingCardCount = (playerCount <= 3) ? 7 : 5;

	//set up player hands
	playerHands = std::map <int, std::vector<Card>>();
	for (int i = 1; i <= playerCount; i++) {
		playerHands[i] = std::vector<Card>();
	}

	//deal cards
	for (int i = 1; i <= startingCardCount; i++) {
		for (int j = 1; j <= playerCount; j++) {
			assignCard(playerHands.at(j));
		}
	}

	//intial scores
	for (int p = 1; p <= playerCount; p++) {
		playerScores[p] = 0;
		for (int r = 0; r < ranks.size(); r++) {
			score(playerHands.at(p), ranks[r], p);
		}
	}

	int gameTurn = 1;

	while (pileWinners.size() < 13) {

		showHands();

		int pTurn = (gameTurn % playerCount == 0) ? playerCount : gameTurn % playerCount;
		if (!playerInGame(pTurn)) {
			std::cout << "p" << pTurn << " is out of the game" << std::endl;
			gameTurn++;
			continue;
		}

		turn(pTurn);

		std::cout << deck.size() << " cards left in the deck" << std::endl;

		gameTurn++;
	}
	
	finalScore();
	std::cout << "Game Over" << std::endl;

}

int Game::randomize(int i)
{
	return std::rand() % i;
}

Card Game::assignCard(std::vector<Card>& hand)
{
	Card card = deck.front();
	hand.push_back(card);
	deck.erase(deck.begin());
	std::sort(hand.begin(), hand.end());
	return card;
}

std::string Game::fish(int player, std::string guess, std::vector<Card>& hand)
{
	Card card = deck.front();
	hand.push_back(card);
	deck.erase(deck.begin());
	std::sort(hand.begin(), hand.end());
	return card.rank;
}

void Game::showHands()
{
	for (int i = 1; i <= playerHands.size(); i++) {
		std::cout << "p" << i << ": [";
		for (unsigned int j = 0; j < playerHands.at(i).size(); j++) {
			std::cout << playerHands.at(i)[j].display << " ";
		}
		std::cout << "] " << std::endl;
	}

	std::cout << "Piles: [";

	std::for_each(pileWinners.begin(), pileWinners.end(), [] (std::pair<std::string, int> pair) {
		std::cout << pair.first << "=" << pair.second << " ";
	});

	std::cout << "]" << std::endl;
}

bool Game::score(std::vector<Card>& hand, std::string rank, int player) 
{
	int cardsInRank = std::count_if(hand.begin(), hand.end(), [rank] (Card& card) { return card.rank == rank; });
	if (cardsInRank == 4) {
		std::cout << "p" << player << " made a pile of " << rank << std::endl;
		pileWinners[rank] = player;
		playerScores[player]++;
		hand.erase(std::remove_if(hand.begin(), hand.end(), [rank] (Card& card) { return card.rank == rank; }), hand.end());
		return true;
	}
	return false;
}

bool Game::playerInGame(int player)
{
	return std::count_if(outOfGame.begin(), outOfGame.end(), [player] (int& outOfGamePlayer) { return player == outOfGamePlayer; }) == 0;
}

void Game::finalScore()
{
	std::cout << "Final Scores" << std::endl;
	std::cout << "============" << std::endl;
	std::cout << "Pile Winners" << std::endl;

	std::for_each(pileWinners.begin(), pileWinners.end(), [] (std::pair<std::string, int> pair) {
		std::cout << pair.first << ":     " << pair.second << std::endl;
	});

	std::cout << "============" << std::endl << std::endl;
	
	std::cout << "Player Points" << std::endl;
	std::cout << "============" << std::endl << std::endl;

	std::for_each(playerScores.begin(), playerScores.end(), [] (std::pair<int, int> pair) {
		std::cout << pair.first << ":     " << pair.second << std::endl;
	});

	std::cout << "============" << std::endl << std::endl;
}

void Game::turn(int player)
{
	std::string guess;

	bool isCpu = player != 0;
	std::vector<Card>& askingHand = playerHands.at(player);

	if (isCpu) {
		if (askingHand.size() == 0) {
			if (deck.size() > 0) {
				std::cout << "p" << player << " has no cards so drawing from deck" << std::endl;
				Card assignedCard = assignCard(playerHands.at(player));
				if (score(askingHand, assignedCard.rank, player)) {
					std::cout << "drawn card made a pile, so have another turn!" << std::endl;
					return turn(player);
				}
			} else {
				std::cout << "p" << player << " has no cards and deck is empty, so player is out of game!" << std::endl;
				outOfGame.push_back(player);
				return;
			}
		} else {
			int askingHandPosition = randomize(askingHand.size());
			guess = askingHand[askingHandPosition].rank;
		}
	} else {
		std::cin >> guess;
	}

	int targetPlayer;
	if (player == 0) {
		std::cout << "p" << player << ", ask which player? ";
		std::cin >> targetPlayer;
	} else {
		do {
			targetPlayer = randomize(playerCount) + 1;
		} while (targetPlayer == player);
	}

	std::vector<Card>& opponentHand = playerHands.at(targetPlayer);

	std::cout << "p" << player << " asks p" << targetPlayer << " for " << guess << std::endl;

	auto findCardPredicate = [guess] (Card& card) { return card.rank == guess; };

	int askHandCount = std::count_if(askingHand.begin(), askingHand.end(), findCardPredicate);
	if (askHandCount == 0) {
		std::cout << "You can't ask for a card you don't have - try again." << std::endl;
		return turn(player);
	}

	int foundCount = std::count_if(opponentHand.begin(), opponentHand.end(), findCardPredicate);
	if (foundCount > 0) {
		std::cout << "found " << foundCount << " of " << guess << " card!" << std::endl;
		std::copy_if(opponentHand.begin(), opponentHand.end(), std::back_inserter(askingHand), findCardPredicate);
		opponentHand.erase(std::remove_if(opponentHand.begin(), opponentHand.end(), findCardPredicate), opponentHand.end());
		std::sort(askingHand.begin(), askingHand.end());
		if (score(askingHand, guess, player)) {
			std::cout << "made pile, so have another turn" << std::endl;
			turn(player);
		}
	} else {
		if (deck.size() > 0) {
			std::cout << "go fish!" << std::endl;
			std::string fishedCard = fish(player, guess, askingHand);
			if (score(askingHand, fishedCard, player)) {
				std::cout << "made pile from fish, so have another turn" << std::endl;
				turn(player);
			}
			else if (fishedCard == guess) {
				std::cout << "caught a fish, so ask again!" << std::endl;
				turn(player);
			}
		} else {
			std::cout << "can't fish becaue deck is empty!" << std::endl;
		}
	}
}

int Game::getPlayerCount()
{
	std::cout << "How many players [2-5]? ";
	int playerCount;
	std::cin >> playerCount;
	if (playerCount < 2 || playerCount > 5) {
		std::cout << "Must be between 2 and 5 players!" << std::endl;
		return getPlayerCount();
	} else {
		return playerCount;
	}
}

