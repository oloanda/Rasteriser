#include <cstdio>
#include <cstring>
#include <cassert>

#include "imgui/imgui.h"

#include "Maths.hpp"

#include "Renderer.hpp"
#include "Mesh.hpp"

Renderer::Renderer(const uint p_width, const uint p_height):
fb(p_width, p_height),viewport(0,0,p_width, p_height)
{
    mModel = Mat4::CreateIdentityMatrix();

}

Renderer::~Renderer()
{

}

Framebuffer *Renderer::GetFrameBuffer()
{
    return(&fb);
}

void Renderer::SetProjection(const float* p_projectionMatrix)
{
    for (int i = 0;i < 16; i++)
        mProjection.e[i] = p_projectionMatrix[i];
    mProjectionInverse = Mat4::CreateInverseMatrix(mProjection);
}

void Renderer::SetView(const float* p_viewMatrix)
{
    for (int i = 0;i < 16; i++)
        mView.e[i] = p_viewMatrix[i];
    mViewInverse = Mat4::CreateInverseMatrix(mView);
}

void Renderer::SetModel(const float* p_modelMatrix)
{
    for (int i = 0;i < 16; i++)
        mModel.e[i] = p_modelMatrix[i];
    mNormal = Mat4::CreateInverseMatrix(mModel);
    mNormal.TransposeMatrix();
}

void Renderer::SetTexture(float* p_colors,
                          const uint p_width,
                          const uint p_height,
                          const int bitsPerPixel)
{
    mTexture.data   = p_colors;
    mTexture.width  = p_width;
    mTexture.height = p_height;
    mTexture.bitsPerPixel = bitsPerPixel;
}

void Renderer::SetLight(const Light &l)
{
    light = l;
}

void DrawPixel(float4* p_colorBuffer, uint p_width, uint p_height, uint p_x, uint p_y, float4 p_color)
{
    if (p_x >= 0 && p_y >= 0 && p_x < p_width && p_y < p_height)
        p_colorBuffer[p_x + (p_y * p_width)] = p_color;
}

float2 ndcToScreenCoords(float3 ndc, const Viewport& viewport)
{
    float2 result = {};

    result.x = (ndc.x + 1) / 2 * viewport.width + 1;
    result.y = (ndc.y + 1) / 2 * viewport.height + 1;
    return result;
}

inline void CalculateLight(Vec3 normal, Vec3 clipCoords,
                           Light light, const Mat4 &projectionInverse, Vec3 &Emissive, Vec3 &Ambient,
                           Vec3 &Diffuse, Vec3 &Specular, Viewport viewport)
{
    //Going back from screen coords to ndc
    clipCoords.x = 1 - (viewport.width / (2 * clipCoords.x));
    clipCoords.y = 1 - (viewport.height / (2 * clipCoords.y));
    //Because the camera is at 0, 0, 0,
    //The direction from camera to vertex is the negation of vertex
    Vec3 viewDirection = 
    {
        -clipCoords.x * -clipCoords.z, // The multiplication with z happens because 
        -clipCoords.y * -clipCoords.z, // we pass ndcCoords, and we need to go back to clip coords
        clipCoords.z,
    };
    viewDirection.v = projectionInverse.mat * viewDirection.v; // we go back to camera space.
    viewDirection.Normalize();

    Vec3 lightDirection;
    if (light.Position.w == 1.0)
        lightDirection = viewDirection - light.Position.v3;
    else
        lightDirection = light.Direction;
    lightDirection.Normalize();

    float cosTheta = Maxf(Vec3::Dot(normal, lightDirection), 0.0);

    //Only if light is visible from the surface point.
    if(cosTheta > 0.0) {

        //Reflection vector around normal.
        Vec3 reflectionDirection = Vec3::Reflect(lightDirection, normal);
        //reflectionDirection.z = -reflectionDirection.z;
        reflectionDirection.Normalize();
        //Diffuse component.
        Diffuse.x = light.Diffuse * cosTheta;
        //Diffuse.y = light.Diffuse * cosTheta;
        Diffuse.z = light.Diffuse * cosTheta;
        //Specular component.
        Specular.x =  pow(Maxf(0.0, Vec3::Dot(reflectionDirection, viewDirection)), 0.3 * light.Shineness) * light.Specular;
        Specular.y =  pow(Maxf(0.0, Vec3::Dot(reflectionDirection, viewDirection)), 0.3 * light.Shineness) * light.Specular;
        Specular.z =  pow(Maxf(0.0, Vec3::Dot(reflectionDirection, viewDirection)), 0.3 * light.Shineness) * light.Specular;
    }
}

static void BoundingBox(float2 &min, float2 &max, float3 *screenCoords, const Viewport &viewport)
{
    min = Minf(Minf(screenCoords[0].xy, screenCoords[1].xy), screenCoords[2].xy);
    max = Maxf(Maxf(screenCoords[0].xy, screenCoords[1].xy), screenCoords[2].xy);

    min.x = Clamp(min.x, 0, viewport.width - 1);
    min.y = Clamp(min.y, 0, viewport.height - 1);
    max.x = Clamp(max.x, 0, viewport.width - 1);
    max.y = Clamp(max.y, 0, viewport.height - 1);
}

