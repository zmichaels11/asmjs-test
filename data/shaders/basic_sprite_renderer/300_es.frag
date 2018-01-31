#version 300 es

precision mediump float;
precision mediump sampler2DArray;

in vec3 fTexCoord;

out vec4 result;

uniform sampler2DArray uImage;

void main() {
    vec4 color = texture(uImage, fTexCoord);

    result = vec4(color.rgb * color.a, color.a);    
}