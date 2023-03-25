#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include <fstream>
#include <string.h>
#include "Maths.hpp"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

Mesh* Mesh::CreateCube()
{
    Mesh* mesh = new Mesh();

    rdrVertex vertices[] = {
        //       pos                  normal                  color              uv
        { 0.5f,-0.5f, 0.5f,      0.0f, 0.0f, 1.0f,      {1.0f, 0.0f, 0.0f, 1.0f}, 1.0f, 0.0f },
        {-0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,      {0.0f, 1.0f, 0.0f, 1.0f}, 0.0f, 1.0f },
        {-0.5f,-0.5f, 0.5f,      0.0f, 0.0f, 1.0f,      {0.0, 0, 1, 1},           0.0f, 0.0f },
        { 0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,      {1.0, 0, 0, 1},           1.0f, 1.0f },

        {0.5,  0.5f, -0.5f,     0.0f, 0.0f, -1.0f,      {1.0, 0, 1, 1},           1.0f, 1.0f},
        {-0.5,-0.5f, -0.5f,     0.0f, 0.0f, -1.0f,      {1.0, 1, 0, 1},           0.0f, 0.0f },
        {-0.5, 0.5f, -0.5f,     0.0f, 0.0f, -1.0f,      {1.0, 0, 1, 1},           0.0f, 1.0f}, 
        {0.5f,-0.5f, -0.5f,     0.0f, 0.0f, -1.0f,      {1.0, 1, 0, 1},           1.0f, 0.0f} ,
    };

    int triangleIndices[36] = 
    {
        0, 1, 2,//Front
        1, 0, 3,
        4, 5, 6,//Back
        5, 4, 7,
        4, 1, 3,//Top
        1, 4, 6,
        5, 0, 2,//Bottom
        0, 5, 7,
        7, 3, 0,//Left
        3, 7, 4,
        6, 2, 1,//Right
        2, 6 ,5
    };

    for(int i = 0; i < 8; i++)
    {
        mesh->pos.push_back(vertices[i].pos);
    }
    float2 uv[] =
    {
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };
    Vec3 normal;
    for(int i = 0; i < 36; i++)
    {
        if (i % 3 == 0)
        {
            normal.v = getNormalFromTriangle(vertices[triangleIndices[i]].pos.v,
                                             vertices[triangleIndices[i + 1]].pos.v,
                                             vertices[triangleIndices[i + 2]].pos.v);
            normal.Normalize();
        }
        mesh->normal.push_back(normal);
        mesh->color.push_back(vertices[triangleIndices[i]].color);
        mesh->indices.push_back(triangleIndices[i]);
        mesh->textureCoordinates.push_back(uv[i % 6]);
    }
    return mesh;
}

