#include<cstring>

#include "Framebuffer.hpp"
#include <math.h>

Framebuffer::Framebuffer(int p_width, int p_height)
: width(p_width)
, height(p_height)
, colorBuffer(p_width* p_height)
, depthBuffer(p_width* p_height)
{
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
}

Framebuffer::~Framebuffer()
{
    glDeleteTextures(1, &colorTexture);
}

void Framebuffer::Clear()
{
    // Clear color buffer
    {
        float4* colors = colorBuffer.data();

        // Fill the first line with the clear color
        for (int i = 0; i < width; ++i)
            std::memcpy(&colors[i], &clearColor, sizeof(float4));

        // Copy the first line onto every line
        for (int i = 1; i < height; ++i)
            std::memcpy(&colors[i * width], &colors[0], width * sizeof(float4));
    }

    // Clear depth buffer
    {
        float inf = INFINITY;
        for (size_t i = 0; i < depthBuffer.size(); i++)
            depthBuffer.data()[i] = inf;
    }
}

void Framebuffer::UpdateTexture()
{
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, colorBuffer.data());
}
