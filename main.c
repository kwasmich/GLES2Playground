#include "eglcore.h"

#include "playground.h"

#include <bcm_host.h>
#include <pthread.h>

#include <assert.h>
#include <iso646.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


extern GLES2Playground_t e_playgroundArabesque;


static const uint16_t s_DISPLAY_NUMBER = 0; // LCD = 0

static pthread_t s_renderingThread;
static bool s_renderingThreadAlive = true;
static uint32_t s_screenWidth = 0;
static uint32_t s_screenHeight = 0;
static uint32_t s_screenFrameRate = 1;

void destroy( void );
void terminated( const int in_SIG );
void glThreadDestroy( void * arg );
void * glThread( void * argument );



void destroy() {
	fputs( "destroy\n", stderr );

	s_renderingThreadAlive = false;
	pthread_join( s_renderingThread, NULL );

	bcm_host_deinit();
}



void terminated( const int in_SIG ) {
	fprintf( stderr, "\nTERMINATING due to signal %i\n", in_SIG );
	exit( -1 );
}



void * glThread( void * argument ) {
	uint32_t frameCounter = 0;

	//initEGL( s_DISPLAY_NUMBER, s_screenWidth/2, 0, s_screenWidth/2, s_screenHeight/2 );
	initEGL( s_DISPLAY_NUMBER, 0, 0, s_screenWidth, s_screenHeight );

	while ( s_renderingThreadAlive ) {
		drawEGL();

		frameCounter++;

		if ( frameCounter == s_screenFrameRate ) {
			fputs( ".", stdout );
			fflush( stdout );
			frameCounter = 0;
		}
	}

	destroyEGL();
	return NULL;
}



int main( int argc, char * argv[] ) {
	atexit( destroy );
	signal( SIGHUP, terminated );
	signal( SIGINT, terminated );
	signal( SIGKILL, terminated );
	signal( SIGPIPE, terminated );
	signal( SIGALRM, terminated );
	signal( SIGTERM, terminated );

	bcm_host_init();

	TV_DISPLAY_STATE_T displayState;
	int result = vc_tv_get_display_state( &displayState );
	assert( result == 0 );
	s_screenWidth = displayState.display.hdmi.width;
	s_screenHeight = displayState.display.hdmi.height;
	s_screenFrameRate = displayState.display.hdmi.frame_rate;
	printf( "%d x %d @ %d\n", s_screenWidth, s_screenHeight, s_screenFrameRate );

	int rc = pthread_create( &s_renderingThread, NULL, glThread, NULL );
	assert(0 == rc);

    float blaub[135];
    //sleep( 5 );
    
	while ( true ) {
		//fputs( "o", stdout );
		fflush( stdout );
        usleep( 100 );
		//sleep( 1 );

		float * bla = e_playgroundArabesque.getDataPointer();
        
        if ( false and bla ) {
            for ( int i = 0; i < 135; i++ ) {
                bla[i] = drand48();
            }
        } else {
            for ( int i = 0; i < 135; i++ ) {
                blaub[i] = drand48();
            }
            
            e_playgroundArabesque.commitData( blaub );
        }
	}

	exit( EXIT_SUCCESS );
}
