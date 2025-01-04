#pragma once
#ifndef CONVERT_TOOLS_H
#define CONVERT_TOOLS_H

//Conversiones de cadenas de 4 bytes que proporcionamos para manejo de bytes ARGB mas comoda

// B1 TO B2                       B2 TO B3                      B3 TO B4                      B4 TO B1
#define CONVERT_RGBA_TO_ARGB(value) ((value & 0xFF000000) >> 8) | ((value & 0x00FF0000) >> 8) | ((value & 0x0000FF00) >> 8) | ((value & 0x000000FF) << 24)

// B1 TO B4                       B2 TO B3                      B3 TO B2                      B4 TO B1
#define CONVERT_ENDIANESS_32(value) ((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) | ((value & 0x0000FF00) << 8) | ((value & 0x000000FF) << 24)


#endif
