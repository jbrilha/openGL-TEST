#ifndef SCENERY_H
#define SCENERY_H

#include "constants.hpp"
#include "shaders.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Scenery {
  public:
    Scenery(std::string vert_shader, std::string frag_shader);
    virtual ~Scenery() = default;
    void draw(glm::mat4 projection, glm::mat4 view);

  private:

  protected:
    virtual void set_shaders() = 0;
    virtual void set_vertices() = 0;
    Shader *shader_program;
    unsigned int VBO, VAO, EBO; // vertex buffer object, vertex array
                                // object, element buffer object
    std::vector<glm::vec3> vertices;
    std::vector<int> indices;
};
#endif
