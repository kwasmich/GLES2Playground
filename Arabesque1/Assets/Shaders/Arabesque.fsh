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
    gl_FragColor = v_color;
}
