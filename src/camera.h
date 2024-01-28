#ifndef CAMERA_H
#define CAMERA_H

#include <algorithm>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

enum camera_movement { FORWARD, BACKWARDS, RIGHT, LEFT };

const float YAW = -90.f;
const float PITCH = 0.f;
const float SPEED = 15.f;
const float SENS = 0.1f;
const float ZOOM = 45.f;

class Camera {
  public:
    glm::vec3 position;

    float JUMP_HEIGHT_MODIFIER      = 4.f;
    float CROUCH_HEIGHT_MODIFIER    = 1.f;
    float SPRINT_SPEED_MODIFIER     = 2.5f;
    float CROUCH_SPEED_MODIFIER     = 0.5f;


    float default_eye_level;
    float eye_level;
    float max_jump_height;
    float crouch_height;

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
        this->crouch_height     = default_eye_level - CROUCH_HEIGHT_MODIFIER;

        this->move_speed    = default_move_speed;
        this->sprint_speed  = default_move_speed * SPRINT_SPEED_MODIFIER;
        this->crouch_speed  = default_move_speed * CROUCH_SPEED_MODIFIER;

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
        this->crouch_height     = default_eye_level - CROUCH_HEIGHT_MODIFIER;

        this->move_speed    = default_move_speed;
        this->sprint_speed  = default_move_speed * SPRINT_SPEED_MODIFIER;
        this->crouch_speed  = default_move_speed * CROUCH_SPEED_MODIFIER;

        this->yaw = yaw;
        this->pitch = pitch;
        update_camera_vectors();
    }

    glm::mat4 get_view_matrix() {
        return glm::lookAt(position, position + front, up);
    }

    void jump(bool jumping) {
        if(jumping) {
            if(position.y < max_jump_height) {
                eye_level += 0.25f;
                position.y = eye_level;
            }
        }
        else {
            if(position.y > default_eye_level) {
                eye_level -= 0.25f;
                position.y = eye_level;
            }
        }
    }
    void crouch(bool crouching) {
        if(crouching) {
            if(position.y > crouch_height) {
                    eye_level -= 0.1f;
                    position.y = eye_level;
            }
            move_speed = crouch_speed;
        }
        else {
            if(position.y < default_eye_level) {
                    eye_level += 0.1f;
                    position.y = eye_level;
            }
            move_speed = default_move_speed;
        }
    }
    void sprint(bool sprinting) {
        if(sprinting) {
            move_speed = sprint_speed;
        }
        else {
            move_speed = default_move_speed;
        }
    }

    void handle_keyboard(camera_movement direction, float delta_time) {
        float velocity = move_speed * delta_time;

        // TODO: movement is stupid slow when looking directly up or down
        switch (direction) {
            case FORWARD:   position += front * velocity; break;
            case BACKWARDS: position -= front * velocity; break;
            case RIGHT:     position += right * velocity; break;
            case LEFT:      position -= right * velocity; break;
            }
        position.y = eye_level;
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
