#include "../global.h"

void startGame() {
    // set up window
    VideoMode desktop = VideoMode::getDesktopMode();
    const double widthRES = desktop.width;   // kich thuoc width toan man hinh
    const double heightRES = desktop.height;  // kich thuoc height toan man hinh
    if (idWindowSize == -1) {
        idWindowSize = listWindowSize.size() - 1;
        while (idWindowSize >= 0 && (listWindowSize[idWindowSize].first > widthRES
            || listWindowSize[idWindowSize].second > heightRES)) {
            --idWindowSize;
        }
    }
    RenderWindow window(
        VideoMode(listWindowSize[idWindowSize].first, listWindowSize[idWindowSize].second),
        "Caro Game!",
        Style::Close
    );

    // set font
    if (!font.getInfo().family.size()) {
        if (!font.loadFromFile("assets/font/RDLatinFontPoint.otf")) {
            cout << "Failed to load font!" << endl;
            return;
        }
    }

    // loading screen
    loadingScreen(window);

    // load settings before starting background music
    setting.LoadSettings(window);

    // start background music after loading screen
    BackGroundMusic(window); // ham goi nhac nen

    // set up board game
    boardGame.setUp();

    // check condition required to show "firstime" menu or normal menu
    if (confirmedSettingsFirstTime)
        stateMenu = 0;

    while (window.isOpen()) {
        Event event;
        window.pollEvent(event);
        if (event.type == Event::Closed)
            window.close();
        else if (event.type == Event::KeyPressed || event.type == Event::KeyReleased)
            keyBoard.setState(window); // update keyboard state


        window.clear();
        menuGUI.handleUI(window);  // update menu & smaller GUI state
        window.display();          // show menu

    }
}