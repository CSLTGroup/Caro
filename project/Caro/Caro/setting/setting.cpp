#include "../global.h"

// for expanding settings in the future (please add ID buttons down below AND a
// section for specific buttons' variables if needed)
const int totalSetButtons = 6;
const vector<string> contextSetButtons = {
    "Sound",
    "Back to Menu",
    "Exit Game",
    "Change your Name",
    "Change your Avatar",
    "Change resolution"
};

// general settings variables
static int SelectSettings = 0; // 0: Sound 1: Back to Menu 2: Exit Game
static int SelectPage = 0;     // 0: first 3 buttons; 1: next 3 buttons; ...
static int numSettingsCurPage = 3;
const int totalPage = (totalSetButtons + 2) / 3;
bool hoverSetButtons[totalSetButtons] = { false }; // hover button settings
bool inSetButtons[totalSetButtons] = { false };    // check in button settings
bool initialized = false;
static bool inSettings = false;
static bool inGeneralSettings = false; // to ignore first enter key

// id buttons in settings
const int IDSFX = 0;
const int IDBACKTOMENU = 1;
const int IDEXITGAME = 2;
const int IDCHANGENAME = 3;
const int IDCHANGEAVATAR = 4;
const int IDCHANGERESOLUTION = 5;

// sound settings variables
int IDSoundButtons = 0; // 0: mute/ unmute sound 1: adjust music volume 2: adjust effect volume
bool isNotMuted = true;     // true: on, false: off
int MusicVolumeLevel = 20;  // from 0 to 20 step 5% in 100% (0-100%)
int EffectVolumeLevel = 20; // from 0 to 20 step 5% in 100% (0-100%)
static bool inSoundSubmenu = false; // track if we're in the sound settings submenu

// username variables
string playerName[2] = { "Player 1", "Player 2" };
static string tmp_name = "";
static bool inNameSubmenu = false;        // track if we're in the name changing submenu
static bool isTypingName = false;         // track if we're currently typing a name
static int IDNameButtons = 0;             // 0: change player 1 name, 1: change player 2 name
static const int MAX_LENGTH_NAME = 14;

// window size variables
vector<pair<int, int> > listWindowSize = {
    {800, 600},
    {1024, 768},
    {1280, 720},
    {1280, 800},
    {1366, 768},
    {1440, 900},
    {1600, 900},
    {1680, 1050},
    {1920, 1080},
    {1920, 1200},
    {2560, 1440},
    {2560, 1600},
    {3440, 1440},
    {3840, 2160},
    {5120, 1440},
    {5120, 2160},
    {7680, 4320}
};
int idWindowSize = -1; // not set yet
int tmp_idWindowSize = -1;

// UI handle
float boxWidth = 0.0f;  // window.getSize().x * 0.4f; (shared with overlay)
float boxHeight = 0.0f; // window.getSize().y * 0.5f; (shared with overlay)


// avatar change variables
static bool inAvatarSubmenu = false; // track if we're in the avatar changing submenu
static int IDAvatarButtons = 0; // 0: change player 1 avatar, 1: change player 2 avatar
static int numberAvatar = 0; // total number of available avatars
int selectedAvatarIndex = -1; // index of currently selected avatar during change
static bool inAvatarBrowsingMode = false; // true: browsing avatars, false: selecting player

void Settings::sfx() {
    if (keyBoard.Up() ^ keyBoard.Down()) {
        if (keyBoard.Up()) {
            --IDSoundButtons;
            if (IDSoundButtons < 0) IDSoundButtons = 2;
        }
        else {
            ++IDSoundButtons;
            if (IDSoundButtons == 3) IDSoundButtons = 0;
        }
        PlaySoundClick();
    }

    if (IDSoundButtons == 0) { // Adjust mute/unmute
        if (keyBoard.Enter()) {
            PlaySoundClick();
            isNotMuted = !isNotMuted;
            SoundMute();
            SaveSettings(); // Save when mute state changes
        }
    }
    else if (IDSoundButtons == 1) { // Adjust music volume (sound == 1)
        if (keyBoard.Left() ^ keyBoard.Right()) {
            float preMusicVolumeLevel = MusicVolumeLevel;
            if (keyBoard.Left()) MusicVolumeLevel = max(MusicVolumeLevel - 1, 0);
            else MusicVolumeLevel = min(MusicVolumeLevel + 1, 20);
            if (preMusicVolumeLevel != MusicVolumeLevel) {
                float newVolume = MusicVolumeLevel * 5.0f;
                PlaySoundClick();
                SetMusicVolume(newVolume);
                SaveSettings(); // Save when music volume changes
            }
        }
    }
    else if (IDSoundButtons == 2) { // Adjust effect volume (sound == 2)
        if (keyBoard.Left() ^ keyBoard.Right()) {
            float preEffectVolumeLevel = EffectVolumeLevel;
            if (keyBoard.Left()) EffectVolumeLevel = max(EffectVolumeLevel - 1, 0);
            else EffectVolumeLevel = min(EffectVolumeLevel + 1, 20);
            if (EffectVolumeLevel != preEffectVolumeLevel) {
                float newVolume = EffectVolumeLevel * 5.0f;
                PlaySoundClick();
                SetEffectVolume(newVolume);
                SaveSettings(); // Save when effect volume changes
            }
        }
    }
}

