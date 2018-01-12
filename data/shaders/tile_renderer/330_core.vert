#version 330

in vec2 vIndex;
in vec4 vPosition;
in vec4 vTexCoord;

out vec2 fTexCoord;

uniform mat4 uProjection;

void main() {
    vec2 pos = mix(vPosition.xy, vPosition.zw, vIndex);

    gl_Position = uProjection * vec4(pos, 0.0, 1.0);
    fTexCoord = mix(vTexCoord.xy, vTexCoord.zw, vIndex);
}