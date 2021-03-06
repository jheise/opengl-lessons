//opengl libs
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//C++ libs
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

//Jpeg libs
#include <jpeglib.h>
#include <jerror.h>

//Camera
#include "Camera.h"

struct Light {
    glm::vec3 position;
    glm::vec3 intensities;
};

Camera camera;

GLuint cubeVBO = 0;
GLuint cubeVAO = 0;

GLuint shaderprogram = 0;

GLuint vertshader;
GLuint fragshader;
bool doRotate = true;

GLuint texture;
//GLuint umvp = -1;
GLuint ucamera = -1;
GLuint umodel = -1;
GLuint ulightpos = -1;
GLuint ulightint = -1;

GLfloat screenwidth = 800.0f;
GLfloat screenheight = 600.0f;

GLfloat degree = 0.0f;

GLfloat data[] = {
    //  X     Y     Z       U     V     normal
    // bottom
    -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
    -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
     1.0f,-1.0f, 1.0f,   1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
    -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,

    // top
    -1.0f, 1.0f,-1.0f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
     1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
     1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
     1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,

    // front
    -1.0f,-1.0f, 1.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
     1.0f,-1.0f, 1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
     1.0f,-1.0f, 1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
     1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,

    // back
    -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
    -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
    -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
     1.0f, 1.0f,-1.0f,   1.0f, 1.0f,   0.0f, 0.0f, -1.0f,

    // left
    -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
    -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
    -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,

    // right
     1.0f,-1.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     1.0f, 1.0f,-1.0f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     1.0f,-1.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
     1.0f, 1.0f,-1.0f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
     1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f
};

double lastTime = glfwGetTime();
double thisTime = lastTime;
float diff = 0;
const float moveSpeed = 4.0;

Light light;

struct imgdata {
    int height;
    int width;
    unsigned char* data;
};

void glPrintError(){

    GLenum errorCode = glGetError();

    if (errorCode != GL_NO_ERROR)
    {
        std::string error = "unknown error";
        std::string description  = "no description";

        // Decode the error code
        switch (errorCode)
        {
            case GL_INVALID_ENUM :
            {
                error = "GL_INVALID_ENUM";
                description = "an unacceptable value has been specified for an enumerated argument";
                break;
            }

            case GL_INVALID_VALUE :
            {
                error = "GL_INVALID_VALUE";
                description = "a numeric argument is out of range";
                break;
            }

            case GL_INVALID_OPERATION :
            {
                error = "GL_INVALID_OPERATION";
                description = "the specified operation is not allowed in the current state";
                break;
            }

            case GL_STACK_OVERFLOW :
            {
                error = "GL_STACK_OVERFLOW";
                description = "this command would cause a stack overflow";
                break;
            }

            case GL_STACK_UNDERFLOW :
            {
                error = "GL_STACK_UNDERFLOW";
                description = "this command would cause a stack underflow";
                break;
            }
            case GL_OUT_OF_MEMORY :
            {
                error = "GL_OUT_OF_MEMORY";
                description = "there is not enough memory left to execute the command";
                break;
            }

            case GL_INVALID_FRAMEBUFFER_OPERATION_EXT :
            {
                error = "GL_INVALID_FRAMEBUFFER_OPERATION_EXT";
                description = "the object bound to FRAMEBUFFER_BINDING_EXT is not \"framebuffer complete\"";
                break;
            }
        }
        std::cerr << "OpenGL Error: "  << error << ", " << description << std::endl;
    }
}

