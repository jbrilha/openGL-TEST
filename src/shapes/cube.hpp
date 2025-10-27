#ifndef CUBE_H
#define CUBE_H

#include "shaders.hpp"
#include "shape.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Cube : public Shape {
  public:
    Cube(glm::mat4 &projection, bool &chase, glm::vec3 pos);
    Cube(std::shared_ptr<Shader> shader, glm::mat4 &projection, bool &chase,
         glm::vec3 pos);
    Cube(std::string vert_shader, std::string frag_shader,
         glm::mat4 &projection, bool &chase, glm::vec3 pos);
    static std::unique_ptr<Cube> create_default(glm::mat4 &projection,
                                                bool &chase,
                                                glm::vec3 pos = glm::vec3(1.f));
    static std::unique_ptr<Cube>
    create_shaderless(glm::mat4 &projection, bool &chase,
                      glm::vec3 pos = glm::vec3(1.f));
    static std::unique_ptr<Cube>
    create_with_shader(std::string vert_shader, std::string frag_shader,
                       glm::mat4 &projection, bool &chase,
                       glm::vec3 pos = glm::vec3(1.f));
    static std::unique_ptr<Cube>
    create_with_shader(std::shared_ptr<Shader> shader, glm::mat4 &projection,
                       bool &chase, glm::vec3 pos = glm::vec3(1.f));
    ~Cube() override;

  private:
    void set_vertices() override;
    void set_indices() override;
    static std::shared_ptr<Shader> shared_shader;
};
#endif
