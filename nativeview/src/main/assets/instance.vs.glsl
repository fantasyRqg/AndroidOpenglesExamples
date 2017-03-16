#version 300 es

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in mat4 aMvpMat;

out vec4 vColor;


void main() {
    vColor = aColor;
    gl_Position = aMvpMat * aPosition;
}
