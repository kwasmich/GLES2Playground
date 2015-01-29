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


//static void error() {
//    GLenum s_error = glGetError();
//    
//    if ( s_error != GL_NO_ERROR ) {
//        fprintf( stderr, "%i", s_error );
//        
//        int j, nptrs;
//        #define SIZE 100
//        void *buffer[SIZE];
//        char **strings;
//        
//        nptrs = backtrace(buffer, SIZE);
//        printf("backtrace() returned %d addresses\n", nptrs);
//        
//        /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
//         would produce similar output to the following: */
//        
//        strings = backtrace_symbols(buffer, nptrs);
//        if (strings == NULL) {
//            perror("backtrace_symbols");
//            exit(EXIT_FAILURE);
//        }
//        
//        for (j = 0; j < nptrs; j++)
//            printf("%s\n", strings[j]);
//        
//        free(strings);
//        
//        assert( s_error == GL_NO_ERROR );
//    }
//}


static int s_rows = 9;
static int s_cols = 15;
static int s_numPoints = 135;
static const float MIN_VALUE = 0.1f;
static float * s_dataRAW = NULL;
static float * s_dataRAW2 = NULL;
static float * s_data = NULL;
static float * s_dataDecay = NULL;
static VertexData_t * s_points = NULL;
static VertexData_t * s_points2 = NULL;
static VertexData_t s_quad[4] = {
    { -1, -1, 255, 255, 255, 255, 0, 0 },
    { -1,  1, 255, 255, 255, 255, 0, 1 },
    {  1, -1, 255, 255, 255, 255, 1, 0 },
    {  1,  1, 255, 255, 255, 255, 1, 1 },
};

static VertexData_t s_quad2[4] = {
    { -1, -1, 255, 255, 255, 255, 0, 0 },
    { -1,  1, 255, 255, 255, 255, 0, 1 },
    {  1, -1, 255, 255, 255, 255, 1, 0 },
    {  1,  1, 255, 255, 255, 255, 1, 1 },
};

// OpenGL|ES state
static Shader_t s_activeShader;
static GLuint s_texture;
static GLuint s_framebuffer;
static GLuint s_framebufferTexture;

static GLuint s_screenWidth = 0;
static GLuint s_screenHeight = 0;
static GLfloat s_screenAspect = 1;


static void loadShaders( void );
static void unloadShaders( void );
static void loadTextures( void );
static void unloadTextures( void );
static void createFrameBufferObject( void );
static void destroyFrameBufferObject( void );


static bool s_idle = true;
static bool s_initialized = false;


//inline static int min( const int in_X, const int in_Y ) {
//	return ( in_X < in_Y ) ? in_X : in_Y;
//}
//
//inline static int max( const int in_X, const int in_Y ) {
//	return ( in_X > in_Y ) ? in_X : in_Y;
//}
//
//inline static float clampi( const int in_X, const int in_MIN, const int in_MAX ) {
//	return min( max( in_X, in_MIN ), in_MAX );
//}



