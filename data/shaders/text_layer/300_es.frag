#version 300 es
 
precision mediump float;
 
in vec2 fTexCoord;
flat in vec4 fColor;
 
out vec4 color;

uniform sampler2D uFont;
 
void main() {
    float alpha = fColor.a * texture(uFont, fTexCoord).r;
 
    color = vec4(fColor.rgb * alpha, alpha);
}