#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices=4) out;

in vec4 gUpperLeft[];
in vec4 gUpperRight[];
in vec4 gLowerLeft[];
in vec4 gLowerRight[];
in vec4 gImageView[];
in mat4 gCT[];
in vec4 gCO[];

out vec2 fImageView;
flat out mat4 fCT;
flat out vec4 fCO;

void main() {
    gl_Position = gUpperLeft[0];
    fImageView = gImageView[0].xy;
    fCT = gCT[0];
    fCO = gCO[0];
    EmitVertex();

    gl_Position = gLowerLeft[0];
    fImageView = gImageView[0].xw;
    fCT = gCT[0];
    fCO = gCO[0];
    EmitVertex();

    gl_Position = gUpperRight[0];
    fImageView = gImageView[0].zy;
    fCT = gCT[0];
    fCO = gCO[0];
    EmitVertex();

    gl_Position = gLowerRight[0];
    fImageView = gImageView[0].zw;
    fCT = gCT[0];
    fCO = gCO[0];
    EmitVertex();
}
