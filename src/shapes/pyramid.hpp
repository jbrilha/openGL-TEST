#ifndef PYRAMID_H
#define PYRAMID_H

#include "shaders.hpp"
#include "shape.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Pyramid : public Shape {
  public:
    Pyramid(glm::mat4 &projection, bool &chase, glm::vec3 pos);
    Pyramid(std::shared_ptr<Shader> shader, glm::mat4 &projection, bool &chase,
         glm::vec3 pos);
    Pyramid(std::string vert_shader, std::string frag_shader,
         glm::mat4 &projection, bool &chase, glm::vec3 pos);
    static std::unique_ptr<Pyramid> create_default(glm::mat4 &projection,
                                                bool &chase,
                                                glm::vec3 pos = glm::vec3(1.f));
    static std::unique_ptr<Pyramid>
    create_shaderless(glm::mat4 &projection, bool &chase,
                      glm::vec3 pos = glm::vec3(1.f));
    static std::unique_ptr<Pyramid>
    create_with_shader(std::string vert_shader, std::string frag_shader,
                       glm::mat4 &projection, bool &chase,
                       glm::vec3 pos = glm::vec3(1.f));
    static std::unique_ptr<Pyramid>
    create_with_shader(std::shared_ptr<Shader> shader, glm::mat4 &projection,
                       bool &chase, glm::vec3 pos = glm::vec3(1.f));
    ~Pyramid() override;

  private:
    // void set_shaders() override;
    void set_vertices() override;
    void set_indices() override;
    static std::shared_ptr<Shader> shared_shader;
};
#endif
