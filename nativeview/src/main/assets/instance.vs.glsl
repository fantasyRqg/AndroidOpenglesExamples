#version 300 es

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
//layout(location = 2) in mat4 aModelMat;

uniform mat4 PVmat;
uniform float timestampS;

out vec4 vColor;


mat4 rotate(in mat4 m ,in float angle,in vec3 v){
    float a = angle;
    float c = cos(a);
    float s = sin(a);

    vec3 axis = normalize(v);
//    vec3 temp((T(1) - c) * axis);
    vec3 temp = (vec3(1.0)  - c) * axis;
    mat4 Rotate = mat4(1.0);
    Rotate[0][0] = c + temp[0] * axis[0];
    Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
    Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

    Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
    Rotate[1][1] = c + temp[1] * axis[1];
    Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

    Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
    Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
    Rotate[2][2] = c + temp[2] * axis[2];

    mat4 Result = mat4(1.0);
    Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
    Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
    Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
    Result[3] = m[3];
    return Result;
}

mat4 translate(in mat4 m,in vec3 v){
        mat4 Result = mat4(m);
		Result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];

		return Result;
}

void main() {
    vColor = vec4(aColor,1.0);

    float angle = timestampS * 5.1;

    int numRows = 10;
    int numColumns = numRows;
    float translateX = float(gl_InstanceID % numRows)  /  float(numRows) * 2.0f - 1.0f;
    float translateY = float(gl_InstanceID / numColumns) /  float(numColumns)  * 2.0f - 1.0f;

    gl_Position = PVmat *
            translate(mat4(1.0),vec3(translateX*20.0,translateY*20.0,-26.0)) *
            rotate(mat4(1.0),angle,vec3(sin(timestampS * 0.4),cos(timestampS* 0.2),sin(timestampS* 0.6)))*
            vec4(aPosition,1.0);
}
