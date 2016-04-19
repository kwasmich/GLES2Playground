//
//  Shader.vsh
//  OpenGLES2Reference
//
//  Created by Michael Kwasnicki on 04.08.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoord;

varying lowp vec4 v_color;
varying lowp vec2 v_texCoord[5];

uniform vec4 u_vec4;
uniform mat4 u_projectionMatrix;

void main()
{
    v_color = a_color;
    v_texCoord[0] = a_texCoord;
    
    for ( int i = 0; i < 4; i++ ) {
        float s = sin( u_vec4[i] );
        float c = cos( u_vec4[i] );
        mat2 rot = mat2( c, s, -s, c );
        v_texCoord[i+1] = (rot * (a_texCoord - 0.5)) + 0.5;
    }
    
    gl_Position = u_projectionMatrix * a_position;
}