Mesh* Mesh::CreateSphere(int latitudeCount, int longitudeCount)
{
    Mesh* mesh = new Mesh();
    float r = 0.5f;

    for (int j = 0; j < latitudeCount; ++j)
    {
        float theta0 = ((j+0) / (float)latitudeCount) * M_PI;
        float theta1 = ((j+1) / (float)latitudeCount) * M_PI;

        for (int i = 0; i < longitudeCount; ++i)
        {
            float phi0 = ((i+0) / (float)longitudeCount) * 2.f * M_PI;
            float phi1 = ((i+1) / (float)longitudeCount) * 2.f * M_PI;

            float3 c0 = GetSphericalCoords(r, theta0, phi0);
            float3 c1 = GetSphericalCoords(r, theta0, phi1);
            float3 c2 = GetSphericalCoords(r, theta1, phi1);
            float3 c3 = GetSphericalCoords(r, theta1, phi0);

            rdrVertex temp[4] = {};

            temp[0].pos.v = c0;
            temp[1].pos.v = c1;
            temp[2].pos.v = c2;
            temp[3].pos.v = c3;

            temp[0].normal.v = c0;
            temp[0].normal.Normalize();
            temp[1].normal.v = c1;
            temp[1].normal.Normalize();
            temp[2].normal.v = c2;
            temp[2].normal.Normalize();
            temp[3].normal.v = c3;
            temp[3].normal.Normalize();

            temp[0].color.r = 1;

            int test = mesh->pos.size();
            mesh->pos.push_back(temp[0].pos);
            mesh->pos.push_back(temp[1].pos);
            mesh->pos.push_back(temp[2].pos);
            mesh->pos.push_back(temp[3].pos);

            mesh->normal.push_back(temp[1].normal);
            mesh->normal.push_back(temp[3].normal);
            mesh->normal.push_back(temp[0].normal);
            mesh->normal.push_back(temp[2].normal);
            mesh->normal.push_back(temp[3].normal);
            mesh->normal.push_back(temp[1].normal);

            mesh->color.push_back(temp[0].color);
            mesh->color.push_back(temp[0].color);
            mesh->color.push_back(temp[0].color);
            mesh->color.push_back(temp[0].color);
            mesh->color.push_back(temp[0].color);
            mesh->color.push_back(temp[0].color);

            mesh->indices.push_back(test + 1);
            mesh->indices.push_back(test + 3);
            mesh->indices.push_back(test + 0);
            mesh->indices.push_back(test + 2);
            mesh->indices.push_back(test + 3);
            mesh->indices.push_back(test + 1);

            float2 uv[] =
            {
                0.0f, 1.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,

                0.0f, 0.0f, 
                1.0f, 0.0f,
                0.0f, 1.0f,
            };

            mesh->textureCoordinates.push_back(uv[0]);
            mesh->textureCoordinates.push_back(uv[1]);
            mesh->textureCoordinates.push_back(uv[2]);
            mesh->textureCoordinates.push_back(uv[3]);
            mesh->textureCoordinates.push_back(uv[4]);
            mesh->textureCoordinates.push_back(uv[5]);
        }
    }
    return mesh;
}

Mesh* Mesh::CreateCone(int res)
{
    Mesh* mesh = new Mesh();
    float r = 0.5f;

    Vec3 vertix0;
    Vec3 vertix1;
    Vec3 vertix2 = {0.0, 0.5f, 0.f};

    Vec3 zero = {0, -0.5, 0};
    mesh->pos.push_back(zero);

    for (int i = 0; i <= res - 1; i++)
    {
        float theta = 2.0f * M_PI * (float)i / res;

        vertix0 = {{r * cosf(theta), -0.5f, r * sinf(theta)}};
        // For the last triangle between last and first vertex.
        if (i == res - 1)
            theta = 0.0f;
        theta = 2.0f * M_PI * (float)(i + 1) / res;
        vertix1 = {{r * cosf(theta), -0.5f, r * sinf(theta)}};

        rdrVertex temp0[3] =
        {
            {vertix0},
            {vertix1},
            {vertix2},
        };

        int index = mesh->pos.size();
        Vec3 normal;
        normal.v = getNormalFromTriangle(vertix0.v, vertix1.v, vertix2.v);
        normal.Normalize();

        temp0[0].color = {0.4, 0.4, 0.4, 1};
        temp0[1].color = {0.4, 0.4, 0.4, 1};
        temp0[2].color = {0.4, 0.4, 0.4, 1};

        mesh->pos.push_back(temp0[0].pos);
        mesh->pos.push_back(temp0[1].pos);
        mesh->pos.push_back(temp0[2].pos);

        mesh->normal.push_back(normal);
        mesh->normal.push_back(normal);
        mesh->normal.push_back(normal);

        mesh->color.push_back(temp0[0].color);
        mesh->color.push_back(temp0[0].color);
        mesh->color.push_back(temp0[0].color);

        mesh->indices.push_back(index + 1);
        mesh->indices.push_back(index);
        mesh->indices.push_back(index + 2);

        mesh->color.push_back(temp0[0].color);
        mesh->color.push_back(temp0[0].color);
        mesh->color.push_back(temp0[0].color);

        mesh->indices.push_back(0);
        mesh->indices.push_back(index);
        mesh->indices.push_back(index + 1);
        normal.v = getNormalFromTriangle(zero.v, vertix0.v, vertix1.v);
        mesh->normal.push_back(normal);
        mesh->normal.push_back(normal);
        mesh->normal.push_back(normal);

        float2 uv[] =
        {
            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

            0.0f, 0.0f, 
            1.0f, 0.0f,
            0.0f, 1.0f,
        };
        mesh->textureCoordinates.push_back(uv[0]);
        mesh->textureCoordinates.push_back(uv[1]);
        mesh->textureCoordinates.push_back(uv[2]);
        mesh->textureCoordinates.push_back(uv[3]);
        mesh->textureCoordinates.push_back(uv[4]);
        mesh->textureCoordinates.push_back(uv[5]);
    }
    return (mesh);
}

