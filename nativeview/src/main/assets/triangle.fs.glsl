#version 300 es
precision mediump float;

smooth in vec3 inColor;
out vec4 fragColor;

void main() {
    fragColor = vec4(inColor,1.0);
}