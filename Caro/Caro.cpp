#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace sf;

Font font;
int state = 0;
struct KeyBoardPressed {
    short mask = 0;
    const short UP = 1 << 0;
    const short RIGHT = 1 << 1;
    const short LEFT = 1 << 2;
    const short DOWN = 1 << 3;
    const short ENTER = 1 << 4;
    const short ESC = 1 << 5;
    const short ALL = (1 << 6) - 1;

    bool Up() {
        return mask & UP;
    }
    bool Right() {
        return mask & RIGHT;
    }
    bool Down() {
        return mask & DOWN;
    }
    bool Left() {
        return mask & LEFT;
    }
    bool Enter() {
        return mask & ENTER;
    }
    bool Esc() {
        return mask & ESC;
    }
    bool isAnyKeyPressed() {
        return mask;
	}
    short setState(RenderWindow& window) {
        short maskINP = 0;
        if (Keyboard::isKeyPressed(Keyboard::Key::W))
            maskINP |= UP;
        if (Keyboard::isKeyPressed(Keyboard::Key::A))
            maskINP |= LEFT;
        if (Keyboard::isKeyPressed(Keyboard::Key::S))
            maskINP |= DOWN;
        if (Keyboard::isKeyPressed(Keyboard::Key::D))
            maskINP |= RIGHT;
        if (Keyboard::isKeyPressed(Keyboard::Key::Enter))
            maskINP |= ENTER;
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
            maskINP |= ESC;
        if (maskINP == 0) {
            mask = 0;
            return 0;
        }
        else {
            short preMask = 0;
            for (int i = 0; i < 6; i++)
                if (!(mask & (1 << i))) {
                    if (maskINP & (1 << i))
						preMask |= (1 << i);
                }
            mask = maskINP;
            return preMask;
        }

	}
} keyBoard;

struct Button {
    int x = 0, y = 0, width = 0, height = 0;
    int ID = 0;
    bool selected = false;
    string context = "";
    Button(){}
    void setPosition(int posX, int posY, int widthX, int heightY) {
        x = posX;
        y = posY;
		width = widthX;
		height = heightY;
    }
    void draw(RenderWindow& window) {

        //rectangle
		RectangleShape buttonGUI = RectangleShape(Vector2f(width, height));
        buttonGUI.setPosition(Vector2f(x, y));
        if (selected) {
            if (state == 0) // menuID
                buttonGUI.setFillColor(Color::Green);
            else buttonGUI.setFillColor(Color(100, 100, 100));
        }
        else {
            if (state == 0)
                buttonGUI.setFillColor(Color::Yellow);
            else buttonGUI.setFillColor(Color(158, 158, 158));
        }
		window.draw(buttonGUI);

        //text
        Text text;
        int heightText = height * 90 / 100;

        text.setFont(font);
        text.setString(context);
        text.setCharacterSize(heightText);
        FloatRect textBounds = text.getLocalBounds();

        if (selected) {
            if (state == 0) { //menuID
                text.setFillColor(Color::White);
            }
            else text.setFillColor(Color(185, 185, 185));
        }
        else {
            if (state == 0){ //menuID
                text.setFillColor(Color(208, 95, 147));
            }
			else text.setFillColor(Color(130, 130, 130));
        }
        text.setPosition(
            x + width / 10.f, 
            y + (height / 2.f - (textBounds.top + textBounds.height / 2.f))
        );
        window.draw(text);
    }
};

struct BoardGame {
    const int defaultSize = 12;
    int resultGame = 0;
    int size = defaultSize;
    vector<vector<int> > board;
    int curX = 0, curY = 0;
    int curPlayer = 1;

    float spacingTop = 0;
    float spacingLeft = 0;
    float spacingBoardBetween = 0;
    float widthBoard = 0;
    float heightBoard = 0;
    float spacingCellX = 0;
    float spacingCellY = 0;
    float cellLenX = 0;
    float cellLenY = 0;

