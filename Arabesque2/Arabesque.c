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
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#define DEBUG 1

#define free_s( X ) free( X ); X = NULL
#define error() if ( ( s_error = glGetError() ) != GL_NO_ERROR ) { fprintf( stderr, "%i", s_error ); assert( s_error == GL_NO_ERROR ); }

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define BUFFER_OFFSET2(x, i) ((char *)x + (i))

static GLuint s_error;

typedef struct {
	GLfloat x, y;
	GLubyte r, g, b, a;
	GLubyte s, t, u, v; // padding for 32bit alignment
} VertexData_t;



static const int ROWS = 9;
static const int COLS = 15;
static float * s_data;
static VertexData_t * s_points;

static Shader_t s_activeShader;
//static GLuint _vertexBuffer;
static GLuint s_texture;


static void loadShaders( void );
static void unloadShaders( void );
static void loadTextures( void );
static void unloadTextures( void );

inline static float clampf( const float in_X, const float in_MIN, const float in_MAX ) {
	float result = in_X;
	result = ( result < in_MIN ) ? in_MIN : result;
	result = ( result > in_MAX ) ? in_MAX : result;
	return result;
}



static void AsetupGL() {
	s_data = malloc( sizeof(float) * ROWS * COLS );
	s_points = malloc( sizeof(VertexData_t) * ROWS * COLS * 6 );

	int x, y, xy = 0;

	for ( y = 0; y < ROWS; y++ ) {
		for ( x = 0; x < COLS; x++ ) {
			GLubyte r = drand48() * 256;
			GLubyte g = drand48() * 256;
			GLubyte b = drand48() * 256;

			s_points[xy * 6 + 1].r = r;
			s_points[xy * 6 + 1].g = g;
			s_points[xy * 6 + 1].b = b;
			s_points[xy * 6 + 1].a = 255;
			s_points[xy * 6 + 2].r = r;
			s_points[xy * 6 + 2].g = g;
			s_points[xy * 6 + 2].b = b;
			s_points[xy * 6 + 2].a = 255;
			s_points[xy * 6 + 3].r = r;
			s_points[xy * 6 + 3].g = g;
			s_points[xy * 6 + 3].b = b;
			s_points[xy * 6 + 3].a = 255;
			s_points[xy * 6 + 4].r = r;
			s_points[xy * 6 + 4].g = g;
			s_points[xy * 6 + 4].b = b;
			s_points[xy * 6 + 4].a = 255;

			s_points[xy * 6 + 1].s = 0;
			s_points[xy * 6 + 1].t = 0;
			s_points[xy * 6 + 2].s = 0;
			s_points[xy * 6 + 2].t = 1;
			s_points[xy * 6 + 3].s = 1;
			s_points[xy * 6 + 3].t = 0;
			s_points[xy * 6 + 4].s = 1;
			s_points[xy * 6 + 4].t = 1;

			xy++;
		}
	}

    float aspectRatio = fabsf( 1920.f / 1200.f );
	mat4 projectionMatrix = mat4MakeOrtho( -aspectRatio, aspectRatio, -1.0f, 1.0f, 0.1f, 100.0f );

    mat4 baseModelViewMatrix = mat4MakeTranslate( (vec3){{ 0.0f, 0.0f, -4.0f }} );

    // Compute the model view matrix for the object rendered with ES2
	mat4 modelViewMatrix = mat4MakeTranslate( (vec3){{ 0.0f, 0.0f, 1.5f }} );
    modelViewMatrix = mat4Mul( baseModelViewMatrix, modelViewMatrix );

	mat4 modelViewProjectionMatrix = mat4Mul( projectionMatrix, modelViewMatrix );

    loadShaders();
	loadTextures();

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	//glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, s_texture );

//	glGenBuffers( 1, &_vertexBuffer );
//	glBindBuffer( GL_ARRAY_BUFFER, _vertexBuffer );
//	glBufferData( GL_ARRAY_BUFFER, sizeof(VertexData_t) * ROWS * COLS * 6, s_points, GL_STATIC_DRAW );

	glUseProgram( s_activeShader.programObject );
	glUniformMatrix4fv( s_activeShader.uniformLocations[UNIFORM_PROJECTION_MATRIX], 1, 0, modelViewProjectionMatrix.m );
	glUniform1i( s_activeShader.uniformLocations[UNIFORM_TEXTURE], 0 );

//	glEnableVertexAttribArray( s_activeShader.attribLocations[ATTRIB_POSITION] );
//	glEnableVertexAttribArray( s_activeShader.attribLocations[ATTRIB_COLOR] );
//	//glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_POSITION], 2, GL_FLOAT, GL_FALSE, 12, BUFFER_OFFSET( 0 ) );
//	//glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_COLOR], 4, GL_UNSIGNED_BYTE, GL_TRUE, 12, BUFFER_OFFSET( 8 ) );
//	glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_POSITION], 2, GL_FLOAT, GL_FALSE, 12, BUFFER_OFFSET2( s_points, 0 ) );
//	glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_COLOR], 4, GL_UNSIGNED_BYTE, GL_TRUE, 12, BUFFER_OFFSET2( s_points, 8 ) );

	error();
}

