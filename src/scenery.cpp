#include "scenery.hpp"

Scenery::Scenery(std::string vert_shader, std::string frag_shader,
                 glm::mat4 projection)
    : projection(projection), model(glm::mat4(1.f)),
      shader_program(new Shader(vert_shader.c_str(), frag_shader.c_str())) {}

void check_gl_errorf() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "Scenery OpenGL error: " << err << std::endl;
    }
}

void Scenery::draw(const glm::mat4 &view) {
    shader_program->use();
    shader_program->set_mat4("projection", projection);
    shader_program->set_mat4("view", view);
    shader_program->set_mat4("model", model);

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size() / 2, 40000);
    glBindVertexArray(0);
    check_gl_errorf();
}

void Scenery::set_projection(glm::mat4 projection) {
    this->projection = projection;
}

void Scenery::set_model(glm::mat4 model) { this->model = model; }
