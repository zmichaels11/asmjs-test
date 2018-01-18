#version 330 core

in vec3 fTexCoord;
flat in vec4 fTint;

out vec4 result;

uniform sampler2DArray uImage;

void main() {
    vec4 color = fTint * texture(uImage, fTexCoord);

    result = vec4(color.rgb * color.a, 1.0);
}