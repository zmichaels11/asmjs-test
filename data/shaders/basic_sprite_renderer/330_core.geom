#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices=4) out;

in vec4 gUpperLeft[];
in vec4 gUpperRight[];
in vec4 gLowerLeft[];
in vec4 gLowerRight[];
in vec4 gImageView[];

out vec2 fImageView;

void main() {
    gl_Position = gUpperLeft[0];
    fImageView = gImageView[0].xy;
    EmitVertex();

    gl_Position = gLowerLeft[0];
    fImageView = gImageView[0].xw;
    EmitVertex();

    gl_Position = gUpperRight[0];
    fImageView = gImageView[0].zy;
    EmitVertex();

    gl_Position = gLowerRight[0];
    fImageView = gImageView[0].zw;
    EmitVertex();
}
