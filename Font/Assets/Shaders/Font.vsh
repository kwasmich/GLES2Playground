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
varying lowp vec2 v_texCoord;

uniform float u_textureSize;
uniform mat4 u_projectionMatrix;

void main()
{
    v_color = a_color;
    v_texCoord = a_texCoord;
    v_texCoord.x = a_texCoord.x / u_textureSize;
    v_texCoord.y = a_texCoord.y / u_textureSize;
    vec4 pos = a_position;
    gl_Position = u_projectionMatrix * pos;
}
