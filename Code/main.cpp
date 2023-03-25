#include <cstdio>

//Define for imgui sse
#define  __SSE__ 1

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glad/gl.h"

#define GLFW_INCLUDE_NONE
#include "glfw3.h"

#include "Renderer.hpp"
#include "Scene.hpp"

#include "Maths.hpp"
#include "Camera.hpp"

#include "Framebuffer.hpp"

#include "App.hpp"

int main(int argc, char* argv[])
{
    //Prepare the init structure
    AppInit init =
    {
        1200, 
        800, 
        "Software renderer tester",
        [](int errorCode, const char* description)
        { 
            printf("GLFW error (code=%d): '%s'\n", errorCode, description);
        },
        [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
        {
            printf("OpenGL error: '%s'\n", message);
        }
    };

    // Init App
    App myApp(init);

    //Check state app
    switch (myApp.GetState())
    {
        case EAppState::NONE:
        case EAppState::GLFWFAILED:
        case EAppState::WINDOWFAILED:
        case EAppState::GLADFAILED:
        printf("You have failed!\n");
        break;
        case EAppState::SUCCESS:
        printf("Success\n");
        break;
        default:
        break;
    }

    myApp.Update();
    return 0;
}