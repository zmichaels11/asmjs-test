#version 330 core

in vec2 fImageView;
in vec4 fTint;

out vec4 result;

uniform sampler2D uImage;

void main() {
    vec4 color = fTint * texture(uImage, fImageView);

    result = vec4(color.rgb * color.a, 1.0);
}