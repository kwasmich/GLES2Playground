//
//  Math3D.h
//  Math3D
//
//  Created by Michael Kwasnicki on 02.08.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Math3D_Math3D_h
#define Math3D_Math3D_h


#define print(X) _Generic((X), \
vec2: vec2Print, \
vec3: vec3Print, \
vec4: vec4Print, \
mat2: mat2Print, \
mat3: mat3Print, \
mat4: mat4Print)(&X)

#define add(X, Y) _Generic((X), \
vec2: vec2Add, \
vec3: vec3Add, \
vec4: vec4Add, \
mat2: mat2Add, \
mat3: mat3Add, \
mat4: mat4Add)(X, Y)

#define sub(X, Y) _Generic((X), \
vec2: vec2Sub, \
vec3: vec3Sub, \
vec4: vec4Sub, \
mat2: mat2Sub, \
mat3: mat3Sub, \
mat4: mat4Sub)(X, Y)

#define mul(X, Y) _Generic((X), \
vec2: _Generic((Y), vec2: vec2Mul, mat2: vec2MulMat2, float: vec2MulScalar, default: 0), \
vec3: _Generic((Y), vec3: vec3Mul, mat3: vec3MulMat3, float: vec3MulScalar, default: 0), \
vec4: _Generic((Y), vec4: vec4Mul, mat4: vec4MulMat4, float: vec4MulScalar, default: 0), \
mat2: _Generic((Y), mat2: mat2Mul, vec2: mat2MulVec2, float: mat2MulScalar, default: 0), \
mat3: _Generic((Y), mat3: mat3Mul, vec3: mat3MulVec3, float: mat3MulScalar, default: 0), \
mat4: _Generic((Y), mat4: mat4Mul, vec4: mat4MulVec4, float: mat4MulScalar, default: 0))(X, Y)

#define length(X) _Generic((X), \
vec2: vec2Length, \
vec3: vec3Length, \
vec4: vec4Length)(X)

#define distance(X, Y) _Generic((X), \
vec2: vec2Distance, \
vec3: vec3Distance, \
vec4: vec4Distance)(X, Y)

#define dot(X, Y) _Generic((X), \
vec2: vec2Dot, \
vec3: vec3Dot, \
vec4: vec4Dot)(X, Y)

#define normalize(X) _Generic((X), vec2: vec2Normalize, vec3: vec3Normalize, vec4: vec4Normalize)(X)

#define matSet(X, S) _Generic((X), \
mat2: mat2Set, \
mat3: mat3Set, \
mat4: mat4Set)(X, S)

#define transpose(X) _Generic((X), \
mat2: mat2Transpose, \
mat3: mat3Transpose, \
mat4: mat4Transpose)(X)

#define inverseTranspose(X) _Generic((X), \
mat2: mat2InverseTranspose, \
mat3: mat3InverseTranspose, \
mat4: mat4InverseTranspose)(X)

#define inverse(X) _Generic((X), \
mat2: mat2Inverse, \
mat3: mat3Inverse, \
mat4: mat4Inverse)(X)

#include <assert.h> // assert
#include <math.h>   // sqrt
#include <stdio.h>  // printf
#include <stdlib.h> // drand48
#include <string.h> // memcpy

//abs, clamp, smoothstep, min, max, floor

static inline float sign( const float in_X );
static inline float fract( const float in_X );
static inline float clamp( const float in_X, const float in_MIN, const float in_MAX );
static inline float mix( const float in_X, const float in_Y, const float in_A );
static inline float step( const float in_EDGE, const float in_X );
static inline float smoothstep( const float in_EDGE_0, const float in_EDGE_1, const float in_X );
static inline float inverseSQRT( const float in_S );
static inline float fastInverseSQRT( const float in_S );

typedef union {
    struct { float x, y; };
    struct { float s, t; };
    float v[2];
} vec2;

typedef union {
    struct { float x, y, z; };
    struct { float r, g, b; };
    struct { float s, t, p; };
    float v[3];
} vec3;

typedef union {
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
    struct { float s, t, p, q; };
    float v[4];
} vec4;


// matrix in column major order
// 0 3 6
// 1 4 7
// 2 5 8
typedef union {
    struct { float m00, m10, m01, m11; };
    struct { vec2 v0, v1; }; 
    float m[4];
} mat2;

typedef union {
    struct { float m00, m10, m20, m01, m11, m21, m02, m12, m22; };
    struct { vec3 v0, v1, v2; }; 
    float m[9];
} mat3;

typedef union {
    struct { float m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33; };
    struct { vec4 v0, v1, v2, v3; }; 
    float m[16];
} mat4;


static inline void vec2Print( const vec2* in_V );
static inline void vec3Print( const vec3* in_V );
static inline void vec4Print( const vec4* in_V );
static inline void mat2Print( const mat2* in_M );
static inline void mat3Print( const mat3* in_M );
static inline void mat4Print( const mat4* in_M );

static inline mat2 mat2Make( const float in_M00, const float in_M10, const float in_M01, const float in_M11 );
static inline mat3 mat3Make( const float in_M00, const float in_M10, const float in_M20, const float in_M01, const float in_M11, const float in_M21, const float in_M02, const float in_M12, const float in_M22 );
static inline mat4 mat4Make( const float in_M00, const float in_M10, const float in_M20, const float in_M30, const float in_M01, const float in_M11, const float in_M21, const float in_M31, const float in_M02, const float in_M12, const float in_M22, const float in_M32, const float in_M03, const float in_M13, const float in_M23, const float in_M33 );

static inline mat2 mat2MakeWithDiagonal( const float in_S );
static inline mat3 mat3MakeWithDiagonal( const float in_S );
static inline mat4 mat4MakeWithDiagonal( const float in_S );

static inline mat2 mat2MakeWithColumns( const vec2 in_V0, const vec2 in_V1 );
static inline mat3 mat3MakeWithColumns( const vec2 in_V0, const vec2 in_V1, const vec2 in_V2 );
static inline mat4 mat4MakeWithColumns( const vec2 in_V0, const vec2 in_V1, const vec2 in_V2, const vec2 in_V3 );

static inline mat4 mat4MakeOrtho( const float in_L, const float in_R, const float in_B, const float in_T, const float in_N, const float in_F );
static inline mat4 mat4MakeFrustum( const float in_L, const float in_R, const float in_B, const float in_T, const float in_N, const float in_F );
static inline mat4 mat4MakePerspective( const float in_FOV_RADIANS, const float in_AR, const float in_N, const float in_F );
static inline mat4 mat4MakeRotate( const float in_RADIANS, const vec3 in_V );
static inline mat4 mat4MakeRotateX( const float in_RADIANS );
static inline mat4 mat4MakeRotateY( const float in_RADIANS );
static inline mat4 mat4MakeRotateZ( const float in_RADIANS );
static inline mat4 mat4MakeScale( const float in_S );
static inline mat4 mat4MakeTranslate( const vec3 in_V );

static inline mat2 mat2FromMat3( const mat3 in_M );
static inline mat3 mat3FromMat4( const mat4 in_M );

static inline mat2 mat2Transpose( const mat2 in_M );
static inline mat3 mat3Transpose( const mat3 in_M );
static inline mat4 mat4Transpose( const mat4 in_M );

static inline mat2 mat2InverseTranspose( const mat2 in_M );
static inline mat3 mat3InverseTranspose( const mat3 in_M );
static inline mat4 mat4InverseTranspose( const mat4 in_M );

static inline mat2 mat2Inverse( const mat2 in_M );
static inline mat3 mat3Inverse( const mat3 in_M );
static inline mat4 mat4Inverse( const mat4 in_M );

