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


        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();

        glm::vec3 bgcolor = GRAPHICS->background_color;
        ImGui::DragFloat3("background color", glm::value_ptr(bgcolor), 0.01f, 0, 1);
        GRAPHICS->SetBackgroundColor(glm::vec4(bgcolor, 1.0f));



            ImGui::End();
        }
        {
            ImGui::Begin("Point GUI");
            if (ImGui::Button("Unselect"))
            {
                if (prev_point != nullptr)
                {
                    prev_point->item_selected = false;
                }
                if (current_point != nullptr)
                {
                    current_point->item_selected = false;
                }
                current_point = nullptr;
                prev_point = nullptr;

            }
            ImGui::SameLine();
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


            }

            ImGui::End();
        }

  
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    cp.clear();
    for (auto p : control_points)
    {
        cp.push_back(p.second->transform.position);
    }
    if (cp.size() > 0)
    {
        glUseProgram(GRAPHICS->GetShader("shader").program_handle);
        GRAPHICS->GetShader("shader").set("model", glm::translate(glm::mat4(1.0f), glm::vec3(0)));
        GRAPHICS->GetShader("shader").set("objectColor", glm::vec3(0.7f));
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * cp.size() * 3), &cp[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(cp.size()));
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        glUseProgram(0);
    }
}

void imGUIManager::Init()
{
    lightNumberChanged = true;

    prev_point = nullptr;
    current_point = nullptr;
    shouldRotatelight = true;
    lightNumber = 1;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(0);

}

imGUIManager::~imGUIManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    std::cout << "imGUIManager Destructor Called" << std::endl;
}
