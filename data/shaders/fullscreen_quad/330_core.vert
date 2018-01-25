#version 330 core

const vec4[4] VERTICES = vec4[] (
    vec4(-1.0, 1.0, 0.0, 1.0), vec4(-1.0, -1.0, 0.0, 1.0),
    vec4(1.0, 1.0, 0.0, 1.0), vec4(1.0, -1.0, 0.0, 1.0));

const vec2[4] FRAGMENTS = vec2[] (
    vec2(0.0, 0.0), vec2(0.0, 1.0),
    vec2(1.0, 0.0), vec2(1.0, 1.0));

out vec2 fTexCoord;

void main() {
    gl_Position = VERTICES[gl_VertexID];
    fTexCoord = FRAGMENTS[gl_VertexID];
}