static inline vec2 vec2Add( const vec2 in_A, const vec2 in_B );
static inline vec3 vec3Add( const vec3 in_A, const vec3 in_B );
static inline vec4 vec4Add( const vec4 in_A, const vec4 in_B );
static inline mat2 mat2Add( const mat2 in_A, const mat2 in_B );
static inline mat3 mat3Add( const mat3 in_A, const mat3 in_B );
static inline mat4 mat4Add( const mat4 in_A, const mat4 in_B );

static inline vec2 vec2Sub( const vec2 in_A, const vec2 in_B );
static inline vec3 vec3Sub( const vec3 in_A, const vec3 in_B );
static inline vec4 vec4Sub( const vec4 in_A, const vec4 in_B );
static inline mat2 mat2Sub( const mat2 in_A, const mat2 in_B );
static inline mat3 mat3Sub( const mat3 in_A, const mat3 in_B );
static inline mat4 mat4Sub( const mat4 in_A, const mat4 in_B );

static inline vec2 vec2Mul( const vec2 in_A, const vec2 in_B );
static inline vec3 vec3Mul( const vec3 in_A, const vec3 in_B );
static inline vec4 vec4Mul( const vec4 in_A, const vec4 in_B );
static inline mat2 mat2Mul( const mat2 in_A, const mat2 in_B );
static inline mat3 mat3Mul( const mat3 in_A, const mat3 in_B );
static inline mat4 mat4Mul( const mat4 in_A, const mat4 in_B );

static inline vec2 vec2MulMat2( const vec2 in_V, const mat2 in_M );
static inline vec3 vec3MulMat3( const vec3 in_V, const mat3 in_M );
static inline vec4 vec4MulMat4( const vec4 in_V, const mat4 in_M );
static inline vec2 mat2MulVec2( const mat2 in_M, const vec2 in_V );
static inline vec3 mat3MulVec3( const mat3 in_M, const vec3 in_V );
static inline vec4 mat4MulVec4( const mat4 in_M, const vec4 in_V );

static inline vec2 vec2MulScalar( const vec2 in_V, const float in_S );
static inline vec3 vec3MulScalar( const vec3 in_V, const float in_S );
static inline vec4 vec4MulScalar( const vec4 in_V, const float in_S );
static inline mat2 mat2MulScalar( const mat2 in_M, const float in_S );
static inline mat3 mat3MulScalar( const mat3 in_M, const float in_S );
static inline mat4 mat4MulScalar( const mat4 in_M, const float in_S );

static inline float vec2Length( const vec2 in_V );
static inline float vec3Length( const vec3 in_V );
static inline float vec4Length( const vec4 in_V );

static inline float vec2Distance( const vec2 in_A, const vec2 in_B );
static inline float vec3Distance( const vec3 in_A, const vec3 in_B );
static inline float vec4Distance( const vec4 in_A, const vec4 in_B );

static inline float vec2Dot( const vec2 in_A, const vec2 in_B );
static inline float vec3Dot( const vec3 in_A, const vec3 in_B );
static inline float vec4Dot( const vec4 in_A, const vec4 in_B );

static inline vec3 vec3Cross( const vec3 in_A, const vec3 in_B );

static inline vec2 vec2Normalize( const vec2 in_V );
static inline vec3 vec3Normalize( const vec3 in_V );
static inline vec4 vec4Normalize( const vec4 in_V );

//void vec2AddInplace( vec2 in_out_a, const vec2 in_B );
//void vec3AddInplace( vec3 in_out_a, const vec3 in_B );
//void vec4AddInplace( vec4 in_out_a, const vec4 in_B );
//void mat2AddInplace( mat2 in_out_a, const mat2 in_B );
//void mat3AddInplace( mat3 in_out_a, const mat3 in_B );
//void mat4AddInplace( mat4 in_out_a, const mat4 in_B );

/*
#warning to be implemented
// use assignment instead?
//void vec2Set( vec2 out_v, const vec2 in_V );
//void vec3Set( vec3 out_v, const vec3 in_V );
//void vec4Set( vec4 out_v, const vec4 in_V );

void vec2Sub( vec2 out_v, const vec2 in_A, const vec2 in_B );
void vec3Sub( vec3 out_v, const vec3 in_A, const vec3 in_B );
void vec4Sub( vec4 out_v, const vec4 in_A, const vec4 in_B );


void vec2MulVec2( vec2 out_v, const vec2 in_A, const float in_S );
void vec3MulVec3( vec3 out_v, const vec3 in_A, const float in_S );
void vec4MulVec4( vec4 out_v, const vec4 in_A, const float in_S );

void vec3Cross( vec3 out_v, const vec3 in_A, const vec3 in_B );

float vec2Distance( const vec2 in_A, const vec2 in_B );
float vec3Distance( const vec3 in_A, const vec3 in_B );
float vec4Distance( const vec4 in_A, const vec4 in_B );



void vec3FromVec4( vec3 out_v, const vec4 in_V );



void mat2Set( mat2 out_m, const float in_S );
void mat3Set( mat3 out_m, const float in_S );
void mat4Set( mat4 out_m, const float in_S );

void mat2Mul( mat2 out_m, const mat2 in_A, const mat2 in_B );
void mat3Mul( mat3 out_m, const mat3 in_A, const mat3 in_B );
void mat4Mul( mat4 out_m, const mat4 in_A, const mat4 in_B );

float mat3Determinant( const mat4 in_M );
void mat3FromMat4( mat3 out_m, const mat4 in_M );
void mat3Inverse( mat3 out_m, const mat3 in_M );
void mat3InverseTranspose( mat3 out_m, const mat3 in_M );
void mat3Transpose( mat3 out_m, const mat3 in_M );

#warning to be implemented
void mat4LookAt( mat4 out_m, const vec3 in_POS, const vec3 in_DIR, const vec3 in_UP );
void mat4Inverse( mat4 out_m, const mat4 in_M );
void mat4MulVec3( vec3 out_v, const mat4 in_M, const vec3 in_V );
void mat4MulVec4( vec4 out_v, const mat4 in_M, const vec4 in_V );

*/



static inline float sign( const float in_X ) {
	float result = 0;
	result = ( result < 0 ) ? -1 : result;
	result = ( result > 0 ) ?  1 : result;
	return result;
}

// I'm not sure if this is correct for negative numbers
static inline float fract( const float in_X ) {
	return in_X - floorf( in_X );
}

static inline float clamp( const float in_X, const float in_MIN, const float in_MAX ) {
	return fminf( fmaxf( in_X, in_MIN ), in_MAX );
}

static inline float mix( const float in_X, const float in_Y, const float in_A ) {
	return fmaf( in_A, in_Y - in_X, in_X );
}

static inline float step( const float in_EDGE, const float in_X ) {
	return ( in_X < in_EDGE ) ? 0 : 1;
}

static inline float smoothstep( const float in_EDGE_0, const float in_EDGE_1, const float in_X ) {
	float t = clamp( ( in_X - in_EDGE_0 ) / ( in_EDGE_1 - in_EDGE_1 ), 0, 1 );
	return t * t * ( 3.0f - 2.0f * t );
}

static inline float inverseSQRT( const float in_S ) {
    return 1.0f / sqrtf( in_S );
}

static inline float fastInverseSQRT( const float in_S ) {
    // use SSE's rsqrtss instead
    float xhalf = 0.5f * in_S;
    
    union {
        float f;
        unsigned int i;
    } fi;
    
    fi.f = in_S;
    fi.i = 0x5f375a86 - (fi.i >> 1);
    fi.f = fi.f * (1.5f - xhalf * fi.f * fi.f);
    return fi.f;
    
    //unsigned int i = *(unsigned int*)&in_S;
    //i = 0x5f375a86 - (i >> 1);
    //float tmp = *(float*)&i;
    //tmp = tmp * (1.5f - xhalf * tmp * tmp);
    //tmp = tmp * (1.5f - xhalf * tmp * tmp);
    //return tmp;
}


