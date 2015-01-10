//
//  Shader.fsh
//  OpenGLES2Reference
//
//  Created by Michael Kwasnicki on 04.08.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

varying lowp vec4 v_color;

void main()
{
    //v_color.a = 1.0;
    gl_FragColor = vec4( v_color.rgb, 1.0 );
}
