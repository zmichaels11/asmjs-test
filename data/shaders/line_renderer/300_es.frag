#version 300 es

precision mediump float;

flat in vec4 fColor;

out vec4 result;

void main() {
    result = vec4(fColor.rgb * fColor.a, 1.0);
}