static inline void vec2Print( const vec2* in_V ) {
    puts( "vec2:" );
    printf( "( %f, %f )\n", in_V->v[0], in_V->v[1] );
}

static inline void vec3Print( const vec3* in_V ) {
    puts( "vec3:" );
    printf( "( %f, %f, %f )\n", in_V->v[0], in_V->v[1], in_V->v[2] );
}

static inline void vec4Print( const vec4* in_V ) {
    puts( "vec4:" );
    printf( "( %f, %f, %f, %f )\n", in_V->v[0], in_V->v[1], in_V->v[2], in_V->v[3] );
}

static inline void mat2Print( const mat2* in_M ) {
    puts( "mat2:" );
    printf( "%f %f\n", in_M->m[0], in_M->m[2] );
    printf( "%f %f\n", in_M->m[1], in_M->m[3] );
}

static inline void mat3Print( const mat3* in_M ) {
    puts( "mat3:" );
    printf( "%f %f %f\n", in_M->m[0], in_M->m[3], in_M->m[6] );
    printf( "%f %f %f\n", in_M->m[1], in_M->m[4], in_M->m[7] );
    printf( "%f %f %f\n", in_M->m[2], in_M->m[5], in_M->m[8] );
}

static inline void mat4Print( const mat4* in_M ) {
    puts( "mat4:" );
    printf( "%f %f %f %f\n", in_M->m[0], in_M->m[4], in_M->m[8],  in_M->m[12] );
    printf( "%f %f %f %f\n", in_M->m[1], in_M->m[5], in_M->m[9],  in_M->m[13] );
    printf( "%f %f %f %f\n", in_M->m[2], in_M->m[6], in_M->m[10], in_M->m[14] );
    printf( "%f %f %f %f\n", in_M->m[3], in_M->m[7], in_M->m[11], in_M->m[15] );
}

static inline mat2 mat2MakeWithDiagonal( const float in_S ) {
    mat2 m;
    m.m[0] = 1;   m.m[2] = 0;
    m.m[1] = 0;   m.m[3] = 1;
    return m;
}

static inline mat3 mat3MakeWithDiagonal( const float in_S ) {
    mat3 m;
    m.m[0] = 1;   m.m[3] = 0;   m.m[6] = 0;
    m.m[1] = 0;   m.m[4] = 1;   m.m[7] = 0;
    m.m[2] = 0;   m.m[5] = 0;   m.m[8] = 1;
    return m;
}

static inline mat4 mat4MakeWithDiagonal( const float in_S ) {
    mat4 m;
    m.m[0] = 1;   m.m[4] = 0;   m.m[8]  = 0;   m.m[12] = 0;
    m.m[1] = 0;   m.m[5] = 1;   m.m[9]  = 0;   m.m[13] = 0;
    m.m[2] = 0;   m.m[6] = 0;   m.m[10] = 1;   m.m[14] = 0;
    m.m[3] = 0;   m.m[7] = 0;   m.m[11] = 0;   m.m[15] = 1;
    return m;
}

static inline vec2 vec2Add( const vec2 in_A, const vec2 in_B ) {
    vec2 v;
    v.v[0] = in_A.v[0] + in_B.v[0];
    v.v[1] = in_A.v[1] + in_B.v[1];
    return v;
}

static inline vec3 vec3Add( const vec3 in_A, const vec3 in_B ) {
    vec3 v;
    v.v[0] = in_A.v[0] + in_B.v[0];
    v.v[1] = in_A.v[1] + in_B.v[1];
    v.v[2] = in_A.v[2] + in_B.v[2];
    return v;
}

static inline vec4 vec4Add( const vec4 in_A, const vec4 in_B ) {
    vec4 v;
    v.v[0] = in_A.v[0] + in_B.v[0];
    v.v[1] = in_A.v[1] + in_B.v[1];
    v.v[2] = in_A.v[2] + in_B.v[2];
    v.v[3] = in_A.v[3] + in_B.v[3];
    return v;
}

static inline mat2 mat2Add( const mat2 in_A, const mat2 in_B ) {
    mat2 m;
    m.m[0] = in_A.m[0] + in_B.m[0];
    m.m[1] = in_A.m[1] + in_B.m[1];
    m.m[2] = in_A.m[2] + in_B.m[2];
    m.m[3] = in_A.m[3] + in_B.m[3];
    return m;
}

static inline mat3 mat3Add( const mat3 in_A, const mat3 in_B ) {
    mat3 m;
    m.m[0] = in_A.m[0] + in_B.m[0];
    m.m[1] = in_A.m[1] + in_B.m[1];
    m.m[2] = in_A.m[2] + in_B.m[2];
    m.m[3] = in_A.m[3] + in_B.m[3];
    m.m[4] = in_A.m[4] + in_B.m[4];
    m.m[5] = in_A.m[5] + in_B.m[5];
    m.m[6] = in_A.m[6] + in_B.m[6];
    m.m[7] = in_A.m[7] + in_B.m[7];
    m.m[8] = in_A.m[8] + in_B.m[8];
    return m;
}

static inline mat4 mat4Add( const mat4 in_A, const mat4 in_B ) {
    mat4 m;
    m.m[0]  = in_A.m[0]  + in_B.m[0];
    m.m[1]  = in_A.m[1]  + in_B.m[1];
    m.m[2]  = in_A.m[2]  + in_B.m[2];
    m.m[3]  = in_A.m[3]  + in_B.m[3];
    m.m[4]  = in_A.m[4]  + in_B.m[4];
    m.m[5]  = in_A.m[5]  + in_B.m[5];
    m.m[6]  = in_A.m[6]  + in_B.m[6];
    m.m[7]  = in_A.m[7]  + in_B.m[7];
    m.m[8]  = in_A.m[8]  + in_B.m[8];
    m.m[9]  = in_A.m[9]  + in_B.m[9];
    m.m[10] = in_A.m[10] + in_B.m[10];
    m.m[11] = in_A.m[11] + in_B.m[11];
    m.m[12] = in_A.m[12] + in_B.m[12];
    m.m[13] = in_A.m[13] + in_B.m[13];
    m.m[14] = in_A.m[14] + in_B.m[14];
    m.m[15] = in_A.m[15] + in_B.m[15];
    return m;
}

static inline vec2 vec2Sub( const vec2 in_A, const vec2 in_B ) {
    vec2 v;
    v.v[0] = in_A.v[0] - in_B.v[0];
    v.v[1] = in_A.v[1] - in_B.v[1];
    return v;
}

static inline vec3 vec3Sub( const vec3 in_A, const vec3 in_B ) {
    vec3 v;
    v.v[0] = in_A.v[0] - in_B.v[0];
    v.v[1] = in_A.v[1] - in_B.v[1];
    v.v[2] = in_A.v[2] - in_B.v[2];
    return v;
}

static inline vec4 vec4Sub( const vec4 in_A, const vec4 in_B ) {
    vec4 v;
    v.v[0] = in_A.v[0] - in_B.v[0];
    v.v[1] = in_A.v[1] - in_B.v[1];
    v.v[2] = in_A.v[2] - in_B.v[2];
    v.v[3] = in_A.v[3] - in_B.v[3];
    return v;
}

