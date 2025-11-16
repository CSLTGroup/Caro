#include "../global.h"

void BackgroundMusic() {
	static Music backgroundMusic;
	if (!backgroundMusic.openFromFile("resources/sfx/background_music.ogg")) {
		std::cerr << "Error loading background music!" << std::endl;
		return;
	}
	backgroundMusic.setLoop(true);
	backgroundMusic.setVolume(50); // Set volume to 50%
	backgroundMusic.play();
}