void Settings::SettingsLogic(RenderWindow& window) {
    if (!initialized) {
        // music part
        MusicVolumeLevel = (int)(GetMusicVolume() / 5.0f);
        EffectVolumeLevel = (int)(GetEffectVolume() / 5.0f);
        initialized = true;
    }

    if (!inSettings) { // avoid first enter key issue
        // initialize settings part
        SelectSettings = IDSFX; // Reset to Sound button - first Button
        SelectPage = 0;
        numSettingsCurPage = min(3, totalSetButtons - SelectPage * 3);
        inSettings = true;
        inGeneralSettings = true;
    }
    else if (inGeneralSettings) {

        // Action in settings board
        // SelectSettings: 0 = Sound, 1 = Back to Menu, 2 = Exit Game
        if (keyBoard.Up() ^ keyBoard.Down()) {
            int preSelectSettings = SelectSettings;
            if (keyBoard.Up()) {
                --SelectSettings;
                if (SelectSettings < SelectPage * 3)
                    SelectSettings += numSettingsCurPage;
            }
            else {
                ++SelectSettings;
                if (SelectSettings == SelectPage * 3 + numSettingsCurPage)
                    SelectSettings = SelectPage * 3;
            }
            if (preSelectSettings != SelectSettings) {
                hoverSetButtons[preSelectSettings] = false;
                hoverSetButtons[SelectSettings] = true;
                PlaySoundClick();
            }
        }

        // page change
        if (keyBoard.Left() ^ keyBoard.Right()) {
            int preSelectPage = SelectPage;
            if (keyBoard.Left()) {
                SelectPage = max(SelectPage - 1, 0);
            }
            else {
                SelectPage = min(SelectPage + 1, totalPage - 1);
            }
            if (preSelectPage != SelectPage) {
                PlaySoundClick();
                SelectSettings = SelectPage * 3; // reset to first button of the page
                numSettingsCurPage = min(3, totalSetButtons - SelectPage * 3);
            }
        }

        // enter action in settings board
        if (keyBoard.Enter() || keyBoard.Esc()) {
            inGeneralSettings = false;
            if (SelectSettings == IDBACKTOMENU || keyBoard.Esc()) { // back to menu
                SelectSettings = 0;
                numSettingsCurPage = min(3, totalSetButtons - SelectPage * 3);
                SelectPage = 0;
                inSettings = false; // Reset flag when leaving
                stateMenu = 0;      // return to main menu
                return;
            }

            // other buttons
            inSetButtons[SelectSettings] = true;
            if (SelectSettings == IDSFX) { // sound settings submenu
                IDSoundButtons = 0;
            }
            else if (SelectSettings == IDEXITGAME) { // exit game
                window.close();
            }
            else if (SelectSettings == IDCHANGERESOLUTION) { // change resolutionm
                tmp_idWindowSize = idWindowSize;
            }
        }
        else if (SelectSettings == IDCHANGENAME) { // change name submenu
            IDNameButtons = 0;
            isTypingName = false;
            tmp_name = "";
        }
        else if (SelectSettings == IDCHANGEAVATAR) { // change avatar submenu
            IDAvatarButtons = 0;
            inAvatarSubmenu = true;
            inAvatarBrowsingMode = false; // Start in player selection mode
            // Load avatars if not already loaded
            if (avatarTextures.empty()) {
                loadAllAvatars();
            }
            // Set initial selection to current player's avatar
            if (numberAvatar > 0) {
                std::string currentPath = player1AvatarPath;
                selectedAvatarIndex = 0; // Default to first if not found
                for (int i = 0; i < avatarPaths.size(); ++i) {
                    if (avatarPaths[i] == currentPath) {
                        selectedAvatarIndex = i;
                        break;
                    }
                }
            }
            else {
                selectedAvatarIndex = -1;
            }
            PlaySoundClick();
        }
    }
    else // for specific submenu handling
        if (inSetButtons[IDSFX]) { // Handle sound submenu navigation
            sfx();
            // Exit sound submenu on Esc
            if (keyBoard.Esc()) {
                PlaySoundClick();
                inSetButtons[IDSFX] = false;
                IDSoundButtons = 0;       // reset to mute/unmute
                inGeneralSettings = true; // back to general settings
            }
        }
        else if (inSetButtons[IDCHANGENAME]) { // change name
            if (!isTypingName) {
                if (keyBoard.Esc()) {
                    PlaySoundClick();
                    inSetButtons[IDCHANGENAME] = false;
                    inGeneralSettings = true; // back to general settings
                }
                else if (keyBoard.Up() ^ keyBoard.Down()) {
                    IDNameButtons ^= 1;
                    PlaySoundClick();
                }
                else if (keyBoard.Enter()) {
                    PlaySoundClick();
                    isTypingName = true;
                    tmp_name = "";
                }
            }
            else {
                if (keyBoard.Backspace()) {
                    if (!tmp_name.empty()) {
                        PlaySoundClick();
                        tmp_name.pop_back();
                    }
                }
                else if (keyBoard.Enter()) {
                    PlaySoundClick();
                    playerName[IDNameButtons] = tmp_name;
                    isTypingName = false;
                    tmp_name = "";
                    SaveSettings(); // save settings when name changes

                }
                else if (keyBoard.Esc()) {
                    PlaySoundClick();
                    isTypingName = false;
                    tmp_name = "";
                }
                else {
                    bool addedChar = false;
                    if (keyBoard.Shift()) {
                        for (char c = 'A'; c <= 'Z'; c++)
                            if (tmp_name.length() < MAX_LENGTH_NAME &&
                                keyBoard.combineAlphabetCheck(c, true)) {
                                tmp_name += c;
                                addedChar = true;
                            }
                    }
                    else {
                        for (char c = 'A'; c <= 'Z'; c++)
                            if (tmp_name.length() < MAX_LENGTH_NAME &&
                                keyBoard.combineAlphabetCheck(c)) {
                                tmp_name += (char)(c - 'A' + 'a');
                                addedChar = true;
                            }
                    }
                    if (addedChar)
                        PlaySoundClick();
                }
            }
        }
        else if (inSetButtons[IDCHANGERESOLUTION]) { // change resolution
            if (keyBoard.Up() ^ keyBoard.Down()) {
                int preIDWindowSize = tmp_idWindowSize;
                static int nWindowList = listWindowSize.size();
                if (keyBoard.Up())
                    tmp_idWindowSize = min(tmp_idWindowSize + 1, nWindowList - 1);
                else if (keyBoard.Down())
                    tmp_idWindowSize = max(tmp_idWindowSize - 1, 0);
                // neu co thay doi
                if (preIDWindowSize != tmp_idWindowSize) {
                    PlaySoundClick();
                }
            }
            else if (keyBoard.Esc()) {
                tmp_idWindowSize = idWindowSize;
                PlaySoundClick();
                inSetButtons[IDCHANGERESOLUTION] = false;
                inGeneralSettings = true; // back to general settings
            }
            else if (keyBoard.Enter()) {
                // kiem tra neu co thay doi
                if (tmp_idWindowSize != idWindowSize) {
                    idWindowSize = tmp_idWindowSize;
                    // change resolution
                    window.create(VideoMode(listWindowSize[idWindowSize].first, listWindowSize[idWindowSize].second),
                        "Caro Game!",
                        Style::Close);
                    SaveSettings();
                    PlaySoundClick();
                }
            }
        }
        else if (inSetButtons[IDCHANGEAVATAR]) { // change avatar submenu
            changeAvatar();
        }
}

