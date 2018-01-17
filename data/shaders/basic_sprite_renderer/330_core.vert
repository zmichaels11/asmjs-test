#version 330 core

in vec2 vUpperLeft;
in vec2 vUpperRight;
in vec2 vLowerLeft;
in float vFrameIndex;
in vec2 vFrameSize;

out vec3 fTexCoord;

uniform mat4 uProjection;

void main() {
    vec2 vLowerRight = vUpperRight + (vLowerLeft - vUpperLeft);
    vec4 select;

    switch (gl_VertexID) {
        case 0: 
            select = vec4(vUpperLeft, 0.0, 1.0);
            fTexCoord = vec3(0.0, 0.0, vFrameIndex);
            break;
        case 1:
            select = vec4(vLowerLeft, 0.0, 1.0);
            fTexCoord = vec3(0.0, 0.0, vFrameIndex);
            break;
        case 2:
            select = vec4(vUpperRight, 0.0, 1.0);
            fTexCoord = vec3(0.0, 0.0, vFrameIndex);
            break;
        case 3:
            select = vec4(vLowerRight, 0.0, 1.0);
            fTexCoord = vec3(0.0, 0.0, vFrameIndex);
            break;
    }

    gl_Position = uProjection * select;
}