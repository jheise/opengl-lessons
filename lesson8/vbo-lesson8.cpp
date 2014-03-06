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

//text processing
#include <ft2build.h>
#include FT_FREETYPE_H

struct Light {
    glm::vec3 position;
    glm::vec3 intensities;
};

GLfloat green[3] = { 0.0f, 1.0f, 0.0f };
GLfloat red[3] = {1.0f, 0.0f, 0.0f};
GLfloat blue[3] = {0.0f, 0.0f, 1.0f};

Camera camera;

GLuint cubeVBO = 0;
GLuint cubeVAO = 0;

GLuint overlayVBO = 0;
GLuint overlayVAO = 0;

GLuint textVBO = 0;
GLuint textVAO = 0;

GLuint shader3dprogram = 0;
GLuint shader2dprogram = 0;
GLuint textprogram = 0;
GLint textprog = -1;

GLuint vertshader3d;
GLuint fragshader3d;
GLuint vertshader2d;
GLuint fragshader2d;
GLuint textvertshader;
GLuint textfragshader;

int frames = 0;
int fps = 0;
bool doRotate = true;

GLuint texture;
GLuint ucamera = -1;
GLuint umodel = -1;
GLuint ulightpos = -1;
GLuint ulightint = -1;

GLuint uoverlaycolor = -1;
GLuint uoverlaytransparency = -1;

GLuint texttex;
GLuint utextcolor = -1;
GLuint utexttransparency = -1;

GLfloat screenwidth = 800.0f;
GLfloat screenheight = 600.0f;

GLfloat degree = 0.0f;

//GLfloat overlay[] = {
    ////X    Y     Z
    //// front
    //0.0f, 0.975f, 0.1f,
    //-0.975f, 0.975f, 0.1f,
    //-0.975f, 0.0f, 0.1f,
    //0.0f, 0.975f, 0.1f,
    //-0.975f, 0.0f, 0.1f,
    //0.0f, 0.0f, 0.1f
//};

GLfloat overlay[] = {
    //X    Y     Z
    // front
    //0.0f, 0.975f, 0.1f,
    -0.975f, 0.975f, 0.1f,
    -0.975f, 0.0f, 0.1f,
    0.0f, 0.975f, 0.1f,
    //-0.975f, 0.0f, 0.1f,
    0.0f, 0.0f, 0.1f
};

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
double seconds = 0;
float diff = 0;
const float moveSpeed = 4.0;

Light light;

struct imgdata {
    int height;
    int width;
    unsigned char* data;
};

FT_Library ft;
FT_Face face;
FT_GlyphSlot g;

void cleanup(){
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

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

void CheckStatus(GLuint program){
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE){
        std::string msg("Program linking failure: ");

        GLint infologlen;
        glGetProgramiv(shader3dprogram, GL_INFO_LOG_LENGTH, &infologlen);
        char* strinfolog = new char[infologlen + 1];
        glGetProgramInfoLog(shader3dprogram, infologlen, NULL, strinfolog);
        msg += strinfolog;
        delete[] strinfolog;

        glDeleteProgram(shader3dprogram);
        throw std::runtime_error(msg);
    }
}

GLuint handleShader(const std::string& shaderpath, GLenum shaderType){

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

    return shader;
}

GLuint AssembleProgram( const std::string& vert_path, const std::string& frag_path){
    GLuint vertshader = handleShader(vert_path, GL_VERTEX_SHADER);
    GLuint fragshader = handleShader(frag_path, GL_FRAGMENT_SHADER);
    GLuint shaderprog = glCreateProgram();
    if(shaderprog == 0){
        throw std::runtime_error("glCreateProgram failed");
    }
    glAttachShader(shaderprog, vertshader);
    glAttachShader(shaderprog, fragshader);
    glLinkProgram(shaderprog);
    CheckStatus(shaderprog);

    return shaderprog;
}

void init_font(){
    printf("Starting Font Load...\t");
    if(FT_Init_FreeType(&ft)){
        throw std::runtime_error(std::string("failed to initalize freetype"));
    }

    if(FT_New_Face(ft, "/usr/share/fonts/truetype/freefont/FreeSans.ttf", 0, &face)){
        throw std::runtime_error(std::string("failed to initialize font"));
    }

    FT_Set_Pixel_Sizes(face, 0, 24);

    g = face->glyph;


    //textprog = -1;

    //setup text
    glGenVertexArrays(1, &textVAO);
    glBindVertexArray(textVAO);
    glGenBuffers(1, &textVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);

    textprogram = AssembleProgram("text-vertex.shader", "text-fragment.shader");
    textprog = glGetAttribLocation(textprogram, "coord");
    glEnableVertexAttribArray(textprog);
    glVertexAttribPointer(textprog, 4, GL_FLOAT, GL_FALSE,4*sizeof(GLfloat),NULL);

    //glBindVertexArray(0);

    glGenTextures( 1, &texttex);

    GLuint utext = glGetUniformLocation(textprogram, "tex");
    if(utext == -1){
        throw std::runtime_error(std::string("program uniform not found: tex"));
    }
    glUniform1i(utext, texttex);

    cleanup();

    printf("Done.\n");
}


