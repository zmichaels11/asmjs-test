#version 300 es

const vec2 SELECT[] = vec2[4] (
    vec2(0.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 0.0),
    vec2(1.0, 1.0));

in vec2 vUpperLeft;
in vec2 vUpperRight;
in vec2 vLowerLeft;
in vec4 vImageView;
in vec4 vTint;

out vec2 fImageView;
flat out vec4 fTint;

uniform mat4 uProjection;

void main() {    
    switch (gl_VertexID) {
        case 0:
            gl_Position = uProjection * vec4(vUpperLeft, 0.0, 1.0);            
            break;
        case 1:
            gl_Position = uProjection * vec4(vUpperRight, 0.0, 1.0);
            break;
        case 2:
            gl_Position = uProjection * vec4(vLowerLeft, 0.0, 1.0);
            break;
        case 3:
            gl_Position = uProjection * vec4(vUpperRight + (vLowerLeft - vUpperLeft), 0.0, 1.0);
            break;
    }

    fImageView = mix(vImageView.xy, vImageView.zw, SELECT[gl_VertexID]);
    fTint = vTint;
}