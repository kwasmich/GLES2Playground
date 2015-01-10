//
//  Shader.vsh
//  OpenGLES2Reference
//
//  Created by Michael Kwasnicki on 04.08.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


attribute vec4 a_position;
attribute vec4 a_color;

varying lowp vec4 v_color;

uniform mat4 u_projectionMatrix;

void main()
{
    v_color = a_color;
    gl_Position = u_projectionMatrix * a_position;
}
