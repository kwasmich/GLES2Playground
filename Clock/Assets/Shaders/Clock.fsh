//
//  Shader.fsh
//  OpenGLES2Reference
//
//  Created by Michael Kwasnicki on 04.08.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

varying lowp vec4 v_color;
varying lowp vec2 v_texCoord[5];

uniform sampler2D u_texture[5];

void main()
{
    lowp vec4 fragColor = v_color;
    fragColor.rgb += texture2D( u_texture[0], v_texCoord[0] ).rgb;
    fragColor.rgb += texture2D( u_texture[1], v_texCoord[1] ).rgb;
    fragColor.rgb += texture2D( u_texture[2], v_texCoord[2] ).rgb;
    fragColor.rgb += texture2D( u_texture[3], v_texCoord[3] ).rgb;
    fragColor.rgb += texture2D( u_texture[4], v_texCoord[4] ).rgb;
    gl_FragColor = fragColor;
}
