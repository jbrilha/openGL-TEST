#include "bullet.hpp"

Bullet::Bullet(glm::vec3 pos, glm::vec3 vel)
    : Projectile("shaders/cube_vert.glsl", "shaders/cube_frag.glsl", pos, vel) {
    set_shaders();
}

Bullet::~Bullet() { shader_program->del(); }

void Bullet::set_shaders() {
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

void Bullet::set_indices() {
    indices = {0, 1, 2, 1, 3, 2, 0, 2, 4, 2, 3, 4, 3, 1, 4, 1, 0, 4};
}

void Bullet::set_vertices() {
    vertices = {
        glm::vec3(0.1f, -0.1f, -0.1f),  // back bot right
        glm::vec3(-0.1f, -0.1f, -0.1f), // back bot left

        glm::vec3(0.1f, -0.1f, 0.1f),  // front bot right
        glm::vec3(-0.1f, -0.1f, 0.1f), // front bot left
        glm::vec3(0.f, 0.1f, 0.f),     // top
    };
}
