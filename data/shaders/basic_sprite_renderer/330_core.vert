#version 330 core

in vec2 vUpperLeft;
in vec2 vUpperRight;
in vec2 vLowerLeft;
in vec4 vImageView;

out vec4 gUpperLeft;
out vec4 gUpperRight;
out vec4 gLowerLeft;
out vec4 gLowerRight;
out vec4 gImageView;

uniform mat4 uProjection;

void main() {
    gUpperLeft = uProjection * vec4(vUpperLeft, 0.0, 1.0);
    gUpperRight = uProjection * vec4(vUpperRight, 0.0, 1.0);
    gLowerLeft = uProjection * vec4(vLowerLeft, 0.0, 1.0);
    gLowerRight = uProjection * vec4(vUpperRight + (vLowerLeft - vUpperLeft), 0.0, 1.0);
    gImageView = vImageView;
}