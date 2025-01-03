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
    Sphere(glm::vec3 pos = glm::vec3(1.f));
    ~Sphere() override;

  private:
    void set_shaders() override;
    void set_vertices() override;
    void set_indices() override;
    
    int sectors = 36;
    int stacks = 18;
    float radius = 1.f;
};
#endif
