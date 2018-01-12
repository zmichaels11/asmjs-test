#version 330

in vec2 fTexCoord;

out vec4 fColor;

uniform sampler2D uTilesheet;

void main() {
    fColor = texture(uTilesheet, fTexCoord);    
}