static GLfloat s_s = 0.01f;
static GLfloat s_t = 0;
static GLfloat s_noiseY = 0.0f;
static GLfloat s_randomWalk[3] = { 0, 0, 0 };
static GLfloat s_randomWalkV[3] = { 0.0001, -0.0003, 0.0002 };
static GLfloat s_randomWalkA[3] = {-0.0003,  0.0002, 0.0001 };


static void Aupdate() {
	int x, y, xy;
	GLfloat scaleX = 3.2f / (GLfloat)( COLS + 1 );
	GLfloat scaleY = 2.0f / (GLfloat)( ROWS + 1 );

	xy = 0;

	for ( y = 0; y < ROWS; y++ ) {
		GLfloat cY = ( y + 1 ) * scaleY - 1.0f;

		for ( x = 0; x < COLS; x++ ) {
			GLfloat cX = ( x + 1 ) * scaleX - 1.6f;
			s_data[xy] = 0.5f + cos( cX + s_t ) * sin( ( cY + s_s ) * ( cX + s_t ) ) * 0.5f;
			s_data[xy] = 0.5f;
			s_data[xy] = clampf( noise( x * 0.2f + s_randomWalk[0], y * 0.2f + s_randomWalk[1], s_randomWalk[2] ) * 1.5f, 0, 1 );
			xy++;
		}
	}

	xy = 0;


	for ( y = 0; y < ROWS; y++ ) {
		GLfloat cY = ( y + 1 ) * scaleY - 1.0f;

		for ( x = 0; x < COLS; x++ ) {
			GLfloat cX = ( x + 1 ) * scaleX - 1.6f;
			GLfloat halfWidth = s_data[xy] * s_data[xy] * scaleX * 2.0f;
			//halfWidth = s_data[xy] * scaleX;
			halfWidth += 0.04;

			s_points[xy * 6 + 0].x = cX - halfWidth;	// degenerate triangle
			s_points[xy * 6 + 0].y = cY - halfWidth;	// degenerate triangle
			s_points[xy * 6 + 1].x = cX - halfWidth;
			s_points[xy * 6 + 1].y = cY - halfWidth;
			s_points[xy * 6 + 2].x = cX - halfWidth;
			s_points[xy * 6 + 2].y = cY + halfWidth;
			s_points[xy * 6 + 3].x = cX + halfWidth;
			s_points[xy * 6 + 3].y = cY - halfWidth;
			s_points[xy * 6 + 4].x = cX + halfWidth;
			s_points[xy * 6 + 4].y = cY + halfWidth;
			s_points[xy * 6 + 5].x = cX + halfWidth;	// degenerate triangle
			s_points[xy * 6 + 5].y = cY + halfWidth;	// degenerate triangle

			float rnd = clampf( 0.5f + noise( x * 0.1f, y * 0.1f + s_noiseY, 0.0f ) * 0.75f, 0, 1 );
			hsl_t hsl = { fmodf( rnd * 360.0f, 360.0f ), 0.7f, 0.5f };
			rgb8_t color = colorspaceConvertHSL2RGB( hsl );
			s_points[xy * 6 + 1].r = color.r;
			s_points[xy * 6 + 1].g = color.g;
			s_points[xy * 6 + 1].b = color.b;
			s_points[xy * 6 + 2].r = color.r;
			s_points[xy * 6 + 2].g = color.g;
			s_points[xy * 6 + 2].b = color.b;
			s_points[xy * 6 + 3].r = color.r;
			s_points[xy * 6 + 3].g = color.g;
			s_points[xy * 6 + 3].b = color.b;
			s_points[xy * 6 + 4].r = color.r;
			s_points[xy * 6 + 4].g = color.g;
			s_points[xy * 6 + 4].b = color.b;

			GLubyte a = ( 1.0f - fabsf( s_data[xy] - 0.5f ) * 2.0f ) * 255;
			s_points[xy * 6 + 1].a = a;
			s_points[xy * 6 + 2].a = a;
			s_points[xy * 6 + 3].a = a;
			s_points[xy * 6 + 4].a = a;

			xy++;
		}
	}

	//s_s += 0.01f;
	s_t += 0.01f;
	s_noiseY += 0.002f;
	s_randomWalkA[0] += ( drand48() - 0.5f ) * 0.0001;
	s_randomWalkA[1] += ( drand48() - 0.5f ) * 0.0001;
	s_randomWalkA[2] += ( drand48() - 0.5f ) * 0.0001;
	s_randomWalkV[0] += s_randomWalkA[0];
	s_randomWalkV[1] += s_randomWalkA[1];
	s_randomWalkV[2] += s_randomWalkA[2];
	s_randomWalkV[0] = clampf( s_randomWalkV[0], -0.001, 0.001 );
	s_randomWalkV[1] = clampf( s_randomWalkV[1], -0.001, 0.001 );
	s_randomWalkV[2] = clampf( s_randomWalkV[2], -0.001, 0.001 );
	s_randomWalk[0] += s_randomWalkV[0];
	s_randomWalk[1] += s_randomWalkV[1];
	s_randomWalk[2] += s_randomWalkV[2];
}


