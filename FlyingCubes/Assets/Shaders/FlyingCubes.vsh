//
//  Shader.vsh
//  OpenGLES2Reference
//
//  Created by Michael Kwasnicki on 04.08.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

attribute vec4 a_position;
attribute vec3 a_normal;

varying lowp vec4 v_color;

uniform mat4 u_projectionMatrix;
uniform mat3 u_normalMatrix;

void main()
{
    vec3 eyeNormal = normalize(u_normalMatrix * a_normal);
    vec3 lightPosition = vec3(0.0, 0.0, 1.0);
    vec4 diffuseColor = vec4(0.4, 0.4, 1.0, 1.0);

    float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));

    v_color = diffuseColor * nDotVP;

    gl_Position = u_projectionMatrix * a_position;
}
