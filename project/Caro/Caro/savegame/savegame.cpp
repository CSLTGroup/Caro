#pragma once
#include "../global.h"

void SaveGameUI(RenderWindow& window, bool success)
{
	// pre set up
	const float winWidth = window.getSize().x;
	const float winHeight = window.getSize().y;

	// annoucement
	Text saveannouncement;
	saveannouncement.setFont(font);
	if (success)
		saveannouncement.setString("Saved successfully!");
	else
		saveannouncement.setString("Fail to save the game");
	saveannouncement.setCharacterSize((int)(winHeight * 0.075f));
	saveannouncement.setFillColor(Color::Yellow);
	saveannouncement.setPosition(winWidth * 0.05, winHeight * 0.1);
	saveannouncement.setOutlineColor(Color(0, 0, 0, 200));
	saveannouncement.setOutlineThickness(2);

	Clock timer;
	timer.restart();
	window.draw(saveannouncement);
	window.display();
	while (timer.getElapsedTime().asSeconds() < 1.f)
	{
		// wait for 1 second
	}
}

void SaveGame(RenderWindow& window)
{
	std::ofstream file("assets/game_save/gamesave.txt", std::ios::app);

	if (!file.is_open()) {
		std::cout << "Failed to open game save file!" << std::endl;
		SaveGameUI(window, 0);
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
	curTime += tostr(now.tm_mday, 2) + "-";
	curTime += tostr(now.tm_hour, 2) + "-";
	curTime += tostr(now.tm_min, 2) + "-";
	curTime += tostr(now.tm_sec, 2);

	std::string record_name = curTime; // default value of record name, can be changed in later update
	file << record_name << "\n";

	std::string components[] = { "TIME", "MODE", "P1_NAME", "P2_NAME", "P1_SCORE", "P2_SCORE", "TURN", "BOARD_SIZE", "BOARDGAME", "RESULT", "P1_AVT", "P2_AVT" }; // components need to be saved
	int nCOMP = sizeof(components) / sizeof(components[0]); // number of components need to be saved
	file << nCOMP << "\n";

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
		{
			if (boardGame.player1Name == "")
				file << "Player 1";
			else
				file << boardGame.player1Name;
		}
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
		else if (component == 10)
			file << setting.player1AvatarPath;
		else if (component == 11)
			file << setting.player2AvatarPath;
		file << "\n";
	}

	file << "END\n\n"; // end of a record

	SaveGameUI(window);

	file.close();
}


/*======= LOAD GAME UI VARIABLES =======*/

int curRecord; // index in records
int firstBtnRecord; // index (multiples of three)
int numPage; // total pages
int curPage; // current pages
int numBtn; // number of button in this page
int curBtn; // 0, 1, 2 in this page

/*======================================*/

void LoadGameFetch()
{
	std::ifstream file("assets/game_save/gamesave.txt");

	if (!file.is_open()) {
		std::cout << "Failed to open game save file!" << std::endl;
		return;
	}

	// file opened successully
	string content;
	menuGUI.records.clear(); // delete records from previous call
	while (file >> content)
	{
		if (content == "END") // end of record, do nothing
			continue;
		else if (content == "BEGIN") // start of record, increase records size
		{
			menuGUI.records.resize(menuGUI.records.size() + 1);
			file >> menuGUI.records.back().record_name;
			file >> menuGUI.records.back().nCOMP;
		}
		else
		{
			menuGUI.records.back().components.push_back(content);
			if (content == "TIME")
				file >> menuGUI.records.back().saved_time;
			else if (content == "MODE")
				file >> menuGUI.records.back().mode;
			else if (content == "P1_NAME")
			{
				file.ignore();
				getline(file, menuGUI.records.back().player1Name);
			}
			else if (content == "P2_NAME")
			{
				file.ignore();
				getline(file, menuGUI.records.back().player2Name);
			}
			else if (content == "P1_SCORE")
				file >> menuGUI.records.back().player1Score;
			else if (content == "P2_SCORE")
				file >> menuGUI.records.back().player2Score;
			else if (content == "TURN")
				file >> menuGUI.records.back().curPlayer;
			else if (content == "BOARD_SIZE")
				file >> menuGUI.records.back().boardsize;
			else if (content == "BOARDGAME")
			{
				vector < vector <int>>& board = menuGUI.records.back().boardGame;
				int& bsize = menuGUI.records.back().boardsize;
				board.assign(bsize, vector <int>(bsize, 0));
				for (int i = 0; i < bsize; i++)
					for (int j = 0; j < bsize; j++)
						file >> board[i][j];
			}
			else if (content == "RESULT")
				file >> menuGUI.records.back().resultGame;
			else if (content == "P1_AVT")
			{
				file.ignore();
				getline(file, menuGUI.records.back().player1AvatarPath);
			}
			else if (content == "P2_AVT")
			{
				file.ignore();
				getline(file, menuGUI.records.back().player2AvatarPath);
			}
		}
	}

	file.close();

	reverse(menuGUI.records.begin(), menuGUI.records.end());
	firstBtnRecord = curPage = curBtn = 0;
	numBtn = min(3, (int)menuGUI.records.size());
	numPage = ((int)menuGUI.records.size() + 2) / 3 - 1;
	curRecord = -1;
}

