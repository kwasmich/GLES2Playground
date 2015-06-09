//
//  glcore.c
//  GLESCompute
//
//  Created by Michael Kwasnicki on 01.12.13.
//
//

#include "glcore.h"

#include "globaltime.h"
#include "OpenGLES2Core.h"
#include "playground.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>



#define free_s( X ) free( X ); X = NULL


extern GLES2Playground_t e_playgroundArabesque;
extern GLES2Playground_t e_playgroundAudioVisualizer;
extern GLES2Playground_t e_playgroundClock;
extern GLES2Playground_t e_playgroundFlyingCubes;
extern GLES2Playground_t e_playgroundFont;

GLES2Playground_t play;


void initGL( const int in_WIDTH, const int in_HEIGHT ) {
    printf( "Screen size: %d x %d\n", in_WIDTH, in_HEIGHT );

    glxPrintInteger( GL_SUBPIXEL_BITS );
    glxPrintInteger( GL_MAX_TEXTURE_SIZE );
    glxPrintInteger( GL_MAX_CUBE_MAP_TEXTURE_SIZE );
    glxPrintInteger2( GL_MAX_VIEWPORT_DIMS );
    glxPrintInteger2( GL_ALIASED_POINT_SIZE_RANGE );
    glxPrintInteger2( GL_ALIASED_LINE_WIDTH_RANGE );
    glxPrintInteger( GL_SAMPLE_BUFFERS );
    glxPrintInteger( GL_SAMPLES );
    glxPrintInteger( GL_NUM_COMPRESSED_TEXTURE_FORMATS );
    
    GLint numCompressedTextureFormats;
    glGetIntegerv( GL_NUM_COMPRESSED_TEXTURE_FORMATS, &numCompressedTextureFormats );
    
    GLint *compressedTextureFormats = malloc( sizeof(GLuint) * numCompressedTextureFormats );
    glGetIntegerv( GL_COMPRESSED_TEXTURE_FORMATS, compressedTextureFormats );
    
    for ( int i = 0; i < numCompressedTextureFormats; i++ ) {
        printf( "%x\n", compressedTextureFormats[i] );
    }
    
    free_s( compressedTextureFormats );
    
    glxPrintInteger( GL_NUM_SHADER_BINARY_FORMATS );

    GLint numShaderBinaryFormats;
    glGetIntegerv( GL_NUM_SHADER_BINARY_FORMATS, &numShaderBinaryFormats );
    
    GLint *shaderBinaryFormats = malloc( sizeof(GLuint) * numShaderBinaryFormats );
    glGetIntegerv( GL_SHADER_BINARY_FORMATS, shaderBinaryFormats );
    
    for ( int i = 0; i < numShaderBinaryFormats; i++ ) {
        printf( "%x\n", shaderBinaryFormats[i] );
    }
    
    free_s( shaderBinaryFormats );

    glxPrintString( GL_EXTENSIONS );
    glxPrintString( GL_RENDERER );
    glxPrintString( GL_SHADING_LANGUAGE_VERSION );
    glxPrintString( GL_VENDOR );
    glxPrintString( GL_VERSION );
    glxPrintInteger( GL_MAX_VERTEX_ATTRIBS );
    glxPrintInteger( GL_MAX_VERTEX_UNIFORM_VECTORS );
    glxPrintInteger( GL_MAX_VARYING_VECTORS );
    glxPrintInteger( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS );
    glxPrintInteger( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS );
    glxPrintInteger( GL_MAX_TEXTURE_IMAGE_UNITS );
    glxPrintInteger( GL_MAX_FRAGMENT_UNIFORM_VECTORS );
    glxPrintInteger( GL_MAX_RENDERBUFFER_SIZE );

    glxPrintInteger( GL_RED_BITS );
    glxPrintInteger( GL_GREEN_BITS );
    glxPrintInteger( GL_BLUE_BITS );
    glxPrintInteger( GL_ALPHA_BITS );
    glxPrintInteger( GL_DEPTH_BITS );
    glxPrintInteger( GL_STENCIL_BITS );
    glxPrintInteger( GL_IMPLEMENTATION_COLOR_READ_TYPE );
    glxPrintInteger( GL_IMPLEMENTATION_COLOR_READ_FORMAT );
    
    timeReset();
    play = e_playgroundFont;
    play.init( in_WIDTH, in_HEIGHT );
}


void drawGL() {
    timeTick();
	play.update();
	play.draw();
}


void destroyGL() {
	play.deinit();
}
