//
//  FreeType3D.h
//  FreeType
//
//  Created by Michael Kwasnicki on 25.01.15.
//
//

#ifndef __FreeType__FreeType3D__
#define __FreeType__FreeType3D__



#include <stdbool.h>
#include <stdint.h>



typedef struct {
    uint8_t charCode;
    uint8_t bitMapWidth, bitMapHeight;
    uint16_t posX, posY;
    int8_t bitMapLeft, bitMapTop;
    uint16_t advance;
} ft3dFontMap_t;


typedef struct RGBA_s {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} RGBA_t;


typedef struct {
    float x, y;
    uint16_t s, t;
    RGBA_t color;
} ft3dVertex_t;



void ft3dStringToVertexArray( ft3dVertex_t ** out_vertices, uint16_t * out_numVertex, float * out_width, const ft3dFontMap_t * in_FONT_MAP, const char * in_STRING, const float in_X, const float in_Y, const float in_LINE_SPACING, const float in_SCALE, const RGBA_t in_COLOR, const bool in_TOP_DOWN );



#endif /* defined(__FreeType__FreeType3D__) */
