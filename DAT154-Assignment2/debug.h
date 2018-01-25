#pragma once

/* Courtesy of Kenny Kerr */

#ifdef _DEBUG 
#include <crtdbg.h> 
#define ASSERT(expression) _ASSERTE(expression) 
#define VERIFY(expression) ASSERT(expression) 
#define VERIFY_(expected, expression) ASSERT(expected == expression) 
#else 
#define ASSERT(expression) ((void)0) 
#define VERIFY(expression) (expression) 
#define VERIFY_(expected, expression) (expression) 
#endif