void update(){
    //update time

    thisTime = glfwGetTime();
    diff = thisTime - lastTime;
    if(doRotate){
        degree += diff * 180.0f;
        if(degree > 360.0f){
            degree -= 360.0f;
        }
    }
    lastTime = thisTime;

    //handle keys
    //forward, back
    if(glfwGetKey('S')){
        camera.offsetPosition( diff * moveSpeed * -camera.forward());
    } else if(glfwGetKey('W')){
        camera.offsetPosition( diff * moveSpeed * camera.forward());
    }

    //left, right
    if(glfwGetKey('A')){
        camera.offsetPosition( diff * moveSpeed * -camera.right());
    } else if(glfwGetKey('D')){
        camera.offsetPosition( diff * moveSpeed * camera.right());
    }

    //up, down
    if(glfwGetKey('Z')){
        camera.offsetPosition( diff * moveSpeed * -glm::vec3(0,1,0));
    }else if(glfwGetKey('X')){
        camera.offsetPosition( diff * moveSpeed * glm::vec3(0,1,0));
    }

    if(glfwGetKey(GLFW_KEY_SPACE)){
        doRotate = false;
    }else{
        doRotate = true;
    }

    if(glfwGetKey('1')){
        light.intensities = glm::vec3(1,1,1);
    }

    if(glfwGetKey('2')){
        light.intensities = glm::vec3(1,0,0);
    }

    if(glfwGetKey('3')){
        light.intensities = glm::vec3(0,1,0);
    }

    if(glfwGetKey('4')){
        light.intensities = glm::vec3(0,0,1);
    }

    if(glfwGetKey('5')){
        light.intensities = glm::vec3(0,0,0);
    }

    const float mouseSensitivity = 0.1;
    int mouseX, mouseY;
    glfwGetMousePos(&mouseX, &mouseY);
    camera.offsetOrientation(mouseSensitivity * mouseY, mouseSensitivity * mouseX);
    glfwSetMousePos(0, 0);

    const float zoomSensitivity = -0.2;
    float fieldOfView = camera.fieldOfView() + zoomSensitivity * (float)glfwGetMouseWheel();
    if(fieldOfView < 5.0f) fieldOfView = 5.0f;
    if(fieldOfView > 130.0f) fieldOfView = 130.0f;
    camera.setFieldOfView(fieldOfView);
    glfwSetMouseWheel(0);

}

void display(){

    glClearColor(1,1,1,1); //white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //load program to use
    glUseProgram(shaderprogram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    //load vertex array to use
    glBindVertexArray(cubeVAO);

    //setup first cube
    glm::mat4 view = camera.matrix();
    glUniformMatrix4fv(ucamera, 1, GL_FALSE, &view[0][0]);

    glUniform3fv(ulightpos, 1, &light.position[0]);
    glUniform3fv(ulightint, 1, &light.intensities[0]);

    //add rotation
    glm::mat4 model = glm::rotate(glm::mat4(), degree, glm::vec3(0,1,1));
    glUniformMatrix4fv(umodel, 1, GL_FALSE, &model[0][0]);


    light.intensities = glm::vec3(1,0,0);
    glUniform3fv(ulightint, 1, &light.intensities[0]);
    //draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);

    //setup second cube
    model = glm::rotate(glm::mat4(), degree, glm::vec3(0,1,0));
    model = glm::translate(model, glm::vec3(-4,0,0));
    glUniformMatrix4fv(umodel, 1, GL_FALSE, &model[0][0]);
    light.intensities = glm::vec3(0,1,0);
    glUniform3fv(ulightint, 1, &light.intensities[0]);

    glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);

    //setup third cube
    model = glm::rotate(glm::mat4(), degree, glm::vec3(0,0,1));
    model = glm::translate(model, glm::vec3(4,0,0));
    glUniformMatrix4fv(umodel, 1, GL_FALSE, &model[0][0]);
    light.intensities = glm::vec3(0,0,1);
    glUniform3fv(ulightint, 1, &light.intensities[0]);

    glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);

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

imgdata loadJpeg( const char* filename){
    unsigned char a,r,g,b;
    int img_width, img_height;

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE *file;
    JSAMPARRAY pJpegBuffer;
    int row_stride;

    std::string image_name(filename);
    file = fopen(image_name.c_str(), "rb");
    if( file == NULL){
        std::string msg("Error opening ");
        throw std::runtime_error(msg + image_name);
    }
    printf("Loading in files %s\n", image_name.c_str());

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);
    img_width  = cinfo.image_width;
    img_height = cinfo.image_height;
    printf("img_width %d img_height %d\n", img_width, img_height);

    unsigned char *img_data = new unsigned char [4 * img_height * img_width];
    unsigned char *pTest = img_data;
    if(!img_data){
        std::string msg("NO MEM FOR JPEG CONVERT!\n");
        throw std::runtime_error(msg);
    }

    row_stride = img_width * cinfo.output_components;
    pJpegBuffer = (*cinfo.mem->alloc_sarray)
      ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    while( cinfo.output_scanline < cinfo.output_height){
        jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
        for( int x=0; x<img_width; x++){
            a = 0;
            r = pJpegBuffer[0][cinfo.output_components*x];
            if( cinfo.output_components > 2){
                g = pJpegBuffer[0][cinfo.output_components*x+1];
                b = pJpegBuffer[0][cinfo.output_components*x+2];
            }else{
                g = r;
                b = r;
            }
            //printf( "r %c g %c b %c\n", r,g,b);
            *(img_data++) = r;
            *(img_data++) = g;
            *(img_data++) = b;
            *(img_data++) = a;
        }
    }
    fclose(file);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    imgdata retval;
    retval.height = img_height;
    retval.width = img_width;
    retval.data = pTest;

    return retval;
}

