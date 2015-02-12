//
//  FreeType3D.c
//  FreeType
//
//  Created by Michael Kwasnicki on 25.01.15.
//
//

#include "FreeType3D.h"


#include <assert.h>
#include <stdlib.h>
#include <string.h>



void ft3dStringToVertexArray( ft3dVertices_t ** out_vertices, uint16_t * out_numVertex, const ft3dFontMap_t * in_FONT_MAP, const char * in_STRING, const float in_X, const float in_Y, const float in_LINE_SPACING ) {
    size_t numChars = strlen( in_STRING );
    
    ft3dVertices_t * vertices = malloc( numChars * 6 * sizeof( ft3dVertices_t ) );
    float penX = in_X;
    float penY = in_Y;
    
    for ( size_t c = 0; c < numChars; c++ ) {
        size_t idx = in_STRING[c];
        
        if ( idx == '\n' ) {
            penX = in_X;
            penY -= in_LINE_SPACING;
        }
        
        ft3dFontMap_t f = in_FONT_MAP[idx];
        vertices[6 * c + 0].x = penX + f.bitMapLeft;
        vertices[6 * c + 0].y = penY + f.bitMapTop;
        vertices[6 * c + 1].x = penX + f.bitMapLeft;
        vertices[6 * c + 1].y = penY + f.bitMapTop;
        vertices[6 * c + 1].s = f.posX;
        vertices[6 * c + 1].t = f.posY;
        vertices[6 * c + 2].x = penX + f.bitMapLeft;
        vertices[6 * c + 2].y = penY + f.bitMapTop - f.bitMapHeight;
        vertices[6 * c + 2].s = f.posX;
        vertices[6 * c + 2].t = f.posY + f.bitMapHeight;
        vertices[6 * c + 3].x = penX + f.bitMapLeft + f.bitMapWidth;
        vertices[6 * c + 3].y = penY + f.bitMapTop;
        vertices[6 * c + 3].s = f.posX + f.bitMapWidth;
        vertices[6 * c + 3].t = f.posY;
        vertices[6 * c + 4].x = penX + f.bitMapLeft + f.bitMapWidth;
        vertices[6 * c + 4].y = penY + f.bitMapTop - f.bitMapHeight;
        vertices[6 * c + 4].s = f.posX + f.bitMapWidth;
        vertices[6 * c + 4].t = f.posY + f.bitMapHeight;
        vertices[6 * c + 5].x = penX + f.bitMapLeft + f.bitMapWidth;
        vertices[6 * c + 5].y = penY + f.bitMapTop - f.bitMapHeight;
        penX += f.advance;
    }
    
    assert( *out_vertices == NULL );
    *out_vertices = vertices;
    *out_numVertex = numChars * 6;
}
