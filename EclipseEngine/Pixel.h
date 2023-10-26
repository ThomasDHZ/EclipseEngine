#pragma once
typedef unsigned char byte;
struct Pixel
{
    byte Red = 0x00;
    byte Green = 0x00;
    byte Blue = 0x00;
    byte Alpha = 0xFF;

    Pixel()
    {

    }

    Pixel(byte Byte)
    {
        Red = Byte;
        Green = Byte;
        Blue = Byte;
    }
    Pixel(byte RedGreen, byte blue)
    {
        Red = RedGreen;
        Green = RedGreen;
        Blue = blue;
    }

    Pixel(byte red, byte green, byte blue)
    {
        Red = red;
        Green = green;
        Blue = blue;
    }

    Pixel(byte red, byte green, byte blue, byte alpha)
    {
        Red = red;
        Green = green;
        Blue = blue;
        Alpha = alpha;
    }

    ~Pixel()
    {

    }

    bool operator==(const Pixel& rhs) const
    {
        return Red == rhs.Red &&
            Green == rhs.Green &&
            Blue == rhs.Blue;
    }
};

struct Pixel32
{
    float Red = 0x00;
    float Green = 0x00;
    float Blue = 0x00;
    float Alpha = 0xFF;

    Pixel32()
    {

    }

    Pixel32(float Byte)
    {
        Red = Byte;
        Green = Byte;
        Blue = Byte;
    }
    Pixel32(float RedGreen, float blue)
    {
        Red = RedGreen;
        Green = RedGreen;
        Blue = blue;
    }

    Pixel32(float red, float green, float blue)
    {
        Red = red;
        Green = green;
        Blue = blue;
    }

    Pixel32(float red, float green, float blue, float alpha)
    {
        Red = red;
        Green = green;
        Blue = blue;
        Alpha = alpha;
    }

    ~Pixel32()
    {

    }

    bool operator==(const Pixel32& rhs) const
    {
        return Red == rhs.Red &&
            Green == rhs.Green &&
            Blue == rhs.Blue;
    }
};
const Pixel NullPixel = Pixel(0x00, 0x00, 0x00, 0xFF);