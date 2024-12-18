#include "rect.hpp"

Rect::Rect(Shader shader_program, float width, float height) : shader_program(shader_program), width(width), height(height), color_set(false) {
    init();
}
Rect::Rect(Shader shader_program, float width, float height, glm::vec3 color) : shader_program(shader_program), width(width), height(height), color_set(true), color(color) {
    init();
}

void Rect::set_color(glm::vec3 color) {
    color_set = true;
    this->color = color;
}

void Rect::draw(unsigned int primitive) {
    shader_program.use();
    if(color_set)
        shader_program.set_vec3("uColor", color);

    glBindVertexArray(VAO);
    glDrawElements(primitive, 6, GL_UNSIGNED_INT, 0);
}

void Rect::init() {
    static const unsigned int indices[INDICES_NR] = {
        0, 1, 3,
        1, 2, 3
    };

    static const float vert_width = width / 2;
    static const float vert_height = height / 2;
    static float vertices[12] = {
        vert_width,  vert_height, 0.f,  // top right
        vert_width, -vert_height, 0.f,  // bot right
       -vert_width, -vert_height, 0.f,  // bot left
       -vert_width,  vert_height, 0.f,  // top left
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
                          (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}
