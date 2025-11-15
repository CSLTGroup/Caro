#include "../global.h"

void loadingScreen(RenderWindow &window)
{
	const int widthApp = window.getSize().x;
	const int heightApp = window.getSize().y;
	const int LOADING_TIME = 4000000; // microsecond - 4s
	const int TOTAL_SECTION = 4; // four sections: 25% - 50% - 75% - 100%
	const int LOADING_PERCENT_TIME = LOADING_TIME / (TOTAL_SECTION + 1) * TOTAL_SECTION; // microsecond - 2.5s

	// set up loading screen animation
	Texture loadingTexture;
	if (!loadingTexture.loadFromFile("assets/image/penguinGOGO.png")) {
		cout << "Failed to load loading screen image!" << endl;
		return;
	}
	const int frameHeight = 32; // kich thuoc chim canh cut
	const int frameWidth = 32;
	const int numFrames = 8; // number of different imgs to load
	Sprite loadingSprite(loadingTexture);
	loadingSprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
	loadingSprite.scale(5.f, 5.f); // tang kich thuoc 3 lan
	int currentFrame = -1;
	int frameDuration = 150000; // 0.15 seconds per frame
	int elapsed = 0.f;

	// set up text
	const string prefixLoadText = "Loading... - ";
	int curSection = 0; // curSection = 0/1/2/3/4

	Text loadingText;
	loadingText.setFont(font);
	loadingText.setCharacterSize(32);
	loadingText.setString(prefixLoadText + "0%");
	loadingText.setFillColor(Color::White);

	FloatRect bounds = loadingText.getLocalBounds();
	loadingText.setOrigin(bounds.width / 2, bounds.height / 2);
	loadingText.setPosition(Vector2f(widthApp * 50.f/100, heightApp * 90.f/100));

	// set up timer
	Clock clock;
	int durLoadScreen = 0;
	loadingSprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
	loadingSprite.setPosition(widthApp * 50.f/100, heightApp * 75.f/100);
	bool changed = true;
	do {
		int duration = clock.restart().asMicroseconds();
		durLoadScreen += duration;
		elapsed += duration;

		// update animation frame
		if (elapsed >= frameDuration) {
			elapsed = 0;
			currentFrame = (currentFrame + 1) % numFrames;
			loadingSprite.setTextureRect(IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));

			changed = true;
		}

		// update loading percent text
		if (curSection < TOTAL_SECTION && durLoadScreen * TOTAL_SECTION / LOADING_PERCENT_TIME != curSection) {
			curSection = durLoadScreen * TOTAL_SECTION / LOADING_PERCENT_TIME; 
			loadingText.setString(prefixLoadText + to_string(curSection * 100 / TOTAL_SECTION) + "%"); // "Loading... - x%"

			changed = true;
		}

		// frame/text changed -> update window
		if (changed) {
			window.clear(Color(28, 168, 168));
			window.draw(loadingSprite);
			window.draw(loadingText);
			window.display();
		}

	} while (durLoadScreen <= LOADING_TIME);

}