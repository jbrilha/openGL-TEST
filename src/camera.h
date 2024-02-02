#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <iomanip>

enum camera_movement { FORWARD, BACKWARDS, RIGHT, LEFT };

const float YAW = -90.f;
const float PITCH = 0.f;
const float SPEED = 15.f;
const float SENS = 0.1f;
const float ZOOM = 45.f;

class Camera {
  public:
    float floating_point_error_solver = 0.000001f;

    glm::vec3 position;

    bool moving;
    bool jumping;
    bool falling;
    bool crouching;
    bool proning;
    bool sprinting;

    float JUMP_HEIGHT_MODIFIER      = 4.f;
    float CROUCH_HEIGHT_MODIFIER    = 1.f;
    float PRONE_HEIGHT              = 1.f;

    float SPRINT_SPEED_MULTIPLIER     = 2.f;
    float CROUCH_SPEED_MULTIPLIER     = 1.f/3.f;
    float PRONE_SPEED_MULTIPLIER     = 0.2f;

    float default_eye_level;
    float eye_level;

    float bob_frequency = 15.f;
    float bob_height = 0.02f;
    float bob_amount = 0.f;
    float max_jump_height;
    float crouch_height;
    float prone_height = 1.f + floating_point_error_solver;

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    float yaw;
    float pitch;

    float default_move_speed;
    float move_speed;
    float sprint_speed;
    float crouch_speed;
    float prone_speed = 5.f;

    float mouse_sens;
    float zoom;

    Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
           glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW,
           float pitch = PITCH)
        : front(glm::vec3(0.f, 0.f, -1.f)), default_move_speed(SPEED), default_eye_level(position.y),
            mouse_sens(SENS), zoom(ZOOM) {

        this->position = position;
        this->world_up = world_up;

        this->eye_level         = default_eye_level;
        this->max_jump_height   = default_eye_level + JUMP_HEIGHT_MODIFIER;
        this->crouch_height     = default_eye_level - CROUCH_HEIGHT_MODIFIER + floating_point_error_solver;

        this->move_speed    = default_move_speed;
        this->sprint_speed  = default_move_speed * SPRINT_SPEED_MULTIPLIER;
        this->crouch_speed  = default_move_speed * CROUCH_SPEED_MULTIPLIER;

        this->yaw = yaw;
        this->pitch = pitch;
        update_camera_vectors();
    }

    Camera(float pos_x, float pos_y, float pos_z, float world_up_x,
           float world_up_y, float world_up_z, float yaw, float pitch)
        : front(glm::vec3(0.f, 0.f, -1.f)), default_move_speed(SPEED), default_eye_level(pos_y),
            mouse_sens(SENS), zoom(ZOOM) {

        this->position = glm::vec3(pos_x, pos_y, pos_z);
        this->world_up = glm::vec3(world_up_x, world_up_y, world_up_z);

        this->eye_level         = default_eye_level;
        this->max_jump_height   = default_eye_level + JUMP_HEIGHT_MODIFIER;
        this->crouch_height     = default_eye_level - CROUCH_HEIGHT_MODIFIER + floating_point_error_solver;

        this->move_speed    = default_move_speed;
        this->sprint_speed  = default_move_speed * SPRINT_SPEED_MULTIPLIER;
        this->crouch_speed  = default_move_speed * CROUCH_SPEED_MULTIPLIER;

        this->yaw = yaw;
        this->pitch = pitch;
        update_camera_vectors();
    }

    glm::mat4 get_view_matrix() {
        return glm::lookAt(position, position + front, up);
    }

    void move() {
        float bob_tolerance = 0.02f;
        if(!moving) { 
            move_speed = 0;
            sprinting = false;
            if(position.y > eye_level) {
                position.y -= bob_tolerance;
            }
            if (position.y < eye_level){
                position.y += bob_tolerance;
            }
            // if(position.y < eye_level + bob_tolerance && position.y > eye_level - bob_tolerance)
            //     position.y = eye_level;
        }

        if(jumping) {
            if(crouching) crouching = false;

            if(eye_level < max_jump_height) {
                eye_level += 0.25f;
                position.y = eye_level;
            }
            if(eye_level >= max_jump_height) {
                eye_level = max_jump_height; //needed so there's no twitching when landing from crouch-jump
                jumping = false;
                falling = true;
            }
        }
        else {
            if(eye_level > default_eye_level) {
                eye_level -= 0.25f;
                position.y = eye_level;
            }
            if(eye_level == default_eye_level) {
                falling = false;
            }
        }

        // if(proning) {
        //     crouching = false;
        //     if(eye_level > prone_height) {
        //         eye_level -= 0.1f;
        //         position.y = eye_level;
        //     }
        //     move_speed = prone_speed;
        // }
        // else {
        //     if(eye_level < default_eye_level) {
        //         eye_level += 0.1f;
        //         position.y = eye_level;
        //     }
        //     move_speed = default_move_speed;
        // }

        if(crouching) {
            if(!falling && eye_level > crouch_height) {
                eye_level -= 0.1f;
                position.y = eye_level;
            }
            bob_frequency = 10.f;
            bob_height = 0.05f;

            if(sprinting) {
                move_speed = crouch_speed * SPRINT_SPEED_MULTIPLIER;
            }
            else {
                move_speed = crouch_speed;
            }
        }
        else {
            if(!proning && eye_level < default_eye_level) {
                    eye_level += 0.1f;
                    position.y = eye_level;
            }
            bob_height = 0.02f;

            if(sprinting) {
                bob_frequency = 15.f;
                move_speed = sprint_speed;
            }
            else {
                bob_frequency = 12.f;
                move_speed = default_move_speed;
            }
        }

        std::cout << "\r"
            << " x: " << position.x << std::setprecision(4) << std::fixed 
            << " y: " << position.y << std::setprecision(4) << std::fixed
            << " z: " << position.z << std::setprecision(4) << std::fixed << std::flush;
    }

    // void view_bob() {
    //     if(bob_up) {
    //         eye_level += 0.01f;
    //         bob_up = false;
    //     }
    //     else {
    //         eye_level -= 0.01f;
    //         bob_up = true;
    //     }
    //
    // }

    void handle_keyboard(camera_movement direction, float delta_time, float time_var) {
        float velocity = move_speed * delta_time;

        // TODO: movement is stupid slow when looking directly up or down
        switch (direction) {
            case FORWARD:   position += front * velocity; break;
            case BACKWARDS: position -= front * velocity; break;
            case RIGHT:     position += right * velocity; break;
            case LEFT:      position -= right * velocity; break;
            }

        bob_amount = sin(time_var * bob_frequency) * move_speed * bob_height;
        position.y = eye_level + bob_amount;
    }

    void handle_mouse_move(float x_offset, float y_offset,
                           GLboolean constraint_pitch = true) {
        x_offset *= mouse_sens;
        y_offset *= mouse_sens;

        yaw += x_offset;
        pitch += y_offset;

        if (constraint_pitch) {
            if (pitch > 89.f)
                pitch = 89.f;
            if (pitch < -89.f)
                pitch = -89.f;
        }

        update_camera_vectors();
    }

    void handle_mouse_scroll(float y_offset) {
        zoom -= (float)y_offset;

        if (zoom < 1.f)
            zoom = 1.f;
        if (zoom > 45.f)
            zoom = 45.f;
    }

  private:
    void update_camera_vectors() {
        glm::vec3 front;

        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);

        right = glm::normalize(glm::cross(this->front, world_up));
        up = glm::normalize(glm::cross(right, this->front));
    }
};

#endif // !CAMERA_H