static inline mat2 mat2Sub( const mat2 in_A, const mat2 in_B ) {
    mat2 m;
    m.m[0] = in_A.m[0] - in_B.m[0];
    m.m[1] = in_A.m[1] - in_B.m[1];
    m.m[2] = in_A.m[2] - in_B.m[2];
    m.m[3] = in_A.m[3] - in_B.m[3];
    return m;
}

static inline mat3 mat3Sub( const mat3 in_A, const mat3 in_B ) {
    mat3 m;
    m.m[0] = in_A.m[0] - in_B.m[0];
    m.m[1] = in_A.m[1] - in_B.m[1];
    m.m[2] = in_A.m[2] - in_B.m[2];
    m.m[3] = in_A.m[3] - in_B.m[3];
    m.m[4] = in_A.m[4] - in_B.m[4];
    m.m[5] = in_A.m[5] - in_B.m[5];
    m.m[6] = in_A.m[6] - in_B.m[6];
    m.m[7] = in_A.m[7] - in_B.m[7];
    m.m[8] = in_A.m[8] - in_B.m[8];
    return m;
}

static inline mat4 mat4Sub( const mat4 in_A, const mat4 in_B ) {
    mat4 m;
    m.m[0]  = in_A.m[0]  - in_B.m[0];
    m.m[1]  = in_A.m[1]  - in_B.m[1];
    m.m[2]  = in_A.m[2]  - in_B.m[2];
    m.m[3]  = in_A.m[3]  - in_B.m[3];
    m.m[4]  = in_A.m[4]  - in_B.m[4];
    m.m[5]  = in_A.m[5]  - in_B.m[5];
    m.m[6]  = in_A.m[6]  - in_B.m[6];
    m.m[7]  = in_A.m[7]  - in_B.m[7];
    m.m[8]  = in_A.m[8]  - in_B.m[8];
    m.m[9]  = in_A.m[9]  - in_B.m[9];
    m.m[10] = in_A.m[10] - in_B.m[10];
    m.m[11] = in_A.m[11] - in_B.m[11];
    m.m[12] = in_A.m[12] - in_B.m[12];
    m.m[13] = in_A.m[13] - in_B.m[13];
    m.m[14] = in_A.m[14] - in_B.m[14];
    m.m[15] = in_A.m[15] - in_B.m[15];
    return m;
}

static inline vec2 vec2Mul( const vec2 in_A, const vec2 in_B ) {
    vec2 v;
    v.v[0] = in_A.v[0] * in_B.v[0];
    v.v[1] = in_A.v[1] * in_B.v[1];
    return v;
}

static inline vec3 vec3Mul( const vec3 in_A, const vec3 in_B ) {
    vec3 v;
    v.v[0] = in_A.v[0] * in_B.v[0];
    v.v[1] = in_A.v[1] * in_B.v[1];
    v.v[2] = in_A.v[2] * in_B.v[2];
    return v;
}

static inline vec4 vec4Mul( const vec4 in_A, const vec4 in_B ) {
    vec4 v;
    v.v[0] = in_A.v[0] * in_B.v[0];
    v.v[1] = in_A.v[1] * in_B.v[1];
    v.v[2] = in_A.v[2] * in_B.v[2];
    v.v[3] = in_A.v[3] * in_B.v[3];
    return v;
}

static inline mat2 mat2Mul( const mat2 in_A, const mat2 in_B ) {
    mat2 m;
    m.m[0] = in_A.m[0] * in_B.m[0] + in_A.m[2] * in_B.m[1];
    m.m[1] = in_A.m[1] * in_B.m[0] + in_A.m[3] * in_B.m[1];
    m.m[2] = in_A.m[0] * in_B.m[2] + in_A.m[2] * in_B.m[3];
    m.m[3] = in_A.m[1] * in_B.m[2] + in_A.m[3] * in_B.m[3];
    return m;
}

static inline mat3 mat3Mul( const mat3 in_A, const mat3 in_B ) {
    mat3 m;
    m.m[0] = in_A.m[0] * in_B.m[0] + in_A.m[3] * in_B.m[1] + in_A.m[6] * in_B.m[2];
    m.m[1] = in_A.m[1] * in_B.m[0] + in_A.m[4] * in_B.m[1] + in_A.m[7] * in_B.m[2];
    m.m[2] = in_A.m[2] * in_B.m[0] + in_A.m[5] * in_B.m[1] + in_A.m[8] * in_B.m[2];
    m.m[3] = in_A.m[0] * in_B.m[3] + in_A.m[3] * in_B.m[4] + in_A.m[6] * in_B.m[5];
    m.m[4] = in_A.m[1] * in_B.m[3] + in_A.m[4] * in_B.m[4] + in_A.m[7] * in_B.m[5];
    m.m[5] = in_A.m[2] * in_B.m[3] + in_A.m[5] * in_B.m[4] + in_A.m[8] * in_B.m[5];
    m.m[6] = in_A.m[0] * in_B.m[6] + in_A.m[3] * in_B.m[7] + in_A.m[6] * in_B.m[8];
    m.m[7] = in_A.m[1] * in_B.m[6] + in_A.m[4] * in_B.m[7] + in_A.m[7] * in_B.m[8];
    m.m[8] = in_A.m[2] * in_B.m[6] + in_A.m[5] * in_B.m[7] + in_A.m[8] * in_B.m[8];
    return m;
}

static inline mat4 mat4Mul( const mat4 in_A, const mat4 in_B ) {
    mat4 m;
    m.m[0]  = in_A.m[0] * in_B.m[0]  + in_A.m[4] * in_B.m[1]  + in_A.m[8]  * in_B.m[2]  + in_A.m[12] * in_B.m[3];
    m.m[1]  = in_A.m[1] * in_B.m[0]  + in_A.m[5] * in_B.m[1]  + in_A.m[9]  * in_B.m[2]  + in_A.m[13] * in_B.m[3];
    m.m[2]  = in_A.m[2] * in_B.m[0]  + in_A.m[6] * in_B.m[1]  + in_A.m[10] * in_B.m[2]  + in_A.m[14] * in_B.m[3];
    m.m[3]  = in_A.m[3] * in_B.m[0]  + in_A.m[7] * in_B.m[1]  + in_A.m[11] * in_B.m[2]  + in_A.m[15] * in_B.m[3];
    m.m[4]  = in_A.m[0] * in_B.m[4]  + in_A.m[4] * in_B.m[5]  + in_A.m[8]  * in_B.m[6]  + in_A.m[12] * in_B.m[7];
    m.m[5]  = in_A.m[1] * in_B.m[4]  + in_A.m[5] * in_B.m[5]  + in_A.m[9]  * in_B.m[6]  + in_A.m[13] * in_B.m[7];
    m.m[6]  = in_A.m[2] * in_B.m[4]  + in_A.m[6] * in_B.m[5]  + in_A.m[10] * in_B.m[6]  + in_A.m[14] * in_B.m[7];
    m.m[7]  = in_A.m[3] * in_B.m[4]  + in_A.m[7] * in_B.m[5]  + in_A.m[11] * in_B.m[6]  + in_A.m[15] * in_B.m[7];
    m.m[8]  = in_A.m[0] * in_B.m[8]  + in_A.m[4] * in_B.m[9]  + in_A.m[8]  * in_B.m[10] + in_A.m[12] * in_B.m[11];
    m.m[9]  = in_A.m[1] * in_B.m[8]  + in_A.m[5] * in_B.m[9]  + in_A.m[9]  * in_B.m[10] + in_A.m[13] * in_B.m[11];
    m.m[10] = in_A.m[2] * in_B.m[8]  + in_A.m[6] * in_B.m[9]  + in_A.m[10] * in_B.m[10] + in_A.m[14] * in_B.m[11];
    m.m[11] = in_A.m[3] * in_B.m[8]  + in_A.m[7] * in_B.m[9]  + in_A.m[11] * in_B.m[10] + in_A.m[15] * in_B.m[11];
    m.m[12] = in_A.m[0] * in_B.m[12] + in_A.m[4] * in_B.m[13] + in_A.m[8]  * in_B.m[14] + in_A.m[12] * in_B.m[15];
    m.m[13] = in_A.m[1] * in_B.m[12] + in_A.m[5] * in_B.m[13] + in_A.m[9]  * in_B.m[14] + in_A.m[13] * in_B.m[15];
    m.m[14] = in_A.m[2] * in_B.m[12] + in_A.m[6] * in_B.m[13] + in_A.m[10] * in_B.m[14] + in_A.m[14] * in_B.m[15];
    m.m[15] = in_A.m[3] * in_B.m[12] + in_A.m[7] * in_B.m[13] + in_A.m[11] * in_B.m[14] + in_A.m[15] * in_B.m[15];
    return m;
}

