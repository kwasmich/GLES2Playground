#include "eglcore.h"
#include "globaltime.h"
#include "lib.h"
#include "playgrounds/playground.h"
#include "input/mouse.h"

#include <bcm_host.h>
#include <pthread.h>

#include <assert.h>
#include <iso646.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>




static const uint16_t s_DISPLAY_NUMBER = 0; // LCD = 0

static pthread_t s_renderingThread;
static volatile sig_atomic_t s_renderingThreadAlive = 1;
static uint32_t s_screenWidth = 0;
static uint32_t s_screenHeight = 0;
static uint32_t s_screenFrameRate = 1;


static void destroy() {
    fputs("destroy\n", stderr);
    bcm_host_deinit();
}



static void terminated(const int in_SIG) {
    fprintf(stderr, "\nTERMINATING due to signal %i\n", in_SIG);
    s_renderingThreadAlive = 0;
}



static void *glThread(void *UNUSED(argument)) {
    float s_timeStamp = 0;
    uint32_t frameCounter = 0;

    mouseInit();
    initEGL( s_DISPLAY_NUMBER, s_screenWidth / 4, 32, s_screenWidth * 3 / 4, s_screenHeight * 3 / 4 );
    //initEGL(s_DISPLAY_NUMBER, 0, 0, s_screenWidth, s_screenHeight);
    
    while (s_renderingThreadAlive) {
        mouseUpdate();
        drawEGL();
        mouseReset();
        
        frameCounter++;

        if (frameCounter == s_screenFrameRate * 2) {
            float now = timeGet();
            float deltaT = now - s_timeStamp;
            printf("%.1f FPS\n", (s_screenFrameRate * 2.0f) / deltaT);
            //fputs( ".", stdout );
            fflush(stdout);
            frameCounter = 0;
            s_timeStamp = now;
        }
    }

    destroyEGL();
    mouseFree();
    pthread_exit(NULL);
}



int main(int UNUSED(argc), char *UNUSED(argv[])) {
    atexit(destroy);
    signal(SIGHUP, terminated);
    signal(SIGINT, terminated);
    signal(SIGKILL, terminated);
    signal(SIGPIPE, terminated);
    signal(SIGALRM, terminated);
    signal(SIGTERM, terminated);

    bcm_host_init();

    TV_DISPLAY_STATE_T displayState;
    int result = vc_tv_get_display_state(&displayState);
    assert(result == 0);
    s_screenWidth = displayState.display.hdmi.width;
    s_screenHeight = displayState.display.hdmi.height;
    s_screenFrameRate = displayState.display.hdmi.frame_rate;
    printf("%d x %d @ %d\n", s_screenWidth, s_screenHeight, s_screenFrameRate);

    extern GLES2Playground_t e_playgroundArabesque;
    extern GLES2Playground_t e_playgroundAudioVisualizer;
    extern GLES2Playground_t e_playgroundClock;
    extern GLES2Playground_t e_playgroundFlyingCubes;
    extern GLES2Playground_t e_playgroundFont;
    extern GLES2Playground_t e_playgroundMouseKeyboard;
    extern GLES2Playground_t e_play;

    e_play = e_playgroundMouseKeyboard;

    int rc = pthread_create(&s_renderingThread, NULL, glThread, NULL);
    assert(0 == rc);

    float blaub[135];
    //sleep( 5 );

    int cnt = 0;

    if (e_play.setIdle) {
        e_play.setIdle(true);
    }

    while (s_renderingThreadAlive) {
        //fputs( "o", stdout );
        //fflush( stdout );
        usleep(500000);
        //sleep( 1 );


        for (int i = 0; i < 135; i++) {
            blaub[i] = drand48();
        }

        if (e_play.commitData) {
            e_play.commitData(blaub);
        }

        if (e_play.setString) {
            switch (cnt) {
                case 1:
                    e_play.setString("Titel\nAlbum\nInterpret");
                    break;

                case 11:
                    e_play.setString("Affe\nTot\nSchade!");
                    break;

                case 21:
                    e_play.setString("Lorem ipsum dolor sit amet, consectetur, adipisci velit.\nStet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\nAt vero eos et accusam et justo duo dolores et ea rebum.");
                    break;

                case 31:
                    e_play.setString("");
                    break;

                case 41:
                    cnt = 0;
                    break;

                default:
                    break;
            }

            cnt++;
        }
    }

    pthread_join(s_renderingThread, NULL);

    exit(EXIT_SUCCESS);
}
