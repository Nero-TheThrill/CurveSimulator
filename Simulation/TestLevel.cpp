#include "TestLevel.h"
#include <iostream>
#include "Graphics.h"
#include "Input.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "TimeManager.h"
TestLevel::TestLevel()
{
    skysphere = nullptr;
    centerobj = nullptr;
}

void TestLevel::Init()
{
    IMGUIMANAGER->useIMGUI = true;
    GRAPHICS->SetBackgroundColor(glm::vec4(0,0.02f,0.07,1.0));
    




    centerobj = new Object("centerobject");
    centerobj->SetMeshGroup(GRAPHICS->GetMeshGroup("bunny"));
    centerobj->SetShader("phong");
    centerobj->material = GRAPHICS->GetMaterial("m_default");
    centerobj->transform.Translate(glm::vec3(-21,-12.1,-10));
    centerobj->transform.Rotate(43,glm::vec3(0, 1.f, -0.21f));
    centerobj->transform.Scale(glm::vec3(3.f));
    centerobj->drawmode = 1;
    centerobj->mapping_mode = 1;


    LightObject* lightsource = new LightObject("light");
    lightsource->SetMeshGroup(GRAPHICS->GetMeshGroup("customsphere"));
    lightsource->SetShader("shader");
    lightsource->transform.Scale(glm::vec3(0.2, 0.2, 0.2));
    lightsource->drawmode = 1;
    lightsource->type = LightType::POINT;
    lightsource->should_draw = false;


    GRAPHICS->centerobj = centerobj;

    GRAPHICS->camera.View(glm::vec3(0.0f, 4.0f, 25));
    GRAPHICS->camera.SetPitch(-10);
    GRAPHICS->camera.SetYaw(-90);
    IMGUIMANAGER->Init();
}

void TestLevel::Update()
{
   
    if (Input::IsPressed(GLFW_KEY_Q))
    {
        GRAPHICS->camera.Move(glm::vec3(0, 2, 0));
    }
    if (Input::IsPressed(GLFW_KEY_E))
    {
        GRAPHICS->camera.Move(glm::vec3(0, -2, 0));
    }
    if (Input::IsPressed(GLFW_KEY_D))
    {
        GRAPHICS->camera.MoveSide(5);
    }
    if (Input::IsPressed(GLFW_KEY_A))
    {
        GRAPHICS->camera.MoveSide(-5);
    }
    if (Input::IsPressed(GLFW_KEY_W))
    {
        GRAPHICS->camera.MoveForward(5);
    }
    if (Input::IsPressed(GLFW_KEY_S))
    {
        GRAPHICS->camera.MoveForward(-5);
    }
    if (Input::IsPressed(GLFW_KEY_RIGHT))
    {
        GRAPHICS->camera.RotateYaxis(3 * TIMEMANAGER->deltaTime * 10.f);
    }
    if (Input::IsPressed(GLFW_KEY_LEFT))
    {
        GRAPHICS->camera.RotateYaxis(-3 * TIMEMANAGER->deltaTime * 10.f);
    }
    if (Input::IsPressed(GLFW_KEY_DOWN))
    {
        GRAPHICS->camera.RotateXaxis(3 * TIMEMANAGER->deltaTime * 10.f);
    }
    if (Input::IsPressed(GLFW_KEY_UP))
    {
        GRAPHICS->camera.RotateXaxis(-3 * TIMEMANAGER->deltaTime * 10.f);
    }
}

void TestLevel::Close()
{
    OBJECTMANAGER->DeleteAll();

}

TestLevel::~TestLevel()
{
    std::cout << "TestLevel Destructor Called" << std::endl;
}
