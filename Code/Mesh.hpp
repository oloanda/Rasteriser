#pragma once
#include <vector>
#include "Maths.hpp"

class Mesh
{
    public:
    Mesh();
    ~Mesh();

    static Mesh* CreateCube();
    static Mesh* CreateSphere(int latitudeCount, int longitudeCount);
    static Mesh* CreateCone(int res);
    static void CreateNobleMesh(Mesh *man, Mesh *anvil, Mesh *floor, Mesh *spark);

public:

    // NOTE(V. Caraulan): I assumed that structure of arrays is going to be faster than arrays of strctures
    // Tests haven't showed a significant difference.
    std::vector<Vec3>   pos = {};
    std::vector<Vec4>   color = {};
    std::vector<int>    indices = {};
    std::vector<float2> textureCoordinates = {};
    std::vector<Vec3>   normal = {};
};