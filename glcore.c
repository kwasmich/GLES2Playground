//
//  glcore.c
//  GLESCompute
//
//  Created by Michael Kwasnicki on 01.12.13.
//
//

#include "glcore.h"

#include "playground.h"

#include <GLES2/gl2.h>
#include <stdio.h>


extern GLES2Playground_t e_playgroundArabesque;
extern GLES2Playground_t e_playgroundFlyingCubes;

GLES2Playground_t play;


void initGL( const int in_WIDTH, const int in_HEIGHT ) {
	puts( (char*)glGetString( GL_EXTENSIONS ) );
    
    play = e_playgroundArabesque;
    play.init( in_WIDTH, in_HEIGHT );
}


void drawGL() {
	play.update();
	play.draw();
}


void destroyGL() {
	play.deinit();
}
