#version 140
varying vec2 textcoord;
uniform sampler2D tex;
//uniform vec4 color;

void main(void){
    //gl_FragColor = vec4(1, 1, 1, texture2D(tex, textcoord).a) * color;
    //gl_FragColor = vec4(1, 1, 1, texture2D(tex, textcoord).a) * vec4(0,0,0,1);
    gl_FragColor = vec4(1, 1,1, texture2D(tex, textcoord).r) * vec4(0,0,0,1);
}
