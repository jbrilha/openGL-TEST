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
enum class Stance {
    STAND,
    JUMP,
    FALL,
    CROUCH,
    PRONE,
    NONE, // initialization
};

const float YAW = -90.f;
const float PITCH = 0.f;
const float SPEED = 15.f;
const float SENS = 0.1f;
const float ZOOM = 45.f;

class Camera {
  public:
    Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
           glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW,
           float pitch = PITCH);
    Camera(float pos_x, float pos_y, float pos_z, float world_up_x,
           float world_up_y, float world_up_z, float yaw, float pitch);

    void handle_keyboard(camera_movement direction, float delta_time,
                         float time_var);
    void handle_mouse_move(float x_offset, float y_offset,
                           GLboolean constraint_pitch = true);
    void handle_mouse_scroll(float y_offset);

    void move();
    void set_stance(Stance stance);
    void toggle_stance(Stance stance);

    glm::mat4 get_view_matrix();

    glm::vec3 position;
    glm::vec3 front;

    float zoom;

    bool moving = false;
    bool sprinting = false;

  private:
    static constexpr float SPRINT_VIEWBOB_FREQ_MULTIPLIER = 1.1f;
    static constexpr float STAND_VIEWBOB_FREQ = 12.f;
    static constexpr float STAND_VIEWBOB_HEIGHT = 0.02f;
    static constexpr float CROUCH_VIEWBOB_FREQ = 10.f;
    static constexpr float CROUCH_VIEWBOB_HEIGHT = 0.05f;
    static constexpr float PRONE_VIEWBOB_FREQ = 5.f;
    static constexpr float PRONE_VIEWBOB_HEIGHT = 0.05f;

    static constexpr float JUMP_HEIGHT_MODIFIER = 5.f;
    static constexpr float CROUCH_HEIGHT_MODIFIER = 1.f;
    static constexpr float PRONE_HEIGHT_MODIFIER = 1.8f;

    static constexpr float SPRINT_SPEED_MULTIPLIER = 2.f;
    static constexpr float CROUCH_SPEED_MULTIPLIER = 1.f / 3.f;
    static constexpr float PRONE_SPEED_MULTIPLIER = 0.2f;

    static constexpr float floating_point_error_solver = 0.00001f;

    float default_eye_level;
    float eye_level = default_eye_level;
    float default_move_speed;
    float move_speed = default_move_speed;

    float max_jump_height = default_eye_level + JUMP_HEIGHT_MODIFIER;
    float crouch_height = default_eye_level - CROUCH_HEIGHT_MODIFIER;
    float prone_height = default_eye_level - PRONE_HEIGHT_MODIFIER;
    float crouch_speed = default_move_speed * CROUCH_SPEED_MULTIPLIER;
    float prone_speed = default_move_speed * PRONE_SPEED_MULTIPLIER;

    float bob_frequency;
    float bob_height;
    float bob_amount;

    float yaw;
    float pitch;
    float mouse_sens;

    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    Stance stance;
    Stance previous_stance;
    inline void make_idle();
    inline void update_stance();
    inline void update_camera_vectors();
    inline void update_stance_modifiers();
};

#endif // !CAMERA_H
