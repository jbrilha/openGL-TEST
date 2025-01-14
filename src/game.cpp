#include "game.hpp"
#include <memory>

Game::Game()
    : win_height(WIN_HEIGHT), win_width(WIN_WIDTH), delta_time(0.f),
      last_frame(0.f), last_x(win_height / 2.f), last_y(win_height / 2.f),
      chase(false), shoot(false), track(false), paused(false), near(NEAR),
      far(FAR), camera(nullptr), cursor(nullptr) {}

Game::~Game() {
    delete camera;
    camera = nullptr;

    delete cursor;
    cursor = nullptr;
}

void Game::init() {
    camera = new Camera(glm::vec3(0.f, 3.f, 7.f));

    init_GLFW();

    cursor = new Cursor(window, 1, 5);
}

void Game::run() {
    glm::vec3 cube_positions[] = {
        glm::vec3(5.0f, 6.0f, -3.0f), glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-7.f, 2.2f, -2.5f), glm::vec3(-3.8f, 6.0f, -12.3f),
        glm::vec3(2.4f, 3.f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, 2.0f, -2.5f), glm::vec3(5.f, 2.0f, -2.5f),
        glm::vec3(1.5f, 5.f, -1.5f),  glm::vec3(-6.f, 10.0f, -1.5f),
    };

    world_objs.push_back(std::make_unique<Floor>(projection));

    for (int i = 0; i < 10; i++) {
        glm::vec3 pos = cube_positions[i];
        shapes.push_back(std::make_unique<Cube>(projection, chase, pos));
        pos += glm::vec3(0.f, 1.f, 0.f);
        shapes.push_back(std::make_unique<Pyramid>(projection, chase, pos));
        pos += glm::vec3(0.f, 1.f, 0.f);
        shapes.push_back(std::make_unique<Sphere>(projection, chase, pos));
    }

    for (int i = 0; i < 1000; i++) {
        projectiles.push_back(std::make_unique<Bullet>(projection));
    }
    size_t cb = 0;

    glEnable(GL_DEPTH_TEST);
    glm::mat4 view;

    float current_frame;
    while (!glfwWindowShouldClose(window)) {
        current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;

        process_input();
        camera->move();

        update_title_bar();

        glClearColor(0.3f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = camera->get_view_matrix();

        if (shoot) { // just to test it out
            if (!projectiles[cb]->active) {
                glm::vec3 inpos = camera->position + glm::vec3(0.f, -0.2f, 0.f);
                glm::vec3 direction = camera->front;
                glm::vec3 trajectory =
                    glm::normalize(direction) * (P_SPEED / 4);
                projectiles[cb]->init(inpos, trajectory);
            }
            if (cb < projectiles.size() - 1) {
                cb++;
            } else {
                cb = 0;
            }
            shoot = false;
        }

        cursor->draw();

        for (const auto &bull : projectiles) {
            if (bull->active) {
                if (!paused)
                    bull->update(delta_time);

                bull->draw(view);
            }
        }

        // TODO figure out optimization for setting projection on all objects
        // instead of on every draw call, check out UBOs?
        for (const auto &shape : shapes) {
            if (track) {
                shape->look_at(camera->position);
            }
            shape->draw(view);
        }

        for (const auto &world_obj : world_objs) {
            world_obj->draw(view);
        }

        last_frame = current_frame;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}

void Game::init_GLFW() {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    win_width = mode->width;
    win_height = mode->height;

    projection =
        glm::perspective(glm::radians(camera->zoom),
                         (float)win_width / (float)win_height, near, far);

    window = glfwCreateWindow(win_width, win_height, "game", NULL, NULL);

    if (!window) {
        std::cout << window << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    set_callbacks();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    // limit frame_rate to display (kinda like V-Sync?)
    // glfwSwapInterval(GLFW_TRUE);
    glfwSwapInterval(GLFW_FALSE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Game::set_callbacks() {
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode,
                                  int action, int mods) {
        Game *instance = static_cast<Game *>(glfwGetWindowUserPointer(window));
        if (instance)
            instance->key_callback(window, key, scancode, action, mods);
    });
    glfwSetErrorCallback([](int error, const char *description) {
        GLFWwindow *curr_window = glfwGetCurrentContext();
        if (curr_window != nullptr) {
            Game *instance =
                static_cast<Game *>(glfwGetWindowUserPointer(curr_window));
            if (instance)
                instance->error_callback(error, description);
        } else {
            fprintf(stderr, "Error (No window available): %s\n", description);
        }
    });
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width,
                                              int height) {
        Game *instance = static_cast<Game *>(glfwGetWindowUserPointer(window));
        if (instance)
            instance->framebuffer_size_callback(window, width, height);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x_pos_in,
                                        double y_pos_in) {
        Game *instance = static_cast<Game *>(glfwGetWindowUserPointer(window));
        if (instance)
            instance->mouse_pos_callback(window, x_pos_in, y_pos_in);
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button,
                                          int action, int mods) {
        Game *instance = static_cast<Game *>(glfwGetWindowUserPointer(window));
        if (instance)
            instance->mouse_button_callback(window, button, action, mods);
    });
}

