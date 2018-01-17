#version 330 core

in vec2 vPosition;
in float vFrameIndex;
in vec2 vFrameSize;

out vec3 fTexCoord;

uniform mat4 uProjection;
uniform float uTileSize;

void main() {
    gl_Position = uProjection * vec4(vPosition * vec2(uTileSize), 0.0, 1.0);
    fTexCoord = vec3(mix(vec2(0.0), vFrameSize, vPosition), 1.0);
}