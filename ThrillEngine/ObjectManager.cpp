#include "ObjectManager.h"
#include <iostream>

#include "Application.h"
#include "Graphics.h"

ObjectManager* OBJECTMANAGER = nullptr;

ObjectManager::ObjectManager()
{
    int width=static_cast<int>(APPLICATION->GetWindowSize().x), height = static_cast<int>(APPLICATION->GetWindowSize().y);
    OBJECTMANAGER = this;
    genObjectsNum = 0;
    std::cout << "Object Manager Constructor Called" << std::endl;


}

void ObjectManager::Init()
{

  
}

void ObjectManager::Update()
{
    glm::vec3 campos = GRAPHICS->camera.cam_position;

    for (std::unordered_map<unsigned, LightObject*>::iterator light = lightobjects.begin(); light != lightobjects.end(); light++)
    {
        if (!light->second->alive)
        {
            light_to_be_erased.push_back(light->first);
        }
    }
    for (std::unordered_map<unsigned, Object*>::iterator obj = objects.begin(); obj != objects.end(); obj++)
    {
        if (obj->second->alive)
        {
            obj->second->Update();
        }
        else
        {
            need_to_be_erased.push_back(obj->first);
        }
    }


    for (auto light : light_to_be_erased)
    {
        lightobjects.erase(light);
    }
    for (auto obj : need_to_be_erased)
    {
        Object* deletethis = objects.find(obj)->second;
        objects.erase(obj);
        delete deletethis;
    }
    need_to_be_erased.clear();
    light_to_be_erased.clear();



}

void ObjectManager::RegisterObject(Object* obj)
{
   
    genObjectsNum += 1;
    objects.insert(std::make_pair(genObjectsNum, obj));
    obj->id = genObjectsNum;
}

void ObjectManager::RegisterLight(LightObject* light)
{
    lightobjects.insert(std::make_pair(light->id, light));
}

void ObjectManager::DeleteAll()
{
    for (std::unordered_map<unsigned, Object*>::iterator obj = objects.begin(); obj != objects.end(); obj++)
    {
        need_to_be_erased.push_back(obj->first);
    }
    for (std::unordered_map<unsigned, LightObject*>::iterator light = lightobjects.begin(); light != lightobjects.end(); light++)
    {
        light_to_be_erased.push_back(light->first);
    }
    for (auto light : light_to_be_erased)
    {
        lightobjects.erase(light);
    }
    for (auto obj : need_to_be_erased)
    {
        Object* deletethis = objects.find(obj)->second;
        objects.erase(obj);
        delete deletethis;
    }
    objects.clear();
    lightobjects.clear();
    need_to_be_erased.clear();
    light_to_be_erased.clear();
}

void ObjectManager::DeleteAllLights()
{
    for (std::unordered_map<unsigned, LightObject*>::iterator light = lightobjects.begin(); light != lightobjects.end(); light++)
    {
        light_to_be_erased.push_back(light->first);
        need_to_be_erased.push_back(light->first);
    }
    for (auto light : light_to_be_erased)
    {
        lightobjects.erase(light);
    }
    for (auto obj : need_to_be_erased)
    {
        Object* deletethis = objects.find(obj)->second;
        objects.erase(obj);
        delete deletethis;
    }
    lightobjects.clear();
    light_to_be_erased.clear();
    need_to_be_erased.clear();
}

Object* ObjectManager::GetObject(std::string id)
{
    Object* result = nullptr;
    for (std::unordered_map<unsigned, Object*>::iterator obj = objects.begin(); obj != objects.end(); obj++)
    {
        if(obj->second->name==id)
        {
            result = obj->second;
        }
    }
    return result;
}

std::unordered_map<unsigned, Object*> ObjectManager::GetAllObjects()
{
    return  objects;
}

std::unordered_map<unsigned, LightObject*> ObjectManager::GetAllLights()
{
    return lightobjects;
}


ObjectManager::~ObjectManager()
{
    for (auto obj : objects)
    {
        delete obj.second;
    }

    std::cout << "ObjectManager Destructor Called" << std::endl;
}

