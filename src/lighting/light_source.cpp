#include "light_source.hpp"

std::shared_ptr<Shader> LightSource::shared_shader = nullptr;

LightSource::LightSource(std::unique_ptr<Shape> shape)
    : shape(std::move(shape)) {}

LightSource::~LightSource() {}

void LightSource::draw(const glm::mat4 &view, const glm::vec3 light_pos) {
    shape->draw_illuminating(view, light_pos);
}
