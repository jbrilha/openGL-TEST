#ifndef SQUARE_H
#define SQUARE_H

#include "shaders.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Square {
  public:
    Square() {
        init();
    }
    void draw(Shader shader_program, unsigned int primitive) {
        shader_program.use();



        glBindVertexArray(VAO);
        glDrawElements(primitive, sizeof(indices), GL_UNSIGNED_INT, 0);
        // glDrawElements(primitive, primitive == GL_LINES ? 12: 36, GL_UNSIGNED_INT, 0);
    }

  private:
    unsigned int VBO, VAO, EBO; // vertex buffer object, vertex array
                                // object, element buffer object

    int indices[6] = {
        0, 1, 2,
        0, 2, 3,
    };

    float vertices[12] = {
         0.5f,  0.5f, 0.f,  // top right
         0.5f, -0.5f, 0.f,  // bot right
        -0.5f, -0.5f, 0.f,  // bot left
        -0.5f,  0.5f, 0.f,  // top left
    };


    void init() {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // glGenBuffers(1, &normal_buffer);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

        // glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals,
        //              GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
                              (void *)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }
};
#endif
