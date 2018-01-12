#version 330 core

in vec2 vPosition;

out vec2 fTexCoord;

uniform vec4 uImageView;

void main() {
    gl_Position = vec4(vIndex, 0.0, 1.0);
    fTexCoord = mix(uImageView.xy, uImageView.zw, vIndex);
}