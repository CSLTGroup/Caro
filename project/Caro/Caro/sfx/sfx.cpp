#include "../global.h"

void BackGroundMusic(RenderWindow& window)
{
    static Music backgroundMusic;
    static bool loaded = false;

    if (!loaded) {
        if (!backgroundMusic.openFromFile("assets/Music&sfx/backgroundMusic/background.ogg")) {
            return;
        }
        backgroundMusic.setLoop(true);
        backgroundMusic.play(); // Start playing immediately after loading
        loaded = true;
    }

    if (backgroundMusic.getStatus() == Music::Stopped) {
        backgroundMusic.play();
    }
}
void PlaySoundClick()
{
    static SoundBuffer clickBuffer;
    static Sound clickSound;
    static bool loaded = false;
    
    if (!loaded) {
        if (!clickBuffer.loadFromFile("assets/Music&sfx/sfx/clicksound.mp3")) {
            return;
        }
        clickSound.setBuffer(clickBuffer);
        loaded = true;
    }
    
    clickSound.play();
}