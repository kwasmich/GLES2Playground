//
//  FlyingCubes.c
//  GLESCompute
//
//  Created by Michael Kwasnicki on 06.12.13.
//
//

#include "playgrounds/playground.h"

#include "Math3D.h"
#include "OpenGLES2Core.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#define DEBUG 1

#define free_s( X ) free( X ); X = NULL
#define error() assert( glGetError() == GL_NO_ERROR );

#define BUFFER_OFFSET(i) ((char *)NULL + (i))



static GLfloat gCubeVertexData[216] = {
    // Data layout for each line below is:
    // positionX, positionY, positionZ,     normalX, normalY, normalZ,
    0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,          1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,         1.0f, 0.0f, 0.0f,

    0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 1.0f, 0.0f,

    -0.5f, 0.5f, -0.5f,        -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,       0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, -1.0f, 0.0f,

    0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, 0.0f, 1.0f,

    0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 0.0f, -1.0f
};

static Shader_t s_activeShader;
static GLuint _vertexBuffer;
static GLfloat _rotation;
static mat4 _modelViewProjectionMatrix;
static mat3 _normalMatrix;


static void loadShaders(void);
static void unloadShaders(void);


static void setupGL() {
    loadShaders();

    glEnable(GL_DEPTH_TEST);

    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gCubeVertexData), gCubeVertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(s_activeShader.attribLocations[ATTRIB_POSITION]);
    glEnableVertexAttribArray(s_activeShader.attribLocations[ATTRIB_NORMAL]);
    glVertexAttribPointer(s_activeShader.attribLocations[ATTRIB_POSITION], 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(0));
    glVertexAttribPointer(s_activeShader.attribLocations[ATTRIB_NORMAL], 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(12));

    error();
}


static void update() {
    float aspect = fabsf(1920.f / 1200.f);
    mat4 projectionMatrix = mat4MakePerspective(65.0f * M_PI / 180.f, aspect, 0.1f, 100.0f);

    mat4 baseModelViewMatrix = mat4MakeTranslate(VEC3(0.0f, 0.0f, -4.0f));
    mat4 rotationMatrix = mat4MakeRotateY(_rotation);
    baseModelViewMatrix = mulm4m4(baseModelViewMatrix, rotationMatrix);

    // Compute the model view matrix for the object rendered with ES2
    mat4 modelViewMatrix = mat4MakeTranslate(VEC3(0.0f, 0.0f, 1.5f));
    rotationMatrix = mat4MakeRotate(_rotation, VEC3(1.0f, 1.0f, 1.0f));
    modelViewMatrix = mulm4m4(modelViewMatrix, rotationMatrix);
    modelViewMatrix = mulm4m4(baseModelViewMatrix, modelViewMatrix);

    _normalMatrix = mat3InverseTranspose(mat3FromMat4(modelViewMatrix));
    _modelViewProjectionMatrix = mulm4m4(projectionMatrix, modelViewMatrix);
    _rotation += 0.01f;
}


static void draw() {
    glClearColor(0.65f, 0.65f, 0.65f, 0.65f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object with ES2
    glUseProgram(s_activeShader.programObject);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

    glEnableVertexAttribArray(s_activeShader.attribLocations[ATTRIB_POSITION]);
    glEnableVertexAttribArray(s_activeShader.attribLocations[ATTRIB_NORMAL]);
    glVertexAttribPointer(s_activeShader.attribLocations[ATTRIB_POSITION], 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(0));
    glVertexAttribPointer(s_activeShader.attribLocations[ATTRIB_NORMAL], 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(12));

    glUniformMatrix4fv(s_activeShader.uniformLocations[UNIFORM_PROJECTION_MATRIX], 1, 0, _modelViewProjectionMatrix.m);
    glUniformMatrix3fv(s_activeShader.uniformLocations[UNIFORM_NORMAL_MATRIX], 1, 0, _normalMatrix.m);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    error();
}


static void tearDownGL() {
    glDeleteBuffers(1, &_vertexBuffer);
    unloadShaders();
    error();
}



#pragma mark - Shader



void loadShaders() {
    GLuint vertShaderObject = createShaderObject("playgrounds/FlyingCubes/Assets/Shaders/FlyingCubes.vsh", GL_VERTEX_SHADER);
    GLuint fragShaderObject = createShaderObject("playgrounds/FlyingCubes/Assets/Shaders/FlyingCubes.fsh", GL_FRAGMENT_SHADER);

    glReleaseShaderCompiler();

    s_activeShader = createProgramObject(vertShaderObject, fragShaderObject);

    glDeleteShader(vertShaderObject);
    glDeleteShader(fragShaderObject);
}


void unloadShaders() {
    glDeleteProgram(s_activeShader.programObject);
    s_activeShader.programObject = 0;
}




GLES2Playground_t e_playgroundFlyingCubes = {
    .name = "FlyingCubes",
    .init = setupGL,
    .deinit = tearDownGL,
    .update = update,
    .draw = draw
};



