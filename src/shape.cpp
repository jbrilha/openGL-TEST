#include "shape.hpp"

Shape::Shape(std::string vert_shader, std::string frag_shader)
    : shader_program(new Shader(vert_shader.c_str(), frag_shader.c_str())) {}

void Shape::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model) {
    shader_program->use();
    shader_program->set_mat4("projection", projection);
    shader_program->set_mat4("view", view);
    shader_program->set_mat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size() * sizeof(int), GL_UNSIGNED_INT,
                   0);
    glBindVertexArray(0);
}
