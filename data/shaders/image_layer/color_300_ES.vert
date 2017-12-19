#version 300 es

const vec2[4] VERTEX_SELECT = vec2[] (
    vec2(0.0, 0.0), vec2(0.0, 1.0),
    vec2(1.0, 0.0), vec2(1.0, 1.0));

const vec4 VERTEX = vec4(-1.0, 1.0, 1.0, -1.0);

uniform vec4 uData;

out vec2 fImageScroll;

void main() {
    vec2 select = VERTEX_SELECT[gl_VertexID];

    gl_Position = vec4(mix(VERTEX.xy, VERTEX.zw, select), 0.0, 1.0);   

    fImageScroll = mix(uData.xy, uData.zw, select);
}

