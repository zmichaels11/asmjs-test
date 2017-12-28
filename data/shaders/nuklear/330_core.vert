#version 330

in vec2 Position;
in vec2 TexCoord;
in vec4 Color;

out vec4 Frag_UV;
out vec4 Frag_Color;

uniform mat4 ProjMtx;

void main() {
    Frag_UV = TexCoord;
    Frag_Color = Color;

    gl_Position = ProjMtx * vec4(Position, 0, 1);
}
