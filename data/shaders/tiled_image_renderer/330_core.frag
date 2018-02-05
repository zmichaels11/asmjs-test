#version 330 core

in vec3 fTexCoord;

out vec4 result;

uniform sampler2DArry uImage;

void main() {
    vec4 color = texture(uImage, fTexCoord);

    result = vec4(color.rgb * color.a, color.a);
}