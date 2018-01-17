#version 330 core

in vec2 fImageView;
flat in mat4 fCT;
flat in vec4 fCO;

out vec4 result;

uniform sampler2D uImage;

void main() {
    vec4 color = texture(uImage, fImageView);

    float alpha = color.a;

    color = fCT * vec4(color.rgb, 1.0) + fCO;

    result = vec4(color.rgb * alpha, 1.0);
}