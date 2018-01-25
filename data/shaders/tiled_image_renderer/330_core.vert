#version 330 core

const vec2 VERTICES[] = vec2[4](
    vec2(0.0, 0.0), vec2(0.0, 1.0),
    vec2(1.0, 0.0), vec2(1.0, 1.0));

in vec2 vPosition;
in vec2 vOffset;
in vec3 vImageView;

out vec3 fTexCoord;

void main() {
    vec2 select = VERTICES[gl_VertexID];        
    vec2 uv = mix(vec2(0.0, 0.0), vImageView.xy, select);

    gl_Position = vec4(vPosition + vOffset + select, 0.0, 1.0);
}