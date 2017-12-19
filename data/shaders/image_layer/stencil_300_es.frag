#version 300 es

precision mediump float;

in vec2 fImageScroll;

uniform sampler2D uImage;

out vec4 result;

void main() {
    vec4 value = texture(uImage, fImageScroll);

    if (value.r == 0.0) {
        discard;
    }

    result = value;
}
