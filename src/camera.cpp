#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 world_up, float yaw, float pitch)
    : front(glm::vec3(0.f, 0.f, -1.f)), default_eye_level(position.y),
      default_move_speed(SPEED), bob_frequency(15.f), bob_height(0.02f),
      bob_amount(0.f), mouse_sens(SENS), zoom(ZOOM), stance(Stance::STAND) {

    this->position = position;
    this->world_up = world_up;

    this->yaw = yaw;
    this->pitch = pitch;
    update_camera_vectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float world_up_x,
               float world_up_y, float world_up_z, float yaw, float pitch)
    : front(glm::vec3(0.f, 0.f, -1.f)), default_eye_level(position.y),
      default_move_speed(SPEED), bob_frequency(15.f), bob_height(0.02f),
      bob_amount(0.f), mouse_sens(SENS), zoom(ZOOM), stance(Stance::STAND) {

    this->position = glm::vec3(pos_x, pos_y, pos_z);
    this->world_up = glm::vec3(world_up_x, world_up_y, world_up_z);

    this->yaw = yaw;
    this->pitch = pitch;
    update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::make_idle() {
    move_speed = 0;
    sprinting = false;

    float bob_tolerance = 0.01f;
    if (position.y > eye_level) {
        position.y -= bob_tolerance;
    }
    if (position.y < eye_level) {
        position.y += bob_tolerance;
    }
    if (std::abs(position.y - eye_level) < bob_tolerance) {
        position.y = eye_level;
    }
}

void Camera::set_stance(Stance stance) {
    previous_stance =
        this->stance != Stance::JUMP ? this->stance : Stance::STAND;
    this->stance = stance;
}

void Camera::toggle_stance(Stance toggle_stance) {
    if (stance != Stance::JUMP && stance != Stance::FALL &&
        stance != toggle_stance) {
        stance = toggle_stance;
    } else {
        stance = Stance::STAND;
    }
}

void Camera::update_stance() {
    switch (stance) {
    case Stance::STAND:
        if (eye_level < default_eye_level) {
            eye_level += 0.1f;
            position.y = eye_level;
        }
        break;
    case Stance::JUMP:
        if (eye_level < max_jump_height) {
            eye_level += 0.20f;
            position.y = eye_level;
        } else {
            eye_level = max_jump_height;
            stance = Stance::FALL;
        }
        break;
    case Stance::FALL:
        if (eye_level > default_eye_level + floating_point_error_solver) {
            eye_level -= 0.20f;
            position.y = eye_level;
        } else {
            stance = previous_stance;
        }
        break;
    case Stance::CROUCH:
        if (eye_level > crouch_height + floating_point_error_solver) {
            eye_level -= 0.1f;
            position.y = eye_level;
        } else if (eye_level < crouch_height) { // get up from prone
            eye_level += 0.1f;
            position.y = eye_level;
        }
        break;
    case Stance::PRONE:
        if (eye_level > prone_height + floating_point_error_solver) {
            eye_level -= 0.1f;
            position.y = eye_level;
        }
        break;
    default:
        break;
    }
}

void Camera::update_stance_modifiers() {

    switch (stance) {
    case Stance::CROUCH:
        bob_frequency = CROUCH_VIEWBOB_FREQ;
        bob_height = CROUCH_VIEWBOB_HEIGHT;
        move_speed =
            sprinting ? crouch_speed * SPRINT_SPEED_MULTIPLIER : crouch_speed;
        break;
    case Stance::PRONE:
        bob_frequency = PRONE_VIEWBOB_FREQ;
        bob_height = PRONE_VIEWBOB_HEIGHT;
        move_speed = prone_speed;
        break;
    default:
        bob_height = STAND_VIEWBOB_HEIGHT;
        if (sprinting) {
            bob_frequency = SPRINT_VIEWBOB_FREQ;
            move_speed = default_move_speed * SPRINT_SPEED_MULTIPLIER;
        } else {
            bob_frequency = STAND_VIEWBOB_FREQ;
            move_speed = default_move_speed;
        }
        break;
    }
}

void Camera::move() {
    if (!moving) {
        make_idle();
    }

    update_stance();
    update_stance_modifiers();

    // std::cout << "\r"
    //           << " x: " << position.x << std::setprecision(4) << std::fixed
    //           << " y: " << position.y << std::setprecision(4) << std::fixed
    //           << " z: " << position.z << std::setprecision(4) << std::fixed
    //           << std::flush;
}

void Camera::handle_keyboard(camera_movement direction, float delta_time,
                             float time_var) {
    float velocity = move_speed * delta_time;

    // FIXED: movement is stupid slow when looking directly up or down
    glm::vec3 h_front = glm::normalize(glm::vec3(front.x, 0.0f, front.z));

    switch (direction) {
    case FORWARD:
        position += h_front * velocity;
        break;
    case BACKWARDS:
        position -= h_front * velocity;
        break;
    case RIGHT:
        position += right * velocity;
        break;
    case LEFT:
        position -= right * velocity;
        break;
    }

    bob_amount = sin(time_var * bob_frequency) * move_speed * bob_height;
    if (stance != Stance::JUMP) {
        position.y = eye_level + bob_amount;
    } else {
        position.y = eye_level;
    }
    moving = true;
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
