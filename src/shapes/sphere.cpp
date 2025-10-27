#include "sphere.hpp"

std::shared_ptr<Shader> Sphere::shared_shader = nullptr;

std::unique_ptr<Sphere> Sphere::create_default(glm::mat4 &projection, bool &chase,
                                           glm::vec3 pos) {
    if (!shared_shader) {
        shared_shader = std::make_shared<Shader>(
            "shaders/lit_object_vert.glsl", "shaders/lit_object_frag.glsl");
    }

    return std::make_unique<Sphere>(shared_shader, projection, chase, pos);
}

std::unique_ptr<Sphere> Sphere::create_shaderless(glm::mat4 &projection,
                                              bool &chase, glm::vec3 pos) {
    return std::make_unique<Sphere>(projection, chase, pos);
}

std::unique_ptr<Sphere> Sphere::create_with_shader(std::shared_ptr<Shader> shader,
                                               glm::mat4 &projection,
                                               bool &chase, glm::vec3 pos) {
    return std::make_unique<Sphere>(shader, projection, chase, pos);
}

std::unique_ptr<Sphere> Sphere::create_with_shader(std::string vert_shader,
                                               std::string frag_shader,
                                               glm::mat4 &projection,
                                               bool &chase, glm::vec3 pos) {
    std::shared_ptr<Shader> shader =
        std::make_shared<Shader>(vert_shader.c_str(), frag_shader.c_str());
    return std::make_unique<Sphere>(shader, projection, chase, pos);
}

Sphere::Sphere(glm::mat4 &projection, bool &chase, glm::vec3 pos)
    : Shape(projection, pos, chase) {}

Sphere::Sphere(std::shared_ptr<Shader> shader, glm::mat4 &projection, bool &chase,
           glm::vec3 pos)
    : Shape(shader, projection, pos, chase) {
    set_shaders();
}

Sphere::~Sphere() {}

/* void Sphere::set_shaders() {
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
} */

void Sphere::set_indices() {
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

void Sphere::set_vertices() {
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
