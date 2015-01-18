//
//  FlyingCubes.c
//  GLESCompute
//
//  Created by Michael Kwasnicki on 06.12.13.
//
//

#include "../playground.h"

#include "../Math3D.h"
#include "../OpenGLES2Core.h"
#include "../noise.h"
#include "../colorspace.h"

#include <assert.h>
#include <iso646.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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


typedef struct {
    uint8_t charCode;
    uint8_t bitMapWidth, bitMapHeight;
    uint16_t posX, posY;
    int8_t bitMapLeft, bitMapTop;
    uint16_t advance;
} FontMap_t;


static VertexData_t * s_points = NULL;
static FontMap_t * s_fontMap = NULL;
static char s_string[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //"abcdefghijklmnopqrstuvwxyz"; //"Lorem ipsum dolor sit amet, consetetur sadipscing elitr";
static uint8_t s_stringLength = 0;

// OpenGL|ES state
static Shader_t s_activeShader;
static GLuint s_fontTexture;

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
    
    printf( "%d x %d\n", in_WIDTH, in_HEIGHT );
    
    mat4 projectionMatrix = mat4MakeOrtho( 0, in_WIDTH, 0, in_HEIGHT, 0, 1 );
    
    loadShaders();
    loadTextures();
    
    int rawSize = 0;
    s_fontMap = (FontMap_t*)rawFromFileContents( "Font/Assets/Vera.map", false, &rawSize );
    
    glClearColor( 0.0f, 0.5f, 0.0f, 0.75f );
//    glEnable( GL_BLEND );
//    glBlendEquation( GL_FUNC_ADD );
    //glBlendFunc( GL_DST_ALPHA, GL_ZERO );
    //glBlendFunc( GL_ONE, GL_DST_ALPHA );
//    glBlendFuncSeparate( GL_ONE, GL_ONE, GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, s_fontTexture );
    
    glUseProgram( s_activeShader.programObject );
    glUniformMatrix4fv( s_activeShader.uniformLocations[UNIFORM_PROJECTION_MATRIX], 1, 0, projectionMatrix.m );
    glUniform1i( s_activeShader.uniformLocations[UNIFORM_TEXTURE], 0 );
    error();
}



static int s_cnt = 0;
static uint32_t s_time = 0;



static void update() {
    //glClearColor( drand48(), drand48(), drand48(), 1 );
    
    size_t numChars = strlen( s_string );
    s_stringLength = numChars;

    if ( true || !s_points ) {
        free_s( s_points );
        s_points = malloc( numChars * 6 * sizeof( VertexData_t ) );
        memset( s_points, 0xFF, numChars * 6 * sizeof( VertexData_t ) );
        float penX = s_cnt;
        float penY = 16;

        for ( size_t c = 0; c < numChars; c++ ) {
            size_t idx = s_string[c];
            FontMap_t f = s_fontMap[idx];
            
            if ( f.charCode == 'e' ) {
//                printf( "%d %d %d %d %d %d\n", f.bitMapLeft, f.bitMapTop, f.bitMapWidth, f.bitMapHeight, f.posX, f.posY );
            }
            
            s_points[6 * c + 0].x = penX + f.bitMapLeft;
            s_points[6 * c + 0].y = penY + f.bitMapTop;
            s_points[6 * c + 1].x = penX + f.bitMapLeft;
            s_points[6 * c + 1].y = penY + f.bitMapTop;
            s_points[6 * c + 1].s = f.posX;
            s_points[6 * c + 1].t = f.posY;
            s_points[6 * c + 2].x = penX + f.bitMapLeft;
            s_points[6 * c + 2].y = penY + f.bitMapTop - f.bitMapHeight;
            s_points[6 * c + 2].s = f.posX;
            s_points[6 * c + 2].t = f.posY + f.bitMapHeight;
            s_points[6 * c + 3].x = penX + f.bitMapLeft + f.bitMapWidth;
            s_points[6 * c + 3].y = penY + f.bitMapTop;
            s_points[6 * c + 3].s = f.posX + f.bitMapWidth;
            s_points[6 * c + 3].t = f.posY;
            s_points[6 * c + 4].x = penX + f.bitMapLeft + f.bitMapWidth;
            s_points[6 * c + 4].y = penY + f.bitMapTop - f.bitMapHeight;
            s_points[6 * c + 4].s = f.posX + f.bitMapWidth;
            s_points[6 * c + 4].t = f.posY + f.bitMapHeight;
            s_points[6 * c + 5].x = penX + f.bitMapLeft + f.bitMapWidth;
            s_points[6 * c + 5].y = penY + f.bitMapTop - f.bitMapHeight;
            penX += f.advance + 4;
        }
    }

int c = 0;
	float b = 0;
	float t = s_cnt; s_screenHeight;
	float l = 0;
	float r = s_cnt; s_screenWidth;
	s_points[4 * c + 0].x = l;
	s_points[4 * c + 0].y = b;
	s_points[0].s = 0;
	s_points[0].t = 0;
	s_points[4 * c + 1].x = l;
	s_points[4 * c + 1].y = t;
	s_points[1].s = 0;
	s_points[1].t = s_cnt; 255;
	s_points[4 * c + 2].x = r;
	s_points[4 * c + 2].y = b;
	s_points[2].s = s_cnt; 255;
	s_points[2].t = 0;
	s_points[4 * c + 3].x = r;
	s_points[4 * c + 3].y = t;
	s_points[3].s = s_cnt; 255;
	s_points[3].t = s_cnt; 255;
    
    s_time++;
}



