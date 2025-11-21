#include "../global.h"



void startGame() {

	// set up window
    VideoMode desktop = VideoMode::getDesktopMode();
    const double widthApp = desktop.width; // kich thuoc width toan man hinh
    const double heightApp = desktop.height; // kich thuoc height toan man hinh
    RenderWindow window(VideoMode(desktop.width, desktop.height), "Caro Game!", Style::Close);


    // set font
    if (!font.getInfo().family.size()) {
        if (!font.loadFromFile("assets/font/RDLatinFontPoint.otf")) {
            cout << "Failed to load font!" << endl;
            return;
        }
    }

    // loading screen
    loadingScreen(window);

    // start background music after loading screen
	BackGroundMusic(window); // ham goi nhac nen

    // set up board game
    boardGame.setUp(); 
    bool firstTime = true; // draw lan dau khi window load

    while (window.isOpen())
    {

        Event event;
        while (firstTime || window.pollEvent(event))
        {
            if (!firstTime && event.type == Event::Closed)
                window.close();
            else if (firstTime || event.type == Event::KeyPressed || event.type == Event::KeyReleased) {

                window.clear();

                keyBoard.setState(window); // update keyboard state
                menuGUI.handleUI(window); // update menu & smaller GUI state
                window.display(); // show menu

                firstTime = false;
            }
        }
    }

}