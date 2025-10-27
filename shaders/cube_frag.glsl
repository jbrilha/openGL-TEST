#version 410 core

out vec4 frag_color;

in vec3 color;

void main()
{
    frag_color = vec4(normalize(color), 1.f);
    // frag_color = vec4(1.f, 0.5f, 0.2f, 1.f);
}