void LoadGameLogic()
{
	if (keyBoard.Esc()) // back to main menu
	{
		stateMenu = 0;
		return;
	}

	if (menuGUI.records.size() == 0) // no records yet
		return;

	numBtn = 0;
	if (curPage == numPage) // last page
		numBtn = menuGUI.records.size() % 3;
	if (numBtn == 0) // last page has 3 buttons or not last page
		numBtn = 3;

	if (keyBoard.Up() || keyBoard.Down()) // change button inside page
	{
		if (keyBoard.Up())
		{
			if (--curBtn < 0)
				curBtn += numBtn;
		}
		else
		{
			if (++curBtn >= numBtn)
				curBtn -= numBtn;
		}
		firstBtnRecord = 3 * curPage;
		curRecord = firstBtnRecord + curBtn;
		PlaySoundClick();
	}
	else if (keyBoard.Left() || keyBoard.Right()) // change page
	{
		int prvPage = curPage;
		if (keyBoard.Left())
			curPage = max(curPage - 1, 0);
		else
			curPage = min(curPage + 1, numPage);
		if (curPage != prvPage)
		{
			curBtn = 0;
			if (curPage == numPage) // last page
				numBtn = menuGUI.records.size() % 3;
			if (numBtn == 0) // last page has 3 buttons or not last page
				numBtn = 3;
			PlaySoundClick();
		}
		firstBtnRecord = 3 * curPage;
		curRecord = firstBtnRecord + curBtn;
	}
	else if (keyBoard.Enter() && curRecord > -1) // choose record
	{
		RecordData& record = menuGUI.records[curRecord];
		for (int COMP = 0; COMP < record.nCOMP; COMP++)
		{
			if (record.components[COMP] == "MODE")
			{
				if (record.mode == "PVE")
					boardGame.mode = BoardGame::GameMode::PVC;
				else
					boardGame.mode = BoardGame::GameMode::PVP;
			}
			else if (record.components[COMP] == "P1_NAME")
				playerName[0] = record.player1Name;
			else if (record.components[COMP] == "P2_NAME")
				playerName[1] = record.player2Name;
			else if (record.components[COMP] == "P1_SCORE")
				boardGame.player1Score = record.player1Score;
			else if (record.components[COMP] == "P2_SCORE")
				boardGame.player2Score = record.player2Score;
			else if (record.components[COMP] == "TURN")
				boardGame.curPlayer = record.curPlayer;
			else if (record.components[COMP] == "BOARD_SIZE")
				boardGame.size = record.boardsize;
			else if (record.components[COMP] == "BOARDGAME")
			{
				for (int i = 0; i < boardGame.size; i++)
					for (int j = 0; j < boardGame.size; j++)
						boardGame.board[i][j] = record.boardGame[i][j];
			}
			else if (record.components[COMP] == "RESULT")
				boardGame.resultGame = record.resultGame;
			else if (record.components[COMP] == "P1_AVT")
			{
				setting.player1AvatarPath = record.player1AvatarPath;
				boardGame.player1Photo.loadFromFile(setting.player1AvatarPath);
			}
			else if (record.components[COMP] == "P2_AVT")
			{
				setting.player2AvatarPath = record.player2AvatarPath;
				boardGame.player2Photo.loadFromFile(setting.player2AvatarPath);
			}
		}
		menuGUI.fromLoadGame = true;
		stateMenu = 1;
		boardGame.checkResult();
		PlaySoundClick();
	}
}

void RecordButtonUI(RenderWindow& window, int Btn, float winWidth, float winHeight, float& frameWidth, float& frameHeight)
{
	RectangleShape buttonBox(Vector2f(frameWidth * 0.75, frameHeight / 10));
	Text buttonText;
	buttonText.setFont(font);
	buttonText.setCharacterSize(0.04 * winHeight);

	// custom button trait
	// color, position, outline thickness
	buttonBox.setFillColor(Btn == curBtn ? Color(100, 150, 200) : Color(70, 70, 70));
	buttonBox.setOutlineThickness(Btn == curBtn ? winHeight * 0.006 : winHeight * -0.003);
	buttonBox.setOutlineColor(Btn == curBtn ? Color::Yellow : Color::White);
	buttonBox.setPosition(winWidth / 2 - frameWidth * 0.375f,
		winHeight / 2 - frameHeight / 2 + 0.11f * winHeight + 0.13f * winHeight * Btn);

	// custom text trait
	// color, string, position
	buttonText.setString(menuGUI.records[firstBtnRecord + Btn].record_name);
	buttonText.setFillColor(Btn == curBtn ? Color::Yellow : Color::White);
	buttonText.setPosition(winWidth / 2 - buttonText.getLocalBounds().width / 2,
		winHeight / 2 - frameHeight + 0.36f * winHeight + 0.13f * winHeight * Btn);

	// draw
	window.draw(buttonBox);
	window.draw(buttonText);
}

