#ifndef SPHERE_H
#define SPHERE_H

#include "shaders.hpp"
#include "shape.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Sphere : public Shape {
  public:
    Sphere(glm::mat4 &projection, bool &chase, glm::vec3 pos);
    Sphere(std::shared_ptr<Shader> shader, glm::mat4 &projection, bool &chase,
         glm::vec3 pos);
    Sphere(std::string vert_shader, std::string frag_shader,
         glm::mat4 &projection, bool &chase, glm::vec3 pos);
    static std::unique_ptr<Sphere> create_default(glm::mat4 &projection,
                                                bool &chase,
                                                glm::vec3 pos = glm::vec3(1.f));
    static std::unique_ptr<Sphere>
    create_shaderless(glm::mat4 &projection, bool &chase,
                      glm::vec3 pos = glm::vec3(1.f));
    static std::unique_ptr<Sphere>
    create_with_shader(std::string vert_shader, std::string frag_shader,
                       glm::mat4 &projection, bool &chase,
                       glm::vec3 pos = glm::vec3(1.f));
    static std::unique_ptr<Sphere>
    create_with_shader(std::shared_ptr<Shader> shader, glm::mat4 &projection,
                       bool &chase, glm::vec3 pos = glm::vec3(1.f));
    ~Sphere() override;

  private:
    // void set_shaders() override;
    void set_vertices() override;
    void set_indices() override;

    int sectors = 36;
    int stacks = 18;
    float radius = 0.5f;

    static std::shared_ptr<Shader> shared_shader;
};
#endif
