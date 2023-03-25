#include "imgui/imgui.h"
#include "Maths.hpp"
#include "Scene.hpp"
#include "Mesh.hpp"

void Scene::LoadData(){
    mCubeMesh   =  Mesh::CreateCube();
    mSphereMesh =  Mesh::CreateSphere(10, 10);
    mConeMesh   =  Mesh::CreateCone(10);

    Texture tex;
    tex.Load("../Assets/blacksmith.bmp");
    mTextures.push_back(tex);
    tex.Load("../Assets/anvil.bmp");
    mTextures.push_back(tex);
    tex.Load("../Assets/spark.bmp");
    mTextures.push_back(tex);
    tex.Load("../Assets/floor.bmp");
    mTextures.push_back(tex);

    Light light;

    light.Position = {0.0f, 1.5f, 0.0f, 1.0};
    light.Direction = {0.0f, 1.0f, 0.0f};

    light.Emissive = 1;
    light.Ambient =  0;
    light.Diffuse = 0.78;
    light.Specular = 0.459;

    mLights.push_back(light);
    Mesh::CreateNobleMesh(&blacksmith, &anvil, &floor, &spark);
}

void Scene::UnloadData(){

    delete mCubeMesh;
    delete mSphereMesh;
    delete mConeMesh;
    for (int i = 0; i < (int)mTextures.size();i++)
        delete []mTextures[i].data;
    mTextures.clear();
    mLights.clear();
}

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Update(float deltaTime, Renderer& renderer)
{
    Mat4 model = {};

    renderer.SetLight(mLights[0]);
    renderer.SetTexture(mTextures[3]);
    renderer.SetModel(model.e);

    model = Mat4::CreateTransformMatrix({mRotation.x, 360 * cosf(time / 4), mRotation.z}, mTranslation + Vec3{0.0f, -0.5f, 0.0f}, {1, 1, 1});
    renderer.SetModel(model.e);
    renderer.SetTexture(mTextures[0]);
    renderer.DrawMesh(&blacksmith);
    renderer.SetTexture(mTextures[1]);
    renderer.DrawMesh(&anvil);
    renderer.SetTexture(mTextures[2]);
    renderer.DrawMesh(&spark);
    renderer.SetTexture(mTextures[3]);
    renderer.DrawMesh(&floor);

    model = Mat4::CreateTransformMatrix({mRotation.x, 360 * cosf(time / 4), mRotation.z}, mTranslation + Vec3{-2.0f, -0.5f, 0.0f}, {1, 1, 1});
    renderer.SetModel(model.e);
    renderer.SetTexture(mTextures[3]);
    renderer.DrawMesh(mSphereMesh);

    model = Mat4::CreateTransformMatrix({mRotation.x, 360 * cosf(time / 4), mRotation.z}, mTranslation + Vec3{2.0f, -0.5f, 0.0f}, {1, 1, 1});
    renderer.SetModel(model.e);
    renderer.SetTexture(mTextures[3]);
    renderer.DrawMesh(mCubeMesh);

    model = Mat4::CreateTransformMatrix({mRotation.x, 360 * cosf(time / 4), mRotation.z}, mTranslation + Vec3{0, 2, 0}, {1, 1, 1});
    renderer.SetModel(model.e);
    renderer.SetTexture(mTextures[3]);
    renderer.DrawMesh(mConeMesh);


    time += deltaTime;
    mDeltaTime = deltaTime;
}

void Scene::ShowImGuiControls()
{
    ImGui::SliderFloat3("position" , mTranslation.e, -2.0, 2.0, "%.3f", 0);
    ImGui::SliderFloat3("rotation" , mRotation.e   , -180, 180, "%.3f", 0);
    ImGui::SliderFloat3("scale"    , mScale.e, 0.f, 10.f);
    ImGui::SliderFloat("Ambient Coef ", &mLights[0].Ambient, 0.0f, 1.0f,"%.3f",0);
    ImGui::SliderFloat("Diffuse Coef ", &mLights[0].Diffuse, 0.0f, 1.0f,"%.3f",0);
    ImGui::SliderFloat("Specular Coef ", &mLights[0].Specular, 0.0f, 1.0f,"%.3f",0);
    ImGui::SliderFloat("Emissive Coef ", &mLights[0].Emissive, 0.0f, 1.0f,"%.3f",0);
    ImGui::SliderFloat("Shineness Coef ", &mLights[0].Shineness, 0.0f, 1.0f,"%.3f",0);
    ImGui::Text       ("FrameTime =  %f ms", mDeltaTime * 1000);
}
