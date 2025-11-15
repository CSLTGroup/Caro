#include "sfx.h"
#include <cmath>

void BackGroundMusic(RenderWindow& window)
{
    static Music backgroundMusic;
    static bool loaded = false;

    if (!loaded) {
        if (!backgroundMusic.openFromFile("assets/sfx/music/background.mp3")) {
            return;
        }
        backgroundMusic.setLoop(true);
        loaded = true;
    }

    if (backgroundMusic.getStatus() == Music::Stopped) {
        backgroundMusic.play();
    }
}

void PlayWinSound()
{
    static SoundBuffer winBuffer;
    static Sound winSound;
    static bool loaded = false;

    if (!loaded) {
        // Try to load a win sound file, if not available, we'll create a simple beep
        // For now, we'll use a simple approach - you can add a win sound file later
        loaded = true;
    }

    // Play a simple beep sound using a generated buffer
    // Note: In a real implementation, you'd load an actual sound file
    // For now, we'll create a simple tone
    if (!winSound.getBuffer()) {
        // Create a simple beep sound buffer
        const int sampleRate = 44100;
        const float duration = 0.5f;
        const int samples = static_cast<int>(sampleRate * duration);
        vector<Int16> samplesData(samples);
        
        for (int i = 0; i < samples; i++) {
            float t = static_cast<float>(i) / sampleRate;
            // Create a pleasant victory sound (chord)
            float freq1 = 523.25f; // C5
            float freq2 = 659.25f; // E5
            float freq3 = 783.99f; // G5
            float sample = (sin(2 * 3.14159f * freq1 * t) * 0.3f +
                           sin(2 * 3.14159f * freq2 * t) * 0.3f +
                           sin(2 * 3.14159f * freq3 * t) * 0.3f) * 0.5f;
            // Apply envelope
            float envelope = 1.0f - (t / duration);
            sample *= envelope;
            samplesData[i] = static_cast<Int16>(sample * 32767);
        }
        
        winBuffer.loadFromSamples(&samplesData[0], samples, 1, sampleRate);
        winSound.setBuffer(winBuffer);
    }
    
    winSound.play();
}
