//
//  lib.h
//  GLTC
//
//  Created by Michael Kwasnicki on 29.02.12.
//  Copyright (c) 2014 Michael Kwasnicki. All rights reserved.
//  This content is released under the MIT License.
//

#ifndef GLTC_lib_h
#define GLTC_lib_h


#include <stdlib.h>


#define REINTERPRET(x) *(x *)&
#define free_s(x) free(x);x = NULL

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

static inline int clampi( const int in_VALUE, const int in_MIN, const int in_MAX ) {
	int result = ( in_VALUE < in_MIN ) ? in_MIN : in_VALUE;
    return ( result > in_MAX ) ? in_MAX : result;
}


char * expandTilde( const char in_PATH[] );


uint64_t switchEndianness64( uint64_t const in_DATA );
uint32_t switchEndianness32( uint32_t const in_DATA );
uint16_t switchEndianness16( uint16_t const in_DATA );


#endif