void Renderer::DrawTriangleFill(rdrVertex *vertices, Vec3 *normals,
                                float3 *screenCoords, Light &l)
{
    //Fill in triangle
    float4 *colorBuffer = reinterpret_cast<float4*>(fb.GetColorBuffer());
    float *depthBuffer = (fb.GetDepthBuffer());

    float2 min;
    float2 max;

    BoundingBox(min, max, screenCoords, viewport);

    float invW[3] =
    {
        (1.0f / screenCoords[0].z),
        (1.0f / screenCoords[1].z),
        (1.0f / screenCoords[2].z)
    };

    float areaABC = 0;
    getBarycentricCoordonates(screenCoords[0].xy,
                              screenCoords[1].xy,
                              screenCoords[2].xy,
                              {(float)0, (float)0},
                              &areaABC);


    if (areaABC < 0)
    {
        for (int y = (int)min.y; y <= (int)max.y; y++)
        {
            for (int x = (int)min.x; x <= (int)max.x; x++)
            {
                float3 bCoords = getBarycentricCoordonates(screenCoords[0].xy,
                                                           screenCoords[1].xy,
                                                           screenCoords[2].xy,
                                                           {(float)x, (float)y},
                                                           nullptr);


                if ((bCoords.y >= 0.0f) &&
                    (bCoords.x >= 0.0f) &&
                    (bCoords.z >= 0.0f))
                {
                    float invDepth = barycentricInterpolation(bCoords, invW);
                    float depth = 1.0f / invDepth;
                    if (depthBuffer[(y * viewport.width) + x] > depth)
                    {
                        //This might look complicated
                        //But all it says is: if wireframes are enabled, draw only the lines (bCoords smaller than 0.02)
                        //Or if the wireframes are disabled, ignore the condition, and draw the whole triangle
                        if ((enableWireframe && (bCoords.x <= 0.02
                                                 || bCoords.y <= 0.02 ||
                                                 bCoords.z <= 0.02))
                            || !enableWireframe)
                        {
                            float4 color = {};
                            if (enableTextures)
                            {
                                float2 textureInv[3] =
                                {
                                    vertices[0].textureCoords * invW[0],
                                    vertices[1].textureCoords * invW[1],
                                    vertices[2].textureCoords * invW[2]
                                };
                                float2 tCoords = barycentricInterpolation(bCoords, textureInv);
                                tCoords = tCoords * depth;

                                tCoords.y = 1 - tCoords.y;
                                int u = (int)(tCoords.x  * mTexture.bitsPerPixel * (mTexture.width - 1));
                                int v = (int)(tCoords.y  * mTexture.bitsPerPixel * (mTexture.height - 1));

                                u -= u % mTexture.bitsPerPixel;
                                v -= v % mTexture.bitsPerPixel;
                                u = Clamp(u, 0, ((mTexture.width - 1) * mTexture.bitsPerPixel));
                                v = Clamp(v, 0, ((mTexture.height - 1)* mTexture.bitsPerPixel));
                                color.r = mTexture.data[(int)(u + (v * mTexture.width))];
                                color.g = mTexture.data[(int)(u + 1 + (v * mTexture.width))];
                                color.b = mTexture.data[(int)(u + 2 + (v * mTexture.width))];
                                color.a = mTexture.data[(int)(u + 3 + (v * mTexture.width))];
                            }
                            else
                            {
                                color.r =
                                    vertices[0].color.r +
                                    bCoords.y * (vertices[1].color.r - vertices[0].color.r) +
                                    bCoords.z * (vertices[2].color.r - vertices[0].color.r);
                                color.g =
                                    vertices[0].color.g +
                                    bCoords.y * (vertices[1].color.g - vertices[0].color.g) +
                                    bCoords.z * (vertices[2].color.g - vertices[0].color.g);
                                color.b =
                                    vertices[0].color.b +
                                    bCoords.y * (vertices[1].color.b - vertices[0].color.b) +
                                    bCoords.z * (vertices[2].color.b - vertices[0].color.b);
                                color.a = vertices[0].color.a +
                                    bCoords.y * (vertices[1].color.a - vertices[0].color.a) +
                                    bCoords.z * (vertices[2].color.a - vertices[0].color.a);
                            }
                            if (enableLights)
                            {
                                Vec3 Emissive = {0, 0, 0};
                                Vec3 Ambient =  {0.3, 0.3, 0.3};
                                Vec3 Diffuse = Vec3{0.3, 0.3, 0.3};
                                Vec3 Specular = Vec3{0.3, 0.3, 0.3};

                                Vec3 normal = 
                                {
                                    normals[0].x +
                                        bCoords.y * (normals[1].x - normals[0].x) +
                                        bCoords.z * (normals[2].x - normals[0].x),
                                    normals[0].y +
                                        bCoords.y * (normals[1].y - normals[0].y) +
                                        bCoords.z * (normals[2].y - normals[0].y),
                                    normals[0].z +
                                        bCoords.y * (normals[1].z - normals[0].z) +
                                        bCoords.z * (normals[2].z - normals[0].z)
                                };
                                CalculateLight(normal, Vec3{(float)x, (float)y, depth}, l, mProjectionInverse, Emissive, Ambient, Diffuse, Specular, viewport);
                                color.xyz = ((light.Ambient * Ambient).v + color.xyz * light.Diffuse) + Specular.v * light.Specular;
                            }
                            depthBuffer[(y * viewport.width) + x] = depth;
                            DrawPixel(colorBuffer, viewport.width, viewport.height, x, y, color);
                        }
                    }
                }
            }
        }
    }
}

