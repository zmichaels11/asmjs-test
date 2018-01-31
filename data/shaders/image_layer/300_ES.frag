#version 300 es

precision mediump float;

in vec2 fImageScroll;
uniform sampler2D uImage;

out vec4 result;

void main() {
    vec4 color = texture(uImage, fImageScroll);

    result = vec4(color.rgb * color.a, color.a);
}
