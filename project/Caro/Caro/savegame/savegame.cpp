#pragma once
#include "../global.h"

std::string tostr(int k, int len = 0)
{
	std::string str = "";
	while (k)
	{
		str += char(k % 10 + 48);
		k /= 10;
	}
	while (str.length() < len)
		str += "0";
	reverse(str.begin(), str.end());
	return str;
}

void SaveGame()
{
	std::ofstream file("assets/game_save/gamesave.txt", std::ios::app);

	if (!file.is_open()) {
		std::cout << "Failed to open game save file!" << std::endl;
		return;
	}

	file << "BEGIN\n"; // start of a record

	// take current time;
	time_t current_time = time(0);
	struct tm now;
	localtime_s(&now, &current_time);
	std::string curTime = "";
	curTime += tostr(now.tm_year + 1900, 4) + "-";
	curTime += tostr(now.tm_mon + 1, 2) + "-";
	curTime += tostr(now.tm_mday, 2);
	curTime += tostr(now.tm_hour, 2) + "-";
	curTime += tostr(now.tm_min, 2) + "-";
	curTime += tostr(now.tm_sec, 2);

	std::string record_name = curTime; // default value of record name, can be changed in later update
	file << record_name << "\n"; 

	int nCOMP = 10; // number of components need to be saved
	file << nCOMP << "\n";

	std::string components[] = {"TIME", "MODE", "P1_NAME", "P2_NAME", "P1_SCORE", "P2_SCORE", "TURN", "BOARD_SIZE", "BOARDGAME", "RESULT"}; // components need to be saved
	for (int component = 0; component < nCOMP; component++)
	{
		file << components[component] << " ";
		if (component == 0)
			file << curTime;
		else if (component == 1)
		{
			if (boardGame.isPVCMode())
				file << "PVE";
			else
				file << "PVP";
		}
		else if (component == 2)
			file << boardGame.player1Name;
		else if (component == 3)
			file << boardGame.player2Name;
		else if (component == 4)
			file << boardGame.player1Score;
		else if (component == 5)
			file << boardGame.player2Score;
		else if (component == 6)
			file << boardGame.curPlayer;
		else if (component == 7)
			file << boardGame.size;
		else if (component == 8)
		{
			for (int i = 0; i < boardGame.board.size(); i++)
			{
				file << "\n";
				for (int j = 0; j < boardGame.board[i].size(); j++)
					file << boardGame.board[i][j] << " ";
			}
		}
		else if (component == 9)
			file << boardGame.resultGame;
		file << "\n";
	}

	file << "END\n\n"; // end of a record

	file.close();
}

void LoadGame()
{
	std::ifstream file("assets/game_save/gamesave.txt");

	if (!file.is_open()) {
		std::cout << "Failed to open game save file!" << std::endl;
		return;
	}

	file.close();
}