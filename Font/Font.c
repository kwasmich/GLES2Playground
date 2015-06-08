//
//  FlyingCubes.c
//  GLESCompute
//
//  Created by Michael Kwasnicki on 06.12.13.
//
//

#include "../playground.h"

#include "../colorspace.h"
#include "../FreeType3D.h"
#include "../globaltime.h"
#include "../Math3D.h"
#include "../noise.h"
#include "../OpenGLES2Core.h"

#include <assert.h>
#include <iso646.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <execinfo.h>

#define DEBUG 1

#define free_s( X ) free( X ); X = NULL
#define error() if ( ( s_error = glGetError() ) != GL_NO_ERROR ) { fprintf( stderr, "%i\n", s_error ); assert( s_error == GL_NO_ERROR ); }

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define BUFFER_OFFSET2(x, i) ((char *)x + (i))

#define glDrawArrays( ... ) glDrawArrays( __VA_ARGS__ ); error();

static GLuint s_error;

static uint16_t s_numFontPoints = 0;
static ft3dVertex_t * s_fontPoints = NULL;
static ft3dFontMap_t * s_fontMap = NULL;
static int s_fontMapSize = 0;
static char * s_string = NULL;
static uint32_t s_lifeTime = 0;
static const float LIFE_TIME = 5.0f;

// OpenGL|ES state
static Shader_t s_shaderFont;
static GLuint s_textureFont;

static GLuint s_screenWidth = 0;
static GLuint s_screenHeight = 0;
static GLfloat s_screenAspect = 1;


static void loadShaders( void );
static void unloadShaders( void );
static void loadTextures( void );
static void unloadTextures( void );


static void init( const int in_WIDTH, const int in_HEIGHT ) {
    s_screenWidth = in_WIDTH;
    s_screenHeight = in_HEIGHT;
    s_screenAspect = in_WIDTH / (float)in_HEIGHT;
    
    mat4 projectionMatrix = mat4MakeOrtho( 0, in_WIDTH, 0, in_HEIGHT, 0, 1 );
    
    loadShaders();
    loadTextures();
    
    int rawSize = 0;
    s_fontMap = (ft3dFontMap_t*)rawFromFileContents( "Font/Assets/Vera512.map", false, &rawSize );
    s_fontMapSize = rawSize / sizeof( ft3dFontMap_t );
    
    glClearColor( 0.0f, 0.5f, 0.0f, 0.75f );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, s_textureFont );
    
    glUseProgram( s_shaderFont.programObject );
    glUniformMatrix4fv( s_shaderFont.uniformLocations[UNIFORM_PROJECTION_MATRIX], 1, 0, projectionMatrix.m );
    glUniform1i( s_shaderFont.uniformLocations[UNIFORM_TEXTURE], 0 );
    glUniform1f( s_shaderFont.uniformLocations[UNIFORM_TEXTURE_SIZE], 512 );
    error();
}



static void update() {
    float time = timeGet();
    
    if ( s_numFontPoints and time > s_lifeTime ) {
        free_s( s_fontPoints );
        s_numFontPoints = 0;
    }
}



static void draw() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    if ( s_numFontPoints > 0 ) {
        glUseProgram( s_shaderFont.programObject );
        glEnableVertexAttribArray( s_shaderFont.attribLocations[ATTRIB_POSITION] );
        glEnableVertexAttribArray( s_shaderFont.attribLocations[ATTRIB_TEX_COORD] );
        error();
        
        glBindTexture( GL_TEXTURE_2D, s_textureFont );
        glVertexAttrib4f( s_shaderFont.attribLocations[ATTRIB_COLOR], 1, 1, 1, 1 );
        glVertexAttribPointer( s_shaderFont.attribLocations[ATTRIB_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(ft3dVertex_t), BUFFER_OFFSET2( s_fontPoints, 0 ) );
        glVertexAttribPointer( s_shaderFont.attribLocations[ATTRIB_TEX_COORD], 2, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(ft3dVertex_t), BUFFER_OFFSET2( s_fontPoints, 8 ) );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, s_numFontPoints );
        error();
    }
}



static void deinit() {
    unloadTextures();
    unloadShaders();
    free_s( s_fontMap );
    free_s( s_fontPoints );
    error();
}



#pragma mark - Shaders



static void loadShaders() {
    GLuint vertShaderObject = createShaderObject( "Font/Assets/Shaders/Font.vsh", GL_VERTEX_SHADER );
    error();
    GLuint fragShaderObject = createShaderObject( "Font/Assets/Shaders/Font.fsh", GL_FRAGMENT_SHADER );
    error();
    
    glReleaseShaderCompiler();
    
    s_shaderFont = createProgramObject( vertShaderObject, fragShaderObject );
    
    glDeleteShader( vertShaderObject );
    glDeleteShader( fragShaderObject );
}



static void unloadShaders() {
    fputs( "unloadShaders\n", stderr );
    glDeleteProgram( s_shaderFont.programObject );
    s_shaderFont.programObject = 0;
    error();
}



#pragma mark - Textures



static void loadTextures() {
    s_textureFont = createTextureObject( "Font/Assets/Textures/Vera512.raw", GL_LUMINANCE );
    //s_textureFont = createTextureObject( "Font/Assets/Textures/checkboard256.raw", GL_LUMINANCE );
    //s_textureFont = createTextureObject( "Arabesque/Assets/Textures/CircleMip.etc1", GL_ETC1_RGB8_OES );
}



static void unloadTextures() {
    fputs( "unloadTextures\n", stderr );
    glDeleteTextures( 1, &s_textureFont );
    s_textureFont = 0;
    error();
}



#pragma mark - public



static void setString( const char * in_STRING ) {
    if ( !s_fontMap ) return;
    
    free_s( s_string );
    const size_t size = strlen( in_STRING ) + 1;
    s_string = malloc( size * sizeof( char ) );
    strncpy( s_string, in_STRING, size );
    
    free_s( s_fontPoints );
    float width = 0;
    ft3dStringToVertexArray( &s_fontPoints, &s_numFontPoints, &width, s_fontMap, s_fontMapSize, s_string, 32, 528, 148, 4.0f, (RGBA_t){ 255, 255, 255, 255 }, false );
    s_lifeTime = timeGet() + LIFE_TIME;
}



#pragma mark - exposed interface



GLES2Playground_t e_playgroundFont = {
    .name = "Font",
    .init = init,
    .deinit = deinit,
    .update = update,
    .draw = draw,
    .setString = setString
};
