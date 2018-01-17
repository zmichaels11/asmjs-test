#version 330 core

const vec2 SELECT[] = vec2[4](
    vec2(0.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 0.0),
    vec2(1.0, 1.0));

in vec2 vPosition;
in vec4 vImageView;

out vec2 fImageView;

uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * vec4(vPosition, 0.0, 1.0);

    vec2 select = SELECT[gl_VertexID];

    fImageView = mix(vImageView.xy, vImageView.zw, select);
}