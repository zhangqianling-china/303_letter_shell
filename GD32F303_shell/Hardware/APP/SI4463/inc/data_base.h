#ifndef _DATA_BASE_H
#define _DATA_BASE_H

#define DEBUG   0
#if DEBUG
#define DBG_Printf(Args...) printf(##Args)
#else
#define DBG_Printf(Args...) 
#endif


typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;
#endif
