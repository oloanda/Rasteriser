#pragma once
#include "Maths.hpp"

struct Light
{
    Vec4 Position;
    Vec3 Direction;

    float Emissive = 0;
    float Ambient  = 0.3;
    float Diffuse  = 0.3;
    float Specular = 0.3;
    float Shineness = 0.3;
    void ShowImGuiControls();
};