#pragma once
#ifndef ENDIANESS_H
#define ENDIANESS_H

#include <SDL_endian.h>

 //little endian / big endian 
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define LIL_ENDIAN
#else
#define BIG_ENDIAN
#endif

#endif // ENDIANESS_H