void drawChars( const char* characters, float x1, float y1, float scalex, float scaley ){

    glBindTexture(GL_TEXTURE_2D, texttex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );

    const char* p;

    for(p=characters; *p; p++){
        if(FT_Load_Char(face, *p, FT_LOAD_RENDER)){
            printf("error with loading character %c\n", *p);
            exit(1);
        }

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            //GL_RGBA,
            GL_RED,
            g->bitmap.width,
            g->bitmap.rows,
            0,
            //GL_RGBA,
            GL_RED,
            GL_UNSIGNED_BYTE,
            g->bitmap.buffer
        );

        //setup spaces
        float xlength = g->bitmap.width * scalex;
        float ylength = g->bitmap.rows * scaley;
        float x2 = x1 + g->bitmap_left * scalex;
        float y2 = y1 + g->bitmap_top  * scaley;

        //setup x vertices
        float xLeft  = x2;
        float xRight = x2 + xlength;

        //setup y vertices
        float yHigh  = y2;
        float yLow   = y2 - ylength;

        GLfloat newbox[] = {
            xLeft,  yHigh, 0,0,
            xLeft,  yLow,  0,1,
            xRight, yHigh, 1,0,
            xRight, yLow,  1,1
        };


        glUseProgram(textprogram);

        glBindVertexArray(textVAO);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(newbox), newbox, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        x1 += ( g->advance.x >> 6) * scalex;
        y1 += ( g->advance.y >> 6) * scaley;
    }

    //clean up
    cleanup();
}

void drawOverlay(float x, float y, float lengthx, float lengthy, float layer, float *color, float transparency){

    float xLeft = x;
    float xRight = x + lengthx;
    float yHigh = y;
    float yLow = y - lengthy;

    //GLfloat newoverlay[] = {
        ////X    Y     Z
        //-0.975f, 0.975f, 0.1f,
        //-0.975f, 0.0f, 0.1f,
        //0.0f, 0.975f, 0.1f,
        //0.0f, 0.0f, 0.1f
    //};
    GLfloat newoverlay[] = {
        xLeft,  yHigh, layer,
        xLeft,  yLow,  layer,
        xRight, yHigh, layer,
        xRight, yLow,  layer
    };

    glBindBuffer(GL_ARRAY_BUFFER, overlayVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(newoverlay), newoverlay, GL_DYNAMIC_DRAW);

    glUseProgram(shader2dprogram);
    glBindVertexArray(overlayVAO);
    glUniform3fv(uoverlaycolor, 1, color);
    glUniform1f(uoverlaytransparency, transparency);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //clean up
    cleanup();
}


void update(){
    //update time

    frames++;
    thisTime = glfwGetTime();
    diff = thisTime - lastTime;
    if(doRotate){
        degree += diff * 180.0f;
        if(degree > 360.0f){
            degree -= 360.0f;
        }
    }
    lastTime = thisTime;
    seconds += diff;
    if( seconds > 1.0){

        fps = frames;
        frames = 0;
        seconds = 0;
    }


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
    //printf("starting diplay loop\n");

    glClearColor(1,1,1,1); //white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //printf("screen cleared\n");

    //load program to use
    glUseProgram(shader3dprogram);

    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    //load vertex array to use
    glBindVertexArray(cubeVAO);

    ////setup first cube
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

    ////setup second cube
    model = glm::rotate(glm::mat4(), degree, glm::vec3(0,1,0));
    model = glm::translate(model, glm::vec3(-4,0,0));
    glUniformMatrix4fv(umodel, 1, GL_FALSE, &model[0][0]);
    light.intensities = glm::vec3(0,1,0);
    glUniform3fv(ulightint, 1, &light.intensities[0]);

    glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);

    ////setup third cube
    model = glm::rotate(glm::mat4(), degree, glm::vec3(0,0,1));
    model = glm::translate(model, glm::vec3(4,0,0));
    glUniformMatrix4fv(umodel, 1, GL_FALSE, &model[0][0]);
    light.intensities = glm::vec3(0,0,1);
    glUniform3fv(ulightint, 1, &light.intensities[0]);

    glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);

    //glBindVertexArray(0);
    //glUseProgram(0);
    cleanup();


    //draw overlay
    //drawOverlay(0.0f, 0.0f, 0.5f, 0.5f, green, 0.3f);
    drawOverlay(-0.975f, 0.975f, 0.975f, 0.975f, 0.2, blue, 0.3f);
    drawOverlay(-0.975f, 0.975f, 0.75f, 0.975f, 0.1, red, 0.9f);
    //drawOverlay(0.0f, 0.0f, 0.5f, 0.5f, red, 0.3f);

    char framerate[80];
    sprintf(framerate, "Frames per second: %d", fps);

    drawChars("Hello World!", -0.95,0.83f, 2.0/screenwidth, 2.0/screenheight);
    drawChars(framerate, -0.95,0.70f, 2.0/screenwidth, 2.0/screenheight);
    drawChars("abcdefghijklmnopqrstuvmxyz", -0.95, 0.0f, 2.0/screenwidth, 2.0/screenheight);


    //unbind for next pass
    cleanup();

    glfwSwapBuffers();

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
    //printf("Loading in files %s\n", image_name.c_str());

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);
    img_width  = cinfo.image_width;
    img_height = cinfo.image_height;
    //printf("img_width %d img_height %d\n", img_width, img_height);

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

    ucamera = glGetUniformLocation(shader3dprogram, "camera");
    if( ucamera == -1){
        throw std::runtime_error(std::string("camera uniform not found"));
    }

    umodel = glGetUniformLocation(shader3dprogram, "model");
    if( umodel == -1){
        throw std::runtime_error(std::string("model uniform not found"));
    }

    ulightpos = glGetUniformLocation(shader3dprogram, "light.position");
    if( ulightpos == -1){
        throw std::runtime_error(std::string("light.position uniform not found"));
    }

    ulightint = glGetUniformLocation(shader3dprogram, "light.intensities");
    if( ulightint == -1){
        throw std::runtime_error(std::string("light.intensities uniform not found"));
    }

}

