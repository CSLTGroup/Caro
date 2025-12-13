#include "../global.h"


Animation::Animation(const std::string& path, int frameW, int frameH, int frames, int cols, int rows)
    : frameWidth(frameW / cols), frameHeight(frameH / rows), frameCount(frames), columns(cols), rows(rows) {
    texture.loadFromFile(path);
}

void updateAnimation(float dt, float fps, int frameCount, int& currentFrame, float& timer)
{
    static float localTime = 0.f;
    localTime += dt;
    timer += dt;

    float frameTime = 1.f / fps;

    while (timer >= frameTime) {
        timer -= frameTime;
        currentFrame++;

        if (currentFrame >= frameCount)
            currentFrame = 0;
    }
}
void applyFrame(sf::Sprite& sprite, const Animation& anim, int frame)
{
    int col = frame % anim.columns;
    int row = frame / anim.columns;

    sprite.setTexture(anim.texture);
    sprite.setTextureRect({
        col * anim.frameWidth,
        row * anim.frameHeight,
        anim.frameWidth,
        anim.frameHeight
        });
}