static inline vec2 vec2MulMat2( const vec2 in_V, const mat2 in_M ) {
    vec2 v;
    v.v[0] = in_V.v[0] * in_M.m[0] + in_V.v[1] * in_M.m[1]; // v dot m.v0
    v.v[1] = in_V.v[0] * in_M.m[2] + in_V.v[1] * in_M.m[3]; // v dot m.v1
    return v;
}

static inline vec3 vec3MulMat3( const vec3 in_V, const mat3 in_M ) {
    vec3 v;
    v.v[0] = in_V.v[0] * in_M.m[0] + in_V.v[1] * in_M.m[1] + in_V.v[2] * in_M.m[2]; // v dot m.v0
    v.v[1] = in_V.v[0] * in_M.m[3] + in_V.v[1] * in_M.m[4] + in_V.v[2] * in_M.m[5]; // v dot m.v1
    v.v[2] = in_V.v[0] * in_M.m[6] + in_V.v[1] * in_M.m[7] + in_V.v[2] * in_M.m[8]; // v dot m.v2
    return v;
}

static inline vec4 vec4MulMat4( const vec4 in_V, const mat4 in_M ) {
    vec4 v;
    v.v[0] = in_V.v[0] * in_M.m[0]  + in_V.v[1] * in_M.m[1]  + in_V.v[2] * in_M.m[2]  + in_V.v[3] * in_M.m[3];  // v dot m.v0
    v.v[1] = in_V.v[0] * in_M.m[4]  + in_V.v[1] * in_M.m[5]  + in_V.v[2] * in_M.m[6]  + in_V.v[3] * in_M.m[7];  // v dot m.v1
    v.v[2] = in_V.v[0] * in_M.m[8]  + in_V.v[1] * in_M.m[9]  + in_V.v[2] * in_M.m[10] + in_V.v[3] * in_M.m[11]; // v dot m.v2
    v.v[3] = in_V.v[0] * in_M.m[12] + in_V.v[1] * in_M.m[13] + in_V.v[2] * in_M.m[14] + in_V.v[3] * in_M.m[15]; // v dot m.v3
    return v;
}

static inline vec2 mat2MulVec2( const mat2 in_M, const vec2 in_V ) {
    vec2 v;
    v.v[0] = in_M.m[0] * in_V.v[0] + in_M.m[2] * in_V.v[1] ;
    v.v[1] = in_M.m[1] * in_V.v[0] + in_M.m[3] * in_V.v[1] ;
    return v;
}

static inline vec3 mat3MulVec3( const mat3 in_M, const vec3 in_V ) {
    vec3 v;
    v.v[0] = in_M.m[0] * in_V.v[0] + in_M.m[3] * in_V.v[1] + in_M.m[6] * in_V.v[2];
    v.v[1] = in_M.m[1] * in_V.v[0] + in_M.m[4] * in_V.v[1] + in_M.m[7] * in_V.v[2];
    v.v[2] = in_M.m[2] * in_V.v[0] + in_M.m[5] * in_V.v[1] + in_M.m[8] * in_V.v[2];
    return v;
}

static inline vec4 mat4MulVec4( const mat4 in_M, const vec4 in_V ) {
    vec4 v;
    v.v[0] = in_M.m[0] * in_V.v[0] + in_M.m[4] * in_V.v[1] + in_M.m[8]  * in_V.v[2] + in_M.m[12] * in_V.v[3];
    v.v[1] = in_M.m[1] * in_V.v[0] + in_M.m[5] * in_V.v[1] + in_M.m[9]  * in_V.v[2] + in_M.m[13] * in_V.v[3];
    v.v[2] = in_M.m[2] * in_V.v[0] + in_M.m[6] * in_V.v[1] + in_M.m[10] * in_V.v[2] + in_M.m[14] * in_V.v[3];
    v.v[3] = in_M.m[3] * in_V.v[0] + in_M.m[7] * in_V.v[1] + in_M.m[11] * in_V.v[2] + in_M.m[15] * in_V.v[3];
    return v;
}

static inline vec2 vec2MulScalar( const vec2 in_V, const float in_S ) {
    vec2 v;
    v.v[0] = in_V.v[0] * in_S;
    v.v[1] = in_V.v[1] * in_S;
    return v;
}

static inline vec3 vec3MulScalar( const vec3 in_V, const float in_S ) {
    vec3 v;
    v.v[0] = in_V.v[0] * in_S;
    v.v[1] = in_V.v[1] * in_S;
    v.v[2] = in_V.v[2] * in_S;
    return v;
}

static inline vec4 vec4MulScalar( const vec4 in_V, const float in_S ) {
    vec4 v;
    v.v[0] = in_V.v[0] * in_S;
    v.v[1] = in_V.v[1] * in_S;
    v.v[2] = in_V.v[2] * in_S;
    v.v[3] = in_V.v[3] * in_S;
    return v;
}

static inline mat2 mat2MulScalar( const mat2 in_M, const float in_S ) {
    mat2 m;
    m.m[0] = in_M.m[0] * in_S;
    m.m[1] = in_M.m[1] * in_S;
    m.m[2] = in_M.m[2] * in_S;
    m.m[3] = in_M.m[3] * in_S;
    return m;
}

static inline mat3 mat3MulScalar( const mat3 in_M, const float in_S ) {
    mat3 m;
    m.m[0] = in_M.m[0] * in_S;
    m.m[1] = in_M.m[1] * in_S;
    m.m[2] = in_M.m[2] * in_S;
    m.m[3] = in_M.m[3] * in_S;
    m.m[4] = in_M.m[4] * in_S;
    m.m[5] = in_M.m[5] * in_S;
    m.m[6] = in_M.m[6] * in_S;
    m.m[7] = in_M.m[7] * in_S;
    m.m[8] = in_M.m[8] * in_S;
    return m;
}

static inline mat4 mat4MulScalar( const mat4 in_M, const float in_S ) {
    mat4 m;
    m.m[0]  = in_M.m[0]  * in_S;
    m.m[1]  = in_M.m[1]  * in_S;
    m.m[2]  = in_M.m[2]  * in_S;
    m.m[3]  = in_M.m[3]  * in_S;
    m.m[4]  = in_M.m[4]  * in_S;
    m.m[5]  = in_M.m[5]  * in_S;
    m.m[6]  = in_M.m[6]  * in_S;
    m.m[7]  = in_M.m[7]  * in_S;
    m.m[8]  = in_M.m[8]  * in_S;
    m.m[9]  = in_M.m[9]  * in_S;
    m.m[10] = in_M.m[10] * in_S;
    m.m[11] = in_M.m[11] * in_S;
    m.m[12] = in_M.m[12] * in_S;
    m.m[13] = in_M.m[13] * in_S;
    m.m[14] = in_M.m[14] * in_S;
    m.m[15] = in_M.m[15] * in_S;
    return m;
}

