/**********************************************************************************
* MIT License																	  *
*																				  *
* Copyright(c) 2021 Mrx42Code                                                     *
* https://github.com/Mrx42Code							 				          *
*																				  *
* Permission is hereby granted, free of charge, to any person obtaining a copy    *
* of this softwareand associated documentation files(the "Software"), to deal	  *
* in the Software without restriction, including without limitation the rights	  *
* to use, copy, modify, merge, publish, distribute, sublicense, and /or sell	  *
* copies of the Software, and to permit persons to whom the Software is			  *
* furnished to do so, subject to the following conditions :						  *
*																				  *
* The above copyright noticeand this permission notice shall be included in all   *
* copies or substantial portions of the Software.								  *
*																				  *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR	  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		  *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE	  *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER		  *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE	  *
* SOFTWARE.																		  *
 **********************************************************************************/

//-----------------------------------------------------------------------------
// framework.h : include file for standard system include files or project specific include files
//-----------------------------------------------------------------------------
#ifndef framework_H
	#define framework_H

#pragma once

//-Public----------------------------------------------------------------------
// Const
//-----------------------------------------------------------------------------
#ifdef _WIN64																	// Exclude rarely-used stuff from Windows headers
	#define WIN64_LEAN_AND_MEAN
#elif _WIN32
	#define WIN32_LEAN_AND_MEAN
#endif

//-Public----------------------------------------------------------------------
// targetver Header Files
//-----------------------------------------------------------------------------
#include "targetver.h"

//-Public----------------------------------------------------------------------
// Windows Header Files
//-----------------------------------------------------------------------------
#include <windows.h>
#include <windowsx.h>

//-Public----------------------------------------------------------------------
// C++ RunTime Header Files
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <string>
#include <iostream>
#include <math.h>
#include <cmath>
#include <complex.h>
#include <complex>
#include <thread>
#include <mutex>
#include <codecvt>
#include <conio.h>
#include <ctime>
#include <ocidl.h>
#include <olectl.h>
#include <codecvt>
#include <commdlg.h>
#include <commctrl.h>

//-Public----------------------------------------------------------------------
// OpenGL RunTime Header Files
//-----------------------------------------------------------------------------
#include <gl\gl.h>
#include <gl\glu.h>

//-Public----------------------------------------------------------------------
// #pragma comment
//-----------------------------------------------------------------------------
#pragma comment(lib,"OpenGl32.lib")
#pragma comment(lib,"GLU32.lib")
#pragma comment(lib,"Comctl32.lib")

//-----------------------------------------------------------------------------
#endif // framework_H
