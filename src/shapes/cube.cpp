#include "cube.hpp"
#include <memory>

std::shared_ptr<Shader> Cube::shared_shader = nullptr;

std::unique_ptr<Cube> Cube::create_default(glm::mat4 &projection, bool &chase,
                                           glm::vec3 pos) {
    if (!shared_shader) {
        shared_shader = std::make_shared<Shader>(
            "shaders/lit_object_vert.glsl", "shaders/lit_object_frag.glsl");
    }

    return std::make_unique<Cube>(shared_shader, projection, chase, pos);
}

std::unique_ptr<Cube> Cube::create_shaderless(glm::mat4 &projection,
                                              bool &chase, glm::vec3 pos) {
    return std::make_unique<Cube>(projection, chase, pos);
}

std::unique_ptr<Cube> Cube::create_with_shader(std::shared_ptr<Shader> shader,
                                               glm::mat4 &projection,
                                               bool &chase, glm::vec3 pos) {
    return std::make_unique<Cube>(shader, projection, chase, pos);
}

std::unique_ptr<Cube> Cube::create_with_shader(std::string vert_shader,
                                               std::string frag_shader,
                                               glm::mat4 &projection,
                                               bool &chase, glm::vec3 pos) {
    std::shared_ptr<Shader> shader =
        std::make_shared<Shader>(vert_shader.c_str(), frag_shader.c_str());
    return std::make_unique<Cube>(shader, projection, chase, pos);
}

Cube::Cube(glm::mat4 &projection, bool &chase, glm::vec3 pos)
    : Shape(projection, pos, chase) {}

Cube::Cube(std::shared_ptr<Shader> shader, glm::mat4 &projection, bool &chase,
           glm::vec3 pos)
    : Shape(shader, projection, pos, chase) {
    set_shaders();
}

Cube::~Cube() {}

void Cube::set_indices() {
    indices = {
        0, 3, 7, 0, 7, 4, 1, 5, 6, 1, 6, 2, 3, 2, 6, 3, 6, 7,
        0, 4, 5, 0, 5, 1, 0, 1, 2, 0, 2, 3, 4, 7, 6, 4, 6, 5,
    };
}

void Cube::set_vertices() {
    vertices = {
        glm::vec3(0.5f, 0.5f, -0.5f),   // 0 - back top right
        glm::vec3(0.5f, -0.5f, -0.5f),  // 1 - back bot right
        glm::vec3(-0.5f, -0.5f, -0.5f), // 2 - back bot left
        glm::vec3(-0.5f, 0.5f, -0.5f),  // 3 - back top left

        glm::vec3(0.5f, 0.5f, 0.5f),   // 4 - front top right
        glm::vec3(0.5f, -0.5f, 0.5f),  // 5 - front bot right
        glm::vec3(-0.5f, -0.5f, 0.5f), // 6 - front bot left
        glm::vec3(-0.5f, 0.5f, 0.5f),  // 7 - front top left
    };
}