void Settings::draw(RenderWindow& window) {
    // pre set up
    float winWidth = window.getSize().x;
    float winHeight = window.getSize().y;

    // UI part
    boxWidth = winWidth * 0.6f;
    boxHeight = winHeight * 0.5f;

    RectangleShape settingsBox(Vector2f(boxWidth, boxHeight));
    settingsBox.setFillColor(Color(50, 50, 50, 150));
    settingsBox.setOutlineColor(Color::White);
    settingsBox.setOutlineThickness(winHeight * 0.003f);
    settingsBox.setPosition(winWidth / 2 - boxWidth / 2,
        winHeight / 2 - boxHeight / 2);
    window.draw(settingsBox);

    Text titleText;
    titleText.setFont(font);
    titleText.setString("Settings");
    titleText.setCharacterSize((int)(winHeight * 0.05f));
    titleText.setFillColor(Color::Yellow);
    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(window.getSize().x / 2 - titleBounds.width / 2,
        winHeight / 2 - boxHeight / 2 + winHeight * 0.03f);
    window.draw(titleText);

    // draw page
    Text pageText;
    pageText.setFont(font);
    pageText.setString("Page " + to_string(SelectPage + 1) + "/" +
        to_string(totalPage));
    pageText.setCharacterSize((int)(winHeight * 0.03f));
    pageText.setStyle(Text::Italic);
    pageText.setFillColor(Color(120, 158, 158));
    FloatRect pageBounds = pageText.getLocalBounds();
    pageText.setOrigin(pageBounds.width / 2, pageBounds.height / 2);
    pageText.setPosition(winWidth / 2 + boxWidth / 2 - boxWidth * 0.125,
        winHeight / 2 + boxHeight / 2 - boxHeight * 0.07f);
    window.draw(pageText);

    // draw navigation arrow
    static Animation leftarrow("assets/image/leftarrow.png", 1125, 1040, 10, 3, 4);
    static Sprite spriteLeft;
    static Animation rightarrow("assets/image/rightarrow.png", 1125, 1040, 10, 3, 4);
    static Sprite spriteRight;

    static Clock clock;
    static int currentFrame = 0;
    static float timer = 0.f;
    const static float fps = 8.0f;   // animation speed
    float dt = clock.restart().asSeconds();

    updateAnimation(dt, fps, leftarrow.frameCount, currentFrame, timer);
    applyFrame(spriteLeft, leftarrow, currentFrame);
    updateAnimation(dt, fps, rightarrow.frameCount, currentFrame, timer);
    applyFrame(spriteRight, rightarrow, currentFrame);

    float scaleX = 3.0f * boxWidth / leftarrow.texture.getSize().x * 0.15; // 15% box's width
    float scaleY = 4.0f * boxHeight / rightarrow.texture.getSize().y * 0.23; // 23% box's height
    if (spriteLeft.getScale().x != scaleX || spriteLeft.getScale().y != scaleY) {
        // scale
        spriteLeft.setScale(scaleX, scaleY);
        spriteRight.setScale(scaleX, scaleY);
        // position
        spriteLeft.setOrigin(spriteLeft.getLocalBounds().getSize().x / 2,
            spriteLeft.getLocalBounds().getSize().y / 2);
        spriteLeft.setPosition(winWidth / 2 - boxWidth / 2, winHeight / 2);

        spriteRight.setOrigin(spriteLeft.getLocalBounds().getSize().x / 2,
            spriteRight.getLocalBounds().getSize().y / 2);
        spriteRight.setPosition(winWidth / 2 + boxWidth / 2, winHeight / 2);
    }

    if (SelectSettings / 3 * 3 + numSettingsCurPage < totalSetButtons)
        window.draw(spriteRight);
    if (SelectPage > 0)
        window.draw(spriteLeft);

    // draw buttons
    SettingButtons(window);
}

