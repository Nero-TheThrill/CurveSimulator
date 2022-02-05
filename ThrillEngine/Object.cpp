#include "Object.h"

#include "Graphics.h"
#include "Input.h"
#include "Material.h"
#include "ObjectManager.h"




void Object::Update()
{

    Draw();
}

void Object::SetShader(std::string id)
{
    shader = GRAPHICS->GetShader(id);
}

Object::Object(std::string n)
{
    id = 0;
    transform.Init();
    OBJECTMANAGER->RegisterObject(this);
    name = n;
    draw_normal_shader = GRAPHICS->GetShader("test");
    mesh = nullptr;
    material = nullptr;
    alive = true;
}

Object::Object(std::string n, Object* obj)
{
    id = 0;
    transform.Init();
    OBJECTMANAGER->RegisterObject(this);
    name = n;
    color = obj->color;
    transform = obj->transform;
    mesh = obj->mesh;
    drawmode = obj->drawmode;
    draw_normal_shader = obj->draw_normal_shader;
    shader = obj->shader;
    material = obj->material;
    alive = true;
}


Object::~Object()
{
}


void Object::SetColor(glm::vec3 inputcolor)
{
    color = inputcolor;
}

void Object::SetMeshGroup(MeshGroup* input)
{
    mesh = input;
}

void Object::Draw()
{
    if (mesh != nullptr&&should_draw)
    {
        mesh->mapping_mode = mapping_mode;
        mesh->should_calculate_uv_in_gpu = should_calculate_uv_in_gpu;
        mesh->mapping_with_normal = mapping_with_normal;

           
        mesh->ChangeMode(drawmode);
        glUseProgram(shader.program_handle);
        shader.set("model", transform.GetTransformMatrix());
        shader.set("objectColor", color);
        shader.set("item_selected", item_selected);
        mesh->Draw(shader, material);
        glUseProgram(0);
    }
}




