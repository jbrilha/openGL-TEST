#include "floor.hpp"

Floor::Floor(glm::mat4 projection)
    : Scenery("shaders/floor_vert.glsl", "shaders/floor_frag.glsl", projection),
      tile_count(TILE_COUNT) {
    set_shaders();
}

Floor::~Floor() { shader_program->del(); }

void Floor::set_shaders() {
    shader_program->use();
    std::vector<glm::mat4> offsets = std::vector<glm::mat4>(tile_count);
    glm::mat4 model;
    int c = 0;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            for (int n = 0, x = i, z = j; n < 4; n++) {
                model = glm::mat4(1.0f);

                if (n % 2 == 0) {
                    x *= -1;
                } else {
                    z *= -1;
                }

                model = glm::translate(model, glm::vec3(6.f * x, 0.f, 6.f * z));
                offsets[c++] = model;
            }
        }
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * tile_count,
                 offsets.data(), GL_STATIC_DRAW);

    set_vertices();

    unsigned int quad_VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &quad_VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3),
                          (void *)0);

    // colors defined here I think
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3),
                          (void *)sizeof(glm::vec3));

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // matrix column 0
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void *)0);
    // matrix column 1
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void *)(sizeof(glm::vec4)));
    // matrix column 2
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void *)(2 * sizeof(glm::vec4)));
    // matrix column 3
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (void *)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Floor::set_vertices() {
    vertices = {
        glm::vec3(3.f, 0.f, -3.f),  glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(-3.f, 0.f, -3.f), glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(3.f, 0.f, 3.f),   glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(-3.f, 0.f, 3.f),  glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(-3.f, 0.f, -3.f), glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(3.f, 0.f, 3.f),   glm::vec3(0.f, 0.f, 0.f),
    };
}
