#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "constants.hpp"
#include "shaders.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Projectile {
  public:
    Projectile(std::string vert_shader, std::string frag_shader, glm::vec3 pos,
               glm::vec3 vel);
    virtual ~Projectile() = default;
    void draw(glm::mat4 projection, glm::mat4 view);
    void update(float d_time);

    bool active;
    glm::vec3 pos;

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

    glm::vec3 vel;
    glm::vec3 acc;
};
#endif
