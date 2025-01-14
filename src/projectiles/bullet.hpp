#ifndef BULLET_H
#define BULLET_H

#include "shaders.hpp"
#include "projectile.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Bullet : public Projectile {
  public:
    Bullet(glm::mat4 &projection, glm::vec3 pos, glm::vec3 vel);
    Bullet(glm::mat4 &projection);
    ~Bullet() override;

  private:
    void set_shaders() override;
    void set_vertices() override;
    void set_indices() override;

    int sectors = 36;
    int stacks = 18;
    float radius = 0.1f;
};
#endif