imgdata loadRaw(const char* filename, int height, int width){

    unsigned char* data;
    FILE *file;

    file = fopen(filename, "rb");
    if( file == NULL){
       throw std::runtime_error("error loading file");
    }

    data = (unsigned char*) malloc( width * height * 3);
    fread( data, width * height * 3, 1, file);
    fclose(file);

    imgdata retval;
    retval.height = height;
    retval.width = width;
    retval.data = data;

    return retval;
}

void PrepVertexUniforms(){

    ucamera = glGetUniformLocation(shaderprogram, "camera");
    if( ucamera == -1){
        throw std::runtime_error(std::string("camera uniform not found"));
    }

    umodel = glGetUniformLocation(shaderprogram, "model");
    if( umodel == -1){
        throw std::runtime_error(std::string("model uniform not found"));
    }

    ulightpos = glGetUniformLocation(shaderprogram, "light.position");
    if( ulightpos == -1){
        throw std::runtime_error(std::string("light.position uniform not found"));
    }

    ulightint = glGetUniformLocation(shaderprogram, "light.intensities");
    if( ulightint == -1){
        throw std::runtime_error(std::string("light.intensities uniform not found"));
    }

}

void LoadTexture(){
    glGenTextures( 1, &texture);
    glBindTexture( GL_TEXTURE_2D, texture);

    imgdata image_data = loadJpeg("wooden-crate.jpg");

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_data.width, image_data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data.data);
    //free( imgdata );

    GLuint utex= glGetUniformLocation(shaderprogram, "tex");
    if(utex == -1){
        throw std::runtime_error(std::string("program uniform not found: tex"));
    }
    glUniform1i(utex, texture);

}

void LoadGeometry(){
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
    GLint verttexprog = glGetAttribLocation(shaderprogram,"vertTexCoord");
    GLint vertnormprog = glGetAttribLocation(shaderprogram,"vertNormal");

    //bind VAO
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    //bind VBO
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);


    glEnableVertexAttribArray(vertprog);
    glVertexAttribPointer(vertprog, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), NULL);

    glEnableVertexAttribArray(verttexprog);
    glVertexAttribPointer(verttexprog, 2, GL_FLOAT, GL_TRUE, 8*sizeof(GLfloat), (const GLvoid*)(3*sizeof(GLfloat)));

    glEnableVertexAttribArray(vertnormprog);
    glVertexAttribPointer(vertnormprog, 3, GL_FLOAT, GL_TRUE, 8*sizeof(GLfloat), (const GLvoid*)(5*sizeof(GLfloat)));


    glBindVertexArray(0);

}

int main(int argc, char *argv[]){

    if(!glfwInit()){
        throw std::runtime_error("glfwInit failed");
    }
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(0, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    if(!glfwOpenWindow(800, 600, 0, 0, 0, 0, 32, 0, GLFW_WINDOW)){
        throw std::runtime_error("glfwOpenWindow failed. does this hardware work with 3.1");
    }

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        throw std::runtime_error("glewInit failed");
    }

    if(!GLEW_VERSION_3_1){
        throw std::runtime_error("OpenGL 3.1 API is not available");
    }

    LoadGeometry();
    LoadTexture();
    PrepVertexUniforms();

    camera.setPosition(glm::vec3(0,0,8));
    camera.setViewportAspectRatio(screenwidth/screenheight);

    light.position = camera.position();
    light.intensities = glm::vec3(1,1,1);

    glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos(0, 0);
    glfwSetMouseWheel(0);


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    while(glfwGetWindowParam(GLFW_OPENED)){

        update();
        display();

        //check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR){
            glPrintError();
        }

        //esc == close
        if(glfwGetKey(GLFW_KEY_ESC)){
            glfwCloseWindow();
        }
    }
    glfwTerminate();

    return 0;

}
