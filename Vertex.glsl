#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

smooth out vec4 fragColor;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main()
{
    gl_Position = P * V * M * position;
    fragColor = color;
}