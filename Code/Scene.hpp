
#pragma once
#include <vector>
#include "Renderer.hpp"
#include "Light.hpp" 
#include "Texture.hpp"
#include "Mesh.hpp"

class Scene
{
    double time = 0.0;

    // We could think about these 
    //as worldspace coordinates

    Vec3  mTranslation {0, 0, 0};
    Vec3  mRotation    {0, 0, 0};
    Vec3  mScale       {1, 1, 1};
    float mDeltaTime;
    std::vector<Texture> mTextures;
    std::vector<Light> mLights;
    class Mesh* mCubeMesh;
    class Mesh* mSphereMesh;
    class Mesh* mConeMesh;
    Mesh blacksmith = {};
    Mesh anvil = {};
    Mesh floor = {};
    Mesh spark = {};
public:
    void LoadData();
    void UnloadData();
    Scene();
    ~Scene();
    void Update(float p_deltaTime, Renderer& p_renderer);

    void ShowImGuiControls();
    void SetCubeMesh(Mesh *cube);
    void SetConeMesh(Mesh *cone);
    void SetSphereMesh(Mesh *sphere);
};


