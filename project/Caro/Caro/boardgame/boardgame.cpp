#include "../global.h"

void BoardGame::setUp() {
    board.assign(size, vector<int>(size, 0));
}
void BoardGame::reset() {
    board.assign(size, vector<int>(size, 0));
    curX = curY = 0;
    curPlayer = 1;
    resultGame = 0;
}
void BoardGame::drawTable(RenderWindow& window) {

    // set up size
    spacingTop = window.getSize().y * 20.f / 100;
    spacingLeft = window.getSize().y * 10.f / 100;
    spacingBoardBetween = window.getSize().y * 5.f / 100; // 1 nua khoang cach giua board va ty so
    widthBoard = window.getSize().x / 2.f - spacingLeft - spacingBoardBetween;
    heightBoard = window.getSize().y - 2.f * spacingTop;
    spacingCellX = widthBoard * 2.f / 100;
    spacingCellY = heightBoard * 2.f / 100;
    cellLenX = (widthBoard - spacingCellX * (size + 1)) / size;
    cellLenY = (heightBoard - spacingCellY * (size + 1)) / size;

    // bg
    RectangleShape boardBG = RectangleShape(Vector2f(widthBoard, heightBoard));
    boardBG.setPosition(Vector2f(spacingLeft, spacingTop));
    boardBG.setFillColor(Color(206, 201, 194));
    window.draw(boardBG);

    // cell
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            drawPosition(i, j, window);
        }
    }
    
    // Draw player names
    drawPlayerNames(window);
    
    // Draw winner message if game is over
    if (resultGame == 1 || resultGame == 2 || resultGame == 3) {
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
    if (x == curX && y == curY)
        cell.setFillColor(Color::Cyan);
    window.draw(cell);

    // text 
    int heightText = min(cellLenX, cellLenY) * 80.f / 100;
    Text text;
    text.setFont(font);
    text.setCharacterSize(heightText);
    FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(
        textBounds.width / 2.f,
        textBounds.height / 2.f
    );
    text.setPosition(
        spacingLeft + spacingCellX + x * (cellLenX + spacingCellX) + cellLenX / 4.f,
        spacingTop + spacingCellY + y * (cellLenY + spacingCellY)
    );
    if (board[x][y] == 1) {
        text.setFillColor(Color::Blue);
        text.setString("O");
    }
    else if (board[x][y] == 2) {
        text.setFillColor(Color::Red);
        text.setString("X");
    }
    window.draw(text);
}
void BoardGame::drawPlayerNames(RenderWindow& window) {
    // Player 1 name (left side)
    Text player1Text;
    player1Text.setFont(font);
    player1Text.setString(player1Name + " (O)");
    player1Text.setCharacterSize(24);
    player1Text.setFillColor(Color::Blue);
    player1Text.setPosition(
        spacingLeft,
        spacingTop / 2 - 12
    );
    window.draw(player1Text);
    
    // Player 2 name (right side)
    Text player2Text;
    player2Text.setFont(font);
    player2Text.setString(player2Name + " (X)");
    player2Text.setCharacterSize(24);
    player2Text.setFillColor(Color::Red);
    FloatRect textBounds = player2Text.getLocalBounds();
    player2Text.setPosition(
        spacingLeft + widthBoard - textBounds.width,
        spacingTop / 2 - 12
    );
    window.draw(player2Text);
    
    // Current player indicator
    Text currentPlayerText;
    currentPlayerText.setFont(font);
    string currentPlayerName = (curPlayer == 1) ? player1Name : player2Name;
    currentPlayerText.setString("Current: " + currentPlayerName);
    currentPlayerText.setCharacterSize(20);
    currentPlayerText.setFillColor(Color::White);
    FloatRect currentBounds = currentPlayerText.getLocalBounds();
    currentPlayerText.setPosition(
        spacingLeft + widthBoard / 2 - currentBounds.width / 2,
        spacingTop + heightBoard + 10
    );
    // Only show current player if game is not over
    if (resultGame == 0) {
        window.draw(currentPlayerText);
    }
}
void BoardGame::drawWinnerMessage(RenderWindow& window) {
    // Semi-transparent overlay
    RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(Color(0, 0, 0, 180));
    window.draw(overlay);
    
    // Winner message box
    float boxWidth = window.getSize().x * 0.6f;
    float boxHeight = window.getSize().y * 0.3f;
    RectangleShape messageBox(Vector2f(boxWidth, boxHeight));
    messageBox.setFillColor(Color(50, 50, 50, 240));
    messageBox.setOutlineColor(Color::Yellow);
    messageBox.setOutlineThickness(5);
    messageBox.setPosition(
        window.getSize().x / 2 - boxWidth / 2,
        window.getSize().y / 2 - boxHeight / 2
    );
    window.draw(messageBox);
    
    // Winner text
    Text winnerText;
    winnerText.setFont(font);
    
    if (resultGame == 1) {
        winnerText.setString(player1Name + " WINS!");
        winnerText.setFillColor(Color::Blue);
    }
    else if (resultGame == 2) {
        winnerText.setString(player2Name + " WINS!");
        winnerText.setFillColor(Color::Red);
    }
    else if (resultGame == 3) {
        winnerText.setString("DRAW!");
        winnerText.setFillColor(Color::Yellow);
    }
    
    winnerText.setCharacterSize(48);
    FloatRect winnerBounds = winnerText.getLocalBounds();
    winnerText.setPosition(
        window.getSize().x / 2 - winnerBounds.width / 2,
        window.getSize().y / 2 - boxHeight / 2 + 40
    );
    window.draw(winnerText);
    
    // Instruction text
    Text instructionText;
    instructionText.setFont(font);
    instructionText.setString("Press ESC to return to menu");
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(Color::White);
    FloatRect instBounds = instructionText.getLocalBounds();
    instructionText.setPosition(
        window.getSize().x / 2 - instBounds.width / 2,
        window.getSize().y / 2 - boxHeight / 2 + 120
    );
    window.draw(instructionText);
}
void BoardGame::setChoice(RenderWindow& window) {
    if (board[curX][curY] >= 1 || resultGame) return;
    board[curX][curY] = curPlayer;
    drawPosition(curX, curY, window);
    
    // Check for winner after placing a piece
    int gameResult = result();
    if (gameResult == 1 || gameResult == 2) {
        // Someone won! Play celebration sound
        PlayWinSound();
    }
    
    curPlayer = 3 - curPlayer; //swap player
}
void BoardGame::setMove(RenderWindow& window) {
    // Check if it's computer's turn in PVC mode
    if (gameMode == 1 && curPlayer == 2 && resultGame == 0) {
        makeComputerMove(window);
        return;
    }
    
    if (keyBoard.Up() && curY > 0)
        --curY;
    if (keyBoard.Down() && curY < size - 1)
        ++curY;
    if (keyBoard.Left() && curX > 0)
        --curX;
    if (keyBoard.Right() && curX < size - 1)
        ++curX;
    if (keyBoard.Enter())
        setChoice(window);
    else if (keyBoard.Esc()) {
        reset();
        state = 0;
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
int BoardGame::result() {
    if (resultGame) return resultGame;
    return checkResult();
}
pair<int, int> BoardGame::findBestMove() {
    // Strategy: 
    // 1. Check if computer can win (4 in a row)
    // 2. Check if need to block player (player has 4 in a row)
    // 3. Try to make 3 in a row
    // 4. Block player's 3 in a row
    // 5. Make a strategic move (center or near opponent)
    
    int computer = 2;
    int player = 1;
    
    // 1. Check if computer can win
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {
                board[i][j] = computer;
                int result = checkResult();
                board[i][j] = 0;
                if (result == computer) {
                    return {i, j};
                }
            }
        }
    }
    
    // 2. Block player from winning
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {
                board[i][j] = player;
                int result = checkResult();
                board[i][j] = 0;
                if (result == player) {
                    return {i, j};
                }
            }
        }
    }
    
    // 3. Try to make 3 in a row (offensive)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {
                board[i][j] = computer;
                // Check if this creates a good position
                int count = 0;
                // Check horizontal
                for (int k = max(0, j-4); k <= min(size-5, j); k++) {
                    int same = 0;
                    for (int l = 0; l < 5; l++) {
                        if (board[i][k+l] == computer) same++;
                        else if (board[i][k+l] != 0) { same = 0; break; }
                    }
                    if (same >= 3) count++;
                }
                // Check vertical
                for (int k = max(0, i-4); k <= min(size-5, i); k++) {
                    int same = 0;
                    for (int l = 0; l < 5; l++) {
                        if (board[k+l][j] == computer) same++;
                        else if (board[k+l][j] != 0) { same = 0; break; }
                    }
                    if (same >= 3) count++;
                }
                board[i][j] = 0;
                if (count > 0) {
                    return {i, j};
                }
            }
        }
    }
    
    // 4. Block player's 3 in a row (defensive)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {
                board[i][j] = player;
                int count = 0;
                // Check horizontal
                for (int k = max(0, j-4); k <= min(size-5, j); k++) {
                    int same = 0;
                    for (int l = 0; l < 5; l++) {
                        if (board[i][k+l] == player) same++;
                        else if (board[i][k+l] != 0) { same = 0; break; }
                    }
                    if (same >= 3) count++;
                }
                // Check vertical
                for (int k = max(0, i-4); k <= min(size-5, i); k++) {
                    int same = 0;
                    for (int l = 0; l < 5; l++) {
                        if (board[k+l][j] == player) same++;
                        else if (board[k+l][j] != 0) { same = 0; break; }
                    }
                    if (same >= 3) count++;
                }
                board[i][j] = 0;
                if (count > 0) {
                    return {i, j};
                }
            }
        }
    }
    
    // 5. Strategic move: prefer center or near opponent pieces
    // Try center first
    int center = size / 2;
    if (board[center][center] == 0) {
        return {center, center};
    }
    
    // Find a move near opponent pieces
    for (int dist = 1; dist < size; dist++) {
        for (int i = max(0, center - dist); i <= min(size-1, center + dist); i++) {
            for (int j = max(0, center - dist); j <= min(size-1, center + dist); j++) {
                if (board[i][j] == 0) {
                    // Check if near opponent piece
                    for (int di = -1; di <= 1; di++) {
                        for (int dj = -1; dj <= 1; dj++) {
                            if (di == 0 && dj == 0) continue;
                            int ni = i + di, nj = j + dj;
                            if (ni >= 0 && ni < size && nj >= 0 && nj < size && board[ni][nj] == player) {
                                return {i, j};
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Fallback: find any empty spot
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {
                return {i, j};
            }
        }
    }
    
    return {center, center};
}
void BoardGame::makeComputerMove(RenderWindow& window) {
    if (resultGame != 0) return;
    
    pair<int, int> move = findBestMove();
    curX = move.first;
    curY = move.second;
    setChoice(window);
}