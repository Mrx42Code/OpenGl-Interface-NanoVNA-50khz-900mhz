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
// File: MC_OpenGL.h: interface for the MC_OpenGL class.
// Desc: Application interface for the MC_OpenGL class.
//-----------------------------------------------------------------------------
#ifndef MC_OpenGL_H
	#define MC_OpenGL_H

#pragma once

#include <gl\gl.h>
#include <gl\glu.h>

namespace NanoVNA {
	//-----------------------------------------------------------------------------
	// Const
	//-----------------------------------------------------------------------------
	#define OPENGL_TIMER_RENDER			1
	#define OPENGL_TIMER_RENDER_TICK	1
	#define OPENGL_TIMER_ID				2
	#define OPENGL_TIMER_TICK			1000

	#define OPENGL_ScreenWidth			1920
	#define OPENGL_ScreenHeight			1080

	#define OPENGL_FPS_FRAMEINTERVAL	1000
	#define OPENGL_THREADTIMEOUT		5000

	#define OPENGL_SCALE_ENGINE			100.0f
	#define OPENGL_ASPECT				((GLdouble)(OPENGL_ScreenWidth) / (GLdouble)(OPENGL_ScreenHeight))
	#define OPENGL_FOV					45.0
	#define OPENGL_NEAR					0.01													// Near clipping plane
	#define OPENGL_FAR					((GLdouble)(OPENGL_SCALE_ENGINE) * 40.0)				// Far clipping plane
	#define OPENGL_KEYMOVEAMOUNT		0.01f
	//-----------------------------------------------------------------------------
	// Struct
	//-----------------------------------------------------------------------------
	typedef struct _OPENGL_FPS
	{
		char				szFPS[32];
		char				szTime[32];
		DWORD				Frames;
		ULONGLONG   		LastTick;
	} OPENGL_FPS;

	typedef struct _OPENGL_VIEWPORT
	{
		GLint				x;
		GLint				y;
		GLsizei				width;
		GLsizei				height;
		GLdouble            Fov;
		GLdouble            Aspect;
		GLdouble            Near;
		GLdouble            Far;
	} OPENGL_VIEWPORT;

	typedef struct _OPENGL_MOUSE
	{
		int					x;
		int					y;
		int					Center_x;
		int					Center_y;
		DWORD				Buttons;
		int					Wheel;
		bool			    Mousing;
		float			    Mousing_x;
		float			    Mousing_y;
	} OPENGL_MOUSE;

	typedef struct _OPENGL_KEYMOUSE
	{
		bool			    Keys[256];
		OPENGL_MOUSE	    Mouse;
		float			    MoveAmount;
		POINT			    LastMouse;
		POINT			    CurrentMouse;
	} OPENGL_KEYMOUSE;

	typedef struct _OPENGL_INFO
	{
		HWND					hWnd;
		HGLRC					hRC;
		HDC						hDC;
		GLuint					PixelFormat;
		PIXELFORMATDESCRIPTOR	pfd;
		DEVMODE					dmScreenSettings;
		RECT					ScreenRect;
	} OPENGL_INFO;

	typedef struct _OPENGL_FONT
	{
		GLuint				Font3D;
		GLuint				Font2D;
	} OPENGL_FONT;

	typedef struct _OPENGL_THREAD
	{
		bool				Exit;
		bool				Running;
	} OPENGL_THREAD;

	//-----------------------------------------------------------------------------
	// Name: class MC_OpenGL
	// Desc: Application class
	//-----------------------------------------------------------------------------
	class MC_OpenGL
	{

		public:
			OPENGL_THREAD		m_Thread_Status;
			OPENGL_INFO			m_Info;
			OPENGL_VIEWPORT		m_ViewPort;
			OPENGL_KEYMOUSE		m_Input;
			OPENGL_FPS			m_FPS;
			OPENGL_FONT			m_Fonts;
			char				m_TmpBuffer[256];
			GLfloat				m_Rotate;

		protected:
			std::thread			mc_Thread_OpenGL;

		private:

			//-----------------------------------------------------------------------------

		public:
								MC_OpenGL();
			virtual				~MC_OpenGL();
			void				OnInit(GLsizei Width, GLsizei Height);
			void				OnCreate(HWND hWnd);
			void				OnDestroy();
			void				OnTimer();
			void				OnReSize(GLsizei Width, GLsizei Height);
			void				OnReSize3d2d(GLsizei Width, GLsizei Height);
			void				OnKeys(bool KeyUpDown, DWORD wParam);
			void				OnMouseMove(int x, int y, DWORD wParam);
			void				OnMouseWheel(short Wheel);
			void				OnDrawScene();
			void				DrawSceneRasterText();
			void				Print3DScreen(const char* text, GLfloat x, GLfloat y, GLfloat z);
			void				Print2DScreen(const char* text, GLfloat x, GLfloat y, GLfloat z);
			void				Print2DRaster(const char* text, GLfloat x, GLfloat y);
			void				PrintFPS();
			void				BeginRasterText();
			void				EndRasterText();
			void				Thread_Create();
			void				Thread_Stop();

		protected:
			void				Thread_CallBack_OpenGL(int MultiThread_ID);
			void				KeyMouseFunction();
			void				BuildFont3D(LPCWSTR FontName, int FontSize);
			void				BuildFont2D(LPCWSTR FontName, int FontSize);
			void				KillFont3D();
			void				KillFont2D();
			void				DrawCube();
			void				DrawImage();
			void				Draw3DFonts();
			void				AutoLineWidth(bool Smooth);

		private:

	};
};
//-----------------------------------------------------------------------------
#endif // MC_OpenGL_H