static void init( const int in_WIDTH, const int in_HEIGHT ) {
    s_screenWidth = in_WIDTH;
    s_screenHeight = in_HEIGHT;
    s_screenAspect = in_WIDTH / (float)in_HEIGHT;
    
    printf( "%d x %d\n", in_WIDTH, in_HEIGHT );
    
    if ( s_screenAspect > 1 ) {
        s_rows = 10 - 1;
        s_cols = 10 * in_WIDTH / in_HEIGHT - 1;
    } else {
        s_rows = 10 * in_HEIGHT / in_WIDTH - 1;
        s_cols = 10 - 1;
    }
    
    printf( "%d x %d\n", s_rows, s_cols );
    //exit( 255 );
    
    s_numPoints = s_cols * s_rows;
    
    s_quad[0].x = -s_screenAspect;
    s_quad[1].x = -s_screenAspect;
    s_quad[2].x =  s_screenAspect;
    s_quad[3].x =  s_screenAspect;
    
    s_quad2[0].x = -s_screenAspect;
    s_quad2[1].x = -s_screenAspect;
    s_quad2[2].x =  s_screenAspect;
    s_quad2[3].x =  s_screenAspect;
    
    s_dataRAW = malloc( sizeof(float) * s_numPoints );
    s_dataRAW2 = malloc( sizeof(float) * s_numPoints );
    s_data = malloc( sizeof(float) * s_numPoints );
    s_dataDecay = malloc( sizeof(float) * s_numPoints );
    s_points = malloc( sizeof(VertexData_t) * s_numPoints * 6 );
    s_points2 = malloc( sizeof(VertexData_t) * s_numPoints * 6 );
    
    int x, y, xy = 0;
    
    for ( y = 0; y < s_rows; y++ ) {
        for ( x = 0; x < s_cols; x++ ) {
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
    
    for ( xy = 0; xy < s_numPoints * 6; xy++ ) {
        s_points2[xy] = s_points[xy];
    }
    
    mat4 projectionMatrix = mat4MakeOrtho( -s_screenAspect, s_screenAspect, -1, 1, 0, 1 );
    
    loadShaders();
    loadTextures();
    createFrameBufferObject();
    
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glEnable( GL_BLEND );
    glBlendEquation( GL_FUNC_ADD );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, s_texture );
    
    glUseProgram( s_activeShader.programObject );
    glUniformMatrix4fv( s_activeShader.uniformLocations[UNIFORM_PROJECTION_MATRIX], 1, 0, projectionMatrix.m );
    glUniform1i( s_activeShader.uniformLocations[UNIFORM_TEXTURE], 0 );
    error();
    
    s_initialized = true;
}



static GLfloat s_randomWalk[3] = { 0, 0, 0 };
static GLfloat s_randomWalkV[3] = { 0.0001, -0.0003, 0.0002 };
static GLfloat s_randomWalkA[3] = {-0.0003,  0.0002, 0.0001 };



static void update() {
    int i, x, y, xy;
    GLfloat time = timeGet();
    GLfloat scaleXY = s_screenAspect * 2.0f / (GLfloat)( s_cols + 1 );
    GLfloat centerX = ( s_cols + 1 ) * scaleXY * 0.5f; //s_screenAspect;
    GLfloat centerY = ( s_rows + 1 ) * scaleXY * 0.5f; //1.0;
    
//    if ( s_screenAspect < 1 ) {
//        scaleX = 2.0f / (GLfloat)( s_cols + 1 );
//        scaleY = 2.0f / (GLfloat)( s_screenAspect * s_rows + 1 );
//        centerX = 1.0;
//        centerY = s_screenAspect;
//    }
    
    GLfloat blend = clamp( 0.95f + noise( time * 0.01f, time * 0.02f, time * 0.04f ), 0, 0.95f );
    //blend = 0;
    
    if ( s_idle ) {
        xy = 0;
        
        for ( y = 0; y < s_rows; y++ ) {
            GLfloat cY = ( y + 1 ) * scaleXY - centerY;
            
            for ( x = 0; x < s_cols; x++ ) {
                GLfloat cX = ( x + 1 ) * scaleXY - centerX;
                s_data[xy] = 0.5f + cos( cX + time ) * sin( cY * ( cX + time ) ) * 0.5f;
                s_data[xy] = 0.5f;
                s_data[xy] = clamp( noise( x * 0.2f + s_randomWalk[0], y * 0.2f + s_randomWalk[1], s_randomWalk[2] ) * 1.5f, 0, 1 );
                xy++;
            }
        }
    } else {
        blend = 0;
        
        for ( xy = 0; xy < s_numPoints; xy++ ) {
            s_data[xy] -= s_dataDecay[xy];
            
            if ( s_data[xy] < 0 ) {
                s_data[xy] = 0;
                s_dataDecay[xy] = 0;
            }
        }
    }
    
    xy = 0;
    
    for ( y = 0; y < s_rows; y++ ) {
        GLfloat cY = ( y + 1 ) * scaleXY - centerY;
        
        for ( x = 0; x < s_cols; x++ ) {
            GLfloat cX = ( x + 1 ) * scaleXY - centerX;
            GLfloat halfWidth = ( s_data[xy] * 1.5f + MIN_VALUE ) * scaleXY;
            //halfWidth = 0.5f * scaleXY;
            
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
            
            float rnd = clamp( 0.5f + noise( x * 0.1f, y * 0.1f + time * 0.25, 0.0f ) * 0.75f, 0, 1 );
            GLfloat alpha = ( 1.0f - fabsf( s_data[xy] - 0.667f ) * 1.5f );
            hsl_t hsl = { fmodf( rnd * 360.0f, 360.0f ), 0.7f, 0.5f };
            rgb8_t color = colorspaceConvertHSL2RGB( hsl );
            
            for ( i = 1; i < 5; i++ ) {
                s_points[xy * 6 + i].r = color.r;
                s_points[xy * 6 + i].g = color.g;
                s_points[xy * 6 + i].b = color.b;
                s_points[xy * 6 + i].a = 255 * alpha * ( 1.0f - blend );
                s_points[xy * 6 + i].u = 127 * alpha;
            }
            
            xy++;
        }
    }
    
    
    for ( xy = 0; xy < s_numPoints * 6; xy++ ) {
        s_points2[xy] = s_points[xy];
        s_points2[xy].a = s_points2[xy].u;
    }
    
    
    s_randomWalkA[0] += ( drand48() - 0.5f ) * 0.0001;
    s_randomWalkA[1] += ( drand48() - 0.5f ) * 0.0001;
    s_randomWalkA[2] += ( drand48() - 0.5f ) * 0.0001;
    s_randomWalkV[0] += s_randomWalkA[0];
    s_randomWalkV[1] += s_randomWalkA[1];
    s_randomWalkV[2] += s_randomWalkA[2];
    s_randomWalkV[0] = clamp( s_randomWalkV[0], -0.001, 0.001 );
    s_randomWalkV[1] = clamp( s_randomWalkV[1], -0.001, 0.001 );
    s_randomWalkV[2] = clamp( s_randomWalkV[2], -0.001, 0.001 );
    s_randomWalk[0] += s_randomWalkV[0];
    s_randomWalk[1] += s_randomWalkV[1];
    s_randomWalk[2] += s_randomWalkV[2];
    
    float scaleFactor = noise( time * .01, time * .02, time * .03 );
    float scale = 0.025f * scaleFactor;
    float shiftX = 0.0125f * noise( time * .02, time * .03, time * .01 ) * fabsf( scaleFactor * 2.0f );
    float shiftY = 0.0125f * noise( time * .03, time * .01, time * .02 ) * fabsf( scaleFactor * 2.0f );
    float rotateZ = 0.00625f * noise( time * .001, time * .002, time * .003 ) * fabsf( scaleFactor * 2.0f );
    float w = s_screenAspect * ( 1.0f + scale );
    float h = 1.0f + scale;
    
    vec2 shift = { { shiftX, shiftY } };
    mat2 rot = mat2MakeRotateZ( rotateZ );
    vec2 vec;
    
    s_quad2[0].x = -w;
    s_quad2[0].y = -h;
    s_quad2[1].x = -w;
    s_quad2[1].y =  h;
    s_quad2[2].x =  w;
    s_quad2[2].y = -h;
    s_quad2[3].x =  w;
    s_quad2[3].y =  h;
    
    for ( i = 0; i < 4; i++ ) {
        vec.x = s_quad2[i].x;
        vec.y = s_quad2[i].y;
        vec = mul( rot, vec );	// rotate
        vec = add( vec, shift );	// shift
        s_quad2[i].x = vec.x;
        s_quad2[i].y = vec.y;
        
        s_quad2[i].a = blend * 255;
    }
}



static void draw() {
    glUseProgram( s_activeShader.programObject );
    glEnableVertexAttribArray( s_activeShader.attribLocations[ATTRIB_POSITION] );
    glEnableVertexAttribArray( s_activeShader.attribLocations[ATTRIB_COLOR] );
    glEnableVertexAttribArray( s_activeShader.attribLocations[ATTRIB_TEX_COORD] );
    error();
    
    if ( s_idle ) {
        glBindFramebuffer( GL_FRAMEBUFFER, s_framebuffer );
        glClear( GL_COLOR_BUFFER_BIT );
        
        {
            glBindTexture( GL_TEXTURE_2D, s_framebufferTexture );
            glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2( s_quad2, 0 ) );
            glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_COLOR], 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData_t), BUFFER_OFFSET2( s_quad2, 8 ) );
            glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_TEX_COORD], 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2( s_quad2, 12 ) );
            glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
        }
        
        {
            glBindTexture( GL_TEXTURE_2D, s_texture );
            glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2( s_points, 0 ) );
            glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_COLOR], 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData_t), BUFFER_OFFSET2( s_points, 8 ) );
            glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_TEX_COORD], 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2( s_points, 12 ) );
            glDrawArrays( GL_TRIANGLE_STRIP, 0, s_numPoints * 6 );
        }
        
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        glClear( GL_COLOR_BUFFER_BIT );
        
        {
            glBindTexture( GL_TEXTURE_2D, s_framebufferTexture );
            glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2( s_quad, 0 ) );
            glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_COLOR], 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData_t), BUFFER_OFFSET2( s_quad, 8 ) );
            glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_TEX_COORD], 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2( s_quad, 12 ) );
            glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
        }
    } else {
        glBindTexture( GL_TEXTURE_2D, s_texture );
        glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2( s_points, 0 ) );
        glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_COLOR], 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData_t), BUFFER_OFFSET2( s_points, 8 ) );
        glVertexAttribPointer( s_activeShader.attribLocations[ATTRIB_TEX_COORD], 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(VertexData_t), BUFFER_OFFSET2( s_points, 12 ) );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, s_numPoints * 6 );
    }
    
    error();
}