void Renderer::DrawTriangle(rdrVertex* vertices)
{

    Vec4 clipCoords[3] = {
        { Vec4{ vertices[0].pos.v, 1.f } },
        { Vec4{ vertices[1].pos.v, 1.f } },
        { Vec4{ vertices[2].pos.v, 1.f } },
    };

    clipCoords[0].v = mView.mat * (mModel.mat * clipCoords[0].v);
    clipCoords[1].v = mView.mat * (mModel.mat * clipCoords[1].v);
    clipCoords[2].v = mView.mat * (mModel.mat * clipCoords[2].v);

    Vec3 normals[3];

    normals[0].v = mNormal.mat * vertices[0].normal.v;
    normals[1].v = mNormal.mat * vertices[1].normal.v;
    normals[2].v = mNormal.mat * vertices[2].normal.v;
    normals[0].Normalize(); 
    normals[1].Normalize(); 
    normals[2].Normalize(); 
    Light l = light;
    l.Position.v = mView.mat * (mModel.mat * light.Position.v);
    // NOTE(V. Caraulan): We have everything in camera space

    clipCoords[0].v = mProjection.mat * clipCoords[0].v;
    clipCoords[1].v = mProjection.mat * clipCoords[1].v;
    clipCoords[2].v = mProjection.mat * clipCoords[2].v;

    clipCoords[0].Homogenize();
    clipCoords[1].Homogenize();
    clipCoords[2].Homogenize();

    if (clipCoords[0].w >= 0 && clipCoords[1].w >= 0 &&
        clipCoords[2].w >= 0)
    {
        float3 screenCoords[3] = {
            { clipCoords[0].xy, clipCoords[0].w},
            { clipCoords[1].xy, clipCoords[1].w},
            { clipCoords[2].xy, clipCoords[2].w},
        };

        screenCoords[0].xy = ndcToScreenCoords(screenCoords[0], viewport);
        screenCoords[1].xy = ndcToScreenCoords(screenCoords[1], viewport);
        screenCoords[2].xy = ndcToScreenCoords(screenCoords[2], viewport);

        DrawTriangleFill(vertices, normals, screenCoords, l);
    }
}

void Renderer::DrawQuad(rdrVertex* vertices)
{
    rdrVertex temp[3] = 
    {
        {vertices[1]},
        {vertices[0]},
        {vertices[3]},
    };
    Vec3 normal;
    normal.v = getNormalFromTriangle(vertices[0].pos.v, vertices[1].pos.v, vertices[3].pos.v);
    for (int i = 0; i < 4; i ++)
    {
        vertices[i].normal = normal;
        vertices[i].normal.Normalize();
    }
    DrawTriangle(vertices);
    DrawTriangle(temp);
}

void Renderer::DrawMesh(class Mesh* mesh)
{
    for(int i = 0; i < (int)mesh->indices.size(); i += 3)
    {
        rdrVertex vertices[3];
        vertices[0].pos = mesh->pos[mesh->indices[i]];
        vertices[1].pos = mesh->pos[mesh->indices[i + 1]];
        vertices[2].pos = mesh->pos[mesh->indices[i + 2]];
        vertices[0].normal = mesh->normal[i];
        vertices[1].normal = mesh->normal[i + 1];
        vertices[2].normal = mesh->normal[i + 2];
        vertices[0].color = mesh->color[i];
        vertices[1].color = mesh->color[i + 1];
        vertices[2].color = mesh->color[i + 2];
        vertices[0].textureCoords = mesh->textureCoordinates[i];
        vertices[1].textureCoords = mesh->textureCoordinates[i + 1];
        vertices[2].textureCoords = mesh->textureCoordinates[i + 2];
        DrawTriangle(vertices);
    }
}

void Renderer::DrawRdrArray(rdrVertex *v, int size)
{
    Light light;
    for(int i = 0; i < size; i += 3)
    {
        rdrVertex vertices[3] = 
        {
            v[i],
            v[i + 1],
            v[i + 2]
        };
        DrawTriangle(vertices);
    }
}

void Renderer::ShowImGuiControls()
{
    ImGui::ColorEdit4 ("lineColor", lineColor.e);
    ImGui::Checkbox   ("textures",  &enableTextures);
    ImGui::Checkbox   ("lights",    &enableLights);
    ImGui::Checkbox   ("wireframe", &enableWireframe);
    ImGui::SliderFloat("Alpha", &alpha,0.0f, 500.0f,"%.3f");
}
