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
    Projectile(std::string vert_shader, std::string frag_shader,
               glm::mat4 projection, glm::vec3 pos, glm::vec3 vel);
    virtual ~Projectile() = default;
    void draw(const glm::mat4 &view);
    void update(float d_time);

    void set_pos(glm::vec3 pos);
    void set_model(glm::mat4 model);
    void set_projection(glm::mat4 projection);

    void init(glm::vec3 pos, glm::vec3 vel);

    bool active;

  private:
  protected:
    virtual void set_shaders() = 0;
    virtual void set_vertices() = 0;
    virtual void set_indices() = 0;

    glm::mat4 model;
    glm::mat4 projection;
    glm::vec3 pos;

    Shader *shader_program;
    unsigned int VBO, VAO, EBO; // vertex buffer object, vertex array
                                // object, element buffer object
    std::vector<glm::vec3> vertices;
    std::vector<int> indices;

    glm::vec3 vel;
    glm::vec3 acc;
    float lifetime;
};
#endif