void Game::mouse_button_callback(GLFWwindow *window, int button, int action,
                                 int mods) {
    (void)window, (void)mods;
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            shoot = true;
        }
    }
}

void Game::mouse_pos_callback(GLFWwindow *window, double x_pos_in,
                              double y_pos_in) {
    (void)window;
    float x_pos = static_cast<float>(x_pos_in);
    float y_pos = static_cast<float>(y_pos_in);

    if (first_mouse) {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos;

    last_x = x_pos;
    last_y = y_pos;

    camera->handle_mouse_move(x_offset, y_offset);
}

void Game::key_callback(GLFWwindow *window, int key, int scancode, int action,
                        int mods) {
    (void)scancode, (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        track = !track;

    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
        camera->sprinting = !camera->sprinting;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        camera->jumping = true;
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        camera->crouching = !camera->crouching;
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        camera->proning = !camera->proning;
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        paused = !paused;
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        chase = !chase;

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        for (const auto &bull : projectiles) {
            bull->active = false;
        }
}

void Game::process_input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
        delta_time >= 0.001f) {
        shoot = true;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->handle_keyboard(FORWARD, delta_time, glfwGetTime());
        camera->moving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->handle_keyboard(LEFT, delta_time, glfwGetTime());
        camera->moving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->handle_keyboard(BACKWARDS, delta_time, glfwGetTime());
        camera->moving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->handle_keyboard(RIGHT, delta_time, glfwGetTime());
        camera->moving = true;
    }

    // GLFW_RELEASE doesn't actually check for release action in this case,
    // just means that the key is not being pressed atm
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        camera->moving = false;
    }
}

void Game::framebuffer_size_callback(GLFWwindow *window, int width,
                                     int height) {
    (void)window;
    glViewport(0, 0, width, height);
    win_width = width;
    win_height = height;

    projection = glm::perspective(glm::radians(camera->zoom),
                                  (float)width / (float)height, near, far);

    // made projection a ref in the objects instead might run into issues later
    // with thread locks but for now makes sense

    // for (const auto &bull : projectiles) {
    //     bull->set_projection(projection);
    // }

    // for (const auto &shape : shapes) {
    //     shape->set_projection(projection);
    // }

    // for (const auto &world_obj : world_objs) {
    //     world_obj->set_projection(projection);
    // }
}

void Game::error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %d — %s\n", error, description);
}

void Game::update_title_bar() {
    double current_time = glfwGetTime();
    nb_frames++;
    if (current_time - last_time >= 1.0) {
        FPS = std::to_string(nb_frames);

        nb_frames = 0;
        last_time = current_time;
    }

    std::string TITLE_BAR = NAME + " — " + FPS + " FPS";

    glfwSetWindowTitle(window, TITLE_BAR.c_str());
}
