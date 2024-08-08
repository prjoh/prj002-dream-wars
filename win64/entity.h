#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <algorithm>

#include <SDL.h>


#define MAX_COMPONENTS 16
#define MAX_ENTITIES 32


// TODO: https://gamedev.stackexchange.com/questions/191333/how-do-i-contiguously-store-and-iterate-over-ecs-components-in-c
struct Entity
{
    static const int TRANSFORM = 0;
    static const int RENDER = 1;
    static const int CAMERA = 2;
    static const int SCENE_LIGHTS = 3;
    static const int INPUT = 4;
    static const int MOVEMENT = 5;
    static const int PARTICLE_GENERATOR = 6;
    static const int VIEW_CONTROL = 7;
    static const int COLLIDER = 8;
    static const int AI = 9;
    static const int GUI = 10;
    static const int HEALTH = 11;
    static const int COLLISION_PLANE = 12;
    int id = -1;
    int components[MAX_COMPONENTS]{};
    bool active = false;

    Entity() {
        for (int i = 0; i < MAX_COMPONENTS; ++i)
          components[i] = -1;
    };
};

class EntityAllocator {
public:
    static int create_entity(std::string key);
    static int get_entity(std::string key);
    static Entity entities[MAX_ENTITIES];
private:
    static std::unordered_map<std::string, int> entity_keys;
    static int entity_counter;
};
