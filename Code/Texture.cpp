#include "Texture.hpp"

#include "glad/gl.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture()
{

}
Texture::~Texture()
{

}

bool Texture::Load(const std::string& fileName)
{
    int bpp;

    bitsPerPixel = 4;
    unsigned char *d = stbi_load(fileName.c_str(), &width, &height, &bpp, 0);

    if(d == nullptr)
    {
        printf("Failed to load texture !!");
        data = (float *)malloc(1 * sizeof(float));
        width = 1;
        height = 1;
        bpp = 4;
        return false;
    }
    data = new float[width * height * bitsPerPixel]; // assumed 1 color component (rgba) per float
    assert(bpp >= 3);

    int j = 0;
    for (int i = 0; i < width * height * bitsPerPixel; i += bitsPerPixel)
    {
        data[i] = d[j] / 255.0f;
        data[i + 1] = d[j + 1] / 255.0f;
        data[i + 2] = d[j + 2] / 255.0f;

        if (bpp < 4)
            data[i + 3] = 1.0f;
        else
            data[i + 3] = d[j + 3] / 255.0f;
        data[i + 3] = 1.0f;
        j += bpp;
    }
    stbi_image_free(d);
    return true;
}
