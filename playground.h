//
//  playground.h
//  GLES2Playground
//
//  Created by Michael Kwasnicki on 26.12.14.
//  Copyright (c) 2014 Michael Kwasnicki. All rights reserved.
//

#ifndef GLES2Playground_playground_h
#define GLES2Playground_playground_h


typedef struct {
    char *name;
    void (*init)( const int in_WIDTH, const int in_HEIGHT );
    void (*deinit)( void );
    void (*update)( void );
    void (*draw)( void );
    void (*commitData)( const float in_DATA[] );
    void (*setString)( const char * in_STRING );
} GLES2Playground_t;


#endif
