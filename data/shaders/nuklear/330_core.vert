#version 330 core

in vec2 vPosition;
in vec2 vTexCoord;
in vec4 vColor;

out vec2 fTexCoord;
out vec4 fColor;

uniform mat4 uProjection;

void main() {
    fTexCoord = vTexCoord;
    fColor = vColor;

    gl_Position = uProjection * vec4(vPosition, 0.0, 1.0);
}
