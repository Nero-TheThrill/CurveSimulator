#pragma once
#include "LightObject.h"
#include "Object.h"
#include "imgui/imgui_impl_glfw.h"
#include "Material.h"
class imGUIManager
{
public:
    imGUIManager(GLFWwindow* window);
    void Update();
    void Init();
    ~imGUIManager();
    LightObject* current_light = nullptr,*prev_light=nullptr;
    bool shouldRotatelight = true;
    bool lightNumberChanged = true;
    int lightNumber = 1;
    int prev_lightNumber = 1;
    std::string light_type[3];
    int lightoption = 0,prev_lightoption=0;
    bool justonce = true;
    int mapping_option = 0;
    bool should_mapping_on_gpu = true;
    bool useIMGUI = true;
    Object* current_point = nullptr,*prev_point=nullptr;
    int point_num = 0;
    std::unordered_map<unsigned, Object*> control_points;
    GLuint VAO,VBO;
    std::vector<glm::vec3> cp;

};
extern imGUIManager* IMGUIMANAGER;
