#include "bullet.hpp"

Bullet::Bullet(glm::mat4 projection, glm::vec3 pos, glm::vec3 vel)
    : Projectile("shaders/cube_vert.glsl", "shaders/cube_frag.glsl", projection,
                 pos, vel) {
    set_shaders();
}

Bullet::Bullet(glm::mat4 projection)
    : Projectile("shaders/cube_vert.glsl", "shaders/cube_frag.glsl", projection,
                 glm::vec3(0.f), glm::vec3(0.f)) {
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
    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void Bullet::set_vertices() {
    float sector_step = 2 * M_PI / sectors;
    float stack_step = M_PI / stacks;

    for (int i = 0; i <= stacks; ++i) {
        float phi = M_PI / 2 - i * stack_step;
        float xy = radius * cosf(phi);
        float z = radius * sinf(phi);

        for (int j = 0; j <= sectors; ++j) {
            float theta = j * sector_step;

            float x = xy * cosf(theta);
            float y = xy * sinf(theta);
            vertices.push_back(glm::vec3(x, y, z));
        }
    }
}
