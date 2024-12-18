#include "square.hpp"

Square::Square(Shader shader_program, float size) : shader_program(shader_program), size(size), color_set(false) {
    init();
}
Square::Square(Shader shader_program, float size, glm::vec3 color) : shader_program(shader_program), size(size), color_set(true), color(color) {
    init();
}

void Square::set_color(glm::vec3 color) {
    color_set = true;
    this->color = color;
}

void Square::draw(unsigned int primitive) {
    shader_program.use();
    if(color_set)
        shader_program.set_vec3("uColor", color);

    glBindVertexArray(VAO);
    glDrawElements(primitive, 6, GL_UNSIGNED_INT, 0);
}

void Square::init() {
    static const unsigned int indices[INDICES_NR] = {
        0, 1, 3,
        1, 2, 3
    };

    static const float vert = size / 2;
    static float vertices[8] = {
        vert,  vert,  // top right
        vert, -vert,  // bot right
       -vert, -vert,  // bot left
       -vert,  vert,  // top left
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

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
                          (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}
