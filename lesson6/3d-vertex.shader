#version 140

uniform mat4 camera;
uniform mat4 model;

in vec3 vert;
in vec2 vertTexCoord;
in vec3 vertNormal;

out vec2 fragTexCoord;
out vec3 fragVert;
out vec3 fragNormal;

void main(){
    //variables to pass on to fragment shader
    fragTexCoord = vertTexCoord;
    fragVert = vert;
    fragNormal = vertNormal;

    //vertext to draw
    gl_Position = camera * model * vec4(vert,1);

}