void Mesh::CreateNobleMesh(Mesh *blacksmith, Mesh *anvil, Mesh *floor, Mesh *spark)
{
    FILE *meshObject = fopen("assets/blacksmith.obj", "rb");
    if (meshObject != nullptr)
    {
        for(int i = 0; i < 19857; i++)
        {
            blacksmith->pos.push_back(Vec3{});
            blacksmith->color.push_back(Vec4{});
            blacksmith->indices.push_back(int{});
            blacksmith->textureCoordinates.push_back(float2{});
            blacksmith->normal.push_back(Vec3{});
            fread(&blacksmith->pos[i], sizeof(Vec3), 1, meshObject);
            fread(&blacksmith->color[i], sizeof(Vec4), 1, meshObject);
            fread(&blacksmith->indices[i], sizeof(int), 1, meshObject);
            fread(&blacksmith->textureCoordinates[i], sizeof(float2), 1, meshObject);
            fread(&blacksmith->normal[i], sizeof(Vec3), 1, meshObject);
        }
        for(int i = 0; i < 804; i++)
        {
            anvil->pos.push_back(Vec3{});
            anvil->color.push_back(Vec4{});
            anvil->indices.push_back(int{});
            anvil->textureCoordinates.push_back(float2{});
            anvil->normal.push_back(Vec3{});
            fread(&anvil->pos[i], sizeof(Vec3), 1, meshObject);
            fread(&anvil->color[i], sizeof(Vec4), 1, meshObject);
            fread(&anvil->indices[i], sizeof(int), 1, meshObject);
            fread(&anvil->textureCoordinates[i], sizeof(float2), 1, meshObject);
            fread(&anvil->normal[i], sizeof(Vec3), 1, meshObject);
        }
        for(int i = 0; i < 6; i++)
        {
            floor->pos.push_back(Vec3{});
            floor->color.push_back(Vec4{});
            floor->indices.push_back(int{});
            floor->textureCoordinates.push_back(float2{});
            floor->normal.push_back(Vec3{});
            fread(&floor->pos[i], sizeof(Vec3), 1, meshObject);
            fread(&floor->color[i], sizeof(Vec4), 1, meshObject);
            fread(&floor->indices[i], sizeof(int), 1, meshObject);
            fread(&floor->textureCoordinates[i], sizeof(float2), 1, meshObject);
            fread(&floor->normal[i], sizeof(Vec3), 1, meshObject);
        }
        for(int i = 0; i < 240; i++)
        {
            spark->pos.push_back(Vec3{});
            spark->color.push_back(Vec4{});
            spark->indices.push_back(int{});
            spark->textureCoordinates.push_back(float2{});
            spark->normal.push_back(Vec3{});
            fread(&spark->pos[i], sizeof(Vec3), 1, meshObject);
            fread(&spark->color[i], sizeof(Vec4), 1, meshObject);
            fread(&spark->indices[i], sizeof(int), 1, meshObject);
            fread(&spark->textureCoordinates[i], sizeof(float2), 1, meshObject);
            fread(&spark->normal[i], sizeof(Vec3), 1, meshObject);
        }
    }
}