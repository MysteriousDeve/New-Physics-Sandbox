#include "ProjectInclude.h"

#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"

#include "UI/InteractionHandler.h"
#include "UI/EventCall.h"

using namespace std;
using namespace glm;


int w = 1400, h = 800;
bool running = true;

void ErrorCallback(int errorCode, const char* description);
void ResizeCallback(GLFWwindow* window, int w, int h);
void InitializeWindow(GLFWwindow* window);
void Panning(GLFWwindow* window);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


Camera cam(Vector2(0.0f, 0.0f), 1, 0, w / float(h));

int main()
{
    














    glfwSetErrorCallback(ErrorCallback);
    glfwInit();

    GLFWwindow* win = glfwCreateWindow(w, h, "My Physics Engine", NULL, NULL);

    if (win == NULL)
    {
        glfwTerminate();
        return -1;
    }
    InitializeWindow(win);

    glfwSetScrollCallback(win, OnScroll);
    
    
    gladLoadGL();
    glViewport(0, 0, w, h);


    GLfloat vert[]{
         -0.5f, -0.5f, 0.0f,
         -0.5f,  0.5f, 0.0f,
          0.5f,  0.5f, 0.0f,
          0.5f, -0.5f, 0.0f
    };
    GLuint index[]{
        0, 1, 2, 2, 3, 0
    };
    Renderer renderer(
        MeshInfo(vert, sizeof(vert), index, sizeof(index)),
        "Common", "Circle"
    );
    renderer.Initialize();
    
    Scene scene;
    scene.InitializeSceneTesting(300);
    
    InteractionHandler handler(win, &cam);
    handler.AddButton(Vector2(), Vector2(300, 300));
    eventHandler.push_back(&handler);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(win))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Panning(win);
        renderer.SetCameraTransformation(cam.GetCameraMatrix(), cam.GetPosition());

        float currentTime = glfwGetTime();
        float dt = currentTime - lastTime;
        if (running)
        {
            scene.Update(dt / 20.0);
        }
        lastTime = currentTime;
        

        renderer.Render(
            scene.geometries.size(),
            [&scene](int i, Shader* shader) 
            {
                scene.RenderScene(i, shader); 
            }
        );

        handler.UIRaycast();
        handler.RenderButtons();

        renderer.SetCameraTransformation(glm::inverse(cam.GetScalingMatrix()), vec2(0, 0));
        Vector2 mousePos = handler.GetMouseWorldPos();
        renderer.Render(
            1,
            [&mousePos](int i, Shader* shader)
            {
                shader->SetUniformVec4("Color", 1, 1, 1, 1);
                shader->SetUniformVec2("Position", mousePos.x, mousePos.y);
                shader->SetUniformVec2("Size", 0.1, 0.1);
            }
        );
        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    renderer.Terminate();
    glfwTerminate();
    return 0;
}

void InitializeWindow(GLFWwindow* window)
{
    glfwSetWindowSizeCallback(window, ResizeCallback);
    glfwSetWindowSizeLimits(window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetKeyCallback(window, KeyCallback);
    glfwMakeContextCurrent(window);
}

void ErrorCallback(int errorCode, const char* description)
{
    MessageBoxA(NULL, description, "Error", MB_ICONERROR | MB_OK);
}

void ResizeCallback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
    cam.SetWHratio(w / float(h));
}

void Panning(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        cam.Translate(Vector2(-1 / 60.0, 0));
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        cam.Translate(Vector2(1 / 60.0, 0));
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cam.Translate(Vector2(0, 1 / 60.0));
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cam.Translate(Vector2(0, -1 / 60.0));
    }
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        running = !running;
    }
}