#version 330 core

in vec2 vSelect;
in vec2 vUpperLeft;
in vec2 vUpperRight;
in vec2 vLowerLeft;
in float vFrameIndex;
in vec2 vFrameSize;

out vec3 fTexCoord;

uniform mat4 uProjection;

void main() {
    vec2 vLowerRight = vUpperRight + (vLowerLeft - vUpperLeft);    

    if (vSelect.x < 0.5) {
        gl_Position = uProjection * vec4(mix(vUpperLeft, vLowerLeft, vSelect.y), 0.0, 1.0);
    } else {
        gl_Position = uProjection * vec4(mix(vUpperRight, vLowerRight, vSelect.y), 0.0, 1.0);
    }

    fTexCoord = vec3(mix(vec2(0.0), vFrameSize, vSelect), vFrameIndex);
}