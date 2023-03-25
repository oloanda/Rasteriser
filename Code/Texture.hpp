#pragma once
#include <string>

class Texture
{
public:
    int width = 0;
    int height = 0;
    int bitsPerPixel = 0;
    float *data = nullptr;

    Texture();
    ~Texture();
    bool Load(const std::string& fileName);
};