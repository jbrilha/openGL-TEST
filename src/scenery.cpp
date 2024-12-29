#include "scenery.hpp"

Scenery::Scenery(std::string vert_shader, std::string frag_shader)
    : shader_program(new Shader(vert_shader.c_str(), frag_shader.c_str())) {}

void check_gl_errorf() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "Scenery OpenGL error: " << err << std::endl;
    }
}

void Scenery::draw(glm::mat4 projection, glm::mat4 view) {
    shader_program->use();
    shader_program->set_mat4("projection", projection);
    shader_program->set_mat4("view", view);
    glm::mat4 model = glm::mat4(1.f);
    shader_program->set_mat4("model", model);

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size() / 2, 40000);
    glBindVertexArray(0);
    check_gl_errorf();
}
