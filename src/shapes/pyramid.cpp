#include "pyramid.hpp"

std::shared_ptr<Shader> Pyramid::shared_shader = nullptr;

std::unique_ptr<Pyramid> Pyramid::create_default(glm::mat4 &projection, bool &chase,
                                           glm::vec3 pos) {
    if (!shared_shader) {
        shared_shader = std::make_shared<Shader>(
            "shaders/lit_object_vert.glsl", "shaders/lit_object_frag.glsl");
    }

    return std::make_unique<Pyramid>(shared_shader, projection, chase, pos);
}

std::unique_ptr<Pyramid> Pyramid::create_shaderless(glm::mat4 &projection,
                                              bool &chase, glm::vec3 pos) {
    return std::make_unique<Pyramid>(projection, chase, pos);
}

std::unique_ptr<Pyramid> Pyramid::create_with_shader(std::shared_ptr<Shader> shader,
                                               glm::mat4 &projection,
                                               bool &chase, glm::vec3 pos) {
    return std::make_unique<Pyramid>(shader, projection, chase, pos);
}

std::unique_ptr<Pyramid> Pyramid::create_with_shader(std::string vert_shader,
                                               std::string frag_shader,
                                               glm::mat4 &projection,
                                               bool &chase, glm::vec3 pos) {
    std::shared_ptr<Shader> shader =
        std::make_shared<Shader>(vert_shader.c_str(), frag_shader.c_str());
    return std::make_unique<Pyramid>(shader, projection, chase, pos);
}

Pyramid::Pyramid(glm::mat4 &projection, bool &chase, glm::vec3 pos)
    : Shape(projection, pos, chase) {}

Pyramid::Pyramid(std::shared_ptr<Shader> shader, glm::mat4 &projection, bool &chase,
           glm::vec3 pos)
    : Shape(shader, projection, pos, chase) {
    set_shaders();
}

Pyramid::~Pyramid() {}

void Pyramid::set_indices() {
    indices = {0, 1, 2, 1, 3, 2, 0, 2, 4, 2, 3, 4, 3, 1, 4, 1, 0, 4};
}

void Pyramid::set_vertices() {
    vertices = {
        glm::vec3(0.5f, -0.5f, -0.5f),  // back bot right
        glm::vec3(-0.5f, -0.5f, -0.5f), // back bot left

        glm::vec3(0.5f, -0.5f, 0.5f),  // front bot right
        glm::vec3(-0.5f, -0.5f, 0.5f), // front bot left
        glm::vec3(0.f, 0.5f, 0.f),     // top
    };
}
