#version 300 es

layout(location =0) in vec3 vPosition;

out vec3 inColor;

void main() {
    gl_Position = vec4(vPosition,1.0);
    inColor = vPosition + 0.5;
}