static void draw() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram( s_activeShader.programObject );
    glEnableVertexAttribArray( s_activeShader.attribLocations[ATTRIB_POSITION] );
    glEnableVertexAttribArray( s_activeShader.attribLocations[ATTRIB_COLOR] );
    glEnableVertexAttribArray( s_activeShader.attribLocations[ATTRIB_TEX_COORD] );
    error();
    
    glBindTexture( GL_TEXTURE_2D, s_fontTexture );
    glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2( s_points, 0 ) );
    glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_COLOR], 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData_t), BUFFER_OFFSET2( s_points, 8 ) );
    glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_TEX_COORD], 2, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData_t), BUFFER_OFFSET2( s_points, 12 ) );
    glDrawArrays( GL_TRIANGLE_STRIP, s_cnt * 0, 4 );
    //glDrawArrays( GL_TRIANGLE_STRIP, 0, s_stringLength * 6 );
    s_cnt++;

    if ( s_cnt == 256 ) s_cnt = 0;

    error();
}



static void deinit() {
    unloadTextures();
    unloadShaders();
    free_s( s_fontMap );
	free_s( s_points );
	error();
}



#pragma mark - Shaders



static void loadShaders() {
    GLuint vertShaderObject = createShaderObject( "Font/Assets/Shaders/Font.vsh", GL_VERTEX_SHADER );
	error();
    GLuint fragShaderObject = createShaderObject( "Font/Assets/Shaders/Font.fsh", GL_FRAGMENT_SHADER );
	error();

    glReleaseShaderCompiler();

    s_activeShader = createProgramObject( vertShaderObject, fragShaderObject );

    glDeleteShader( vertShaderObject );
    glDeleteShader( fragShaderObject );
}



static void unloadShaders() {
	fputs( "unloadShaders\n", stderr );
    glDeleteProgram( s_activeShader.programObject );
    s_activeShader.programObject = 0;
	error();
}



#pragma mark - Textures



static void loadTextures() {
	//s_fontTexture = createTextureObject( "Font/Assets/Textures/Vera.raw", GL_LUMINANCE );
	s_fontTexture = createTextureObject( "Font/Assets/Textures/checkboard256.raw", GL_LUMINANCE );
    //s_fontTexture = createTextureObject( "Arabesque/Assets/Textures/CircleMip.etc1", GL_ETC1_RGB8_OES );
}



static void unloadTextures() {
	fputs( "unloadTextures\n", stderr );
	glDeleteTextures( 1, &s_fontTexture );
	s_fontTexture = 0;
	error();
}



#pragma mark - exposed interface



GLES2Playground_t e_playgroundFont = {
    .name = "Font",
    .init = init,
    .deinit = deinit,
    .update = update,
    .draw = draw
};
