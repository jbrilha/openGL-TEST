#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iomanip>
#include <iostream>

enum camera_movement {
    FORWARD,
    BACKWARDS,
    RIGHT,
    LEFT,
};
enum movement_type { JUMP, CROUCH, SPRINT, WALK };

const float YAW = -90.f;
const float PITCH = 0.f;
const float SPEED = 15.f;
const float SENS = 0.1f;
const float ZOOM = 45.f;

class Camera {
  public:
    float floating_point_error_solver = 0.00001f;

    glm::vec3 position;

    bool moving = false;
    bool jumping = false;
    bool falling = false;
    bool crouching = false;
    bool proning = false;
    bool sprinting = false;

    float JUMP_HEIGHT_MODIFIER = 5.f;
    float CROUCH_HEIGHT_MODIFIER = 1.f;
    // float PRONE_HEIGHT              = 1.f;

    float SPRINT_SPEED_MULTIPLIER = 2.f;
    float CROUCH_SPEED_MULTIPLIER = 1.f / 3.f;
    // float PRONE_SPEED_MULTIPLIER     = 0.2f;

    float default_eye_level;
    float eye_level = default_eye_level;

    float bob_frequency = 15.f;
    float bob_height = 0.02f;
    float bob_amount = 0.f;
    float max_jump_height = default_eye_level + JUMP_HEIGHT_MODIFIER;
    float crouch_height = default_eye_level - CROUCH_HEIGHT_MODIFIER;
    ;
    // float prone_height = 1.f + floating_point_error_solver;

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    float yaw;
    float pitch;

    float default_move_speed;
    float move_speed = default_move_speed;
    float sprint_speed = default_move_speed * SPRINT_SPEED_MULTIPLIER;
    float crouch_speed = default_move_speed * CROUCH_SPEED_MULTIPLIER;

    float mouse_sens;
    float zoom;

    Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
           glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW,
           float pitch = PITCH);

    Camera(float pos_x, float pos_y, float pos_z, float world_up_x,
           float world_up_y, float world_up_z, float yaw, float pitch);

    glm::mat4 get_view_matrix();

    void move();

    void handle_keyboard(camera_movement direction, float delta_time,
                         float time_var) ;

    void handle_mouse_move(float x_offset, float y_offset,
                           GLboolean constraint_pitch = true) ;

    void handle_mouse_scroll(float y_offset) ;

  private:
    void update_camera_vectors() ;
};

#endif // !CAMERA_H