void Settings::SettingButtons(RenderWindow& window) {
    // window size
    float winWidth = window.getSize().x;
    float winHeight = window.getSize().y;

    // fixed size and indentations
    float startY = winHeight / 2 - boxHeight / 2 + winHeight * 0.12f;
    float buttonHeight = winHeight * 0.06f;
    float buttonWidth = boxWidth * 0.75f;
    float buttonSpacing = winHeight * 0.12f;
    float outlineThick = winHeight * 0.004f;
    float outlineThickSelected = winHeight * 0.006f;

    // draw general settings buttons
    if (inGeneralSettings || !inSettings) { // fix auto enter issue
        for (int IDButton = SelectPage * 3, cntBut = 0; cntBut < numSettingsCurPage;
            ++IDButton, ++cntBut) {
            generalSettingsBox(window, IDButton, cntBut);
        }
    }
    else // draw specific settings
        if (inSetButtons[IDSFX]) { // change sound effect
            // draw sound setting
            generalSettingsBox(window, IDSFX, 0); // highlight Sound button
            subSoundSettingBox(window);
        }
        else if (inSetButtons[IDCHANGENAME]) { // change name
            if (isTypingName) {
                string display_name = tmp_name;
                if (display_name.empty())
                    display_name = " ";
                generalSettingsBox(window, IDCHANGENAME, 0, "Typing: " + display_name,
                    1);
                generalSettingsBox(window, IDCHANGENAME, 1, "Press Enter to confirm",
                    0);
                generalSettingsBox(window, IDCHANGENAME, 2, "Press Esc to cancel", 0);
            }
            else {
                generalSettingsBox(window, IDCHANGENAME, 0, playerName[0],
                    IDNameButtons == 0); // highlight Change Name button
                generalSettingsBox(window, IDCHANGENAME, 1, playerName[1],
                    IDNameButtons == 1); // highlight Change Name button
            }
        }
        else if (inSetButtons[IDCHANGERESOLUTION]) { // change resolution
            string display_width = to_string(listWindowSize[tmp_idWindowSize].first);
            string display_height = to_string(listWindowSize[tmp_idWindowSize].second);
            string display_res = "Resolution: " + display_width + ":" + display_height;
            generalSettingsBox(window, IDCHANGERESOLUTION, 0, display_res, 1);
            generalSettingsBox(window, IDCHANGERESOLUTION, 1, "Press Enter to confirm", 0);
            generalSettingsBox(window, IDCHANGERESOLUTION, 2, "Press W & S to change", 0);
        }
        else if (inSetButtons[IDCHANGEAVATAR]) {
            // Highlight Change Avatar button
            generalSettingsBox(window, IDCHANGEAVATAR, 0);
            // Draw avatar selection UI
            drawAvatarChangeBox(window);
        }

    // others
    // sound Mute/Unmute indicator
    if (SelectPage == 0) { // First Page contains Sound button
        // Mute/Unmute indicator inside Sound box (always visible)
        std::string muteStatus = isNotMuted ? "Unmute" : "Mute";
        Text muteText;
        muteText.setFont(font);
        muteText.setString(muteStatus);
        muteText.setCharacterSize((int)(winHeight * 0.03f));
        if (inSoundSubmenu && IDSoundButtons == 0) {
            muteText.setFillColor(Color::Yellow);
        }
        else {
            muteText.setFillColor(isNotMuted ? Color(100, 200, 100)
                : Color(200, 100, 100));
        }
        FloatRect muteTextBounds = muteText.getLocalBounds();
        float muteX = winWidth / 2 + boxWidth * 0.375f - muteTextBounds.width -
            winWidth * 0.005f;
        muteText.setPosition(muteX,
            startY + buttonHeight / 2 - muteTextBounds.height / 2);
        window.draw(muteText);
    }
}

void Settings::generalSettingsBox(RenderWindow& window, int IDButton, int row, string contextString, int selectedC) {
    bool selected = false;
    if (selectedC == 1 || (selectedC == -1 && SelectSettings == IDButton))
        selected = true;

    // window size
    float winWidth = window.getSize().x;
    float winHeight = window.getSize().y;

    if (!boxHeight) {
        boxWidth = winWidth * 0.6f;
        boxHeight = winHeight * 0.5f;
    }


    // fixed size and indentations
    float startY = winHeight / 2 - boxHeight / 2 + winHeight * 0.12f;
    float buttonHeight = winHeight * 0.06f;
    float buttonWidth = boxWidth * 0.75f;
    float buttonSpacing = winHeight * 0.12f;
    float outlineThick = winHeight * 0.004f;
    float outlineThickSelected = winHeight * 0.006f;

    // Box for each button
    RectangleShape buttonBox(Vector2f(buttonWidth, buttonHeight));
    buttonBox.setFillColor(selected ? Color(100, 150, 200) : Color(70, 70, 70));
    buttonBox.setOutlineColor(selected ? Color::Yellow : Color::White);
    buttonBox.setOutlineThickness(selected ? outlineThickSelected : outlineThick);
    buttonBox.setPosition(winWidth / 2 - boxWidth * 0.375f,
        startY + buttonSpacing * row);

    // Button text (centered)
    Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(contextString.empty() ? contextSetButtons[IDButton]
        : contextString);
    buttonText.setCharacterSize((int)(winHeight * 0.04f));
    buttonText.setFillColor(selected ? Color::Yellow : Color::White);

    FloatRect buttonTextBounds = buttonText.getLocalBounds();
    buttonText.setPosition(winWidth / 2 - buttonTextBounds.width / 2,
        startY + buttonSpacing * row + buttonHeight / 2 -
        buttonTextBounds.height / 2 - winHeight * 0.01f);

    window.draw(buttonBox);
    window.draw(buttonText);
}

