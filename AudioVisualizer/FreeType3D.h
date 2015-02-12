//
//  FreeType3D.h
//  FreeType
//
//  Created by Michael Kwasnicki on 25.01.15.
//
//

#ifndef __FreeType__FreeType3D__
#define __FreeType__FreeType3D__



#include <stdint.h>



typedef struct {
    uint8_t charCode;
    uint8_t bitMapWidth, bitMapHeight;
    uint16_t posX, posY;
    int8_t bitMapLeft, bitMapTop;
    uint16_t advance;
} ft3dFontMap_t;



typedef struct {
    float x, y;
    uint16_t s, t;
} ft3dVertices_t;



void ft3dStringToVertexArray( ft3dVertices_t ** out_vertices, uint16_t * out_numVertex, const ft3dFontMap_t * in_FONT_MAP, const char * in_STRING, const float in_X, const float in_Y, const float in_LINE_SPACING );



#endif /* defined(__FreeType__FreeType3D__) */
