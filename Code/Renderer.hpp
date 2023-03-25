#pragma once

#include "Types.hpp"
#include "Framebuffer.hpp"

#include "Maths.hpp"
#include "Light.hpp"
#include "Texture.hpp"

// Vertex format (only one supported for now)

struct rdrVertex
{
    Vec3   pos;
    Vec3   normal;
    Vec4   color;
    float2 textureCoords;
};

struct Viewport
{
    Viewport(const int p_x, const int p_y, uint p_width, uint p_height):
    x(p_x),y(p_y),width(p_width),height(p_height){}

    const int x;
    const int y;
    uint width; 
    uint height;
};

class Renderer
{
    Framebuffer fb;
    Viewport  viewport;

    float4 lineColor = { 1.f, 1.f, 1.f, 1.f };

    Mat4 mModel = {};
    Mat4 mView = {};
    Mat4 mViewInverse = {};
    Mat4 mProjection = {};
    Mat4 mProjectionInverse = {};
    Mat4 mNormal = {};
    Light light = {};
    Texture mTexture;

    float alpha = 0.0f;
    bool enableTextures = true;
    bool enableLights = true;
    bool enableWireframe = false;
    bool doubleFaced = false;
public:
    Renderer(uint p_width, uint p_height);
    ~Renderer();
    //Setters
    void SetProjection   (const float* p_projectionMatrix);
    void SetView         (const float* p_viewMatrix);
    void SetViewInverse  (const float* p_viewInverse);
    void SetModel        (const float* p_modelMatrix);
    void SetTexture      (float* p_colors, const uint p_width,
                          const uint p_height, const int bitsPerPixel);
    void SetTexture      (Texture texture){mTexture = texture;};
    void SetLight(const Light &l);

    //Getters
    Framebuffer *GetFrameBuffer();
    //Draw functions
    void DrawLine        (float2 p_0, float2 p_1, const float4& p_color);
    void DrawTriangleFill(struct rdrVertex *vertices, Vec3 *normals, float3 *screenCoords, Light &l);
    void DrawTriangle    (rdrVertex* p_vertices);
    void DrawQuad        (rdrVertex* vertices);
    void DrawMesh        (class Mesh* mesh);
    void DrawRdrArray    (rdrVertex *v, int size);
    void ShowImGuiControls();
    void ToggleWireframe() { enableWireframe = !enableWireframe;};
    void ToggleDoubleFaced() { doubleFaced = !doubleFaced;};
};