static inline float vec2Length( const vec2 in_V ) {
    float sqr = in_V.v[0] * in_V.v[0] + in_V.v[1] * in_V.v[1];
    return sqrtf( sqr );
}

static inline float vec3Length( const vec3 in_V ) {
    float sqr = in_V.v[0] * in_V.v[0] + in_V.v[1] * in_V.v[1] + in_V.v[2] * in_V.v[2];
    return sqrtf( sqr );
}

static inline float vec4Length( const vec4 in_V ) {
    float sqr = in_V.v[0] * in_V.v[0] + in_V.v[1] * in_V.v[1] + in_V.v[2] * in_V.v[2] + in_V.v[3] * in_V.v[3];
    return sqrtf( sqr );
}

static inline float vec2Distance( const vec2 in_A, const vec2 in_B ) {
    vec2 v;
    v.v[0] = in_A.v[0] - in_B.v[0];
    v.v[1] = in_A.v[1] - in_B.v[1];
    return vec2Length( v );
}

static inline float vec3Distance( const vec3 in_A, const vec3 in_B ) {
    vec3 v;
    v.v[0] = in_A.v[0] - in_B.v[0];
    v.v[1] = in_A.v[1] - in_B.v[1];
    v.v[2] = in_A.v[2] - in_B.v[2];
    return vec3Length( v );
}

static inline float vec4Distance( const vec4 in_A, const vec4 in_B ) {
    vec4 v;
    v.v[0] = in_A.v[0] - in_B.v[0];
    v.v[1] = in_A.v[1] - in_B.v[1];
    v.v[2] = in_A.v[2] - in_B.v[2];
    v.v[3] = in_A.v[3] - in_B.v[3];
    return vec4Length( v );
}

static inline float vec2Dot( const vec2 in_A, const vec2 in_B ) {
    return in_A.v[0] * in_B.v[0] + in_A.v[1] * in_B.v[1];
}

static inline float vec3Dot( const vec3 in_A, const vec3 in_B ) {
    return in_A.v[0] * in_B.v[0] + in_A.v[1] * in_B.v[1] + in_A.v[2] * in_B.v[2];
}

static inline float vec4Dot( const vec4 in_A, const vec4 in_B ) {
    return in_A.v[0] * in_B.v[0] + in_A.v[1] * in_B.v[1] + in_A.v[2] * in_B.v[2] + in_A.v[3] * in_B.v[3];
}

static inline vec3 vec3Cross( const vec3 in_A, const vec3 in_B ) {
    vec3 v;
    v.v[0] = in_A.v[1] * in_B.v[2] - in_A.v[2] * in_B.v[1];
    v.v[1] = in_A.v[2] * in_B.v[0] - in_A.v[0] * in_B.v[2];
    v.v[2] = in_A.v[0] * in_B.v[1] - in_A.v[1] * in_B.v[0];
    return v;
}

static inline vec2 vec2Normalize( const vec2 in_V ) {
    vec2 v;
    float sqr = in_V.v[0] * in_V.v[0] + in_V.v[1] * in_V.v[1];
    float invSQRT = inverseSQRT( sqr );
    v.v[0] = in_V.v[0] * invSQRT;
    v.v[1] = in_V.v[1] * invSQRT;
    return v;
}

static inline vec3 vec3Normalize( const vec3 in_V ) {
    vec3 v;
    const float sqr = in_V.v[0] * in_V.v[0] + in_V.v[1] * in_V.v[1] + in_V.v[2] * in_V.v[2];
    const float invSQRT = inverseSQRT( sqr );
    v.v[0] = in_V.v[0] * invSQRT;
    v.v[1] = in_V.v[1] * invSQRT;
    v.v[2] = in_V.v[2] * invSQRT;
    return v;
}

static inline vec4 vec4Normalize( const vec4 in_V ) {
    vec4 v;
    const float sqr = in_V.v[0] * in_V.v[0] + in_V.v[1] * in_V.v[1] + in_V.v[2] * in_V.v[2] + in_V.v[3] * in_V.v[3];
    const float invSQRT = inverseSQRT( sqr );
    v.v[0] = in_V.v[0] * invSQRT;
    v.v[1] = in_V.v[1] * invSQRT;
    v.v[2] = in_V.v[2] * invSQRT;
    v.v[3] = in_V.v[3] * invSQRT;
    return v;
}



static inline mat4 mat4MakeOrtho( const float in_L, const float in_R, const float in_B, const float in_T, const float in_N, const float in_F ) {
    mat4 m;
    const float rl = 1.0f / ( in_R - in_L );
    const float tb = 1.0f / ( in_T - in_B );
    const float fn = 1.0f / ( in_N - in_F );
    m.m[0] = 2 * rl;    m.m[4] = 0;         m.m[8]  = 0;        m.m[12] = -( in_L + in_R ) * rl;
    m.m[1] = 0;         m.m[5] = 2 * tb;    m.m[9]  = 0;        m.m[13] = -( in_B + in_T ) * tb;
    m.m[2] = 0;         m.m[6] = 0;         m.m[10] = 2 * fn;   m.m[14] = ( in_N + in_F ) * fn;
    m.m[3] = 0;         m.m[7] = 0;         m.m[11] = 0;        m.m[15] = 1;
    return m;
}


static inline mat4 mat4MakeFrustum( const float in_L, const float in_R, const float in_B, const float in_T, const float in_N, const float in_F ) {
    mat4 m;
    const float rl = 1.0f / ( in_R - in_L );
    const float tb = 1.0f / ( in_T - in_B );
    const float fn = 1.0f / ( in_N - in_F );
    const float n2 = 2 * in_N;
    m.m[0] = n2 * rl;   m.m[4] = 0;         m.m[8]  = ( in_L + in_R ) * rl;   m.m[12] = 0;
    m.m[1] = 0;         m.m[5] = n2 * tb;   m.m[9]  = ( in_B + in_T ) * tb;   m.m[13] = 0;
    m.m[2] = 0;         m.m[6] = 0;         m.m[10] = ( in_N + in_F ) * fn;   m.m[14] = 2.0f * in_F * in_N * fn;
    m.m[3] = 0;         m.m[7] = 0;         m.m[11] = -1;                     m.m[15] = 0;
    return m;
}


static inline mat4 mat4MakePerspective( const float in_FOV_RADIANS, const float in_AR, const float in_N, const float in_F ) {
    mat4 m;
    const float f = tanf( 2.0f / in_FOV_RADIANS );
    const float fn = 1.0f / ( in_N - in_F );
    m.m[0] = f / in_AR; m.m[4] = 0;     m.m[8]  = 0;                    m.m[12] = 0;
    m.m[1] = 0;         m.m[5] = f;     m.m[9]  = 0;                    m.m[13] = 0;
    m.m[2] = 0;         m.m[6] = 0;     m.m[10] = ( in_N + in_F ) * fn; m.m[14] = 2.0f * in_F * in_N * fn;
    m.m[3] = 0;         m.m[7] = 0;     m.m[11] = -1;                   m.m[15] = 0;
    return m;
}


