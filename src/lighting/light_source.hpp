#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "shaders.hpp"
#include "shape.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

class LightSource {
  public:
    LightSource(std::unique_ptr<Shape> shape);
    LightSource(std::unique_ptr<Shape> shape, glm::mat4 &projection);
    ~LightSource();
    void draw(const glm::mat4 &view, const glm::vec3 light_pos);

    template <typename T>
    static std::unique_ptr<LightSource> from_shape(glm::mat4 &projection,
                                                   bool &chase, glm::vec3 pos) {
        if (!shared_shader) {
            shared_shader =
                std::make_shared<Shader>("shaders/light_source_vert.glsl",
                                         "shaders/light_source_frag.glsl");
        }

        auto shape = std::make_unique<T>(shared_shader, projection, chase, pos);

        return make_unique<LightSource>(std::move(shape));
    }

  private:
    std::unique_ptr<Shape> shape;
    static std::shared_ptr<Shader> shared_shader;
};
#endif
