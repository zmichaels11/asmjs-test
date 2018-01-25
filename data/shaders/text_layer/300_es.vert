#version 300 es

in vec2 vPosition;
in vec2 vTexCoord;
in vec4 vColor;

out vec2 fTexCoord;
flat out vec4 fColor;

uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * vec4(vPosition, 0.0, 1.0);

    fTexCoord = vTexCoord;
    fColor = vColor;
}