#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

void main()
{
    gl_Position = view * projection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    texCoord = aTexCoord;
}