#pragma once

#include <sstream>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "input.h"
#include "common.h"
#include "transform.h"


struct Camera {
    glm::vec3 position;
    glm::quat orientation = {1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 front;  // Position where the camera is looking at
    glm::vec3 up;  // Normalized up vector, how the camera is oriented
    glm::vec3 right;  // Normalized right vector, derived from up and front
    glm::mat4 view_mat;
    glm::mat4 projection_mat;
    bool inverted_pitch = true;

    Transform transform;  // TODO: Make this an index??
};

void setup_camera(Camera& camera,
                  glm::vec3 position,
                  glm::vec3 center = {0.0f, 0.0f, 0.0f},
                  glm::vec3 up = WORLD_UP,
                  float aspect = 16.0f / 9.0f,
                  float fov = 45.0f,
                  float z_near = 0.01f,
                  float z_far = 1000.0f);
void move_camera(Input& input, Camera& camera, float delta_time_s);
