#version 140

attribute vec4 coord;
varying vec2 textcoord;

void main(void){
    gl_Position = vec4(coord.xy, 0, 1);
    textcoord = coord.zw;
}
