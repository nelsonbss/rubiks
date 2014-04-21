// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(STDAFX_H_INCLUDED)
#define STDAFX_H_INCLUDED

#pragma once

#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// Local Header Files
// OpenGL support
#include <gl/gl.h>
#include <gl/glu.h>

#include "sgCore.h"

/*************************/
#define NTH_BIT(n) (1 << n)
#define TEST_ANY_BITS(x, y) (((x) & (y)) != 0)
#define TEST_ALL_BITS(x, y) (((x) & (y)) == (y))
#define TEST_NTH_BIT(x, n) TEST_ANY_BITS(x, NTH_BIT(n))

//////////////////////////
#define BIN___(x)                                        \
  (                                                \
  ((x / 01ul) % 010)*(2>>1) +                      \
  ((x / 010ul) % 010)*(2<<0) +                     \
  ((x / 0100ul) % 010)*(2<<1) +                    \
  ((x / 01000ul) % 010)*(2<<2) +                   \
  ((x / 010000ul) % 010)*(2<<3) +                  \
  ((x / 0100000ul) % 010)*(2<<4) +                 \
  ((x / 01000000ul) % 010)*(2<<5) +                \
  ((x / 010000000ul) % 010)*(2<<6)                 \
  )

#define BIN8(x) BIN___(0##x)

#define BIN16(x1,x2) \
  ((BIN8(x1)<<8)+BIN8(x2))
#define BIN24(x1,x2,x3) \
  ((BIN8(x1)<<16)+(BIN8(x2)<<8)+BIN8(x3))
#define BIN32(x1,x2,x3,x4) \
  ((BIN8(x1)<<24)+(BIN8(x2)<<16)+(BIN8(x3)<<8)+BIN8(x4))

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(STDAFX_H_INCLUDED)
