#version 410 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec4 model_c0;
layout (location = 3) in vec4 model_c1;
layout (location = 4) in vec4 model_c2;
layout (location = 5) in vec4 model_c3;

out vec3 color;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    mat4 model = mat4(model_c0, model_c1, model_c2, model_c3);
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
    color = a_color;
}