void Settings::subSoundSettingBox(RenderWindow& window) {
    float winHeight = window.getSize().y;
    float winWidth = window.getSize().x;

    float volBoxwidth = boxWidth * 0.75f;
    float volBoxHeight = winHeight * 0.06f;
    float startY = window.getSize().y / 2 - boxHeight / 2 + winHeight * 0.12f;
    float buttonHeight = winHeight * 0.06f;
    float spacing = winHeight * 0.05f;

    // Position music volume box right under Sound button
    float volBoxY = startY + buttonHeight + spacing;

    // Music Volume Box
    RectangleShape MusicSettingBox(Vector2f(volBoxwidth, volBoxHeight));
    MusicSettingBox.setFillColor((IDSoundButtons == 1) ? Color(100, 150, 200)
        : Color(70, 70, 70));
    MusicSettingBox.setOutlineColor((IDSoundButtons == 1) ? Color::Yellow
        : Color::White);
    float outlineThick = winHeight * 0.003f;         // line while not selected
    float outlineThickSelected = winHeight * 0.005f; // line when selected
    MusicSettingBox.setOutlineThickness(
        (IDSoundButtons == 1) ? outlineThickSelected : outlineThick);
    MusicSettingBox.setPosition(window.getSize().x / 2 - boxWidth * 0.375f,
        volBoxY);
    window.draw(MusicSettingBox);

    // Music Volume Text
    Text musicText;
    musicText.setFont(font);
    musicText.setString("Music Volume: " + std::to_string(MusicVolumeLevel * 5) +
        "%");
    musicText.setCharacterSize((int)(winHeight * 0.02f));
    musicText.setFillColor((IDSoundButtons == 1) ? Color::Yellow : Color::White);
    FloatRect musicTextBounds = musicText.getLocalBounds();
    musicText.setPosition(window.getSize().x / 2 - musicTextBounds.width / 2,
        volBoxY + winHeight * 0.0065f);
    window.draw(musicText);

    // Music Volume Bar Background
    float barWidth = volBoxwidth * 0.8f;
    float barHeight = winHeight * 0.008f;
    float barX = window.getSize().x / 2 - barWidth / 2;
    float barY = volBoxY + winHeight * 0.036f;

    RectangleShape musicBarBg(Vector2f(barWidth, barHeight));
    musicBarBg.setFillColor(Color(30, 30, 30));
    musicBarBg.setOutlineColor(Color::White);
    musicBarBg.setOutlineThickness(winHeight * 0.0008f); // 0.08% of height
    musicBarBg.setPosition(barX, barY);
    window.draw(musicBarBg);

    // Music Volume Bar Fill
    float musicFillWidth = barWidth * (MusicVolumeLevel / 20.0f);
    RectangleShape musicBarFill(Vector2f(musicFillWidth, barHeight));
    musicBarFill.setFillColor((IDSoundButtons == 1) ? Color(100, 200, 100)
        : Color(50, 150, 50));
    musicBarFill.setPosition(barX, barY);
    window.draw(musicBarFill);

    // Effect Volume Box (right under Music Volume)
    float effectBoxY = volBoxY + volBoxHeight + spacing;
    RectangleShape EffectSettingBox(Vector2f(volBoxwidth, volBoxHeight));
    EffectSettingBox.setFillColor((IDSoundButtons == 2) ? Color(100, 150, 200)
        : Color(70, 70, 70));
    EffectSettingBox.setOutlineColor((IDSoundButtons == 2) ? Color::Yellow
        : Color::White);
    EffectSettingBox.setOutlineThickness(
        (IDSoundButtons == 2) ? outlineThickSelected : outlineThick);
    EffectSettingBox.setPosition(window.getSize().x / 2 - boxWidth * 0.375f,
        effectBoxY);
    window.draw(EffectSettingBox);

    // Effect Volume Text
    Text effectText;
    effectText.setFont(font);
    effectText.setString(
        "Effect Volume: " + std::to_string(EffectVolumeLevel * 5) + "%");
    effectText.setCharacterSize((int)(winHeight * 0.02f)); // 2.9% of height
    effectText.setFillColor((IDSoundButtons == 2) ? Color::Yellow : Color::White);
    FloatRect effectTextBounds = effectText.getLocalBounds();
    effectText.setPosition(window.getSize().x / 2 - effectTextBounds.width / 2,
        effectBoxY + winHeight * 0.0065f);
    window.draw(effectText);

    // Effect Volume Bar Background
    RectangleShape effectBarBg(Vector2f(barWidth, barHeight));
    effectBarBg.setFillColor(Color(30, 30, 30));
    effectBarBg.setOutlineColor(Color::White);
    effectBarBg.setOutlineThickness(winHeight * 0.0008f);
    effectBarBg.setPosition(barX, effectBoxY + winHeight * 0.035f);
    window.draw(effectBarBg);

    // Effect Volume Bar Fill
    float effectFillWidth = barWidth * (EffectVolumeLevel / 20.0f);
    RectangleShape effectBarFill(Vector2f(effectFillWidth, barHeight));
    effectBarFill.setFillColor((IDSoundButtons == 2) ? Color(100, 200, 100)
        : Color(50, 150, 50));
    effectBarFill.setPosition(barX, effectBoxY + winHeight * 0.035f);
    window.draw(effectBarFill);
}

