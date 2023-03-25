#pragma once

#include <vector>
#include "glad/gl.h"

#include "Types.hpp"

class Framebuffer
{
    int width = 0;
    int height = 0;

    // In-RAM buffers
    // Color and depth buffer have to be valid until the shutdown of the renderer
    // Color buffer is RGBA, each component is a 32 bits float
    // Depth buffer is a buffer of 32bits floats
    std::vector<float4> colorBuffer;
    std::vector<float> depthBuffer;
    std::vector<float16> antiAliasingBuffer;

    // OpenGL texture (in VRAM)
    GLuint colorTexture = 0;

public:
    float4 clearColor = { 0.f, 0.f, 0.f, 1.f };

    Framebuffer(int width, int height);
    ~Framebuffer();

    void Clear();
    void UpdateTexture();

    float* GetColorBuffer() { return reinterpret_cast<float*>(colorBuffer.data()); }
    float* GetDepthBuffer() { return depthBuffer.data(); }
    int GetWidth()  const   { return width; }
    int GetHeight() const   { return height; }

    GLuint GetColorTexture() const { return colorTexture; }



};