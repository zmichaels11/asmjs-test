#version 330

const vec2[4] VERTEX_SELECT = vec2[] (
    vec2(0.0, 0.0), vec2(0.0, 1.0),
    vec2(1.0, 0.0), vec2(1.0, 1.0));

out vec2 fTexCoord;

uniform mat4 uProjection;
uniform vec4 uImageView;

void main() {
    vec2 select = VERTEX_SELECT[gl_VertexID];

    gl_Position = uProjection * vec4(select, 0.0, 1.0);
    fTexCoord = mix(uImageView.xy, uImageView.zw, select);
}