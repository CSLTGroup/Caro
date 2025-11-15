#include "global.h"
using namespace std;
using namespace sf;

Font font;
int state = 0;
KeyBoardPressed keyBoard;
Menu menuGUI;
BoardGame boardGame;

int main()
{
    VideoMode desktop = VideoMode::getDesktopMode();
	const float widthApp = desktop.width * 60.f / 100; // kich thuoc width 60% toan man hinh
	const float heightApp = desktop.height * 70.f / 100; // kich thuoc height 70% toan man hinh

    RenderWindow window(VideoMode(widthApp, heightApp), "Caro Game!");

    //setFont
    if (!font.getInfo().family.size()) {
        if (!font.loadFromFile("assets/font/arial.ttf")) {
            cout << "Failed to load font!" << endl;
            return -1;
        }
	}

    boardGame.setUp(); // set up board game
    bool firstTime = true; // draw lan dau khi window load
    
    while (window.isOpen())
    {   
        
        Event event;
        while (firstTime || window.pollEvent(event))
        {
            if (!firstTime && event.type == Event::Closed)
                window.close();
            else if (firstTime || event.type == Event::KeyPressed || event.type == Event::KeyReleased || event.type == Event::TextEntered){

                window.clear();

                // Handle text input for name entry
                if (state == 4 || state == 5) {
                    if (event.type == Event::TextEntered && !firstTime && event.text.unicode < 128) {
                        char c = static_cast<char>(event.text.unicode);
                        if (c == '\b') { // Backspace
                            if (state == 4 && !boardGame.player1Name.empty()) {
                                boardGame.player1Name.pop_back();
                            }
                            else if (state == 5 && !boardGame.player2Name.empty()) {
                                boardGame.player2Name.pop_back();
                            }
                        }
                        else if (c >= 32 && c < 127) { // Printable characters
                            if (state == 4 && boardGame.player1Name.length() < 20) {
                                boardGame.player1Name += c;
                            }
                            else if (state == 5 && boardGame.player2Name.length() < 20) {
                                boardGame.player2Name += c;
                            }
                        }
                    }
                    if (event.type == Event::KeyPressed && !firstTime && event.key.code == Keyboard::Enter) {
                        if (state == 4) {
                            // Move to player 2 name input
                            if (boardGame.player1Name.empty()) {
                                boardGame.player1Name = "Player 1";
                            }
                            state = 5;
                        }
                        else if (state == 5) {
                            // Start the game
                            if (boardGame.player2Name.empty()) {
                                boardGame.player2Name = "Player 2";
                            }
                            state = 1; // Go to game state (newGameID = 0, so listButton[0].ID = 1)
                            boardGame.reset();
                        }
                    }
                }

                keyBoard.setState(window); // update keyboard state
				menuGUI.handleUI(window); // update menu & smaller GUI state
                window.display(); // show menu

                firstTime = false;
            }
        }
    }

    return 0;
}