#pragma once
#include <string>
#include "Material.h"
#include "MeshGroup.h"
#include "Texture.h"
#include "Transform.h"

class Object
{
public:
    Object(std::string n);
    Object(std::string n, Object* obj);
    ~Object();
    void SetColor(glm::vec3 inputcolor);
    void SetMeshGroup(MeshGroup* input);
    void Update();
    void SetShader(std::string id);
    void Draw();

    bool alive = true;
    unsigned id;
    int drawmode = 0;// 0: facenormal 1: vertexnormal
    Transform transform;
    MeshGroup* mesh;
    Shader shader, draw_normal_shader;
    Material* material;
    glm::vec3 color{ 1.0 };
    std::string name;
    bool item_selected =false;
    int mapping_mode = 0; // 0: default, 1: spherical, 2: cylindrical, 3: planar
    bool should_calculate_uv_in_gpu = true;
    bool mapping_with_normal = false;
    bool should_draw = true;
    bool is_input_point = false;
    int pointnum = 0;

};
