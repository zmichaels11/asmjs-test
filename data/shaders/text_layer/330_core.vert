#version 330 core

in vec2 vPosition;
in vec2 vTexCoord;
in vec4 vColor;

uniform mat4 uProjection;

out vec2 fTexCoord;
out vec4 fColor;

void main() {
    gl_Position = uProjection * vec4(vPosition, 0.0, 1.0);

    fTexCoord = vTexCoord;
    fColor = vColor;
}
