#version 440 core
layout(location = 0) out vec3 color;

in vec2 texCoord;

void main()
{
    color = vec3(1.0f, 0.5f, 0.2f);
}