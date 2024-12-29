#ifndef FLOOR_H
#define FLOOR_H

#include "scenery.hpp"
#include <vector>

class Floor : public Scenery {
  public:
    Floor();
    ~Floor() override;

  private:
    void set_shaders() override;
    void set_vertices() override;
    int tile_count;
};
#endif