static void deinit() {
    s_initialized = false;
    
	destroyFrameBufferObject();
	unloadTextures();
    unloadShaders();
	free_s( s_data );
	free_s( s_points );
	error();
}



static float * getDataPointer() {
    if ( !s_idle ) {
        return s_data;
    } else {
        return NULL;
    }
}



static int s_commit = 0;
static int s_shuffleOffset = 0;
const static int SHUFFLE[] = {
    38,  70, 130,  80,   2,  52,   6,   9,  26,   4, 109,  75,   5,  39, 121,
    40, 108,  84,  62,  36,  21,  50,  99,  43, 117,  17,  11,  33, 119,  71,
    110,   0,  58,  74,  92,  12,  18,  37, 100,  61, 131, 133,  57, 115,  15,
    72,  88,  60,  94, 124,  48,  78,  90,  35, 118,  93,  42,  19,  67,   7,
    134,  14,  22,  54, 132, 122,  56,  44,  31,  96,  59,  32,  97,  13, 129,
    76,  28, 120, 126,  30,  20, 128, 101,  85, 127,  29,   1,  83, 105,  23,
    82, 102,  24, 116,  64, 104,  41,  66,  63, 107,  55,  95,  51,  47,  27,
    10,   8,  69,  86,   3,  98, 103,  79,  45,  87,  89,  53, 111, 125,  81,
    34, 112, 114,  46, 106,  68,  73,  16, 113,  65,  77,  91,  49,  25, 123
};



