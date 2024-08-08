#pragma once

#include "transform.h"
#include "mesh.h"
#include "camera.h"
#include "lights.h"
#include "input.h"
#include "player.h"
#include "particle.h"
#include "collision.h"
#include "ai.h"
#include "gui.h"
#include "health.h"


#define MAX_TRANSFORM_COMPONENT_COUNT 32
#define MAX_RENDER_COMPONENT_COUNT 32
#define MAX_CAMERA_COMPONENT_COUNT 1
#define MAX_SCENE_LIGHTS_COMPONENT_COUNT 1
#define MAX_INPUT_COMPONENT_COUNT 1
#define MAX_PLAYER_MOVEMENT_COUNT 1
#define MAX_PARTICLE_GENERATOR_COUNT 16
#define MAX_VIEW_CONTROL_COUNT 1
#define MAX_COLLIDER_COUNT 32
#define MAX_AI_COUNT 32
#define MAX_GUI_COMPONENT_COUNT 8
#define MAX_HEALTH_COUNT 16
#define MAX_COLLISION_PLANE_COUNT 32


class ComponentAllocator
{
public:
    static TransformComponent transform_components[MAX_TRANSFORM_COMPONENT_COUNT];
    static RenderComponent render_components[MAX_RENDER_COMPONENT_COUNT];
    static Camera camera_components[MAX_CAMERA_COMPONENT_COUNT];
    static SceneLights scene_lights_components[MAX_SCENE_LIGHTS_COMPONENT_COUNT];
    static Input input_components[MAX_INPUT_COMPONENT_COUNT];
    static PlayerMovement player_movement_components[MAX_PLAYER_MOVEMENT_COUNT];
    static ParticleGenerator particle_generators[MAX_PARTICLE_GENERATOR_COUNT];
    static ViewControl view_controls[MAX_VIEW_CONTROL_COUNT];
    static BoundingBox bounding_boxes[MAX_COLLIDER_COUNT];
    static AIComponent ais[MAX_AI_COUNT];
    static GUIComponent gui_components[MAX_GUI_COMPONENT_COUNT];
    static HealthComponent health_components[MAX_HEALTH_COUNT];
    static CollisionPlane collision_planes[MAX_COLLISION_PLANE_COUNT];

    static int create_transform_component();
    static int create_render_component();
    static int create_camera();
    static int create_scene_lights();
    static int create_input();
    static int create_player_movement();
    static int create_particle_generator();
    static int create_view_control();
    static int create_bounding_box();
    static int create_ai();
    static int create_gui_component();
    static int create_health_component();
    static int create_collision_plane();
private:
    static int transform_components_index;
    static int render_components_index;
    static int camera_components_index;
    static int scene_lights_components_index;
    static int input_components_index;
    static int player_movement_components_index;
    static int particle_generators_index;
    static int view_controls_index;
    static int bounding_boxes_index;
    static int ais_index;
    static int gui_components_index;
    static int health_components_index;
    static int collision_planes_index;
};
