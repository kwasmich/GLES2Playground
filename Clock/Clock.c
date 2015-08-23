//
//  FlyingCubes.c
//  GLESCompute
//
//  Created by Michael Kwasnicki on 06.12.13.
//
//

#include "../playground.h"

#include "../colorspace.h"
#include "../globaltime.h"
#include "../Math3D.h"
#include "../OpenGLES2Core.h"
#include "../noise.h"

#include <assert.h>
#include <iso646.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <execinfo.h>

#define DEBUG 1

#define free_s( X ) free( X ); X = NULL
#define error() if ( ( s_error = glGetError() ) != GL_NO_ERROR ) { fprintf( stderr, "%i\n", s_error ); assert( s_error == GL_NO_ERROR ); }

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define BUFFER_OFFSET2(x, i) ((char *)x + (i))

#define glDrawArrays( ... ) glDrawArrays( __VA_ARGS__ ); error();

static GLuint s_error;


typedef struct {
    GLfloat x, y;
    GLubyte r, g, b, a;
    GLubyte s, t, u, v; // padding for 32bit alignment
} VertexData_t;


static VertexData_t s_vertices[4] = {
    {    0,    0, 255,   0,   0, 255, 0, 1, 0, 0 },
    {    0, 1024,   0, 255,   0, 255, 0, 0, 0, 0 },
    { 1024,    0,   0,   0, 255, 255, 1, 1, 0, 0 },
    { 1024, 1024, 255, 255, 255, 255, 1, 0, 0, 0 },
};

static vec4 s_clockArrows;


// OpenGL|ES state
static Shader_t s_shaderClock;
static GLuint s_textureClockBase;
static GLuint s_textureClockHour;
static GLuint s_textureClockMinute;
static GLuint s_textureClockLine;
static GLuint s_textureClockArrow;

static GLuint s_screenWidth = 0;
static GLuint s_screenHeight = 0;
static GLfloat s_screenAspect = 1;


static void loadShaders(void);
static void unloadShaders(void);
static void loadTextures(void);
static void unloadTextures(void);


static void init(const int in_WIDTH, const int in_HEIGHT) {
    s_screenWidth = in_WIDTH;
    s_screenHeight = in_HEIGHT;
    s_screenAspect = in_WIDTH / (float)in_HEIGHT;

    mat4 projectionMatrix = mat4MakeOrtho(0, s_screenWidth, 0, s_screenHeight, 0, 1);

    loadShaders();
    loadTextures();

    glClearColor(0.0f, 0.5f, 0.0f, 0.75f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_textureClockBase);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, s_textureClockHour);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, s_textureClockMinute);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, s_textureClockLine);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, s_textureClockArrow);

    glUseProgram(s_shaderClock.programObject);
    glUniformMatrix4fv(s_shaderClock.uniformLocations[UNIFORM_PROJECTION_MATRIX], 1, 0, projectionMatrix.m);
    glUniform1iv(s_shaderClock.uniformLocations[UNIFORM_TEXTURE], 5, (GLint[5]) {
        0, 1, 2, 3, 4
    });
    error();
}



static void update() {
    glClearColor(0.1f + drand48() * 0.1f, 0.1f + drand48() * 0.1f, 0.1f + drand48() * 0.1f, 0.5f);

    struct timeval now;
    gettimeofday(&now, NULL);

    struct tm *localNow = localtime(&(now.tv_sec));
    assert(localNow);

    float s = localNow->tm_sec + now.tv_usec / 1000000.0f;
    float m = localNow->tm_min + s / 60.0f;
    float h = localNow->tm_hour + m / 60.0f;

    s = s * 2 * M_PI / 60.0f;
    m = m * 2 * M_PI / 60.0f;
    h = h * 2 * M_PI / 12.0f;

    s_clockArrows = VEC4(M_PI_2 - h, -m, M_PI_2 - s, M_PI_2 - s);
    //s_clockArrows = VEC4( 0, 0, 0, 0 );

}



static void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(s_shaderClock.programObject);
    glEnableVertexAttribArray(s_shaderClock.attribLocations[ATTRIB_POSITION]);
    glEnableVertexAttribArray(s_shaderClock.attribLocations[ATTRIB_COLOR]);
    glEnableVertexAttribArray(s_shaderClock.attribLocations[ATTRIB_TEX_COORD]);
    error();

    //    glBindTexture( GL_TEXTURE_2D, s_textureClockBase );
    glUniform4fv(s_shaderClock.uniformLocations[UNIFORM_VEC4], 1, s_clockArrows.v);
    glVertexAttribPointer(s_shaderClock.attribLocations[ATTRIB_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2(s_vertices, 0));
    glVertexAttribPointer(s_shaderClock.attribLocations[ATTRIB_COLOR], 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData_t), BUFFER_OFFSET2(s_vertices, 8));
    glVertexAttribPointer(s_shaderClock.attribLocations[ATTRIB_TEX_COORD], 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2(s_vertices, 12));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    error();
}



static void deinit() {
    unloadTextures();
    unloadShaders();
    error();
}



#pragma mark - Shaders



static void loadShaders() {
    GLuint vertShaderObject = createShaderObject("Clock/Assets/Shaders/Clock.vsh", GL_VERTEX_SHADER);
    error();
    GLuint fragShaderObject = createShaderObject("Clock/Assets/Shaders/Clock.fsh", GL_FRAGMENT_SHADER);
    error();

    glReleaseShaderCompiler();

    s_shaderClock = createProgramObject(vertShaderObject, fragShaderObject);

    glDeleteShader(vertShaderObject);
    glDeleteShader(fragShaderObject);
}



static void unloadShaders() {
    fputs("unloadShaders\n", stderr);
    glDeleteProgram(s_shaderClock.programObject);
    s_shaderClock.programObject = 0;
    error();
}



#pragma mark - Textures



static void loadTextures() {
    s_textureClockBase = createTextureObject("Clock/Assets/Textures/Base.raw", GL_LUMINANCE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    s_textureClockHour = createTextureObject("Clock/Assets/Textures/Hour.raw", GL_LUMINANCE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    s_textureClockMinute = createTextureObject("Clock/Assets/Textures/Minute.raw", GL_LUMINANCE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    s_textureClockLine = createTextureObject("Clock/Assets/Textures/Line.raw", GL_LUMINANCE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    s_textureClockArrow = createTextureObject("Clock/Assets/Textures/Arrow.raw", GL_LUMINANCE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}



static void unloadTextures() {
    fputs("unloadTextures\n", stderr);
    glDeleteTextures(1, &s_textureClockBase);
    glDeleteTextures(1, &s_textureClockHour);
    glDeleteTextures(1, &s_textureClockMinute);
    glDeleteTextures(1, &s_textureClockLine);
    glDeleteTextures(1, &s_textureClockArrow);
    s_textureClockBase = 0;
    s_textureClockHour = 0;
    s_textureClockMinute = 0;
    s_textureClockLine = 0;
    s_textureClockArrow = 0;
    error();
}



#pragma mark - public





#pragma mark - exposed interface



GLES2Playground_t e_playgroundClock = {
    .name = "Clock",
    .init = init,
    .deinit = deinit,
    .update = update,
    .draw = draw,
};
