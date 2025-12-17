#include "../global.h"
#include "../bot_ai/bot_ai.h"

void BoardGame::setUp() {
    board.assign(size, vector<int>(size, 0));
    ensurePlayerAssets();
}
void BoardGame::reset() {
    board.assign(size, vector<int>(size, 0));
    curX = curY = 0;
    curPlayer = 1;
    resultGame = 0;
}
void BoardGame::drawTable(RenderWindow& window) {

    // set up size
    const float winWidth = window.getSize().x;
    const float winHeight = window.getSize().y;

    float widthBoard = min(winWidth * 48 / 100, winHeight * 68 / 100);
    float heightBoard = widthBoard;  // square board
    spacingLeft = (winWidth - widthBoard) / 2.f;
    spacingTop = (winHeight - heightBoard) / 2.f;
    cellLenX = widthBoard / size * 90 / 100;
    cellLenY = heightBoard / size * 90 / 100;
    spacingCellX = (widthBoard - cellLenX * size) / (size + 1);
    spacingCellY = (heightBoard - cellLenY * size) / (size + 1);

    // draw transparent board
    RectangleShape boardShape(Vector2f(widthBoard, heightBoard));
    boardShape.setPosition(spacingLeft, spacingTop);
    boardShape.setFillColor(Color(0, 0, 0, 150));
    boardShape.setOutlineColor(Color(0, 0, 0)); // black outline
    boardShape.setOutlineThickness(3);
    window.draw(boardShape);

    // cell
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            drawPosition(i, j, window);
        }
    }

    // draw player info panel
    if (showPlayerPanel) {
        drawPlayerInfoPanel(window);
    }

    // draw transparent box's tip
	RectangleShape tipBox(Vector2f(widthBoard, (winHeight - heightBoard) / 3.5f));
	tipBox.setPosition(spacingLeft, winHeight - (winHeight - heightBoard) / 3.5f);
	tipBox.setFillColor(Color(0, 0, 0, 150));
    window.draw(tipBox);

    // draw text tip
	Text tipText;
	tipText.setFont(font);
	float sizeTipText = max((winWidth - heightBoard) / 25, 50.0f);
    tipText.setCharacterSize(getCharacterSizeForLineHeight(font, sizeTipText));
	tipText.setFillColor(Color::Yellow); // light gray
	tipText.setString("Press L to save game");
	FloatRect bounds = tipText.getLocalBounds();
	tipText.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
	float posY = winHeight - (winHeight - heightBoard) / 3.5f + ((winHeight - heightBoard) / 3.5f) / 2;
	tipText.setPosition(winWidth / 2, posY);
	window.draw(tipText);

    // ve man hinh thang
    if (resultGame == 1 || resultGame == 2) {
        drawWinnerMessage(window);
    }
    else if (resultGame == 3) {
        drawWinnerMessage(window);
    }
}
void BoardGame::drawPosition(int x, int y, RenderWindow& window) {

    // shape
    RectangleShape cell = RectangleShape(Vector2f(cellLenX, cellLenY));
    cell.setPosition(
        spacingLeft + spacingCellX + x * (cellLenX + spacingCellX),
        spacingTop + spacingCellY + y * (cellLenY + spacingCellY)
    );
    if (x == curX && y == curY) {
        if (curPlayer == 1) {
            cell.setFillColor(Color(0, 255, 0, 0.8 * 255)); // green
        }
        else {
			cell.setFillColor(Color(255, 0, 0, 0.8 * 255)); // red
        }
    }
    else if ((x + y) % 2 == 0)
        cell.setFillColor(Color(240, 224, 125, 120)); // light yellow
    else
		cell.setFillColor(Color(0, 0, 0, 180)); // black
    window.draw(cell);

    // text 
    int heightText = min(cellLenX, cellLenY) * 80.f / 100;
    Text text;
    text.setFont(font);
    text.setCharacterSize(heightText);
    if (board[x][y] == 1) {
        text.setFillColor(Color::Green);
		text.setString("O");
    }
    else if (board[x][y] == 2) {
		text.setFillColor(Color::Red);
        text.setString("X");
    }
    FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(
        textBounds.left + textBounds.width / 2.f,
        textBounds.top + textBounds.height / 2.f
    );
    text.setPosition(
        spacingLeft + spacingCellX + x * (cellLenX + spacingCellX) + cellLenX / 2.f,
        spacingTop + spacingCellY + y * (cellLenY + spacingCellY) + cellLenY / 2.f
    );
    text.setOutlineThickness(2.0f);
    text.setOutlineColor(Color(20, 20, 20));
    window.draw(text);
}
void BoardGame::setChoice(RenderWindow& window) {
    if (board[curX][curY] >= 1 || resultGame)
        return;
    board[curX][curY] = curPlayer;
    curPlayer = 3 - curPlayer; //swap player
    // ktra
    result();

    if (isPVCMode() && curPlayer == aiPlayer && resultGame == 0) {
        makeBotMove();
    }
    if (resultGame) {
        if (resultGame == 1 || resultGame == 2)
            PlaySoundWin();
        else PlaySoundDraw();
    }
}
void BoardGame::setMove(RenderWindow& window) {
    if (resultGame) {
        if (keyBoard.Enter()) {
            reset();
            if (mode == GameMode::PVC)
                curPlayer = 1;
            return;
        }
        else if (keyBoard.Esc()) {
            reset();
            stateMenu = 0;
        }
        return;
    }

    if (isPVCMode() && curPlayer == aiPlayer) {
        makeBotMove();
        return;
    }

    if (keyBoard.Up() && curY > 0) {
        --curY;
        PlaySoundClick(); // Play click sound when moving up
    }
    if (keyBoard.Down() && curY < size - 1) {
        ++curY;
        PlaySoundClick(); // Play click sound when moving down
    }
    if (keyBoard.Left() && curX > 0) {
        --curX;
        PlaySoundClick(); // Play click sound when moving left
    }
    if (keyBoard.Right() && curX < size - 1) {
        ++curX;
        PlaySoundClick(); // Play click sound when moving right
    }
    if (keyBoard.combineAlphabetCheck('L')) // save game
        SaveGame(window);
    if (keyBoard.Enter())
        setChoice(window);
    else if (keyBoard.Esc()) {
        reset();
        stateMenu = 0;
    }
}
bool BoardGame::checkTheSame(vector<int> listCheck) {
    // Null
    if (listCheck[0] == 0) // chua ai chon
        return false;

    // Check the same
    for (int i = 0; i < listCheck.size() - 1; i++)
        if (listCheck[i] != listCheck[i + 1])
            return false;

    return true;
}
int BoardGame::checkResult() {
    // column
    for (int i = 0; i < size - 5 + 1; i++)
        for (int j = 0; j < size; j++)
            if (checkTheSame({ board[i][j], board[i + 1][j], board[i + 2][j], board[i + 3][j], board[i + 4][j] })) {
                return resultGame = board[i][j];
            }

    // row
    for (int j = 0; j < size - 5 + 1; j++)
        for (int i = 0; i < size; i++)
            if (checkTheSame({ board[i][j], board[i][j + 1], board[i][j + 2], board[i][j + 3], board[i][j + 4] })) {
                return resultGame = board[i][j];
            }

    // main cross
    for (int j = 0; j < size - 5 + 1; j++)
        for (int i = 0; i < size - 5 + 1; i++)
            if (checkTheSame({ board[i][j], board[i + 1][j + 1], board[i + 2][j + 2], board[i + 3][j + 3], board[i + 4][j + 4] })) {
                return resultGame = board[i][j];
            }

    // side cross
    for (int j = 0; j < size - 5 + 1; j++)
        for (int i = size - 1; i >= 4; i--)
            if (checkTheSame({ board[i][j], board[i - 1][j + 1], board[i - 2][j + 2], board[i - 3][j + 3], board[i - 4][j + 4] })) {
                return resultGame = board[i][j];
            }

    // not draw
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == 0)
                return resultGame = 0;

    // draw
    return resultGame = 3;
}
void BoardGame::drawWinnerMessage(RenderWindow& window) {

    RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(Color(0, 0, 0, 180));
    window.draw(overlay);

    // tao chu
    Text winnerText;
    winnerText.setFont(font);
    winnerText.setCharacterSize(80);


    std::string message = "";
    Color textColor;

    if (resultGame == 1) {
        message = player1Name + " Wins!";
        textColor = Color::Blue;
    }
    else if (resultGame == 2) {
        message = ((mode == GameMode::PVC) ? player2Name : "Computer") + " Wins!";
        textColor = Color::Red;
    }
    else if (resultGame == 3) {
        message = "It's a Draw!";
        textColor = Color::Yellow;
    }
    else {
        // ko ve
        return;
    }

    winnerText.setString(message);
    winnerText.setFillColor(textColor);

    // can giua
    FloatRect textBounds = winnerText.getLocalBounds();
    winnerText.setOrigin(
        textBounds.width / 2.f,
        textBounds.height / 2.f
    );
    winnerText.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 2.f - 50
    );

    // Add outline for better visibility
    winnerText.setOutlineColor(Color::White);
    winnerText.setOutlineThickness(3);

    window.draw(winnerText);

    // Add instruction text
    Text instructionText;
    instructionText.setFont(font);
    instructionText.setCharacterSize(30);
    instructionText.setString("Press ENTER to play again  |  ESC to return");
    instructionText.setFillColor(Color::White);

    FloatRect instructionBounds = instructionText.getLocalBounds();
    instructionText.setOrigin(
        instructionBounds.width / 2.f,
        instructionBounds.height / 2.f
    );
    instructionText.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 2.f + 80
    );

    window.draw(instructionText);
}
int BoardGame::result() {
    if (resultGame) return resultGame;
    int prev = resultGame;
    int res = checkResult();
    if (showPlayerPanel && prev == 0) {
        if (res == 1) ++player1Score;
        else if (res == 2) ++player2Score;
    }
    return res;
}
void BoardGame::setMode(GameMode newMode) {
    mode = newMode;
    reset();
    player1Score = 0;
    player2Score = 0;

    if (mode == GameMode::PVC) {
        curPlayer = 1; // human starts
        player1Name = playerName[0];
        player2Name = "Computer";
    }
    else if (mode == GameMode::PVP) {
        player1Name = playerName[0];
        player2Name = playerName[1];
        if (player2Name.empty())
			player2Name = "Player 2";
    }

    showPlayerPanel = (mode == GameMode::PVP || mode == GameMode::PVC);
    if (showPlayerPanel) {
        ensurePlayerAssets();
    }
}
void BoardGame::makeBotMove() {
    if (!isPVCMode() || curPlayer != aiPlayer || resultGame)
        return;

    auto move = CalculateBotMove(board, aiPlayer);
    if (move.first < 0 || move.second < 0)
        return;

    if (board[move.first][move.second] != 0)
        return;

    curX = move.first;
    curY = move.second;
    board[move.first][move.second] = aiPlayer;
    curPlayer = 3 - curPlayer;
    result();
}
void BoardGame::ensurePlayerAssets() {
    // set avatar player 1
    // Use saved avatar path from settings, or default if not set
    std::string avatarPath = setting.getPlayer1AvatarPath();
    if (avatarPath.empty()) {
        avatarPath = "assets/image/Avatar/player1_egg-egg-sheeran.png";
    }
    player1PhotoLoaded = player1Photo.loadFromFile(avatarPath);

    // set avatar player 2
    avatarPath = setting.getPlayer2AvatarPath();
    if (avatarPath.empty()) {
        avatarPath = "assets/image/Avatar/player2_onepunchman.png";
    }
    player2PhotoLoaded = player2Photo.loadFromFile(avatarPath);
}
void BoardGame::drawPlayerInfoPanel(RenderWindow& window) {
    const float winWidth = window.getSize().x;
    const float winHeight = window.getSize().y;
    float indentLeft = winWidth * 4 / 100;
    float indentRight = indentLeft;
    float boxWidth = winWidth * 17 / 100;

    // draw player 1 & 2's box
    static Texture playerFrame;
	static Sprite spritePlayer1Frame;
	static Sprite spritePlayer2Frame;
	static bool initialized = false;
    if (!initialized) {
        playerFrame.loadFromFile("assets/image/playerFrame.png");
		spritePlayer1Frame.setTexture(playerFrame);
		spritePlayer2Frame.setTexture(playerFrame);
	}
	float scaleX = boxWidth / playerFrame.getSize().x;
    if (spritePlayer1Frame.getScale().x != scaleX) {
        spritePlayer1Frame.setScale(scaleX, scaleX);
        spritePlayer2Frame.setScale(scaleX, scaleX);
    }
	spritePlayer1Frame.setPosition(indentLeft, (winHeight - spritePlayer1Frame.getGlobalBounds().height) / 2);
	spritePlayer2Frame.setPosition(winWidth - indentRight - spritePlayer2Frame.getGlobalBounds().width, 
        (winHeight - spritePlayer2Frame.getGlobalBounds().height) / 2);

    float boxHeight = spritePlayer1Frame.getGlobalBounds().height;


	// draw player avatar photo
	if (player1PhotoLoaded) {
		spritePlayer1Photo.setTexture(player1Photo, true);
	}
	if (player2PhotoLoaded) {
		spritePlayer2Photo.setTexture(player2Photo, true);
	}
	float scalePhoto1X = boxWidth * 80 / 100 / player1Photo.getSize().x;
	float scalePhoto2X = boxWidth * 80 / 100 / player2Photo.getSize().x;
	float scalePhoto1Y = boxHeight * 60 / 100 / player1Photo.getSize().y;
	float scalePhoto2Y = boxHeight * 60 / 100 / player2Photo.getSize().y;
    spritePlayer1Photo.setScale(scalePhoto1X, scalePhoto1Y);
    spritePlayer2Photo.setScale(scalePhoto2X, scalePhoto2Y);
    spritePlayer1Photo.setPosition(indentLeft + boxWidth * 10 / 100, 
		(winHeight - spritePlayer1Photo.getGlobalBounds().height) / 2 - winHeight * 3 / 100) ;
	spritePlayer2Photo.setPosition(winWidth - indentRight - boxWidth + boxWidth * 10 / 100,
		(winHeight - spritePlayer2Photo.getGlobalBounds().height) / 2 - winHeight * 3 / 100) ;
	window.draw(spritePlayer1Photo);
	window.draw(spritePlayer2Photo);
    window.draw(spritePlayer1Frame);
    window.draw(spritePlayer2Frame);

    // draw text info
    // player 1 name
    float textHeight = boxWidth * 12 / 100;
    float posX = indentLeft + boxWidth / 2;
    float indentTextY = (winHeight - boxHeight) / 2 + boxHeight * 72 / 100;
	static Text text;
    static Font fontInfo;
    if (!initialized) {
	    fontInfo.loadFromFile("assets/font/PixelPurl.ttf");
        text.setFont(fontInfo);
    }
    text.setString(playerName[0]);
	text.setCharacterSize(getCharacterSizeForLineHeight(fontInfo, textHeight));
    text.setFillColor(Color::Yellow);
    FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2, bounds.top);
	text.setPosition(posX, indentTextY);
    window.draw(text);

    // player 2 name
	posX = winWidth - indentRight - boxWidth / 2;
	text.setString(mode == GameMode::PVC ? "Computer" : playerName[1]);
	text.setCharacterSize(getCharacterSizeForLineHeight(fontInfo, textHeight));
	bounds = text.getLocalBounds();
	text.setOrigin(bounds.left + bounds.width / 2, bounds.top);
	text.setPosition(posX, indentTextY);
	window.draw(text);

    // player 1 score
	float scoreHeight = boxWidth * 15 / 100;
	posX = indentLeft + boxWidth / 2;
	text.setString("Score: " + to_string(player1Score));
	text.setCharacterSize(getCharacterSizeForLineHeight(fontInfo, scoreHeight));
	bounds = text.getLocalBounds();
	text.setOrigin(bounds.left + bounds.width / 2, bounds.top);
	text.setPosition(posX, indentTextY + boxHeight * 10 / 100);
	text.setFillColor(Color::White);
	window.draw(text);

    // player 2 score
	posX = (winWidth - indentRight) - boxWidth / 2;
	text.setString("Score: " + to_string(player2Score));
	text.setCharacterSize(getCharacterSizeForLineHeight(fontInfo, scoreHeight));
	bounds = text.getLocalBounds();
	text.setOrigin(bounds.left + bounds.width / 2, bounds.top);
	text.setPosition(posX, indentTextY + boxHeight * 10 / 100);
	window.draw(text);
    
    initialized = true;
}