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
// File: OpenGLEngine.h: interface for the OpenGLEngine.h.
//-----------------------------------------------------------------------------
#ifndef OpenGLEngineL_H
	#define OpenGLEngine_H

#pragma once

#include <windows.h>

//-Public----------------------------------------------------------------------
// Const
//-----------------------------------------------------------------------------
#define MAX_LOADSTRING 100
//-Public----------------------------------------------------------------------
// Struct
//-----------------------------------------------------------------------------

//-Public----------------------------------------------------------------------
// Name: main App
// Desc: Application
//-----------------------------------------------------------------------------
int APIENTRY		wWinMain(_In_ HINSTANCE , _In_opt_ HINSTANCE , _In_ LPWSTR , _In_ int );
ATOM                MyRegisterClass(HINSTANCE);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
bool				WndProcMenu(HWND, UINT);
INT_PTR CALLBACK    FormAbout_Proc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	FormSerialPort_Proc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	FormCalibration_Proc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	FromFrequencySweep_Proc(HWND, UINT, WPARAM, LPARAM);

//-----------------------------------------------------------------------------
#endif // OpenGLEngine_H