void Settings::loadAllAvatars() {
    avatarTextures.clear();
    avatarSprites.clear();
    avatarPaths.clear();

    std::string folderPath = "assets/image/Avatar/"; // path to avatar folder
    std::string searchPath = folderPath + "*.*"; // find all files

    WIN32_FIND_DATAA findData; // Use A version for std::string (ANSI)
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    int i = 0;

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // Skip current (.) and parent (..) directories
            std::string fileName = findData.cFileName;
            if (fileName == "." || fileName == "..")
                continue;

            // check file extension
            // If you are sure the folder only contains images, this check is not too strict
            if (fileName.find(".png") != std::string::npos ||
                fileName.find(".jpg") != std::string::npos ||
                fileName.find(".gif") != std::string::npos) {

                std::string fullPath = folderPath + fileName;
                sf::Texture tex;

                if (tex.loadFromFile(fullPath)) {
                    avatarTextures.push_back(tex);
                    avatarPaths.push_back(fullPath); // Store the path

                    sf::Sprite sprite;
                    sprite.setTexture(avatarTextures.back());
                    avatarSprites.push_back(sprite);
                    i++;
                }
            }
        } while (FindNextFileA(hFind, &findData));

        FindClose(hFind);
    }
    else {
        std::cerr << "Cannot find the directory: " << folderPath << std::endl;
    }

    numberAvatar = avatarTextures.size(); // Update total number of avatars
}

void Settings::changeAvatar() {
    // Load avatars if not already loaded
    if (avatarTextures.empty()) {
        loadAllAvatars();
    }

    // If no avatars found, return
    if (avatarTextures.empty()) {
        return;
    }

    if (!inAvatarBrowsingMode) {
        // Player selection mode - only Up/Down works
        if (keyBoard.Up() ^ keyBoard.Down()) {
            int oldPlayer = IDAvatarButtons;
            IDAvatarButtons ^= 1; // Toggle between 0 and 1
            PlaySoundClick();

            // Update selected avatar to show current player's avatar
            if (numberAvatar > 0) {
                std::string currentPath =
                    (IDAvatarButtons == 0) ? player1AvatarPath : player2AvatarPath;
                selectedAvatarIndex = 0; // Default to first if not found
                for (int i = 0; i < avatarPaths.size(); ++i) {
                    if (avatarPaths[i] == currentPath) {
                        selectedAvatarIndex = i;
                        break;
                    }
                }
            }
        }

        // Enter avatar browsing mode with Enter
        if (keyBoard.Enter()) {
            inAvatarBrowsingMode = true;
            PlaySoundClick();
        }
    }
    else {
        // Avatar browsing mode - Left/Right to browse, Enter to confirm
        if (keyBoard.Left() ^ keyBoard.Right()) {
            if (keyBoard.Left()) {
                --selectedAvatarIndex;
                if (selectedAvatarIndex < 0)
                    selectedAvatarIndex = numberAvatar - 1;
            }
            else {
                ++selectedAvatarIndex;
                if (selectedAvatarIndex >= numberAvatar)
                    selectedAvatarIndex = 0;
            }
            PlaySoundClick();
        }

        // Confirm avatar selection with Enter
        if (keyBoard.Enter()) {
            if (selectedAvatarIndex >= 0 &&
                selectedAvatarIndex < avatarPaths.size()) {
                if (IDAvatarButtons == 0) { // Player 1
                    player1AvatarPath = avatarPaths[selectedAvatarIndex];
                }
                else { // Player 2
                    player2AvatarPath = avatarPaths[selectedAvatarIndex];
                }
                PlaySoundClick();
                SaveSettings(); // Save the new avatar selection
                // Return to player selection mode
                inAvatarBrowsingMode = false;
            }
        }
    }

    // Exit avatar browsing mode back to player selection with Esc (if in browsing
    // mode) Or exit avatar submenu completely if in player selection mode
    if (keyBoard.Esc()) {
        PlaySoundClick();
        if (inAvatarBrowsingMode) {
            // Go back to player selection mode
            inAvatarBrowsingMode = false;
        }
        else {
            // Exit avatar submenu completely
            inSetButtons[IDCHANGEAVATAR] = false;
            inAvatarSubmenu = false;
            inGeneralSettings = true; // Back to general settings
            selectedAvatarIndex = -1;
        }
    }
}