    void setUp() {
        board.assign(size, vector<int>(size, 0));
    }
    void reset() {
        board.assign(size, vector<int>(size, 0));
        curX = curY = 0;
        curPlayer = 1;
		resultGame = 0;
    }
    void drawTable(RenderWindow& window) {

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
    }
    void drawPosition(int x, int y, RenderWindow& window) {

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
        else if (board[x][y] == 2){
            text.setFillColor(Color::Red);
            text.setString("X");
        }
        window.draw(text);
	}
    void setChoice(RenderWindow& window) {
        if (board[curX][curY] || resultGame) return;
        board[curX][curY] = curPlayer;
        drawPosition(curX, curY, window);
		curPlayer = 3 - curPlayer;
    }
    void setMove(RenderWindow& window) {
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
    bool checkTheSame(vector<int> listCheck) {
        // Null
        if (listCheck[0] == 0)
            return false;

        // Check the sane
        for (int i = 0; i < listCheck.size() - 1; i++)
            if (listCheck[i] != listCheck[i + 1])
                return false;

        return true;
    }
    int checkResult() {
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
    int result() {
        if (resultGame) return resultGame;
        return checkResult();
    }
} boardGame;

void handleNewGame(RenderWindow& window){
	boardGame.drawTable(window);
}

void handleLoadGame(RenderWindow& window){

}

void handleSettings(RenderWindow& window){

}

struct Menu {
    const int ID = 0;
    vector<Button> listButton;
    const int newGameID = 0, loadGameID = 1, settingID = 2;
    int width = 0, height = 0;
    int selectedButton = 0;
    void draw(RenderWindow& window) {

		//make new buttons if not exist
        if (!listButton.size()) {
            listButton.assign(3, Button());

			listButton[newGameID].context = "New Game";
			listButton[loadGameID].context = "Load Game";
			listButton[settingID].context = "Settings";
			listButton[newGameID].ID = 1;
			listButton[loadGameID].ID = 2;
			listButton[settingID].ID = 3;

            listButton[newGameID].selected = true;
            state = ID;
        }

        // size menu
        width = window.getSize().x * 80 / 100;
        height = window.getSize().y * 80 / 100;

        // size button
		int widthButton = width * 60 / 100;
		int heightButton = height * 10 / 100;
        int spacingTop = height * 20 / 100;
        int spacingBetween = (height - (spacingTop * 2) - listButton.size() * heightButton) / (listButton.size() - 1);

        //draw
        for (int i = 0; i < 3; i++) {
            auto& button = listButton[i];
            button.setPosition(
                window.getSize().x / 2 - widthButton / 2,                                                   // x
                window.getSize().y / 2 - height / 2 + spacingTop + (heightButton + spacingBetween) * i,     // y
                widthButton,                                                                                // x_len
                heightButton                                                                                // y_len
            );
            button.draw(window);
        }
	}
    void handleUI(RenderWindow& window) {
        draw(window);
        if (state == listButton[0].ID)
            handleNewGame(window);
        else if (state == listButton[1].ID)
            handleLoadGame(window);
        else if (state == listButton[2].ID)
			handleSettings(window);
    }
    void updateState(RenderWindow& window) {
        if (state == ID) {
            if (keyBoard.Up() ^ keyBoard.Down()) {
                listButton[selectedButton].selected = false;

                if (keyBoard.Up()) {
                    --selectedButton;
                    if (selectedButton < 0)
                        selectedButton = listButton.size() - 1;
                }
                else {
                    ++selectedButton;
                    if (selectedButton == listButton.size())
                        selectedButton = 0;
                }

                listButton[selectedButton].selected = true;
            }
            else if (keyBoard.Enter()) {
                state = listButton[selectedButton].ID;
            }
            else if (keyBoard.Esc()) {
                window.close();
			}
        }
        else if (state == listButton[newGameID].ID) {
			boardGame.setMove(window);
        }
    }

} menuGUI;

int main()
{
    VideoMode desktop = VideoMode::getDesktopMode();
	const float widthApp = desktop.width * 60.f / 100;
	const float heightApp = desktop.height * 70.f / 100;

    RenderWindow window(VideoMode(widthApp, heightApp), "Caro Game!");

    //setFont
    if (!font.getInfo().family.size()) {
        if (!font.loadFromFile("../../../assets/font/arial.ttf")) {
            cout << "Failed to load font!" << endl;
            return -1;
        }
	}

    boardGame.setUp(); // set up board game
    bool firstTime = true; // first time draw

    while (window.isOpen())
    {   

        window.clear();
        Event event;
        while (firstTime || window.pollEvent(event))
        {
            if (!firstTime && event.type == Event::Closed)
                window.close();
            else if (firstTime || event.type == Event::KeyPressed || event.type == Event::KeyReleased){
                firstTime = false;

                //struct kem linh hoat -> xuli mask o ngoai ham
                int preMask = keyBoard.setState(window);
				int nxtMask = keyBoard.mask;
                keyBoard.mask = preMask;
				menuGUI.updateState(window);
                keyBoard.mask = nxtMask;

				menuGUI.handleUI(window);
                window.display();
            }
        }
    }

    return 0;
}