void LoadGameUI(RenderWindow& window)
{
	// activate choosing mode
	if (curRecord == -1)
		curRecord = 0;

	// setup
	const float winWidth = window.getSize().x;
	const float winHeight = window.getSize().y;

	// draw load game frame
	float frameWidth = winWidth * 0.6;
	float frameHeight = winHeight * 0.5;
	RectangleShape loadgameframe(Vector2f(frameWidth, frameHeight));
	loadgameframe.setFillColor(Color(50, 50, 50, 150));
	loadgameframe.setOutlineColor(Color::White);
	loadgameframe.setOutlineThickness(winHeight * 0.003f);
	loadgameframe.setPosition(winWidth / 2 - frameWidth / 2,
		winHeight / 2 - frameHeight / 2);
	window.draw(loadgameframe);

	// draw title
	Text titleText;
	titleText.setFont(font);
	titleText.setString("Load game");
	titleText.setCharacterSize((int)(winHeight * 0.05f));
	titleText.setFillColor(Color::Yellow);
	FloatRect titleBounds = titleText.getLocalBounds();
	titleText.setPosition(window.getSize().x / 2 - titleBounds.width / 2,
		winHeight / 2 - frameHeight / 2 + winHeight * 0.03f);
	window.draw(titleText);

	// draw page
	Text pageText;
	pageText.setString("Page " + tostr(curPage + 1) + "/" + tostr(numPage));
	pageText.setCharacterSize((int)(winHeight * 0.03f));
	pageText.setStyle(Text::Italic);
	//pageText.setFillColor(Color(120, 158, 158));
	pageText.setFillColor(Color::Blue);
	pageText.setPosition(winWidth / 2 + frameWidth / 2 - pageText.getLocalBounds().width * 1.05,
		winHeight / 2 + frameHeight / 2 - pageText.getLocalBounds().height * 1.05);
	window.draw(pageText);

	// draw buttons
	if (menuGUI.records.size() == 0) // no records yet
	{
		Text announcement;
		announcement.setFont(font);
		announcement.setString("No records yet!");
		announcement.setCharacterSize((int)(winHeight * 0.05f));
		announcement.setFillColor(Color::Yellow);
		FloatRect announcementBounds = announcement.getLocalBounds();
		announcement.setPosition(window.getSize().x / 2 - announcementBounds.width / 2,
			winHeight / 2 - announcement.getLocalBounds().height / 2);
		window.draw(announcement);
		return;
	}

	for (int i = 0; i < numBtn; i++)
		RecordButtonUI(window, i, winWidth, winHeight, frameWidth, frameHeight);

	// draw navigation arrow
	static Animation leftarrow("assets/image/leftarrow.png", 1125, 1040, 10, 3, 4);
	static Sprite spriteLeft;
	static Animation rightarrow("assets/image/rightarrow.png", 1125, 1040, 10, 3, 4);
	static Sprite spriteRight;

	static Clock clock;
	static int currentFrame = 0;
	static float timer = 0.f;
	const static float fps = 8.0f;   // animation speed
	float dt = clock.restart().asSeconds();

	updateAnimation(dt, fps, leftarrow.frameCount, currentFrame, timer);
	applyFrame(spriteLeft, leftarrow, currentFrame);
	updateAnimation(dt, fps, rightarrow.frameCount, currentFrame, timer);
	applyFrame(spriteRight, rightarrow, currentFrame);

	float scaleX = 3.0f * frameWidth / leftarrow.texture.getSize().x * 0.15; // 15% box's width
	float scaleY = 4.0f * frameHeight / rightarrow.texture.getSize().y * 0.23; // 23% box's height
	if (spriteLeft.getScale().x != scaleX || spriteLeft.getScale().y != scaleY) {
		// scale
		spriteLeft.setScale(scaleX, scaleY);
		spriteRight.setScale(scaleX, scaleY);
		// position
		spriteLeft.setOrigin(spriteLeft.getLocalBounds().getSize().x / 2,
			spriteLeft.getLocalBounds().getSize().y / 2);
		spriteLeft.setPosition(winWidth / 2 - frameWidth / 2, winHeight / 2);

		spriteRight.setOrigin(spriteLeft.getLocalBounds().getSize().x / 2,
			spriteRight.getLocalBounds().getSize().y / 2);
		spriteRight.setPosition(winWidth / 2 + frameWidth / 2, winHeight / 2);
	}

	if (firstBtnRecord + numBtn < menuGUI.records.size())
		window.draw(spriteRight);
	if (firstBtnRecord > 0)
		window.draw(spriteLeft);

}