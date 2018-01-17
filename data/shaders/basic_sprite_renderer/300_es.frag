#version 300 es

precision mediump float;

in vec2 fImageView;

out vec4 result;

uniform sampler2D uImage;

void main() {
    vec4 color = texture(uImage, fImageView);

    result = vec4(color.rgb * color.a, 1.0);
}