#version 330 core

in vec2 fTexCoord;

out vec4 rColor;

uniform sampler2D uImage;

void main() {
    rColor = texture(uImage, fTexCoord);
}