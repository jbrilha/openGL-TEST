#include "projectile.hpp"
#include "glm/ext/matrix_transform.hpp"

Projectile::Projectile(std::string vert_shader, std::string frag_shader,
                       glm::vec3 pos, glm::vec3 vel)
    : active(true), pos(pos),
      shader_program(new Shader(vert_shader.c_str(), frag_shader.c_str())),
      vel(vel), acc(0.f) {}

void Projectile::draw(glm::mat4 projection, glm::mat4 view) {
    if (!active)
        return;

    shader_program->use();
    shader_program->set_mat4("projection", projection);
    shader_program->set_mat4("view", view);
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, pos);
    shader_program->set_mat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size() * sizeof(int), GL_UNSIGNED_INT,
                   0);
    glBindVertexArray(0);
}

void Projectile::update(float d_time) {
    vel += acc * d_time;
    pos += vel * d_time;
}
