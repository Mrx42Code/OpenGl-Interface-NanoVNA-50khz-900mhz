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
// File: ExternNamespace.h: interface for the ExternNamespace.h.
//-----------------------------------------------------------------------------
#ifndef ExternNamespace_H
	#define ExternNamespacea_H

#pragma once

#include "framework.h"
#include "resource.h"

#include "OpenGLEngine.h"
#include "MC_OpenGL.h"
#include "MC_Camera.h"
#include "MC_OpenGLTextureLoader.h"
#include "MC_OpenGLChart.h"
#include "MC_OpenGLSmithChart.h"
#include "MC_Oscilloscope.h"
#include "MC_Forms.h"
#include "MC_NanoVNA.h"
#include "MC_VnaMath.h"
#include "MC_RS232.h"

//-Public----------------------------------------------------------------------
// Const
//-----------------------------------------------------------------------------

//-Public----------------------------------------------------------------------
// Struct
//-----------------------------------------------------------------------------

//-Public----------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------
extern NanoVNA::MC_OpenGL				mc_OpenGL;
extern NanoVNA::MC_Camera				mc_Camra;
extern NanoVNA::MC_OpenGLTextureLoader	mc_OpenGLTextureLoader;
extern NanoVNA::MC_OpenGLChart			mc_OpenGLChart;
extern NanoVNA::MC_OpenGLSmithChart		mc_OpenGLSmithChart;
extern NanoVNA::MC_OpenGLTextureLoader	mc_OpenGLTextureSmithChart;
extern NanoVNA::MC_Oscilloscope			mc_Oscilloscope;
extern NanoVNA::MC_Forms				mc_Forms;
extern NanoVNA::MC_NanoVNA				mc_NanoVNA;
extern NanoVNA::MC_VnaMath				mc_VnaMath;
extern NanoVNA::MC_RS232				mc_RS232;

//-----------------------------------------------------------------------------
#endif // ExternNamespacea_H