static void Adraw() {
    glClear( GL_COLOR_BUFFER_BIT );

	glUseProgram( s_activeShader.programObject );
	glEnableVertexAttribArray( s_activeShader.attribLocations[ATTRIB_POSITION] );
	glEnableVertexAttribArray( s_activeShader.attribLocations[ATTRIB_COLOR] );
	glEnableVertexAttribArray( s_activeShader.attribLocations[ATTRIB_TEX_COORD] );
	glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2( s_points, 0 ) );
	glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_COLOR], 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData_t), BUFFER_OFFSET2( s_points, 8 ) );
	glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_TEX_COORD], 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2( s_points, 12 ) );

	//glBindBuffer( GL_ARRAY_BUFFER, _vertexBuffer );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, COLS * ROWS * 6 );

	error();
}


static void AtearDownGL() {
//    glDeleteBuffers(1, &_vertexBuffer);
	unloadTextures();
    unloadShaders();
	free_s( s_data );
	error();
}



#pragma mark - Shaders



void loadShaders() {
    GLuint vertShaderObject = createShaderObject( "Arabesque2/Assets/Shaders/Arabesque.vsh", GL_VERTEX_SHADER );
	error();
    GLuint fragShaderObject = createShaderObject( "Arabesque2/Assets/Shaders/Arabesque.fsh", GL_FRAGMENT_SHADER );
	error();

    glReleaseShaderCompiler();

    s_activeShader = createProgramObject( vertShaderObject, fragShaderObject );

    glDeleteShader( vertShaderObject );
    glDeleteShader( fragShaderObject );
}



void unloadShaders() {
    glDeleteProgram( s_activeShader.programObject );
    s_activeShader.programObject = 0;
}



#pragma mark - Textures



void loadTextures() {
	s_texture = createTextureObject( "Arabesque2/Assets/Textures/CircleMip.etc1", GL_ETC1_RGB8_OES );
}



void unloadTextures() {
	glDeleteTextures( 1, &s_texture );
	s_texture = 0;
}



GLES2Playground_t e_playgroundArabesque2 = {
    .name = "Arabesque2",
    .init = AsetupGL,
    .deinit = AtearDownGL,
    .update = Aupdate,
    .draw = Adraw
};
