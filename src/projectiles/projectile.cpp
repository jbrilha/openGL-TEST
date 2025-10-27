#include "projectile.hpp"
#include "glm/ext/matrix_transform.hpp"

Projectile::Projectile(std::string vert_shader, std::string frag_shader,
                       glm::mat4 &projection, glm::vec3 pos, glm::vec3 vel)
    : active(false), model(1.f), projection(projection), pos(pos),
      shader_program(new Shader(vert_shader.c_str(), frag_shader.c_str())),
      vel(vel), acc(glm::vec3(0.f, -9.81f, 0.f)), lifetime(P_LIFETIME) {}

void Projectile::init(glm::vec3 pos, glm::vec3 vel) {
    this->pos = pos;
    this->vel = vel;
    lifetime = P_LIFETIME;
    active = true;
}

void Projectile::draw(const glm::mat4 &view) {
    if (!active)
        return;

    shader_program->use();
    shader_program->set_mat4("projection", projection);
    shader_program->set_mat4("view", view);
    shader_program->set_mat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size() * sizeof(int), GL_UNSIGNED_INT,
                   0);
    glBindVertexArray(0);
}

void Projectile::draw_illuminated(const glm::mat4 &view, const glm::vec3 camera_pos,const glm::vec3 light_pos) {
    shader_program->use();
    shader_program->set_vec3("object_color", 1.0f, 0.5f, 0.31f);
    shader_program->set_vec3("light_color", 1.0f, 1.0f, 1.0f);
    shader_program->set_vec3("light_pos", light_pos);
    shader_program->set_vec3("view_pos", camera_pos);
    shader_program->set_mat4("projection", this->projection);
    shader_program->set_mat4("view", view);
    shader_program->set_mat4("model", this->model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size() * sizeof(int), GL_UNSIGNED_INT,
                   0);
    glBindVertexArray(0);
}

void Projectile::set_projection(glm::mat4 projection) {
    this->projection = projection;
}

void Projectile::set_model(glm::mat4 model) { this->model = model; }

void Projectile::set_pos(glm::vec3 new_pos) {
    pos = new_pos;
    model = glm::translate(glm::mat4(1.f), pos);
}

void Projectile::update(float d_time) {
    if (!active)
        return;

    vel += acc * d_time;
    pos += vel * d_time;

    if (pos.y <= 0.1f) { // bounce
        pos.y = 0.1f;
        vel.y *= -0.7f;

        if (vel.x != 0.f && vel.z != 0.f) { // friction on bounce
            float friction = 1.0f - (0.7f * d_time);
            vel.x *= friction;
            vel.z *= friction;

            if (abs(vel.x) <= 0.2f && abs(vel.z) <= 0.2f) {
                float h_vel = std::sqrt(vel.x * vel.x + vel.z * vel.z);
                if (h_vel <= 0.2f) { // abrupt-ish stop
                    vel = glm::vec3(0.f);
                }
            }
        }
    }
    model = glm::translate(glm::mat4(1.f), pos);
    lifetime -= d_time;

    if (lifetime <= 0) {
        active = false;
    }
}
