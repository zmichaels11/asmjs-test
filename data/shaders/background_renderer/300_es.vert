#version 300 es

const vec4[4] VERTEX_SELECT = vec4[] (
    vec4(-1.0, 1.0, 0.0, 1.0), vec4(-1.0, -1.0, 0.0, 1.0),
    vec4(1.0, 1.0, 0.0, 1.0), vec4(1.0, -1.0, 0.0, 1.0));

const vec2[4] FRAGMENT_SELECT = vec2[] (
    vec2(0.0, 0.0), vec2(0.0, 1.0),
    vec2(1.0, 0.0), vec2(1.0, 1.0));

out vec2 fTexCoord;

uniform vec2 uOrigin;
uniform vec2 uScroll;
uniform mat2 uTransform;

void main() {
    vec2 xy = FRAGMENT_SELECT[gl_VertexID] + uScroll - uOrigin;

    fTexCoord = uTransform * xy + uOrigin;
    gl_Position = VERTEX_SELECT[gl_VertexID];
}