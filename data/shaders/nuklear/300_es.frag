#version 300 es

precision mediump float;

in vec2 fTexCoord;
in vec4 fColor;

out vec4 result;

uniform sampler2D uTexture;

void main() {
    vec4 color = fColor * texture(uTexture, fTexCoord);

    result = vec4(color.rgb * color.a, color.a);
}