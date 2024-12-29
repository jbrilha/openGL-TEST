#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 world_up, float yaw, float pitch)
    : default_eye_level(position.y), front(glm::vec3(0.f, 0.f, -1.f)),
      default_move_speed(SPEED), mouse_sens(SENS), zoom(ZOOM) {

    this->position = position;
    this->world_up = world_up;

    this->yaw = yaw;
    this->pitch = pitch;
    update_camera_vectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float world_up_x,
               float world_up_y, float world_up_z, float yaw, float pitch)
    : default_eye_level(pos_y), front(glm::vec3(0.f, 0.f, -1.f)),
      default_move_speed(SPEED), mouse_sens(SENS), zoom(ZOOM) {

    this->position = glm::vec3(pos_x, pos_y, pos_z);
    this->world_up = glm::vec3(world_up_x, world_up_y, world_up_z);

    this->yaw = yaw;
    this->pitch = pitch;
    update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::move() {
    float bob_tolerance = 0.01f;
    if (!moving) {
        move_speed = 0;
        sprinting = false;
        if (position.y > eye_level) {
            position.y -= bob_tolerance;
        }
        if (position.y < eye_level) {
            position.y += bob_tolerance;
        }
        if (position.y > eye_level - bob_tolerance &&
            position.y < eye_level + bob_tolerance) {
            position.y = eye_level;
        }
    }

    if (jumping) {
        if (crouching)
            crouching = false;

        if (eye_level < max_jump_height) {
            eye_level += 0.20f;
            position.y = eye_level;
        }
        if (eye_level >= max_jump_height) {
            eye_level = max_jump_height; // needed so there's no twitching when
                                         // landing from crouch-jump
            jumping = false;
            falling = true;
        }
    } else {
        if (eye_level > default_eye_level + floating_point_error_solver) {
            eye_level -= 0.20f;
            position.y = eye_level;
        }
        if (eye_level <= default_eye_level + floating_point_error_solver) {
            falling = false;
        }
    }

    if (crouching) {
        if (!falling &&
            eye_level > crouch_height + floating_point_error_solver) {
            eye_level -= 0.1f;
            position.y = eye_level;
        }
        bob_frequency = 10.f;
        bob_height = 0.05f;

        if (sprinting) {
            move_speed = crouch_speed * SPRINT_SPEED_MULTIPLIER;
        } else {
            move_speed = crouch_speed;
        }
    } else {
        if (eye_level < default_eye_level) {
            eye_level += 0.1f;
            position.y = eye_level;
        }
        bob_height = 0.02f;

        if (sprinting) {
            bob_frequency = 15.f;
            move_speed = sprint_speed;
        } else {
            bob_frequency = 12.f;
            move_speed = default_move_speed;
        }
    }

    // std::cout << "\r"
    //           << " x: " << position.x << std::setprecision(4) << std::fixed
    //           << " y: " << position.y << std::setprecision(4) << std::fixed
    //           << " z: " << position.z << std::setprecision(4) << std::fixed
    //           << std::flush;
}

void Camera::handle_keyboard(camera_movement direction, float delta_time,
                             float time_var) {
    float velocity = move_speed * delta_time;

    // TODO: movement is stupid slow when looking directly up or down
    switch (direction) {
    case FORWARD:
        position += front * velocity;
        break;
    case BACKWARDS:
        position -= front * velocity;
        break;
    case RIGHT:
        position += right * velocity;
        break;
    case LEFT:
        position -= right * velocity;
        break;
    }

    bob_amount = sin(time_var * bob_frequency) * move_speed * bob_height;
    if (!jumping)
        position.y = eye_level + bob_amount;
    else
        position.y = eye_level;
}

void Camera::handle_mouse_move(float x_offset, float y_offset,
                               GLboolean constraint_pitch) {
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

void Camera::handle_mouse_scroll(float y_offset) {
    zoom -= (float)y_offset;

    if (zoom < 1.f)
        zoom = 1.f;
    if (zoom > 45.f)
        zoom = 45.f;

    std::cout << zoom << std::endl;
}

void Camera::update_camera_vectors() {
    glm::vec3 front;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);

    right = glm::normalize(glm::cross(this->front, world_up));
    up = glm::normalize(glm::cross(right, this->front));
}
