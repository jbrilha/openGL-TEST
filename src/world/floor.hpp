#ifndef FLOOR_H
#define FLOOR_H

#include "scenery.hpp"
#include <vector>

class Floor : public Scenery {
  public:
    Floor(glm::mat4 &projection);
    ~Floor() override;

  private:
    void set_shaders() override;
    void set_vertices() override;
    int tile_count;
};
#endif
