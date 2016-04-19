//
//  Shader.fsh
//  OpenGLES2Reference
//
//  Created by Michael Kwasnicki on 04.08.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

varying lowp vec4 v_color;
varying lowp vec2 v_texCoord;

uniform sampler2D u_texture;

void main()
{
    lowp vec4 fragColor = v_color;
    fragColor.a = texture2D( u_texture, v_texCoord ).r;
    gl_FragColor = fragColor;
}