static void commitData( const float in_DATA[] ) {
    if ( ( not s_initialized ) or s_idle ) return;
    
    int i;
    float steepness = 0;
    float * temp = s_dataRAW2;
    s_dataRAW2 = s_dataRAW;
    s_dataRAW = temp;
    
    for ( i = 0; i < s_numPoints; i++ ) {
        int shuffeledIndex = ( SHUFFLE[i] + s_shuffleOffset ) % 135;
        s_dataRAW[i] = in_DATA[i];
        steepness = s_dataRAW[i] - s_dataRAW2[i];
        
        if ( steepness > 0.05 and s_dataRAW[i] > s_data[shuffeledIndex] ) {
            s_data[shuffeledIndex] = s_dataRAW[i];
            s_dataDecay[shuffeledIndex] = s_dataRAW[i] * 0.025f;
        }
    }
    
    s_commit++;
    
    if ( s_commit == 100 ) {
        s_commit = 0;
        s_shuffleOffset += 123;
    }
}



static void setIdle( const bool in_IDLE ) {
    int i;
    
    for ( i = 0; i < s_numPoints; i++ )
        s_dataDecay[i] = s_data[i] * 0.025f;
    
    s_idle = in_IDLE;
}


#pragma mark - Shaders



static void loadShaders() {
    GLuint vertShaderObject = createShaderObject( "Arabesque/Assets/Shaders/Arabesque.vsh", GL_VERTEX_SHADER );
	error();
    GLuint fragShaderObject = createShaderObject( "Arabesque/Assets/Shaders/Arabesque.fsh", GL_FRAGMENT_SHADER );
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
	s_texture = createTextureObject( "Arabesque/Assets/Textures/CircleMip.etc1", GL_ETC1_RGB8_OES );
}



static void unloadTextures() {
	fputs( "unloadTextures\n", stderr );
	glDeleteTextures( 1, &s_texture );
	s_texture = 0;
	error();
}



#pragma mark - Framebuffer



static void createFrameBufferObject( void ) {
	glGenFramebuffers( 1, &s_framebuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, s_framebuffer );

	//	GLuint colorRenderBuffer;
	//	glGenRenderbuffers( 1, &colorRenderBuffer );
	//	glBindRenderbuffer( GL_RENDERBUFFER, colorRenderBuffer );
	//	glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA8_OES, s_screenWidth, s_screenHeight );
	//	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer );

	glGenTextures( 1, &s_framebufferTexture );
	glBindTexture( GL_TEXTURE_2D, s_framebufferTexture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, s_screenWidth, s_screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s_framebufferTexture, 0 );

	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	assert( status == GL_FRAMEBUFFER_COMPLETE );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	error();
}



static void destroyFrameBufferObject( void ) {
	fputs( "destroyFrameBufferObject\n", stderr );
	glDeleteTextures( 1, &s_framebufferTexture );
	s_framebufferTexture = 0;
	glDeleteFramebuffers( 1, &s_framebuffer );
	s_framebuffer = 0;
	error();
}



#pragma mark - exposed interface



GLES2Playground_t e_playgroundArabesque = {
    .name = "Arabesque",
    .init = init,
    .deinit = deinit,
    .update = update,
    .draw = draw,
    .getDataPointer = getDataPointer,
    .commitData = commitData
};
