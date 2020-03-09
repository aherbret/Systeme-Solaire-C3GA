#version 300 es
precision mediump float;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoords;

void main(){

    vec4 vertexPosition = vec4(aPosition, 1);
    vec4 vertexNormal = vec4(aNormal, 0);

    vTexCoords = aTexCoords;
    vPosition = vec3(uMVMatrix*vertexPosition);
    vNormal = vec3(uNormalMatrix*vertexNormal);

    gl_Position = uMVPMatrix*vertexPosition;
}
