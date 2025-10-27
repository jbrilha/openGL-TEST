#version 410 core

layout (location = 0) in vec2 a_pos;

uniform vec2 cursor_pos;

out vec3 color;

void main() {
    gl_Position = vec4(a_pos + cursor_pos, 0.f, 1.0);

    color = vec3(1.f, 1.f, 1.f);
}
