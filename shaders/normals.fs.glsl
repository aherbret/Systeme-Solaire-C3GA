#version 300 es
precision mediump float;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

out vec3 fFragColor;

void main() {
    fFragColor = normalize(vNormal);
}