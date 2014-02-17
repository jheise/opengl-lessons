#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

GLuint cubeVBO = 0;
GLuint cubeVAO = 0;

GLuint shaderprogram = 0;

GLuint vertshader;
GLuint fragshader;

 /* Vertices of a triangle (counter-clockwise winding) */
GLfloat data[] = {
    //X   Y    Z
    0.0f, 0.8f, 0.0f,
    -0.8f, -0.8f, 0.0f,
    0.8f, -0.8f, 0.0f,
};

void display(void){

    glClearColor( 0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT );

    //load program to use
    glUseProgram(shaderprogram);

    //load vertex array to use
    glBindVertexArray(cubeVAO);

    //draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //unbind for next pass
    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapBuffers();

}

void handleShader(const std::string& shaderpath, GLenum shaderType){

    std::ifstream f;
    f.open(shaderpath.c_str(), std::ios::in | std::ios::binary);
    if(!f.is_open()){
        throw std::runtime_error(std::string("failed to open file: ") + shaderpath);
    }

    std::stringstream buffer;
    buffer << f.rdbuf();
    std::string bufferstr = buffer.str();

    GLuint shader;
    shader = glCreateShader(shaderType);
    if(shader == 0){
        throw std::runtime_error("glCreateShader failed");
    }
    const char* code = bufferstr.c_str();
    glShaderSource(shader, 1, (const GLchar**)&code, NULL);
    glCompileShader(shader);
    //test if shader compiled
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if( status == GL_FALSE) {
        std::string msg("Failed to compile shader:\n");

        GLint infologlen;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infologlen);
        char* strinfolog = new char[infologlen + 1];
        glGetShaderInfoLog( shader, infologlen, NULL, strinfolog);
        msg += strinfolog;
        delete[] strinfolog;

        glDeleteShader(shader);
        shader = 0;
        throw std::runtime_error(msg);
    }

    if( shaderType == GL_VERTEX_SHADER){
        vertshader = shader;
    } else {
        fragshader = shader;
    }
}

int main(int argc, char *argv[]){

    if(!glfwInit()){
        throw std::runtime_error("glfwInit failed");
    }

    glfwOpenWindowHint(0, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    if(!glfwOpenWindow(800, 600, 8, 8, 8, 8, 0, 0, GLFW_WINDOW)){
        throw std::runtime_error("glfwOpenWindow failed. does this hardware work with 3.1");
    }

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        throw std::runtime_error("glewInit failed");
    }

    if(!GLEW_VERSION_3_1){
        throw std::runtime_error("OpenGL 3.1 API is not available");
    }

    printf("loading vertex shader\n");
    handleShader("vertex.shader", GL_VERTEX_SHADER);
    printf("loading fragment shader\n");
    handleShader("fragment.shader", GL_FRAGMENT_SHADER);
    printf("Shaders loaded\n");

    printf("Building shader program\n");
    shaderprogram = glCreateProgram();
    if(shaderprogram == 0){
        throw std::runtime_error("glCreateProgram failed");
    }

    glAttachShader(shaderprogram, vertshader);
    glAttachShader(shaderprogram, fragshader);

    glLinkProgram(shaderprogram);

    GLint status;
    glGetProgramiv(shaderprogram, GL_LINK_STATUS, &status);
    if(status == GL_FALSE){
        std::string msg("Program linking failure: ");

        GLint infologlen;
        glGetProgramiv(shaderprogram, GL_INFO_LOG_LENGTH, &infologlen);
        char* strinfolog = new char[infologlen + 1];
        glGetProgramInfoLog(shaderprogram, infologlen, NULL, strinfolog);
        msg += strinfolog;
        delete[] strinfolog;

        glDeleteProgram(shaderprogram);
        throw std::runtime_error(msg);
    }

    GLint vertprog = glGetAttribLocation(shaderprogram,"vert");

    //bind VAO
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    //bind VBO
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);


    glEnableVertexAttribArray(vertprog);
    glVertexAttribPointer(vertprog, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);

    //glEnableVertexAttribArray(verttexprog);
    //glVertexAttribPointer(verttexprog, 2, GL_FLOAT, GL_TRUE, 5*sizeof(GLfloat), (const GLvoid*)(3*sizeof(GLfloat)));


    glBindVertexArray(0);

    while(glfwGetWindowParam(GLFW_OPENED)){
        display();
    }
    glfwTerminate();

    return 0;

}
