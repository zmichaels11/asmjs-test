#version 330 core

in vec2 fImageScroll;
uniform sampler2D uImage;

out vec4 result;

void main() {
    vec4 color = texture(uImage, fImageScroll);

    result = vec4(color.rgb * color.a, 1.0);
}
