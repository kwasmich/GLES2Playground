//
//  FreeType3D.c
//  FreeType
//
//  Created by Michael Kwasnicki on 25.01.15.
//
//

#include "FreeType3D.h"


#include "encoding.h"


#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>



void ft3dStringToVertexArray( ft3dVertex_t ** out_vertices, uint16_t * out_numVertex, float * out_width, const ft3dFontMap_t * in_FONT_MAP, const char * in_STRING, const float in_X, const float in_Y, const float in_LINE_SPACING, const float in_SCALE, const RGBA_t in_COLOR, const bool in_TOP_DOWN ) {
    wchar_t *wStr = NULL;
    utf8ToWChar( &wStr, in_STRING );
    
    size_t numChars = wcslen( wStr );
    ft3dVertex_t * vertices = malloc( numChars * 6 * sizeof( ft3dVertex_t ) );
    float penX = in_X;
    float penY = in_Y;
    
    for ( size_t c = 0; c < numChars; c++ ) {
        size_t idx = wStr[c];
        
        if ( idx >= 256 ) {
            continue;
        }
        
        if ( idx == '\n' ) {
            if ( in_TOP_DOWN ) {
                penX -= in_LINE_SPACING;
                penY = in_Y;
            } else {
                penX = in_X;
                penY -= in_LINE_SPACING;
            }
        }
        
        ft3dFontMap_t f = in_FONT_MAP[idx];
        vertices[6 * c + 0].x = (f.bitMapLeft) * in_SCALE;
        vertices[6 * c + 0].y = (f.bitMapTop) * in_SCALE;
        vertices[6 * c + 0].color = in_COLOR;
        vertices[6 * c + 1].x = (f.bitMapLeft) * in_SCALE;
        vertices[6 * c + 1].y = (f.bitMapTop) * in_SCALE;
        vertices[6 * c + 1].s = f.posX;
        vertices[6 * c + 1].t = f.posY;
        vertices[6 * c + 1].color = in_COLOR;
        vertices[6 * c + 2].x = (f.bitMapLeft) * in_SCALE;
        vertices[6 * c + 2].y = (f.bitMapTop - f.bitMapHeight) * in_SCALE;
        vertices[6 * c + 2].s = f.posX;
        vertices[6 * c + 2].t = f.posY + f.bitMapHeight;
        vertices[6 * c + 2].color = in_COLOR;
        vertices[6 * c + 3].x = (f.bitMapLeft + f.bitMapWidth) * in_SCALE;
        vertices[6 * c + 3].y = (f.bitMapTop) * in_SCALE;
        vertices[6 * c + 3].s = f.posX + f.bitMapWidth;
        vertices[6 * c + 3].t = f.posY;
        vertices[6 * c + 3].color = in_COLOR;
        vertices[6 * c + 4].x = (f.bitMapLeft + f.bitMapWidth) * in_SCALE;
        vertices[6 * c + 4].y = (f.bitMapTop - f.bitMapHeight) * in_SCALE;
        vertices[6 * c + 4].s = f.posX + f.bitMapWidth;
        vertices[6 * c + 4].t = f.posY + f.bitMapHeight;
        vertices[6 * c + 4].color = in_COLOR;
        vertices[6 * c + 5].x = (f.bitMapLeft + f.bitMapWidth) * in_SCALE;
        vertices[6 * c + 5].y = (f.bitMapTop - f.bitMapHeight) * in_SCALE;
        vertices[6 * c + 5].color = in_COLOR;
        
        if ( in_TOP_DOWN ) {
            for ( int i = 0; i < 6; i++ ) {
                float x = vertices[6 * c + i].x;
                float y = vertices[6 * c + i].y;
                vertices[6 * c + i].x =  y;
                vertices[6 * c + i].y = -x;
            }
        }
        
        for ( int i = 0; i < 6; i++ ) {
            vertices[6 * c + i].x += penX;
            vertices[6 * c + i].y += penY;
        }
        
        if ( in_TOP_DOWN ) {
            penY -= f.advance * in_SCALE;
        } else {
            penX += f.advance * in_SCALE;
        }
    }
    
    assert( *out_vertices == NULL );
    *out_vertices = vertices;
    *out_numVertex = numChars * 6;
    
    if ( in_TOP_DOWN ) {
        *out_width = in_Y - penY;
    } else {
        *out_width = penX - in_X;
    }
}
