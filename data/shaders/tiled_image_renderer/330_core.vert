#version 330 core

in vec2 vSelect;
in vec2 vPosition;
in float vFrameIndex;
in vec2 vFrameView;

out vec3 fTexCoord;

uniform vec2 uTileSize;

void main() {
    gl_Position = vPosition + vSelect * uTileSize;
    fTexCoord = vec3(mix(vec2(0.0), vFrameView, vSelect), vFrameIndex);
}