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
		backgroundMusic.setVolume(50.f); // set volume to 50%
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
void PlaySoundWin()
{
    static SoundBuffer winBuffer;
    static Sound winSound;
    static bool loaded = false;
    
    if (!loaded) {
        if (!winBuffer.loadFromFile("assets/Music&sfx/sfx/winsound.wav")) {
            return;
        }
        winSound.setBuffer(winBuffer);
        loaded = true;
    }
    
    winSound.play();
}
void PlaySoundDraw()
{
    static SoundBuffer drawBuffer;
    static Sound drawSound;
    static bool loaded = false;

    if (!loaded) {
        if (!drawBuffer.loadFromFile("assets/Music&sfx/sfx/drawsound.wav")) {
            return;
        }
        drawSound.setBuffer(drawBuffer);
        loaded = true;
    }

    drawSound.play();
}