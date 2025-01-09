#include "shape.hpp"

Shape::Shape(std::string vert_shader, std::string frag_shader,
             glm::mat4 &projection, glm::vec3 pos, bool &chase)
    : chase(chase), projection(projection), pos(pos),
      shader_program(new Shader(vert_shader.c_str(), frag_shader.c_str())) {
    glm::mat4 m = glm::mat4(1.f);
    model = glm::translate(m, pos);
}

void Shape::draw(const glm::mat4 &view) {
    shader_program->use();
    shader_program->set_mat4("projection", this->projection);
    shader_program->set_mat4("view", view);
    shader_program->set_mat4("model", this->model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size() * sizeof(int), GL_UNSIGNED_INT,
                   0);
    glBindVertexArray(0);
}

void Shape::set_projection(glm::mat4 projection) {
    this->projection = projection;
}

void Shape::set_model(glm::mat4 model) { this->model = model; }

void Shape::set_pos(glm::vec3 new_pos) {
    this->pos = new_pos;
    this->model = glm::translate(glm::mat4(1.f), pos);
}

void Shape::look_at(glm::vec3 look_at_pos) {
    glm::vec3 direction = glm::normalize(look_at_pos - pos);

    // assuming y = 1.f is world up
    glm::vec3 right =
        glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), direction));
    glm::vec3 up = glm::cross(direction, right);

    if (chase) {
        pos += direction * 0.1f;
    }

    model = glm::mat4(glm::vec4(right, 0.0f), glm::vec4(up, 0.0f),
                      glm::vec4(direction, 0.0f), glm::vec4(pos, 1.0f));
}
