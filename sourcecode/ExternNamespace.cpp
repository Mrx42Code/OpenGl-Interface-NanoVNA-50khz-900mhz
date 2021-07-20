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
// File: ExternNamespace.cpp: implementation of the ExternNamespace c++.
//-----------------------------------------------------------------------------
#define STRICT

#ifdef _WIN64
    #define WIN64_LEAN_AND_MEAN
#elif _WIN32
    #define WIN32_LEAN_AND_MEAN
#endif

#include "ExternNamespace.h"

using namespace std;

//-Protected-------------------------------------------------------------------
// Const
//-----------------------------------------------------------------------------

//-Protected-------------------------------------------------------------------
// Struct
//-----------------------------------------------------------------------------

//*****************************************************************************  
// Public Code
//*****************************************************************************

//-----------------------------------------------------------------------------
// IMPLEMENT_DYNCREATE
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// message handlers
//-----------------------------------------------------------------------------

//-Public----------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------
NanoVNA::MC_OpenGL				mc_OpenGL;
NanoVNA::MC_Camera				mc_Camra;
NanoVNA::MC_OpenGLTextureLoader	mc_OpenGLTextureLoader;
NanoVNA::MC_OpenGLChart			mc_OpenGLChart;
NanoVNA::MC_OpenGLSmithChart	mc_OpenGLSmithChart;
NanoVNA::MC_OpenGLTextureLoader	mc_OpenGLTextureSmithChart;
NanoVNA::MC_Oscilloscope		mc_Oscilloscope;
NanoVNA::MC_Forms				mc_Forms;
NanoVNA::MC_NanoVNA				mc_NanoVNA;
NanoVNA::MC_VnaMath				mc_VnaMath;
NanoVNA::MC_RS232				mc_RS232;






//-Protected-------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------
