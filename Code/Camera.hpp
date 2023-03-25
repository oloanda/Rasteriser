#pragma once

#include "Types.hpp"

struct CameraInputs
{
    float deltaX;
    float deltaY;
    bool moveForward;
    bool moveBackward;
    bool moveLeft;
    bool moveRight;
    // ... and more if needed
};

class Camera
{
    Mat4 mCameraProjection = {};
    Mat4 mCameraView       = {};

    Vec3 rotation = {};
    Vec3 position = {};
    Vec3 acceleration = {};
    Vec3 scale = {};
public:
    Camera(const uint p_width, const uint p_height);

    void Update(const float p_deltaTime, const CameraInputs& p_inputs);
    m4x4 GetViewMatrix();
    m4x4 GetProjection();
    void RotateYCamera(float angle);
    void ShowImGuiControls();
};