#include "eglcore.h"
#include "globaltime.h"
#include "playground.h"

#include <bcm_host.h>
#include <pthread.h>

#include <assert.h>
#include <iso646.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


//extern GLES2Playground_t e_playgroundArabesque;
extern GLES2Playground_t e_playgroundFont;
extern GLES2Playground_t e_playgroundAudioVisualizer;

static const uint16_t s_DISPLAY_NUMBER = 0; // LCD = 0

static pthread_t s_renderingThread;
volatile static sig_atomic_t s_renderingThreadAlive = 1;
static uint32_t s_screenWidth = 0;
static uint32_t s_screenHeight = 0;
static uint32_t s_screenFrameRate = 1;


static void destroy() {
	fputs( "destroy\n", stderr );
	bcm_host_deinit();
}



static void terminated( const int in_SIG ) {
	fprintf( stderr, "\nTERMINATING due to signal %i\n", in_SIG );
    s_renderingThreadAlive = 0;
}



//static int get_mouse(CUBE_STATE_T *state, int *outx, int *outy)
//{
//    static int fd = -1;
//    const int width=state->screen_width, height=state->screen_height;
//    static int x=800, y=400;
//    const int XSIGN = 1<<4, YSIGN = 1<<5;
//    if (fd<0) {
//        fd = open("/dev/input/mouse0",O_RDONLY|O_NONBLOCK);
//    }
//    if (fd>=0) {
//        struct {char buttons, dx, dy; } m;
//        while (1) {
//            int bytes = read(fd, &m, sizeof m);
//            if (bytes < (int)sizeof m) goto _exit;
//            if (m.buttons&8) {
//                break; // This bit should always be set
//            }
//            read(fd, &m, 1); // Try to sync up again
//        }
//        if (m.buttons&3)
//            return m.buttons&3;
//        x+=m.dx;
//        y+=m.dy;
//        if (m.buttons&XSIGN)
//            x-=256;
//        if (m.buttons&YSIGN)
//            y-=256;
//        if (x<0) x=0;
//        if (y<0) y=0;
//        if (x>width) x=width;
//        if (y>height) y=height;
//    }
//_exit:
//    if (outx) *outx = x;
//    if (outy) *outy = y;
//    return 0;
//}




static void * glThread( void * argument ) {
    float s_timeStamp = 0;
	uint32_t frameCounter = 0;
    
	//initEGL( s_DISPLAY_NUMBER, s_screenWidth / 4, 32, s_screenWidth * 3 / 4, s_screenHeight * 3 / 4 );
	initEGL( s_DISPLAY_NUMBER, 0, 0, s_screenWidth, s_screenHeight );

	while ( s_renderingThreadAlive ) {
		drawEGL();
		frameCounter++;

		if ( frameCounter == s_screenFrameRate * 2 ) {
            float now = timeGet();
            float deltaT = now - s_timeStamp;
            printf( "%.1f FPS\n", ( s_screenFrameRate * 2.0f ) / deltaT );
			//fputs( ".", stdout );
			fflush( stdout );
			frameCounter = 0;
            s_timeStamp = now;
		}
	}

	destroyEGL();
	pthread_exit( NULL );
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
    
    int cnt = 0;
    
	while ( s_renderingThreadAlive ) {
		//fputs( "o", stdout );
		//fflush( stdout );
        usleep( 500000 );
		//sleep( 1 );
        
        
        for ( int i = 0; i < 135; i++ ) {
            blaub[i] = drand48();
        }
        
        e_playgroundAudioVisualizer.commitData( blaub );
        
        switch ( cnt ) {
            case 1:
                e_playgroundFont.setString( "Titel\nAlbum\nInterpret" );
                e_playgroundAudioVisualizer.setString( "Titel\nAlbum\nInterpret" );
                break;
                
            case 11:
                e_playgroundFont.setString( "Affe\nTot\nSchade!" );
                e_playgroundAudioVisualizer.setString( "Affe\nTot\nSchade!" );
                break;

            case 21:
                e_playgroundFont.setString( "Lorem ipsum dolor sit amet, consectetur, adipisci velit.\nStet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\nAt vero eos et accusam et justo duo dolores et ea rebum." );
                e_playgroundAudioVisualizer.setString( "Lorem ipsum dolor sit amet, consectetur, adipisci velit.\nStet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\nAt vero eos et accusam et justo duo dolores et ea rebum." );
                break;

            case 31:
                e_playgroundFont.setString( "" );
                e_playgroundAudioVisualizer.setString( "" );
                break;
                
            case 41:
                cnt = 0;
                break;
                
            default:
                break;
        }
        
        cnt++;
	}
    
    pthread_join( s_renderingThread, NULL );

	exit( EXIT_SUCCESS );
}
