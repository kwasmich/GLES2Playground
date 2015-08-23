//
//  OpenGLES2Core.h
//  GLESCompute
//
//  Created by Michael Kwasnicki on 06.12.13.
//
//

#ifndef GLESCompute_OpenGLES2Core_h
#define GLESCompute_OpenGLES2Core_h


#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


#include <stdbool.h>


typedef enum {
    UNIFORM_MODEL_MATRIX,
    UNIFORM_VIEW_MATRIX,
    UNIFORM_PROJECTION_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    UNIFORM_EC_LIGHT_POSITION,
    UNIFORM_COLOR,
    UNIFORM_TEXTURE,
    UNIFORM_TEXTURE_SIZE,
    UNIFORM_VEC4,
    NUM_UNIFORMS
} uniforms_t;

typedef enum {
    ATTRIB_POSITION,
    ATTRIB_NORMAL,
    ATTRIB_TANGENT,
    ATTRIB_BI_TANGENT,
    ATTRIB_COLOR,
    ATTRIB_TEX_COORD,
    NUM_ATTRIBS
} attributes_t;

typedef struct {
    GLuint programObject;
    GLuint uniformLocations[NUM_UNIFORMS];
    GLuint attribLocations[NUM_ATTRIBS];
} Shader_t;



Shader_t createProgramObject(const GLuint in_VERTEX_SHADER, const GLuint in_FRAGMENT_SHADER);
GLuint createShaderObject(const char *in_FILE_NAME, const GLenum in_SHADER_TYPE);

GLuint createTextureObject(const char *in_FILE_NAME, const GLenum in_INTERNAL_FORMAT);

char *createStringFromFileContents(const char *in_FILE_NAME);
char *glEnumToCString(const GLenum in_ENUM);

unsigned char *rawFromFileContents(const char *in_FILE_NAME, const bool in_ZERO_TERMINATE, int *out_fileSize);


#define glxPrintInteger(X) GLint x_##X; glGetIntegerv( X, &x_##X ); printf( "%-40s: %i\n", #X, x_##X );
#define glxPrintInteger2(X) GLint x_##X[2]; glGetIntegerv( X, x_##X ); printf( "%-40s: %i, %i\n", #X, x_##X[0], x_##X[1] );
#define glxPrintString(X) printf( "%-40s: %s\n", #X, (char*)glGetString( X ) );

#endif
