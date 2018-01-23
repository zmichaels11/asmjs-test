#version 330 core

in vec2 fTexCoord;

out vec4 result;

uniform sampler2D uImage;

void main() {    
    vec4 color = texture2D(uImage, fTexCoord);

    result = vec4(color.rgb * color.a, 1.0);
}