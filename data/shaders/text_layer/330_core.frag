#version 330 core

in vec2 fTexCoord;
in vec4 fColor;

uniform sampler2D uFont;

out vec4 color;

void main() {
    float alpha = fColor.a * texture(uFont, fTexCoord).r;

    color = vec4(fColor.rgb * alpha, alpha);
}
