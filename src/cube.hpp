#ifndef CUBE_H
#define CUBE_H

#include "shaders.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Cube {
  public:
    Cube();
    ~Cube();
    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model);
    Shader shader_program;

  private:
    unsigned int VBO, VAO, EBO; // vertex buffer object, vertex array
                                // object, element buffer object

    std::vector<glm::vec3> vertices;
    std::vector<int> indices;

    void set_shaders();
    void set_vertices();
    void set_indices();
};
#endif
