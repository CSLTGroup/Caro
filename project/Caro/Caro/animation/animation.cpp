#include "../global.h"


Animation::Animation(const std::string& path, int frameW, int frameH, int frames, int cols, int rows)
    : frameWidth(frameW / cols), frameHeight(frameH / rows), frameCount(frames), columns(cols), rows(rows) {
    texture.loadFromFile(path);
}
// hàm cập nhật thời gian thực và frame hiện tại
void updateAnimation(float dt, float fps, int frameCount, int& currentFrame, float& timer)
{
    timer += dt;
    float frameTime = 1.f / fps;
    while (timer >= frameTime) { // thời gian tích lũy >= chu kì của 1 frame -> chuyển frame
        timer -= frameTime;
        currentFrame++;
		if (currentFrame >= frameCount) // vượt số frame trong bức ảnh -> quay về frame đầu
            currentFrame = 0;
    }
}
// hàm áp dụng frame vào sprite
void applyFrame(sf::Sprite& sprite, const Animation& anim, int frame)
{
    int col = frame % anim.columns;
    int row = frame / anim.columns;

    sprite.setTexture(anim.texture);
    sprite.setTextureRect({
        col * anim.frameWidth, // tọa độ x trong bức ảnh
        row * anim.frameHeight, // tọa độ y trong bức ảnh
        anim.frameWidth, // kích thước ngang
        anim.frameHeight // kích thước dọc
        });
}
