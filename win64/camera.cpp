#include "camera.h"


void setup_camera(Camera& camera, glm::vec3 position, glm::vec3 center, glm::vec3 up, float aspect, float fov, float z_near, float z_far) {
  camera.position = position;
  camera.front = glm::normalize(center - position);
  camera.up = glm::normalize(up);
  camera.right = glm::normalize(glm::cross(camera.front, camera.up)),
  camera.orientation = glm::quatLookAt(camera.front, camera.up);
  camera.orientation.x *= -1.0f;  // TODO: Why??
//  camera.view_mat = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
  camera.projection_mat = glm::perspective(glm::radians(fov), aspect, z_near, z_far);

  camera.transform.position = position;
  camera.transform.orientation = camera.orientation;
  camera.view_mat = glm::inverse(camera.transform.model_matrix());
}

void move_camera(Input& input, Camera& camera, float delta_time_s) {
  if (input.mouse_0) {
    const float sensitivity = 0.1f;

    float pitch = input.mouse_dy * sensitivity;
    float yaw = input.mouse_dx * sensitivity;
    glm::quat q_pitch = glm::angleAxis(glm::radians(pitch), X_AXIS);
    glm::quat q_yaw = glm::angleAxis(glm::radians(yaw), Y_AXIS);

    camera.orientation = glm::normalize(q_pitch * camera.orientation * q_yaw);
  }
  else {
    float pitch = input.pitch;
    float yaw = input.yaw;
    float roll = input.roll;

    if (camera.inverted_pitch)
      pitch *= -1.0f;

    glm::quat q_pitch = glm::angleAxis(glm::radians(pitch), X_AXIS);
    glm::quat q_yaw = glm::angleAxis(glm::radians(yaw), Y_AXIS);
    glm::quat q_roll = glm::angleAxis(glm::radians(roll), Z_AXIS);
    // TODO: Grddual roll correction? https://gamedev.stackexchange.com/questions/136174/im-rotating-an-object-on-two-axes-so-why-does-it-keep-twisting-around-the-thir
    camera.orientation = glm::normalize(q_pitch * q_yaw * q_roll) * camera.orientation;
  }

  glm::mat3 rotation = glm::toMat3(camera.orientation);
  camera.front = glm::normalize(-Z_AXIS * rotation);
  camera.right = glm::normalize(glm::cross(camera.front, WORLD_UP));
  camera.up = glm::normalize(glm::cross(camera.right, camera.front));

  float velocity = 5.0f * delta_time_s;
  if (input.up)
    camera.position += camera.front * velocity;
  if (input.down)
    camera.position -= camera.front * velocity;
  if (input.left)
    camera.position -= camera.right * velocity;
  if (input.right)
    camera.position += camera.right * velocity;

  camera.view_mat = glm::mat4_cast(camera.orientation) * glm::translate(glm::mat4(1.0f), -camera.position);

  ImGui::Begin("Camera");
  std::stringstream ss;
  ss << "x=" <<  camera.position.x  << " , "
     << "y=" <<  camera.position.y  << " , "
     << "z=" <<  camera.position.z;
  ImGui::LabelText("Position", ss.str().c_str());
  ImGui::End();

}
