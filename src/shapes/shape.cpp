#include "shape.hpp"
#include "shaders.hpp"

/* Shape::Shape(std::string vert_shader, std::string frag_shader,
             glm::mat4 &projection, glm::vec3 pos, bool &chase)
    : chase(chase), projection(projection), pos(pos),
      shader_program(new Shader(vert_shader.c_str(), frag_shader.c_str())) {
    glm::mat4 m = glm::mat4(1.f);
    model = glm::translate(m, pos);
} */

Shape::Shape(std::shared_ptr<Shader> shader, glm::mat4 &projection,
             glm::vec3 pos, bool &chase)
    : chase(chase), projection(projection), pos(pos), shader_program(shader) {
    glm::mat4 m = glm::mat4(1.f);
    model = glm::translate(m, pos);
}

Shape::Shape(glm::mat4 &projection, glm::vec3 pos, bool &chase)
    : chase(chase), projection(projection), pos(pos) {
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

void Shape::draw_illuminated(const glm::mat4 &view, const glm::vec3 camera_pos,
                             const glm::vec3 light_pos) {
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

void Shape::draw_illuminating(const glm::mat4 &view,
                              const glm::vec3 light_pos) {
    shader_program->use();
    shader_program->set_mat4("projection", this->projection);
    shader_program->set_mat4("view", view);

    this->model = glm::translate(glm::mat4(1.0f), light_pos);
    shader_program->set_mat4("model", this->model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size() * sizeof(int), GL_UNSIGNED_INT,
                   0);
    glBindVertexArray(0);
}

void Shape::set_shaders() {
    shader_program->use();
    set_indices();
    set_vertices();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int),
                 indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Shape::set_shader_program(std::shared_ptr<Shader> new_shader) {
    shader_program.reset();
    shader_program = std::move(new_shader);
}

void Shape::set_shader_program(std::string vert_shader,
                               std::string frag_shader) {
    shader_program.reset();
    shader_program =
        std::make_shared<Shader>(vert_shader.c_str(), frag_shader.c_str());
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
