#pragma once
#include "../global.h"

struct RecordData
{
	string record_name;
	int nCOMP = 10;
	vector <string> components;
	string saved_time;
	string mode;
	string player1Name;
	string player2Name;
	int player1Score = 0;
	int player2Score = 0;
	int curPlayer = 1;
	int boardsize = 12;
	vector < vector <int> > boardGame;
	int resultGame = 0;
	RecordData() {}
};

void SaveGameUI(RenderWindow& window, bool = 1);

void SaveGame(RenderWindow& window);

void LoadGameFetch();

void LoadGameLogic();

void LoadGameUI(RenderWindow& window);