static inline mat4 mat4MakeRotate( const float in_RADIANS, const vec3 in_V ) {
	mat4 m;
	const float c = cosf( in_RADIANS );
    const float s = sinf( in_RADIANS );
	vec3 n = vec3Normalize( in_V );
    m.m[0] = n.x * n.x * ( 1.0f - c ) + c;			m.m[4] = n.y * n.x * ( 1.0f - c ) - n.z * s;	m.m[8]  = n.z * n.x * ( 1.0f - c ) + n.y * s;	m.m[12] = 0;
    m.m[1] = n.x * n.y * ( 1.0f - c ) + n.z * s;	m.m[5] = n.y * n.y * ( 1.0f - c ) + c;			m.m[9]  = n.z * n.y * ( 1.0f - c ) - n.x * s;	m.m[13] = 0;
    m.m[2] = n.x * n.z * ( 1.0f - c ) - n.y * s;	m.m[6] = n.y * n.z * ( 1.0f - c ) + n.x * s;	m.m[10] = n.z * n.z * ( 1.0f - c ) + c;			m.m[14] = 0;
    m.m[3] = 0;										m.m[7] = 0;										m.m[11] =  0;									m.m[15] = 1;
	return m;
}


static inline mat4 mat4MakeRotateX( const float in_RADIANS ) {
    mat4 m;
    const float c = cosf( in_RADIANS );
    const float s = sinf( in_RADIANS );
    m.m[0] = 1;   m.m[4] = 0;   m.m[8]  =  0;   m.m[12] = 0;
    m.m[1] = 0;   m.m[5] = c;   m.m[9]  = -s;   m.m[13] = 0;
    m.m[2] = 0;   m.m[6] = s;   m.m[10] =  c;   m.m[14] = 0;
    m.m[3] = 0;   m.m[7] = 0;   m.m[11] =  0;   m.m[15] = 1;
    return m;
}


static inline mat4 mat4MakeRotateY( const float in_RADIANS ) {
    mat4 m;
    const float c = cosf( in_RADIANS );
    const float s = sinf( in_RADIANS );
    m.m[0] =  c;   m.m[4] = 0;   m.m[8]  = s;   m.m[12] = 0;
    m.m[1] =  0;   m.m[5] = 1;   m.m[9]  = 0;   m.m[13] = 0;
    m.m[2] = -s;   m.m[6] = 0;   m.m[10] = c;   m.m[14] = 0;
    m.m[3] =  0;   m.m[7] = 0;   m.m[11] = 0;   m.m[15] = 1;
    return m;
}


static inline mat2 mat2MakeRotateZ( const float in_RADIANS ) {
    mat2 m;
    const float c = cosf( in_RADIANS );
    const float s = sinf( in_RADIANS );
    m.m[0] = c;   m.m[2] = -s;
    m.m[1] = s;   m.m[3] =  c;
    return m;
}


static inline mat4 mat4MakeRotateZ( const float in_RADIANS ) {
    mat4 m;
    const float c = cosf( in_RADIANS );
    const float s = sinf( in_RADIANS );
    m.m[0] = c;   m.m[4] = -s;   m.m[8]  = 0;   m.m[12] = 0;
    m.m[1] = s;   m.m[5] =  c;   m.m[9]  = 0;   m.m[13] = 0;
    m.m[2] = 0;   m.m[6] =  0;   m.m[10] = 1;   m.m[14] = 0;
    m.m[3] = 0;   m.m[7] =  0;   m.m[11] = 0;   m.m[15] = 1;
    return m;
}


static inline mat4 mat4MakeTranslate( const vec3 in_V ) {
    mat4 m;
    m.m[0] = 1;   m.m[4] = 0;   m.m[8]  = 0;   m.m[12] = in_V.v[0];
    m.m[1] = 0;   m.m[5] = 1;   m.m[9]  = 0;   m.m[13] = in_V.v[1];
    m.m[2] = 0;   m.m[6] = 0;   m.m[10] = 1;   m.m[14] = in_V.v[2];
    m.m[3] = 0;   m.m[7] = 0;   m.m[11] = 0;   m.m[15] = 1;
    return m;
}


static inline mat2 mat2FromMat3( const mat3 in_M ) {
    mat2 m;
    m.m[0] = in_M.m[0]; m.m[2] = in_M.m[3];
    m.m[1] = in_M.m[1]; m.m[3] = in_M.m[4];
    return m;
}


static inline mat3 mat3FromMat4( const mat4 in_M ) {
    mat3 m;
    m.m[0] = in_M.m[0]; m.m[3] = in_M.m[4]; m.m[6] = in_M.m[8];
    m.m[1] = in_M.m[1]; m.m[4] = in_M.m[5]; m.m[7] = in_M.m[9];
    m.m[2] = in_M.m[2]; m.m[5] = in_M.m[6]; m.m[8] = in_M.m[10];
    return m;
}


static inline mat2 mat2Transpose( const mat2 in_M ) {
    mat2 m;
    m.m[0] = in_M.m[0]; m.m[2] = in_M.m[1];
    m.m[1] = in_M.m[2]; m.m[3] = in_M.m[3];
    return m;
}


static inline mat3 mat3Transpose( const mat3 in_M ) {
    mat3 m;
    m.m[0] = in_M.m[0]; m.m[3] = in_M.m[1]; m.m[6] = in_M.m[2];
    m.m[1] = in_M.m[3]; m.m[4] = in_M.m[4]; m.m[7] = in_M.m[5];
    m.m[2] = in_M.m[6]; m.m[5] = in_M.m[7]; m.m[8] = in_M.m[8];
    return m;
}


static inline mat4 mat4Transpose( const mat4 in_M ) {
    mat4 m;
    m.m[0] = in_M.m[0];  m.m[4] = in_M.m[1];  m.m[8]  = in_M.m[2];  m.m[12] = in_M.m[3];
    m.m[1] = in_M.m[4];  m.m[5] = in_M.m[5];  m.m[9]  = in_M.m[6];  m.m[13] = in_M.m[7];
    m.m[2] = in_M.m[8];  m.m[6] = in_M.m[9];  m.m[10] = in_M.m[10]; m.m[14] = in_M.m[11];
    m.m[3] = in_M.m[12]; m.m[7] = in_M.m[13]; m.m[11] = in_M.m[14]; m.m[15] = in_M.m[15];
    return m;
}


static inline mat2 mat2InverseTranspose( const mat2 in_M ) {
    mat2 m;
    float invDet = 1.0f / ( in_M.m[0] * in_M.m[3] - in_M.m[1] * in_M.m[2] );
    m.m[0] = invDet * in_M.m[3];
    m.m[1] = invDet * -in_M.m[2];
    m.m[2] = invDet * -in_M.m[1];
    m.m[3] = invDet * in_M.m[1];
    return m;
}

static inline mat3 mat3InverseTranspose( const mat3 in_M ) {
    mat3 m;
    float invDet = 1.0f / ( in_M.m[0] * in_M.m[4] * in_M.m[8]
                           + in_M.m[3] * in_M.m[7] * in_M.m[2]
                           + in_M.m[6] * in_M.m[1] * in_M.m[5]
                           - in_M.m[2] * in_M.m[4] * in_M.m[6]
                           - in_M.m[5] * in_M.m[7] * in_M.m[0]
                           - in_M.m[8] * in_M.m[1] * in_M.m[3] );
    m.m[0] = invDet * ( in_M.m[4] * in_M.m[8] - in_M.m[5] * in_M.m[7] );
    m.m[1] = invDet * ( in_M.m[5] * in_M.m[6] - in_M.m[3] * in_M.m[8] );
    m.m[2] = invDet * ( in_M.m[3] * in_M.m[7] - in_M.m[4] * in_M.m[6] );
    m.m[3] = invDet * ( in_M.m[7] * in_M.m[2] - in_M.m[8] * in_M.m[1] );
    m.m[4] = invDet * ( in_M.m[0] * in_M.m[8] - in_M.m[2] * in_M.m[6] );
    m.m[5] = invDet * ( in_M.m[6] * in_M.m[1] - in_M.m[7] * in_M.m[0] );
    m.m[6] = invDet * ( in_M.m[1] * in_M.m[5] - in_M.m[2] * in_M.m[4] );
    m.m[7] = invDet * ( in_M.m[2] * in_M.m[3] - in_M.m[0] * in_M.m[5] );
    m.m[8] = invDet * ( in_M.m[0] * in_M.m[4] - in_M.m[1] * in_M.m[3] );
    return m;
}

