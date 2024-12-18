#include <cstdlib>
#include <iostream>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "shaders.h"
#include "camera.h"
#include "cube.h"
#include "square.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static void process_input(GLFWwindow *window);
static void error_callback(int error, const char *description);
static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height);
static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);
static void mouse_button_callback(GLFWwindow *window, int button, int action,
                                  int mods);
static void mouse_scroll_callback(GLFWwindow *window, double x_offset, double y_offset);
static void mouse_move_callback(GLFWwindow *window, double x_pos_in, double y_pos_in);
static void display_FPS(GLFWwindow *window, double current_time);
void grid(Shader shader_program, unsigned int VAO, int i, int j);
void draw_floor(Shader shader_program, unsigned int VAO);
glm::mat4 track_camera(glm::mat4 model, glm::vec3 *position, int i);

std::string GAME_NAME = "TESTING";
std::string VERSION = "v0.0.1";
std::string GAME_VERSION_NAME = VERSION + " | " + GAME_NAME;

// fps
int nb_frames = 0;
int last_time = 0;

// viewport settings
int width = 800;
int height = 800;

// camera
Camera camera(glm::vec3(0.f, 3.f, 7.f));
float last_x = width / 2.f;
float last_y = height / 2.f;
bool first_mouse = true;

bool paused = false;

float delta_time = 0.f;
float last_frame = 0.f;

bool track = false;
bool chase = false;

float near = 0.1f;
float far = 100.f;
glm::mat4 projection; // = glm::perspective(glm::radians(camera.zoom), (float) width / (float) height, near, far);

