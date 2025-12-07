#ifndef _SETTING_H_
#define _SETTING_H_
#include "../global.h"

struct Settings {
    void SettingsLogic(RenderWindow& window);
    void sfx();
    void draw(RenderWindow& window);
    void SettingButtons(RenderWindow& window);
    void subSoundSettingBox(RenderWindow& window);
    void generalSettingsBox(RenderWindow& window, int IDButton, int row,
        string contextString = "", int selectedC = -1);

    // For overlay during gameplay
    void handleSettingsOverlay(RenderWindow& window);
    void settingBoxOverlay(RenderWindow& window);
    void SettingButtonsOverlay(RenderWindow& window);
    void generalSettingsBoxOverlay(RenderWindow& window, int IDButton, int row);

    // Change player avatar
    void changeAvatar();
    void drawAvatarChangeBox(RenderWindow& window);

    // Get avatar paths for boardgame
    std::string getPlayer1AvatarPath() const { return player1AvatarPath; }
    std::string getPlayer2AvatarPath() const { return player2AvatarPath; }

    // Save/Load settings
    void SaveSettings();
    void LoadSettings(RenderWindow&);
private:
    std::vector<sf::Texture> avatarTextures; // image data
    std::vector<sf::Sprite> avatarSprites;   // up the image for drawing
    std::vector<std::string> avatarPaths;   // store avatar file paths
    int selectedAvatarIndex = -1;            // currently selected avatar index
    std::string player1AvatarPath = "assets/image/Avatar/player1_egg-egg-sheeran.gif"; // Default avatar paths
    std::string player2AvatarPath = "assets/image/Avatar/player2_onepunchman.jpg";
    void loadAllAvatars(); // load images automatically
};

#endif