GLuint handleUniform(GLuint program, const char* uname){
    printf("going to handle a uniform for %s\n", uname);
    fflush(stdout);
    GLuint uniform = -1;
    uniform = glGetUniformLocation(program, uname);
    if( uniform == -1){
        //throw std::runtime_error(std::string(strcat("program uniform not found: ", uname)));
        throw std::runtime_error(std::string("program uniform not found: "));
    }
    return uniform;
}

void LoadTexture(){
    printf("Loading Textures...\t");
    fflush(stdout);
    glGenTextures( 1, &texture);
    glBindTexture( GL_TEXTURE_2D, texture);

    GLuint utex= glGetUniformLocation(shader3dprogram, "tex");
    if(utex == -1){
        throw std::runtime_error(std::string("program uniform not found: tex"));
    }
    glUniform1i(utex, texture);

    imgdata image_data = loadJpeg("wooden-crate.jpg");

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_data.width, image_data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data.data);

    uoverlaycolor = handleUniform(shader2dprogram, "color");
    uoverlaytransparency = handleUniform(shader2dprogram, "trans");


    printf("Done.\n");

}


void LoadGeometry(){
    printf("Loading Geometry...\t");
    /* seutp 3d shader */
    shader3dprogram = AssembleProgram("3d-vertex.shader", "3d-fragment.shader");

    GLint vertprog = glGetAttribLocation(shader3dprogram,"vert");
    GLint verttexprog = glGetAttribLocation(shader3dprogram,"vertTexCoord");
    GLint vertnormprog = glGetAttribLocation(shader3dprogram,"vertNormal");

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

    /* setup 2d shaders */
    shader2dprogram = AssembleProgram("2d-vertex.shader", "2d-fragment.shader");

    GLint overlayprog = glGetAttribLocation(shader2dprogram, "vert");

    //setup overlay
    glGenVertexArrays(1, &overlayVAO);
    glBindVertexArray(overlayVAO);
    glGenBuffers(1, &overlayVBO);
    glBindBuffer(GL_ARRAY_BUFFER, overlayVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(overlay), overlay, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(overlay), overlay, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(overlayprog);
    glVertexAttribPointer(overlayprog, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat),NULL);

    glBindVertexArray(0);

    printf("Done.\n");

}

void init_camera(){
    printf("Initializing Camera...\t");
    camera.setPosition(glm::vec3(0,0,8));
    camera.setViewportAspectRatio(screenwidth/screenheight);

    light.position = camera.position();
    light.intensities = glm::vec3(1,1,1);

    glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos(0, 0);
    glfwSetMouseWheel(0);

    printf("Done.\n");
}

void init_gl(){
    printf("Enabling GL Tests...\t");
    //turn on depth test so things dont look stupid
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    printf("Done.\n");
}

void init_engine(){
    printf("Running...\t\t");
    fflush(stdout);
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
    printf("Done.\n");
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
    if(!glfwOpenWindow(screenwidth, screenheight, 0, 0, 0, 0, 32, 0, GLFW_WINDOW)){
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
    init_font();
    init_camera();
    init_gl();

    init_engine();
    return 0;
}
