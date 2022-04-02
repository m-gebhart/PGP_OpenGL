#version 330

in vec2 uv;
smooth in vec4 fragColor;

layout (location = 0) out vec4 color;

uniform sampler2D textureSampler;

void main(void)
{
    color = texture(textureSampler, uv);
}