#version 300 es

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
//layout(location = 2) in mat4 aModelMat;

uniform mat4 PVmat;

out vec4 vColor;


void main() {
    vColor = vec4(aColor,1.0);
    gl_Position = PVmat  * vec4(aPosition,1.0);
}