void Settings::drawAvatarChangeBox(RenderWindow& window) {
    float winWidth = window.getSize().x;
    float winHeight = window.getSize().y;

    // Draw background panel
    RectangleShape panel(Vector2f(boxWidth * 1.5f, boxHeight * 1.2f));
    panel.setFillColor(Color(50, 50, 50, 240));
    panel.setOutlineColor(Color::White);
    panel.setOutlineThickness(winHeight * 0.003f);
    panel.setPosition(winWidth / 2 - boxWidth * 0.75f,
        winHeight / 2 - boxHeight * 0.6f);
    window.draw(panel);

    // Title
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Change Avatar");
    titleText.setCharacterSize((int)(winHeight * 0.05f));
    titleText.setFillColor(Color::Yellow);
    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(winWidth / 2 - titleBounds.width / 2,
        winHeight / 2 - boxHeight * 0.55f + winHeight * 0.03f);
    window.draw(titleText);

    // New layout: Player labels on left, selected avatar on left, grid on right
    float contentStartY = winHeight / 2 - boxHeight * 0.55f + winHeight * 0.12f;
    float leftSideX = winWidth / 2 - boxWidth * 0.5f + boxWidth * 0.15f;
    float rightSideX = winWidth / 2 + boxWidth * 0.1f;
    float spacing = winHeight * 0.05f;

    // Draw player labels on the left (P1 and P2)
    float playerLabelY = contentStartY;
    float labelHeight = winHeight * 0.05f;

    // Player 1 label
    Text player1Label;
    player1Label.setFont(font);
    player1Label.setString("Player 1");
    player1Label.setCharacterSize((int)(winHeight * 0.04f));
    player1Label.setFillColor((IDAvatarButtons == 0 && !inAvatarBrowsingMode)
        ? Color::Yellow
        : Color::White);
    FloatRect p1LabelBounds = player1Label.getLocalBounds();
    player1Label.setPosition(leftSideX, playerLabelY);
    window.draw(player1Label);

    // Player 2 label
    Text player2Label;
    player2Label.setFont(font);
    player2Label.setString("Player 2");
    player2Label.setCharacterSize((int)(winHeight * 0.04f));
    player2Label.setFillColor((IDAvatarButtons == 1 && !inAvatarBrowsingMode)
        ? Color::Yellow
        : Color::White);
    FloatRect p2LabelBounds = player2Label.getLocalBounds();
    player2Label.setPosition(leftSideX, playerLabelY + labelHeight + spacing);
    window.draw(player2Label);

    // Draw selected avatar preview on the left side (below player labels)
    float selectedAvatarY = playerLabelY + (labelHeight + spacing) * 2;
    float selectedAvatarSize = min(winHeight * 0.2f, boxWidth * 0.25f);
    float previewX = leftSideX;

    if (!avatarSprites.empty() && selectedAvatarIndex >= 0 &&
        selectedAvatarIndex < avatarSprites.size()) {
        // Draw preview box
        RectangleShape previewBox(
            Vector2f(selectedAvatarSize + 20, selectedAvatarSize + 20));
        previewBox.setFillColor(Color(30, 30, 30));
        previewBox.setOutlineColor(inAvatarBrowsingMode ? Color::Yellow
            : Color::White);
        previewBox.setOutlineThickness(inAvatarBrowsingMode ? 4 : 2);
        previewBox.setPosition(previewX - 10, selectedAvatarY - 10);
        window.draw(previewBox);

        // Draw the selected avatar - use texture directly to avoid copy issues
        sf::Vector2u texSize = avatarTextures[selectedAvatarIndex].getSize();
        float scaleX = selectedAvatarSize / texSize.x;
        float scaleY = selectedAvatarSize / texSize.y;
        float scale = min(scaleX, scaleY);

        sf::Sprite previewSprite;
        previewSprite.setTexture(avatarTextures[selectedAvatarIndex]);
        previewSprite.setScale(scale, scale);
        float spriteX = previewX + (selectedAvatarSize - texSize.x * scale) / 2;
        float spriteY =
            selectedAvatarY + (selectedAvatarSize - texSize.y * scale) / 2;
        previewSprite.setPosition(spriteX, spriteY);
        window.draw(previewSprite);

        // Show selection info below preview
        Text selectionText;
        selectionText.setFont(font);
        selectionText.setString("Avatar " + to_string(selectedAvatarIndex + 1) +
            " / " + to_string(numberAvatar));
        selectionText.setCharacterSize((int)(winHeight * 0.025f));
        selectionText.setFillColor(Color(200, 200, 200));
        FloatRect selBounds = selectionText.getLocalBounds();
        selectionText.setPosition(previewX + selectedAvatarSize / 2 -
            selBounds.width / 2,
            selectedAvatarY + selectedAvatarSize + 15);
        window.draw(selectionText);
    }

    // Draw avatar grid on the right side (always visible, not just in browsing
    // mode)
    if (!avatarSprites.empty() && selectedAvatarIndex >= 0 &&
        selectedAvatarIndex < avatarSprites.size()) {
        float gridY = contentStartY;
        float gridAvatarSize = min(winHeight * 0.1f, boxWidth * 0.12f);
        float gridSpacing = gridAvatarSize * 0.2f;
        int avatarsPerRow = 4;
        float totalGridWidth =
            avatarsPerRow * gridAvatarSize + (avatarsPerRow - 1) * gridSpacing;

        // Draw avatar grid
        for (int i = 0; i < avatarSprites.size() && i < 12; ++i) {
            int row = i / avatarsPerRow;
            int col = i % avatarsPerRow;
            float x = rightSideX + col * (gridAvatarSize + gridSpacing);
            float y = gridY + row * (gridAvatarSize + gridSpacing);

            // Draw box for each avatar
            RectangleShape avatarBox(
                Vector2f(gridAvatarSize + 4, gridAvatarSize + 4));
            avatarBox.setFillColor(Color(30, 30, 30));
            avatarBox.setOutlineColor((i == selectedAvatarIndex) ? Color::Yellow
                : Color::White);
            avatarBox.setOutlineThickness((i == selectedAvatarIndex) ? 3 : 1);
            avatarBox.setPosition(x - 2, y - 2);
            window.draw(avatarBox);

            // Scale sprite to fit
            sf::Vector2u texSize = avatarTextures[i].getSize();
            float scaleX = gridAvatarSize / texSize.x;
            float scaleY = gridAvatarSize / texSize.y;
            float scale = min(scaleX, scaleY);

            sf::Sprite gridSprite;
            gridSprite.setTexture(avatarTextures[i]);
            gridSprite.setScale(scale, scale);
            float spriteX = x + (gridAvatarSize - texSize.x * scale) / 2;
            float spriteY = y + (gridAvatarSize - texSize.y * scale) / 2;
            gridSprite.setPosition(spriteX, spriteY);
            window.draw(gridSprite);
        }
    }
}

