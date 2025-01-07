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
    Cube(glm::mat4 projection, glm::vec3 pos = glm::vec3(1.f));
    ~Cube() override;

  private:
    void set_shaders() override;
    void set_vertices() override;
    void set_indices() override;
};
#endif
