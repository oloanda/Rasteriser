
#include "imgui/imgui.h"
#include "Maths.hpp"
#include "Camera.hpp"

Camera::Camera(const uint width, const uint height)
{
    mCameraProjection        = Mat4::CreatePerspectiveMatrix(70, (float)width / height, 0.1, 100.0f);
    rotation  = {0, 0, 0};
    position  = {0, 0, 2};
    scale     = {1, 1, 1};
    mCameraView = Mat4::CreateInverseMatrix(Mat4::CreateTransformMatrix(rotation,
                                                                        position,
                                                                        scale));
    //mCameraProjection        = Mat4::CreateOrthoMatrix(-1.7f, 1.7f, -1.f, 1.f, -5.0f, 5.0f);
}

void Camera::Update(const float p_deltaTime, const CameraInputs& inputs)
{
    float speed = p_deltaTime * 0.5;
    if (inputs.moveForward)
    {
        Vec3 pos = position;
        pos.Normalize();
        acceleration.x += -pos.x * speed;
        acceleration.y += -pos.y * speed;
        acceleration.z += -pos.z * speed;

    }
    if (inputs.moveBackward)
    {
        Vec3 pos = position;
        pos.Normalize();
        acceleration.x += pos.x * speed;
        acceleration.y += pos.y * speed;
        acceleration.z += pos.z * speed;
    }
    if (inputs.moveRight)
    {
        Mat4 rot = Mat4::CreateYRotationMatrix(3);
        position.v = rot.mat * position.v;
        rotation.y += 3;
    }
    if (inputs.moveLeft)
    {
        Mat4 rot = Mat4::CreateYRotationMatrix(-3);
        position.v = rot.mat * position.v;
        rotation.y -= 3;
    }
    rotation.x -= inputs.deltaY;
    rotation.y -= inputs.deltaX;
    position += acceleration;
    acceleration *= 0.8f;
    mCameraView.mat = Mat4::CreateInverseMatrix(Mat4::CreateTransformMatrix(rotation, position,scale )).mat;
}

m4x4 Camera::GetViewMatrix()
{
    return (mCameraView.mat);
}

m4x4 Camera::GetProjection()
{
    return (mCameraProjection.mat);
}

void Camera::RotateYCamera(float angle)
{
    Mat4 Rotation = Mat4::CreateYRotationMatrix(angle);
    mCameraView.mat = Rotation.mat * mCameraView.mat;
}
void Camera::ShowImGuiControls()
{
    ImGui::SliderFloat3("Position"       , position.e, -10, 10, "%.3f", 0);
    ImGui::SliderFloat3("Degree Rotation", rotation.e, 0, 360, "%.3f", 0);
    ImGui::SliderFloat3("Scale"          , scale   .e, 0.25, 3, "%.3f", 0);
}