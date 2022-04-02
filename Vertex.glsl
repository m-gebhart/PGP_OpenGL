#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texture;

smooth out vec4 fragColor;
out vec2 uv;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main()
{
    gl_Position = P * V * M * position;
    fragColor = color;
    uv = texture;
}