void Settings::SaveSettings() {
    std::ofstream file("assets/setting_save&load/settings.txt");
    if (!file.is_open()) {
        std::cout << "Failed to save settings file!" << std::endl;
        return;
    }

    // Save settings in a simple format
    file << "isNotMuted=" << (isNotMuted ? "1" : "0") << std::endl;
    file << "MusicVolumeLevel=" << MusicVolumeLevel << std::endl;
    file << "EffectVolumeLevel=" << EffectVolumeLevel << std::endl;
    file << "confirmedSettingsFirstTime=" << confirmedSettingsFirstTime << std::endl;
    file << "Player1Name=" << playerName[0] << std::endl;
    file << "Player2Name=" << playerName[1] << std::endl;
    file << "ResolutionID=" << idWindowSize << std::endl;
    file << "Player1Avatar=" << player1AvatarPath << std::endl;
    file << "Player2Avatar=" << player2AvatarPath << std::endl;

    file.close();
}

void Settings::LoadSettings(RenderWindow& window) {
    std::ifstream file("assets/setting_save&load/settings.txt");
    if (!file.is_open()) {
        std::cout << "Settings file not found. Using default settings."
            << std::endl;
        return;
    }

    // Load all values first
    bool loadedIsNotMuted = isNotMuted; // default value
    int loadedMusicVolume = MusicVolumeLevel;
    int loadedEffectVolume = EffectVolumeLevel;
    bool loadedConfirmedSettingsFirstTime = confirmedSettingsFirstTime;
    string loadedPlayer1Name = playerName[0];
    string loadedPlayer2Name = playerName[1];
    int loadedIDWindowSize = idWindowSize;
    std::string loadedPlayer1Avatar = player1AvatarPath;
    std::string loadedPlayer2Avatar = player2AvatarPath;

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        if (key == "isNotMuted") {
            loadedIsNotMuted = (value == "1");
        }
        else if (key == "MusicVolumeLevel") {
            try {
                int vol = std::stoi(value);
                loadedMusicVolume = max(0, min(20, vol));
            }
            catch (...) {
                // Invalid value, skip
            }
        }
        else if (key == "EffectVolumeLevel") {
            try {
                int vol = std::stoi(value);
                loadedEffectVolume = max(0, min(20, vol));
            }
            catch (...) {
                // Invalid value, skip
            }
        }
        else if (key == "confirmedSettingsFirstTime") {
            try {
                loadedConfirmedSettingsFirstTime = (value == "1");
            }
            catch (...) {
                // Invalid value, skip
            }
        }
        else if (key == "Player1Name") {
            loadedPlayer1Name = value;
        }
        else if (key == "Player2Name") {
            loadedPlayer2Name = value;
        }
        else if (key == "ResolutionID") {
            // chuyen doi xau thanh so nguyen
            loadedIDWindowSize = 0;
            for (int i = value.size() - 1; i >= 0; i--) {
                loadedIDWindowSize *= 10;
                loadedIDWindowSize += value[i] - '0';
            }
        }
        else if (key == "Player1Avatar") {
            loadedPlayer1Avatar = value;
        }
        else if (key == "Player2Avatar") {
            loadedPlayer2Avatar = value;
        }
    }

    file.close();

    // Apply volumes first
    MusicVolumeLevel = loadedMusicVolume;
    EffectVolumeLevel = loadedEffectVolume;
    SetMusicVolume(MusicVolumeLevel * 5.0f);
    SetEffectVolume(EffectVolumeLevel * 5.0f);

    // Then apply mute state (this will override volumes if muted)
    isNotMuted = loadedIsNotMuted;
    // Sync with soundMute: isNotMuted == !soundMute
    SetSoundMute(!isNotMuted);

    // apply name
    confirmedSettingsFirstTime = loadedConfirmedSettingsFirstTime;
    playerName[0] = loadedPlayer1Name;
    playerName[1] = loadedPlayer2Name;

    // apply new resolution
    if (loadedIDWindowSize != idWindowSize) {
        idWindowSize = loadedIDWindowSize;
        window.create(VideoMode(listWindowSize[idWindowSize].first, listWindowSize[idWindowSize].second),
            "Caro Game!",
            Style::Close);
    }

    // apply avatar paths
    player1AvatarPath = loadedPlayer1Avatar;
    player2AvatarPath = loadedPlayer2Avatar;

}