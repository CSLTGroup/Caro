#include "../global.h"

float getCharacterSizeForLineHeight(const sf::Font& font, float targetHeight)
{
    float low = 0, high = 10000;
    while (high - low > 1e-2)
    {
        float mid = (low + high) / 2;
        float ls = font.getLineSpacing(mid);

        if (ls < targetHeight)
            low = mid;
        else
            high = mid;
    }
    return (low + high) / 2;
}