int main(int argc, char *argv[]) {

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    int window_width = mode->width;
    int window_height = mode->height;

    // width = window_width;
    // height = window_height;
    projection = glm::perspective(glm::radians(camera.zoom), (float) width / (float) height, near, far);

    std::cout << window_width << "x" << window_height << " | " << width << "x" << height << std::endl;

    GLFWwindow *window =
        // glfwCreateWindow(width, height, GAME_VERSION_NAME.c_str(), glfwGetPrimaryMonitor(), NULL);
        glfwCreateWindow(width, height, GAME_VERSION_NAME.c_str(), NULL, NULL);


    if (!window) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // set callback functions
    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwMakeContextCurrent(window);
    // gladLoadGL();
    // limit frame_rate to display (kinda like V-Sync?)
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    Shader shader_program("shaders/shader.vert", "shaders/shader.frag");
    Cube cube;
    Square square;
    // cube.init();

    float floor[] = {
          3.f, 0.f, -3.f, 0.5f, 0.5f, 0.5f,
         -3.f, 0.f, -3.f, 0.5f, 0.5f, 0.5f, // right to left far
          3.f, 0.f,  3.f, 0.5f, 0.5f, 0.5f,

         -3.f, 0.f,  3.f, 0.f, 0.f, 0.f,
         -3.f, 0.f, -3.f, 0.f, 0.f, 0.f,
          3.f, 0.f,  3.f, 0.f, 0.f, 0.f,
    };

    glm::vec3 cube_positions[] = {
        glm::vec3( 5.0f,  6.0f,  -3.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-7.f, 2.2f, -2.5f),
        glm::vec3(-3.8f, 6.0f, -12.3f),
        glm::vec3( 2.4f, 3.f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, 2.0f, -2.5f),
        glm::vec3( 5.f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  5.f, -1.5f),
        glm::vec3(-6.f,  10.0f, -1.5f)
    };

    unsigned int VBO, VAO; /*, EBO*/; // vertex buffer object, vertex array
                                     // object, element buffer object
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    //colors defined here I think
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    // shader_program.use();

    while (!glfwWindowShouldClose(window)) {
        if(!paused) {
            process_input(window); // using callback instead
            camera.move();

            float current_frame = static_cast<float>(glfwGetTime());
            delta_time = current_frame - last_frame;
            last_frame = current_frame;

            double current_time = glfwGetTime();
            nb_frames++;
            if (current_time - last_time >= 1.0)
                display_FPS(window, current_time);

            glClearColor(0.4f, 0.4f, 0.4f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader_program.use();

            shader_program.set_mat4("projection", projection);

            glm::mat4 view = camera.get_view_matrix();
            shader_program.set_mat4("view", view);

            glm::mat4 model;

            draw_floor(shader_program, VAO);

            for (unsigned int i = 0; i < 10; i++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, cube_positions[i]);

                float angle = 20.f * i;
                if(track) {
                    model = track_camera(model, &cube_positions[i], i);
                }

                shader_program.set_mat4("model", model);

                // if(chase && i % 3 == 0)
                //     cube.draw(shader_program, GL_LINES);
                // else
                    cube.draw(shader_program, GL_TRIANGLES);

                model = glm::translate(model, glm::vec3(0.f, 1.f, 0.f));
                shader_program.set_mat4("model", model);

                square.draw(shader_program, GL_TRIANGLES);
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    // glDeleteVertexArrays(2, VAO);
    // glDeleteBuffers(2, VBO);
    glDeleteProgram(shader_program.program_ID);
    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

glm::mat4 track_camera(glm::mat4 model, glm::vec3 *position, int i) {

    glm::vec3 direction = glm::normalize(camera.position - *position);

    glm::vec3 right = glm::normalize(glm::cross(camera.world_up, direction));
    glm::vec3 up = glm::cross(direction, right);

    if(chase && i % 3 == 0) {
        std::cout << "chasing" << std::endl;
        *position += direction * 0.1f;
        model = glm::translate(glm::mat4(1.f), *position);
    }

    return glm::mat4(
        glm::vec4(right, 0.0f),
        glm::vec4(up, 0.0f),
        glm::vec4(direction, 0.0f),
        glm::vec4(*position, 1.0f)
    );
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.handle_keyboard(FORWARD, delta_time, glfwGetTime());
        camera.moving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.handle_keyboard(LEFT, delta_time, glfwGetTime());
        camera.moving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.handle_keyboard(BACKWARDS, delta_time, glfwGetTime());
        camera.moving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.handle_keyboard(RIGHT, delta_time, glfwGetTime());
        camera.moving = true;
    }

    // GLFW_RELEASE doesn't actually check for release action in this case,
    // just means that the key is not being pressed atm
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE
        && glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE
        && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE
        && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        camera.moving = false;
    }
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        track = !track;

    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
        camera.sprinting = !camera.sprinting;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        camera.jumping = true;
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        camera.crouching = !camera.crouching;
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        camera.proning = !camera.proning;
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        paused = !paused;
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        chase = !chase;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    
    projection = glm::perspective(glm::radians(camera.zoom), (float) width / (float) height, near, far);
}

static void mouse_scroll_callback(GLFWwindow *window, double x_offset, double y_offset) {
    camera.handle_mouse_scroll(static_cast<float>(y_offset));
}

static void mouse_button_callback(GLFWwindow *window, int button, int action,
                                  int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS)
            std::cout << "marco" << std::endl;
        if (action == GLFW_RELEASE)
            std::cout << "polo" << std::endl;
    }
}

static void mouse_move_callback(GLFWwindow *window, double x_pos_in, double y_pos_in) {
    float x_pos = static_cast<float>(x_pos_in);
    float y_pos = static_cast<float>(y_pos_in);

    if(first_mouse) {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos;

    last_x = x_pos;
    last_y = y_pos;

    camera.handle_mouse_move(x_offset, y_offset);
}

static void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}


void display_FPS(GLFWwindow *window, double current_time) {
    std::string NAME_FPS =
        GAME_VERSION_NAME + " - " + std::to_string(nb_frames) + " FPS";

    nb_frames = 0;
    last_time = current_time;

    glfwSetWindowTitle(window, NAME_FPS.c_str());
}
void grid(Shader shader_program, unsigned int VAO, int i, int j) {
    glm::mat4 model;
    glBindVertexArray(VAO);

    for(int n = 0; n < 4; n++) {
        model = glm::mat4(1.0f);

        if(n % 2 == 0) {
            i *= -1;
        }
        else {
            j *= -1;
        }

        model = glm::translate(model, glm::vec3(6.f * j, 0.f, 6.f * i));
        shader_program.set_mat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

}

void draw_floor(Shader shader_program, unsigned int VAO){
    
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            grid(shader_program, VAO, i, j);
        }
    }

}
//! [code]
