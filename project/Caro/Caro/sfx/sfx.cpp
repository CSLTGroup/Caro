#include "../global.h"

// Sound objects
static sf::Music backgroundMusic;
static sf::SoundBuffer clickBuffer;
static sf::Sound clickSound;
static sf::SoundBuffer winBuffer;
static sf::Sound winSound;
static sf::SoundBuffer drawBuffer;
static sf::Sound drawSound;
static sf::SoundBuffer loadingBuffer;
static sf::Sound loadingSound;

// initial settings
bool soundMute = false;
float musicVolume = 100.0f;
float EffectVolume = 100.0f;

void BackGroundMusic(RenderWindow &window) {
  static bool loaded = false;
  if (!loaded) {
    if (!backgroundMusic.openFromFile(
            "assets/Music&sfx/backgroundMusic/background.ogg")) {
      return;
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(musicVolume); // set volume initially to 100%
    backgroundMusic.play(); // Start playing immediately after loading
    loaded = true;
  }

  if (backgroundMusic.getStatus() == Music::Stopped) {
    backgroundMusic.play();
  }
}
void PlaySoundClick() {
  static bool loaded = false;

  if (!loaded) {
    if (!clickBuffer.loadFromFile("assets/Music&sfx/sfx/clicksound.mp3")) {
      return;
    }
    clickSound.setBuffer(clickBuffer);
    loaded = true;
  }

  // Set volume based on mute state
  clickSound.setVolume(soundMute ? 0.0f : EffectVolume);
  clickSound.play();
}
void PlaySoundWin() {
  static bool loaded = false;

  if (!loaded) {
    if (!winBuffer.loadFromFile("assets/Music&sfx/sfx/winsound.wav")) {
      return;
    }
    winSound.setBuffer(winBuffer);
    loaded = true;
  }

  winSound.play(); // Volume is managed by SoundMute() and SetEffectVolume()
}
void PlaySoundDraw() {
  static bool loaded = false;

  if (!loaded) {
    if (!drawBuffer.loadFromFile("assets/Music&sfx/sfx/drawsound.wav")) {
      return;
    }
    drawSound.setBuffer(drawBuffer);
    loaded = true;
  }

  drawSound.play(); // Volume is managed by SoundMute() and SetEffectVolume()
}
void PlaySoundLoading() {
  static bool loaded = false;
  if (!loaded) {
    if (!loadingBuffer.loadFromFile(
            "assets/Music&sfx/backgroundMusic/loading.mp3")) {
      return;
    }
    loadingSound.setBuffer(loadingBuffer);
    loaded = true;
  }
  // Set volume based on mute state and effect volume
  loadingSound.setVolume(soundMute ? 0.0f : EffectVolume);
  loadingSound.play();
}
// Ajust sound option
// Mute/Unmute all features
void SoundMute() {
  soundMute = !soundMute;
  if (soundMute) {
    backgroundMusic.setVolume(0.0f);
    winSound.setVolume(0.0f);
    drawSound.setVolume(0.0f);
  } else {
    backgroundMusic.setVolume(musicVolume);
    winSound.setVolume(EffectVolume);
    drawSound.setVolume(EffectVolume);
  }
}

// Set volume_level of music
void SetMusicVolume(float volume) {
  musicVolume = max(0.0f, min(100.0f, volume));
  if (!soundMute) {
    backgroundMusic.setVolume(musicVolume);
  }
}
float GetMusicVolume() { return musicVolume; }

// Set volume_level of effects
void SetEffectVolume(float volume) {
  EffectVolume = max(0.0f, min(100.0f, volume));
  if (!soundMute) {
    clickSound.setVolume(EffectVolume);
    winSound.setVolume(EffectVolume);
    drawSound.setVolume(EffectVolume);
  }
}
float GetEffectVolume() { return EffectVolume; }