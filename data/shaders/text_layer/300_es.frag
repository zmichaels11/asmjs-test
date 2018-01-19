#version 300 es

precision mediump float;

in vec2 fTexCoord;
flat in vec4 fColor;

out vec4 result;

uniform sampler2D uFont;

void main() {
    float alpha = fColor.a * texture(uFont, fTexCoord).a;

    result = vec4(fColor.rgb * alpha, 1.0);
}