#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <jpeglib.h>
#include <jerror.h>

GLuint cubeVBO = 0;
GLuint cubeVAO = 0;

GLuint shaderprogram = 0;

GLuint vertshader;
GLuint fragshader;

GLuint texture;

GLint uniform;

const float NUM_OF_VERTICES_IN_DATA=3;
 /* Vertices of a triangle (counter-clockwise winding) */
//GLfloat data[] = {
    ////X   Y    Z     U    V
    //0.0f, 0.8f, 0.0f,      0.5f, 1.0f,
    //-0.8f, -0.8f, 0.0f,    0.0f, 0.0f,
    //0.8f, -0.8f, 0.0f,     1.0f, 0.0f
//};
GLfloat data[] = {
    0.8f, 0.8f, 0.0f, 2.0f, 2.0f,
    -0.8f, 0.8f, 0.0f, 0.0f, 2.0f,
    -0.8f, -0.8f, 0.0f, 0.0f, 0.0f,
    -0.8f, -0.8f, 0.0f, 0.0f, 0.0f,
    0.8f, -0.8f, 0.0f, 2.0f, 0.0f,
    0.8f, 0.8f, 0.0f, 2.0f, 2.0f
};
struct imgdata {
    int height;
    int width;
    unsigned char* data;
};

void keyboard(unsigned char key, int x, int y){

    if( key == 'q' || key == 'Q' || key ==27){
        exit(0);
    }
}


void display(void){

    glClearColor( 1.f, 0.f, 0.f, 0.f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT );

    //load program to use
    glUseProgram(shaderprogram);

    if(uniform == -1){
        throw std::runtime_error(std::string("program uniform not found: tex"));
    }

    // bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(uniform, 0);

    //load vertex array to use
    glBindVertexArray(cubeVAO);

    //draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 6);

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

void reshape (int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

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
    GLint verttexprog = glGetAttribLocation(shaderprogram,"vertTexCoord");

    //bind VAO
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    //bind VBO
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);


    glEnableVertexAttribArray(vertprog);
    glVertexAttribPointer(vertprog, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);

    glEnableVertexAttribArray(verttexprog);
    glVertexAttribPointer(verttexprog, 2, GL_FLOAT, GL_TRUE, 5*sizeof(GLfloat), (const GLvoid*)(3*sizeof(GLfloat)));

    uniform = glGetUniformLocation(shaderprogram, "tex");

    glBindVertexArray(0);

    glGenTextures( 1, &texture);
    glBindTexture( GL_TEXTURE_2D, texture);

    imgdata image_data = loadJpeg("new.jpg");
    //imgdata image_data = loadRaw("texture.bmp", 256, 256);


    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data.data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_data.width, image_data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data.data);
    //free( imgdata );

    while(glfwGetWindowParam(GLFW_OPENED)){
        display();
    }
    glfwTerminate();

    return 0;

}
