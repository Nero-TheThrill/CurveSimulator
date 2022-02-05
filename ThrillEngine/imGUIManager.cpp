#include "imGUIManager.h"
#include <iostream>
#include "Graphics.h"
#include "ObjectManager.h"
#include "imgui/imgui_impl_opengl3.h"
imGUIManager* IMGUIMANAGER = nullptr;

imGUIManager::imGUIManager(GLFWwindow* window)
{

    IMGUIMANAGER = this;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    light_type[0] = "POINT";
    light_type[1] = "DIRECTIONAL";
    light_type[2] = "SPOT";
    std::cout << "imGUI Initialize" << std::endl;
}

void imGUIManager::Update()
{
    if (useIMGUI)
    {


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();


        ImGui::NewFrame();
        {
        ImGui::Begin("GraphicsEngine GUI");
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        if (ImGui::Button("Reload Shader"))
            GRAPHICS->ReLoadShader();
        ImGui::SameLine();
        if (ImGui::Button("Unselect All"))
        {
            if (current_item != nullptr)
                current_item->item_selected = false;
            if (current_light != nullptr)
                current_light->item_selected = false;
            current_light = nullptr;
            current_item = nullptr;

        }
        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();

        glm::vec3 bgcolor = GRAPHICS->background_color;
        ImGui::DragFloat3("background color", glm::value_ptr(bgcolor), 0.01f, 0, 1);
        GRAPHICS->SetBackgroundColor(glm::vec4(bgcolor, 1.0f));


        std::unordered_map<unsigned, Object*> objects = OBJECTMANAGER->GetAllObjects();

        if (ImGui::BeginCombo("select object", current_item != nullptr ? current_item->name.c_str() : ""))
        {

            prev_item = current_item;

            for (auto obj : objects)
            {
                if (obj.second != nullptr)
                {
                    bool is_selected = (current_item == obj.second);
                    if (ImGui::Selectable(obj.second->name.c_str(), is_selected))
                    {
                        current_item = obj.second;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();

                }
            }
            if (current_item != prev_item)
            {
                if (prev_item != nullptr)
                {
                    prev_item->item_selected = false;
                }
                if (current_item != nullptr)
                {
                    current_item->item_selected = true;
                }

            }
            ImGui::EndCombo();
        }
        if (current_item != nullptr)
        {
            glm::vec3 pos = current_item->transform.position;
            ImGui::DragFloat3("translation", glm::value_ptr(pos), 0.1f, -FLT_MAX, FLT_MAX);
            current_item->transform.Translate(pos);

            glm::vec3 scale = current_item->transform.current_scale;
            ImGui::DragFloat3("scale", glm::value_ptr(scale), 0.1f, -FLT_MAX, FLT_MAX);
            current_item->transform.Scale(scale);

            glm::vec3 rotate = current_item->transform.current_rotate_axis;
            float degree = current_item->transform.current_rotate_degree;
            ImGui::DragFloat3("rotate axis", glm::value_ptr(rotate), 0.01f, -1, 1);
            ImGui::DragFloat("degree", &degree);
            current_item->transform.Rotate(degree, rotate);

            std::unordered_map < std::string, std::pair<Shader, std::pair<std::string, std::string>>> shaders = GRAPHICS->GetAllShaders();
            std::string current_shader = current_item->shader.name;

            if (ImGui::BeginCombo("select shader", current_shader.c_str()))
            {
                for (auto shader : shaders)
                {
                    bool is_selected = (current_shader == shader.second.first.name);
                    if (ImGui::Selectable(shader.second.first.name.c_str(), is_selected))
                    {
                        current_shader = shader.second.first.name;
                        current_item->SetShader(shader.second.first.name);
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            std::unordered_map<std::string, MeshGroup*> meshes = GRAPHICS->GetAllMeshGroups();
            std::string current_mesh = current_item->mesh->name;
            if (ImGui::BeginCombo("select mesh", current_mesh.c_str()))
            {
                for (auto mesh : meshes)
                {
                    bool is_selected = (current_mesh == mesh.second->name);
                    if (ImGui::Selectable(mesh.second->name.c_str(), is_selected))
                    {
                        current_mesh = mesh.second->name;
                        current_item->SetMeshGroup(mesh.second);
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }
            ImGui::End();
        }
        {
            ImGui::Begin("Point GUI");

            if (ImGui::Button("Add Point"))
            {
                if (prev_point != nullptr)
                {
                    prev_point->item_selected = false;
                }
                if (current_point != nullptr)
                {
                    current_point->item_selected = false;
                }
                current_point = new Object("point"+std::to_string(point_num));
                current_point->SetMeshGroup(GRAPHICS->GetMeshGroup("customsphere"));
                current_point->SetShader("phong");
                current_point->material = GRAPHICS->GetMaterial("m_default");
                current_point->transform.Scale(glm::vec3(0.5f));
                current_point->drawmode = 1;
                current_point->mapping_mode = 1;
                current_point->pointnum = point_num;
                current_point->item_selected = true;
                control_points.insert(std::pair<unsigned, Object*>(point_num, current_point));

                point_num++;
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete Point"))
            {
                if (current_point != nullptr)
                {
                   
                    control_points.erase(current_point->pointnum);
                    current_point->alive = false;
                    current_point = nullptr;
                }
            }
       

            if (ImGui::BeginCombo("select point", current_point != nullptr ? current_point->name.c_str() : ""))
            {

                prev_point = current_point;

                for (auto obj : control_points)
                {
                    if (obj.second != nullptr)
                    {
                        bool is_selected = (current_point == obj.second);
                        if (ImGui::Selectable(obj.second->name.c_str(), is_selected))
                        {
                            current_point = obj.second;
                        }
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();

                    }
                }
                if (current_point != prev_point)
                {
                    if (prev_point != nullptr)
                    {
                        prev_point->item_selected = false;
                    }
                    if (current_point != nullptr)
                    {
                        current_point->item_selected = true;
                    }

                }

                ImGui::EndCombo();
            }
            if (current_point != nullptr)
            {
                glm::vec3 pos = current_point->transform.position;
                ImGui::DragFloat3("translation", glm::value_ptr(pos), 0.1f, -FLT_MAX, FLT_MAX);
                current_point->transform.Translate(pos);

                glm::vec3 scale = current_point->transform.current_scale;
                ImGui::DragFloat3("scale", glm::value_ptr(scale), 0.1f, -FLT_MAX, FLT_MAX);
                current_point->transform.Scale(scale);

                glm::vec3 rotate = current_point->transform.current_rotate_axis;
                float degree = current_point->transform.current_rotate_degree;
                ImGui::DragFloat3("rotate axis", glm::value_ptr(rotate), 0.01f, -1, 1);
                ImGui::DragFloat("degree", &degree);
                current_point->transform.Rotate(degree, rotate);

            }
            ImGui::End();
        }

  
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void imGUIManager::Init()
{
    lightNumberChanged = true;
    current_light = nullptr;
    current_item = nullptr;
    prev_item = nullptr;
    prev_point = nullptr;
    current_point = nullptr;
    shouldRotatelight = true;
    lightNumber = 1;

}

imGUIManager::~imGUIManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    std::cout << "imGUIManager Destructor Called" << std::endl;
}
