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
            else if (firstTime || event.type == Event::KeyPressed || event.type == Event::KeyReleased){

                window.clear();

                keyBoard.setState(window); // update keyboard state
				menuGUI.handleUI(window); // update menu & smaller GUI state
                window.display(); // show menu

                firstTime = false;
            }
        }
    }

    return 0;
}