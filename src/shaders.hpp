#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

class Shader {
  public:
    unsigned int program_ID;

    Shader(const char *vert_path, const char *frag_path);
    ~Shader();

    void use();

    void del();

    void set_bool(const std::string &name, bool value) const;

    void set_int(const std::string &name, int value) const;

    void set_float(const std::string &name, float value) const;

    void set_vec2(const std::string &name, float x, float y) const;

    void set_vec2(const std::string &name, const glm::vec2 &value) const;

    void set_vec3(const std::string &name, float x, float y, float z) const;

    void set_vec3(const std::string &name, const glm::vec3 &value) const;

    void set_vec4(const std::string &name, float x, float y, float z,
                  float w) const;

    void set_vec4(const std::string &name, const glm::vec4 &value) const;

    void set_mat2(const std::string &name, const glm::mat2 &mat) const;

    void set_mat3(const std::string &name, const glm::mat3 &mat) const;

    void set_mat4(const std::string &name, const glm::mat4 &mat) const;

  private:
    void check_for_errors(unsigned int ID, std::string type);
};

#endif // !SHADERS_H
