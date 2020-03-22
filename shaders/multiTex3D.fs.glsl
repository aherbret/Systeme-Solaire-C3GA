#version 300 es
precision mediump float;

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

uniform sampler2D uTexture;
uniform sampler2D uTexture2;

out vec3 fFragColor;

void main() {
    vec4 color = texture(uTexture, vTexCoords);
    vec4 color2 = texture(uTexture2, vTexCoords);

    fFragColor = color.rgb + color2.rgb;
}