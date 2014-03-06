#version 140
uniform vec3 color;
uniform float trans;
out vec4 finalColor;

void main() {

    finalColor = vec4(color, trans);

}