static inline mat4 mat4InverseTranspose( const mat4 in_M );

static inline mat2 mat2Inverse( const mat2 in_M ) {
    mat2 m;
    float invDet = 1.0f / ( in_M.m[0] * in_M.m[3] - in_M.m[1] * in_M.m[2] );
    m.m[0] = invDet * in_M.m[3];
    m.m[1] = invDet * -in_M.m[1];
    m.m[2] = invDet * -in_M.m[2];
    m.m[3] = invDet * in_M.m[1];
    return m;
}

static inline mat3 mat3Inverse( const mat3 in_M ) {
    mat3 m;
    float invDet = 1.0f / ( in_M.m[0] * in_M.m[4] * in_M.m[8]
                           + in_M.m[3] * in_M.m[7] * in_M.m[2]
                           + in_M.m[6] * in_M.m[1] * in_M.m[5]
                           - in_M.m[2] * in_M.m[4] * in_M.m[6]
                           - in_M.m[5] * in_M.m[7] * in_M.m[0]
                           - in_M.m[8] * in_M.m[1] * in_M.m[3] );
    m.m[0] = invDet * ( in_M.m[4] * in_M.m[8] - in_M.m[5] * in_M.m[7] );
    m.m[1] = invDet * ( in_M.m[7] * in_M.m[2] - in_M.m[8] * in_M.m[1] );
    m.m[2] = invDet * ( in_M.m[1] * in_M.m[5] - in_M.m[2] * in_M.m[4] );
    m.m[3] = invDet * ( in_M.m[5] * in_M.m[6] - in_M.m[3] * in_M.m[8] );
    m.m[4] = invDet * ( in_M.m[0] * in_M.m[8] - in_M.m[2] * in_M.m[6] );
    m.m[5] = invDet * ( in_M.m[2] * in_M.m[3] - in_M.m[0] * in_M.m[5] );
    m.m[6] = invDet * ( in_M.m[3] * in_M.m[7] - in_M.m[4] * in_M.m[6] );
    m.m[7] = invDet * ( in_M.m[6] * in_M.m[1] - in_M.m[7] * in_M.m[0] );
    m.m[8] = invDet * ( in_M.m[0] * in_M.m[4] - in_M.m[1] * in_M.m[3] );
    return m;
}

static inline mat4 mat4Inverse( const mat4 in_M );


/*
void vec2Add( vec2 out_v, const vec2 in_A, const vec2 in_B ) {
    out_v[0] = in_A[0] + in_B[0];
    out_v[1] = in_A[1] + in_B[1];
}

void vec3Add( vec3 out_v, const vec3 in_A, const vec3 in_B ) {
    out_v[0] = in_A[0] + in_B[0];
    out_v[1] = in_A[1] + in_B[1];
    out_v[2] = in_A[2] + in_B[2];
}

void vec4Add( vec4 out_v, const vec4 in_A, const vec4 in_B ) {
    out_v[0] = in_A[0] + in_B[0];
    out_v[1] = in_A[1] + in_B[1];
    out_v[2] = in_A[2] + in_B[2];
    out_v[3] = in_A[3] + in_B[3];
}

//void vec3Cross( vec3 out_v, const vec3 in_A, const vec3 in_B ) {
//    assert( out_v != in_A );
//    assert( out_v != in_B );
//    $
//}

float vec2Length( const vec2 in_V ) {
    float sqr = in_V[0] * in_V[0] + in_V[1] * in_V[1];
    return sqrtf( sqr );
}

float vec3Length( const vec3 in_V ) {
    float sqr = in_V[0] * in_V[0] + in_V[1] * in_V[1] + in_V[2] * in_V[2];
    return sqrtf( sqr );
}

float vec4Length( const vec4 in_V ) {
    float sqr = in_V[0] * in_V[0] + in_V[1] * in_V[1] + in_V[2] * in_V[2] + in_V[3] * in_V[3];
    return sqrtf( sqr );
}







void mat2Set( mat2 out_m, const float in_S ) {
    out_m[0] = in_S; out_m[2] =    0;
    out_m[1] =    0; out_m[3] = in_S;
}

void mat3Set( mat3 out_m, const float in_S ) {
    out_m[0] = in_S; out_m[3] =    0; out_m[6] =    0;
    out_m[1] =    0; out_m[4] = in_S; out_m[7] =    0;
    out_m[2] =    0; out_m[5] =    0; out_m[8] = in_S;
}

void mat4Set( mat4 out_m, const float in_S ) {
    out_m[0] = in_S; out_m[4] =    0; out_m[8]  =    0; out_m[12] =    0;
    out_m[1] =    0; out_m[5] = in_S; out_m[9]  =    0; out_m[13] =    0;
    out_m[2] =    0; out_m[6] =    0; out_m[10] = in_S; out_m[14] =    0;
    out_m[3] =    0; out_m[7] =    0; out_m[11] =    0; out_m[15] = in_S;
}

void mat4Scale( mat4 out_m, const mat4 in_M, const float in_S ) {
    if ( out_m != in_M ) {
        memcpy( out_m, in_M, sizeof( mat4 ) );
        //out_m[0] = in_M[0]; out_m[4] = in_M[4]; out_m[8]  = in_M[8];  out_m[12] = in_M[12];
        //out_m[1] = in_M[1]; out_m[5] = in_M[5]; out_m[9]  = in_M[9];  out_m[13] = in_M[13];
        //out_m[2] = in_M[2]; out_m[6] = in_M[6]; out_m[10] = in_M[10]; out_m[14] = in_M[14];
        //out_m[3] = in_M[3]; out_m[7] = in_M[7]; out_m[11] = in_M[11]; out_m[15] = in_M[15];
    }
    
    out_m[0] *= in_S; out_m[4] *= in_S; out_m[8]  *= in_S; 
    out_m[1] *= in_S; out_m[5] *= in_S; out_m[9]  *= in_S; 
    out_m[2] *= in_S; out_m[6] *= in_S; out_m[10] *= in_S;
}

void mat4Translate( mat4 out_m, const mat4 in_M, const vec3 in_V ) {
    if ( out_m != in_M ) {
        memcpy( out_m, in_M, sizeof( mat4 ) );
        //out_m[0] = in_M[0]; out_m[4] = in_M[4]; out_m[8]  = in_M[8];  out_m[12] = in_M[12];
        //out_m[1] = in_M[1]; out_m[5] = in_M[5]; out_m[9]  = in_M[9];  out_m[13] = in_M[13];
        //out_m[2] = in_M[2]; out_m[6] = in_M[6]; out_m[10] = in_M[10]; out_m[14] = in_M[14];
        //out_m[3] = in_M[3]; out_m[7] = in_M[7]; out_m[11] = in_M[11]; out_m[15] = in_M[15];
    }
    
    out_m[12] += in_M[0] * in_V[0] + in_M[4] * in_V[1] + in_M[8]  * in_V[2];
    out_m[13] += in_M[1] * in_V[0] + in_M[5] * in_V[1] + in_M[9]  * in_V[2];
    out_m[14] += in_M[2] * in_V[0] + in_M[6] * in_V[1] + in_M[10] * in_V[2];
    out_m[15] += in_M[3] * in_V[0] + in_M[7] * in_V[1] + in_M[11] * in_V[2];
}
*/

#endif
