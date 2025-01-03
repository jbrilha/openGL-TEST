#ifndef SHAPE_H
#define SHAPE_H

#include "constants.hpp"
#include "shaders.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Shape {
  public:
    Shape(std::string vert_shader, std::string frag_shader, glm::vec3 pos);
    virtual ~Shape() = default;
    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model);
    void update_pos(glm::vec3 pos);
    void look_at(glm::vec3 pos);
    bool active;
    glm::vec3 pos;
    glm::mat4 model;

  private:
  protected:
    virtual void set_shaders() = 0;
    virtual void set_vertices() = 0;
    virtual void set_indices() = 0;
    Shader *shader_program;
    unsigned int VBO, VAO, EBO; // vertex buffer object, vertex array
                                // object, element buffer object
    std::vector<glm::vec3> vertices;
    std::vector<int> indices;
};
#endif
