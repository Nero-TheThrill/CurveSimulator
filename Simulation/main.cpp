#include "Engine.h"
#include "Application.h"
#include "Graphics.h"
#include "LevelManager.h"
#include "ObjectLoader.h"
#include "ObjectManager.h"
#include "TestLevel.h"

int main()
{

    auto* ThrillEngine = new Engine();
    ObjectLoader objloader;


    GRAPHICS->LoadShader("../shaders/v_shader.vert", "v_shader", ShaderType::VERTEX);
    GRAPHICS->LoadShader("../shaders/f_shader.frag", "f_shader", ShaderType::FRAGMENT);
    GRAPHICS->CompileShader("v_shader", "f_shader", "shader");

    GRAPHICS->LoadShader("../shaders/phong_shading.vert", "v_phong_shading", ShaderType::VERTEX);
    GRAPHICS->LoadShader("../shaders/phong_shading.frag", "f_phong_shading", ShaderType::FRAGMENT);
    GRAPHICS->CompileShader("v_phong_shading", "f_phong_shading", "phong");

    objloader.loadObject("../models/bunny_high_poly.obj", "bunny");

    GRAPHICS->AddSphereMesh();




    ThrillEngine->Init();


    new Material("m_default");




    LEVELMANAGER->InsertLevel(new TestLevel(), 1);
    LEVELMANAGER->ChangeLevel(1);





    ThrillEngine->Loop();
    ThrillEngine->Quit();
    delete ThrillEngine;
    return 0;
}

