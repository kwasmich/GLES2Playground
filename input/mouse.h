//
//  mouse.h
//  GLES2Playground
//
//  Created by Michael Kwasnicki on 21.02.16.
//  Copyright (c) 2016 Michael Kwasnicki. All rights reserved.
//

#ifndef __mouse_h__
#define __mouse_h__


#include <stdint.h>


typedef struct Mouse_s {
    int fd;
    int32_t x;
    int32_t y;
    int8_t dx;
    int8_t dy;
    int8_t sx;
    int8_t sy;
    uint8_t lmb : 1;
    uint8_t mmb : 1;
    uint8_t rmb : 1;
    uint8_t smb : 1;
    uint8_t emb : 1;
    
    struct {
        uint16_t dx : 1;
        uint16_t dy : 1;
        uint16_t sx : 1;
        uint16_t sy : 1;
        uint16_t lmbUp : 1;
        uint16_t lmbDn : 1;
        uint16_t mmbUp : 1;
        uint16_t mmbDn : 1;
        uint16_t rmbUp : 1;
        uint16_t rmbDn : 1;
        uint16_t smbUp : 1;
        uint16_t smbDn : 1;
        uint16_t embUp : 1;
        uint16_t embDn : 1;
    } chg;
} Mouse_s;


Mouse_s s_mouse;


void mouseInit();
void mouseFree();
void mouseUpdate();
void mouseGet(void (* const in_MOUSE_MOVE)(const int8_t in_DX, const int8_t in_DY), void (* const in_MOUSE_DOWN)(const uint8_t in_BUTTON), void (* const in_MOUSE_UP)(const uint8_t in_BUTTON), void (* const in_MOUSE_SCROLL)(const int8_t in_DX, const int8_t in_DY));
void mouseReset();


#endif
