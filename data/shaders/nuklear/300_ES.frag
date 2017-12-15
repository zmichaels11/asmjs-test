#version 300 es

precision mediump float;

in vec2 Frag_UV;
in vec4 Frag_Color;

out vec4 Out_Color;

uniform sampler2D Texture;

void main() {
    vec4 color = Frag_Color * texture(Texture, Frag_UV);

    Out_Color = vec4(color.rgb * color.a, color.a);
}
