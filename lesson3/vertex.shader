#version 140

uniform mat4 mvp;

in vec3 vert;
in vec2 vertTexCoord;

out vec2 fragTexCoord;

void main(){
    fragTexCoord = vertTexCoord;
    gl_Position = mvp * vec4(vert,1);
    //gl_Position = vec4(vert,1);

}
