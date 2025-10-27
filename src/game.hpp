#ifndef GAME_HPP
#define GAME_HPP

#include "light_source.hpp"
#include "scenery.hpp"
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <memory>

#include "cursor.hpp"
#include "camera.hpp"
#include "constants.hpp"
#include "projectile.hpp"
#include "shape.hpp"
#include "bullet.hpp"
#include "cube.hpp"
#include "pyramid.hpp"
#include "shape.hpp"
#include "floor.hpp"
#include "sphere.hpp"

class Game {
  public:
    Game();
    ~Game();

    void run();
    void init();

  private:
    std::vector<std::unique_ptr<Shape>> shapes;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    std::vector<std::unique_ptr<Scenery>> world_objs;
    std::vector<std::unique_ptr<LightSource>> lights;
    void update_title_bar();
    int nb_frames;
    int last_time;

    std::string FPS;
    std::string NAME = "test";
    void process_input();
    void set_callbacks();
    void error_callback(int error, const char *description);
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void key_callback(GLFWwindow *window, int key, int scancode, int action,
                      int mods);
    void drop_callback(GLFWwindow *window, int count, const char **paths);
    void mouse_pos_callback(GLFWwindow *window, double x_pos_in,
                            double y_pos_in);
    void mouse_button_callback(GLFWwindow *window, int button, int action,
                               int mods);
    void track_camera(glm::mat4 *model, glm::vec3 *position, int i);
    GLFWwindow *window;
    void init_GLFW();

    int win_height;
    int win_width;

    float delta_time;
    float last_frame;
    float last_x;
    float last_y;
    bool first_mouse;
    bool chase;
    bool shoot;
    bool track;
    bool paused;
    glm::mat4 projection;
    float near;
    float far;

    Camera *camera;
    Cursor *cursor;
};

#endif
