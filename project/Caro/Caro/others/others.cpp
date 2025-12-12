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

std::string tostr(int k, int len)
{
    std::string str = "";
    while (k)
    {
        str += char(k % 10 + 48);
        k /= 10;
    }
    while (str.length() < len)
        str += "0";
    reverse(str.begin(), str.end());
    return str;
}