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
// File: MC_OpenGL.cpp: implementation of the MC_OpenGL class.
//-----------------------------------------------------------------------------
#include "ExternNamespace.h"

using namespace std;

namespace NanoVNA {
	//*****************************************************************************  
	// Public Code
	//*****************************************************************************

	//-----------------------------------------------------------------------------
	// IMPLEMENT_DYNCREATE
	//-----------------------------------------------------------------------------
	static GLfloat m_MatSpec[] = { 1.0,1.0,1.0,1.0 };
	static GLfloat m_MatShininess[] = { 0.5f };

	//-----------------------------------------------------------------------------
	// message handlers
	//-----------------------------------------------------------------------------

	//-Public----------------------------------------------------------------------
	// Name: MC_OpenGL()
	// Desc: MC_OpenGL class
	//-----------------------------------------------------------------------------
	MC_OpenGL::MC_OpenGL()
	{
		memset(&m_Thread_Status, 0, sizeof(m_Thread_Status));
		memset(&m_Info, 0, sizeof(m_Info));
		memset(&m_ViewPort, 0, sizeof(m_ViewPort));
		memset(&m_Input, 0, sizeof(m_Input));
		memset(&m_FPS, 0, sizeof(m_FPS)),
		memset(&m_Fonts, 0, sizeof(m_Fonts));
		memset(&m_TmpBuffer, 0, sizeof(m_TmpBuffer));
		m_Input.MoveAmount = OPENGL_KEYMOVEAMOUNT * 1.0f;
		m_ViewPort.x = 0;
		m_ViewPort.y = 0;
		m_ViewPort.width = OPENGL_ScreenWidth;
		m_ViewPort.height = OPENGL_ScreenHeight;
		m_Info.hWnd = NULL;
		m_Info.PixelFormat = NULL;
		m_Info.hRC = NULL;
		m_Info.hDC = NULL;
		m_Info.pfd = {							// Pixel Format Descriptor
			sizeof(PIXELFORMATDESCRIPTOR),		// Size Of This Pixel Format Descriptor
			1,									// Version Number (?)
			PFD_DRAW_TO_WINDOW |				// Format Must Support Window
			PFD_SUPPORT_OPENGL |				// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,					// Must Support Double Buffering
			PFD_TYPE_RGBA,						// Request An RGBA Format
			24,									// Select A 24Bit Color Depth
			0, 0, 0, 0, 0, 0,					// Color Bits Ignored (?)
			0,									// No Alpha Buffer
			0,									// Shift Bit Ignored (?)
			0,									// No Accumulation Buffer
			0, 0, 0, 0,							// Accumulation Bits Ignored (?)
			24,									// 24Bit Z-Buffer (Depth Buffer)  
			0,									// No Stencil Buffer
			0,									// No Auxiliary Buffer (?)
			PFD_MAIN_PLANE,						// Main Drawing Layer
			0,									// Reserved (?)
			0, 0, 0								// Layer Masks Ignored (?)
		};

		m_Rotate = 0.0f;
	}
	//-Public----------------------------------------------------------------------
	// Name: ~MC_OpenGL()
	// Desc: ~MC_OpenGL Destruction class
	//-----------------------------------------------------------------------------
	MC_OpenGL::~MC_OpenGL()
	{

	}
	//-Public----------------------------------------------------------------------
	// Name: OnInit(GLsizei Width, GLsizei Height)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::OnInit(GLsizei Width, GLsizei Height)
	{
		m_ViewPort.x = 0;
		m_ViewPort.y = 0;
		m_ViewPort.width = Width;
		m_ViewPort.height = Height;
		m_ViewPort.Fov = OPENGL_FOV;
		m_ViewPort.Aspect = OPENGL_ASPECT;
		m_ViewPort.Near = OPENGL_NEAR;
		m_ViewPort.Far = OPENGL_FAR;

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glClearDepth(1.0);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		BuildFont3D(_T("Comic Sans MS"), -12);
		BuildFont2D(_T("Terminal"), -9);
		mc_Camra.Init();
		glEnable(GL_TEXTURE_2D);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_MatSpec);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, m_MatShininess);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glEnable(GL_NORMALIZE);
		mc_Camra.SetShape(m_ViewPort.Fov, m_ViewPort.Aspect, m_ViewPort.Near, m_ViewPort.Far);
		mc_Camra.LookAt(1, 1, 3, 0, 0, 1, 0, 0, 1);
		SetTimer(m_Info.hWnd, OPENGL_TIMER_ID, OPENGL_TIMER_TICK, (TIMERPROC)NULL);
		Thread_Create();

		mc_OpenGLTextureLoader.LoadTextureFromDisk("Images\\Image1.jpg");
	}
	//-Public----------------------------------------------------------------------
	// Name: OnCreate(HWND hWnd)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::OnCreate(HWND hWnd)
	{
		m_Info.hWnd = hWnd;
		m_Info.hDC = GetDC(hWnd);
		m_Info.PixelFormat = ChoosePixelFormat(m_Info.hDC, &m_Info.pfd);
		if (!m_Info.PixelFormat) {
			MessageBox(m_Info.hWnd, _T("Can't Find A Suitable PixelFormat."), _T("Error"), MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			return;
		}
		if (!SetPixelFormat(m_Info.hDC, m_Info.PixelFormat, &m_Info.pfd)) {
			MessageBox(m_Info.hWnd, _T("Can't Set The PixelFormat."), _T("Error"), MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			return;
		}
		m_Info.hRC = wglCreateContext(m_Info.hDC);
		if (!m_Info.hRC) {
			MessageBox(m_Info.hWnd, _T("Can't Create A GL Rendering Context."), _T("Error"), MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			return;
		}
		if (!wglMakeCurrent(m_Info.hDC, m_Info.hRC)) {
			MessageBox(m_Info.hWnd, _T("Can't Activate GLRC."), _T("Error"), MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			return;
		}
		m_Info.dmScreenSettings.dmSize = sizeof(DEVMODE);
		m_Info.dmScreenSettings.dmPelsWidth = OPENGL_ScreenWidth;
		m_Info.dmScreenSettings.dmPelsHeight = OPENGL_ScreenHeight;
		m_Info.dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings(&m_Info.dmScreenSettings, CDS_FULLSCREEN);
		GetClientRect(hWnd, &m_Info.ScreenRect);
		wglMakeCurrent(m_Info.hDC, m_Info.hRC);
		OnInit(m_Info.ScreenRect.right, m_Info.ScreenRect.bottom);

		mc_OpenGLChart.OnCreate();
	}
	//-Public----------------------------------------------------------------------
	// Name: OnDestroy()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::OnDestroy()
	{
		Thread_Stop();
		mc_OpenGLTextureLoader.FreeTexture();
		KillFont3D();
		KillFont2D();
		wglMakeCurrent(m_Info.hDC, NULL);
		wglDeleteContext(m_Info.hRC);
		ReleaseDC(m_Info.hWnd, m_Info.hDC);
	}
	//-Public----------------------------------------------------------------------
	// Name: OnTimer()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::OnTimer()
	{

	}
	//-Public----------------------------------------------------------------------
	// Name: OnReSize(GLsizei Width, GLsizei Height)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::OnReSize(GLsizei Width, GLsizei Height)
	{
		if (Height == 0) {
			Height = 1;
		}
		if (Width == 0) {
			Width = 1;
		}
		m_ViewPort.x = 0;
		m_ViewPort.y = 0;
		m_ViewPort.width = Width;
		m_ViewPort.height = Height;
		m_ViewPort.Aspect = ((GLdouble)(m_ViewPort.width) / (GLdouble)(m_ViewPort.height));
		glViewport(m_ViewPort.x, m_ViewPort.y, m_ViewPort.width, m_ViewPort.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		mc_Camra.SetShape(m_ViewPort.Fov, m_ViewPort.Aspect, m_ViewPort.Near, m_ViewPort.Far);
		glMatrixMode(GL_MODELVIEW);
		OnDrawScene();
	}
	//-Public----------------------------------------------------------------------
	// Name: OnReSize3d2d(GLsizei Width, GLsizei Height)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::OnReSize3d2d(GLsizei Width, GLsizei Height)
	{
		if (Height == 0) {
			Height = 1;
		}
		if (Width == 0) {
			Width = 1;
		}
		m_ViewPort.x = 0;
		m_ViewPort.y = 0;
		m_ViewPort.width = Width;
		m_ViewPort.height = Height;
		m_ViewPort.Aspect = ((GLdouble)(m_ViewPort.width) / (GLdouble)(m_ViewPort.height));
		glViewport(m_ViewPort.x, m_ViewPort.y, m_ViewPort.width, m_ViewPort.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		mc_Camra.SetShape(m_ViewPort.Fov, m_ViewPort.Aspect, m_ViewPort.Near, m_ViewPort.Far);
		glMatrixMode(GL_MODELVIEW);
	}
	//-Public----------------------------------------------------------------------
	// Name: OnKeys(bool KeyUpDown, DWORD wParam)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::OnKeys(bool KeyUpDown, DWORD wParam)
	{
		m_Input.Keys[wParam & 0xFF] = KeyUpDown;
	}
	//-Public----------------------------------------------------------------------
	// Name: OnMouseMove(int x, int y, DWORD wParam)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::OnMouseMove(int x, int y, DWORD wParam)
	{
		m_Input.Mouse.x = x;
		m_Input.Mouse.y = y;
		m_Input.Mouse.Center_x = m_Input.Mouse.x - (m_ViewPort.width / 2);
		m_Input.Mouse.Center_y = m_Input.Mouse.y - (m_ViewPort.height / 2);
		m_Input.Mouse.Buttons = wParam;
		m_Input.CurrentMouse.x = x;
		m_Input.CurrentMouse.y = y;
		if ((wParam & 0x01) == 1) {
			m_Input.Mouse.Mousing_x = 0.0f;
			m_Input.Mouse.Mousing_y = 0.0f;
			m_Input.Mouse.Mousing = TRUE;
		} else {
			m_Input.Mouse.Mousing = FALSE;
		}
		if (m_Input.Mouse.Mousing != FALSE) {
			m_Input.Mouse.Mousing_x -= ((FLOAT)(m_Input.CurrentMouse.x - m_Input.LastMouse.x)) / 6000.0f;
			m_Input.Mouse.Mousing_y -= ((FLOAT)(m_Input.CurrentMouse.y - m_Input.LastMouse.y)) / 6000.0f;
		}
		m_Input.LastMouse.x = m_Input.CurrentMouse.x;
		m_Input.LastMouse.y = m_Input.CurrentMouse.y;
	}
	//-Public----------------------------------------------------------------------
	// Name: OnMouseWheel(short Wheel)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::OnMouseWheel(short Wheel)
	{
		int direction = 0;
		if (Wheel > 0) {
			m_Input.Mouse.Wheel++;
			direction = 1;
		} else if (Wheel < 0) {
			m_Input.Mouse.Wheel--;
			direction = -1;
		}

		if (direction == -1) {
			mc_OpenGLChart.m_Marker.Trace[0].Index--;
		} else if (direction == +1) {
			mc_OpenGLChart.m_Marker.Trace[0].Index++;
		}
		if (mc_OpenGLChart.m_Marker.Trace[0].Index < 0) {
			mc_OpenGLChart.m_Marker.Trace[0].Index = 0;
		} else if (mc_OpenGLChart.m_Marker.Trace[0].Index > 100 * mc_OpenGLChart.m_X10Samples) {
			mc_OpenGLChart.m_Marker.Trace[0].Index = int(100.0f * mc_OpenGLChart.m_X10Samples);
		}
	}
	//-Public----------------------------------------------------------------------
	// Name: OnDrawScene()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::OnDrawScene()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		AutoLineWidth(false);
		glLoadIdentity();
		KeyMouseFunction();
		mc_Camra.SetModelViewMatrix();

		GLfloat  specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
		glMateriali(GL_FRONT, GL_SHININESS, 128);

		GLfloat  lightPos[] = { 0.0f, 150.0f, 150.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

#if 0
		static GLfloat       fogcolor[4] = { 1.0, 1.0, 1.0, 1.0 };
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_FOG);
		glFogf(GL_FOG_DENSITY, 0.0025f);
		glFogi(GL_FOG_MODE, GL_EXP);
		glFogfv(GL_FOG_COLOR, fogcolor);
#endif

		mc_OpenGLChart.DrawChart();

#if 0
		DrawCube();

		glTranslatef(0.0f, 0.0f, -10.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		Print2DRaster("OpenGL 2D Raster Fonts Test.", 0.0f, 0.0f);

		glColor3f(1.0f, 1.0f, 1.0f);
		Print2DScreen("OpenGL 2D Screen Fonts Test.", 1.0f, 1.0f, -20.0f);

		DrawImage();

		Draw3DFonts();
#endif

		DrawSceneRasterText();
		glFlush();
		SwapBuffers(m_Info.hDC);
	}
	//-Public----------------------------------------------------------------------
	// Name: DrawScreenRasterText()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::DrawSceneRasterText()
	{
		//	int i;
		int Key = 0;

		BeginRasterText();

		glColor3f(0.0f, 1.0f, 1.0f);
		PrintFPS();

#if 0
		for (i = 0; i < 256; i++) {
			if (m_Input.Keys[i]) {
				Key = i;
			}
		}
		sprintf_s(m_TmpBuffer, "x %d y %d cx %d cy %d Buttons %ld Wheel %d Key(%d) %s", m_Input.Mouse.x, m_Input.Mouse.y, m_Input.Mouse.Center_x, m_Input.Mouse.Center_y, m_Input.Mouse.Buttons, m_Input.Mouse.Wheel, Key, (char*)&Key);
		glColor3f(0.0f, 1.0f, 0.0f);
		Print2DRaster(m_TmpBuffer, 0.0f, 0.0f);
#endif

		EndRasterText();
	}
	//-Public----------------------------------------------------------------------
	// Name: Print3D(const char* text)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::Print3DScreen(const char* text, GLfloat x, GLfloat y, GLfloat z)
	{
		if (text == NULL) {
			return;
		}
		glPushAttrib(GL_LIST_BIT);
		glTranslatef(x, y, z);
		glListBase(m_Fonts.Font3D);
		glCallLists(GLsizei(strlen(text)), GL_UNSIGNED_BYTE, text);
		glPopAttrib();
	}
	//-Public----------------------------------------------------------------------
	// Name: Print2DScreen(const char* text, GLfloat x, GLfloat y, GLfloat z)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::Print2DScreen(const char* text, GLfloat x, GLfloat y, GLfloat z)
	{
		if (text == NULL) {
			return;
		}
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glRasterPos3f(x, y, z);
		glListBase(m_Fonts.Font2D);
		glCallLists(GLsizei(strlen(text)), GL_UNSIGNED_BYTE, text);
		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);
	}
	//-Public----------------------------------------------------------------------
	// Name: Print2DRaster(const char* text, GLfloat x, GLfloat y)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::Print2DRaster(const char* text, GLfloat x, GLfloat y)
	{
		if (text == NULL) {
			return;
		}
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glRasterPos3f(x, y, 0.0f);
		glListBase(m_Fonts.Font2D);
		glCallLists(GLsizei(strlen(text)), GL_UNSIGNED_BYTE, text);
		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);
	}
	//-Public----------------------------------------------------------------------
	// Name: PrintFPS()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::PrintFPS()
	{
		ULONGLONG	nTick = GetTickCount64();
		FLOAT	fFPS;
		time_t curr_time;
		tm curr_tm;

		if (nTick - m_FPS.LastTick >= OPENGL_FPS_FRAMEINTERVAL) {
			fFPS = 1000.0f * (GLfloat)m_FPS.Frames / (GLfloat)(nTick - m_FPS.LastTick);
			m_FPS.LastTick = nTick;
			m_FPS.Frames = 0;
			time(&curr_time);
			localtime_s(&curr_tm, &curr_time);
			strftime(m_FPS.szTime, sizeof(m_FPS.szTime), "%T", &curr_tm);
			sprintf_s(m_FPS.szFPS, "%.1f FPS %s", fFPS, m_FPS.szTime);
		}
		Print2DRaster(m_FPS.szFPS, 0.0f, m_ViewPort.height - 10.0f);
		m_FPS.Frames++;
	}
	//-Public----------------------------------------------------------------------
	// Name: BeginRasterText()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::BeginRasterText()
	{
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, m_ViewPort.width, 0, m_ViewPort.height);
		glMatrixMode(GL_MODELVIEW);
	}
	//-Public----------------------------------------------------------------------
	// Name: EndScreenText()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::EndRasterText()
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
	//-Public----------------------------------------------------------------------
	// Name: Thread_Create()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::Thread_Create()
	{
		m_Thread_Status.Exit = false;
		m_Thread_Status.Running = false;
		mc_Thread_OpenGL = std::thread(&MC_OpenGL::Thread_CallBack_OpenGL, this, 0);
		//	Sleep(10);
		if (mc_Thread_OpenGL.joinable()) {
			//       m_Thread_OpenGL.join();
		}
	}
	//-Public----------------------------------------------------------------------
	// Name: Thread_Stop()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::Thread_Stop()
	{
		bool TimeOut = false;
		ULONGLONG	nTick = GetTickCount64();
		ULONGLONG   nLastTick = nTick;

		m_Thread_Status.Exit = true;
		while (m_Thread_Status.Running == true && TimeOut == false) {
			if (nTick - nLastTick > OPENGL_THREADTIMEOUT) {
				TimeOut = true;
				MessageBox(m_Info.hWnd, _T("Thread_Stop Timeout"), _T("Error"), MB_OK | MB_ICONERROR);
			}
			Sleep(1);
		}
	}











	//*****************************************************************************  
	// Protected Code
	//*****************************************************************************

	//-Protected-------------------------------------------------------------------
	// Name: Thread_CallBack_OpenGL(int MultiThread_ID)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::Thread_CallBack_OpenGL(int MultiThread_ID)
	{
		m_Thread_Status.Running = true;
		while (m_Thread_Status.Exit == false) {
			Sleep(1000);
			mc_OpenGLChart.Tick();
		}
		m_Thread_Status.Running = false;
	}
	//-Protected-------------------------------------------------------------------
	// Name: KeyMouseFunction()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::KeyMouseFunction()
	{
		DWORD	VirtKey;

		for (VirtKey = 0; VirtKey < 256; ++VirtKey) {
			if (m_Input.Mouse.Mousing) {
				mc_Camra.Yaw(m_Input.Mouse.Mousing_x);
				mc_Camra.Pitch(-m_Input.Mouse.Mousing_y);
			}
			if (m_Input.Keys[VirtKey]) {
				//			m_Input.Keys[VirtKey] = false;
				switch (VirtKey) {
					case 'W': { mc_Camra.Slide(0, 0, -m_Input.MoveAmount / 2);							} break;
					case 'S': { mc_Camra.Slide(0, 0, m_Input.MoveAmount / 2);							} break;
					case 'D': { mc_Camra.Slide(-m_Input.MoveAmount / 2, 0, 0);							} break;
					case 'A': { mc_Camra.Slide(m_Input.MoveAmount / 2, 0, 0);							} break;
					case 'Q': { mc_Camra.Slide(0, -m_Input.MoveAmount / 2, 0);							} break;
					case 'E': { mc_Camra.Slide(0, m_Input.MoveAmount / 2, 0);							} break;
					case 'T': { mc_Camra.Pitch(0.5f);													} break;
					case 'R': { mc_Camra.Pitch(-0.5f);													} break;
					case 'G': { mc_Camra.Yaw(-0.5f);													} break;
					case 'F': { mc_Camra.Yaw(0.5f);														} break;
					case 'V': { mc_Camra.Roll(0.5f);													} break;
					case 'B': { mc_Camra.Roll(-0.5f);													} break;
					case 219: { m_Input.MoveAmount += OPENGL_KEYMOVEAMOUNT;								} break;
					case 221: { m_Input.MoveAmount -= OPENGL_KEYMOVEAMOUNT;								} break;
					case 'L': { mc_Camra.Init(); mc_Camra.LookAt(1, 1, 3, 0, 0, 1, 0, 0, 1);			} break;
					case 188: { mc_OpenGLChart.m_Marker.Trace[0].Index--;								} break;
					case 190: { mc_OpenGLChart.m_Marker.Trace[0].Index++;								} break;
					default: { 																			} break;
				}
				if (mc_OpenGLChart.m_Marker.Trace[0].Index < 0) {
					mc_OpenGLChart.m_Marker.Trace[0].Index = 0;
				} else if (mc_OpenGLChart.m_Marker.Trace[0].Index > 100.0f * mc_OpenGLChart.m_X10Samples) {
					mc_OpenGLChart.m_Marker.Trace[0].Index = int(100.0f * mc_OpenGLChart.m_X10Samples);
				}
			}
		}
	}
	//-Protected-------------------------------------------------------------------
	// Name: BuildFont3D(LPCWSTR FontName, int FontSize)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::BuildFont3D(LPCWSTR FontName, int FontSize)
	{
		GLYPHMETRICSFLOAT gmf[256];
		HFONT font = NULL;

		m_Fonts.Font3D = glGenLists(256);
		font = CreateFont(FontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, FontName);
		SelectObject(m_Info.hDC, font);
		wglUseFontOutlines(m_Info.hDC, 0, 255, m_Fonts.Font3D, 0.0f, 0.4f, WGL_FONT_POLYGONS, gmf);
	}
	//-Protected-------------------------------------------------------------------
	// Name: BuildFont2D(LPCWSTR FontName, int FontSize)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::BuildFont2D(LPCWSTR FontName, int FontSize)
	{
		HFONT font = NULL;

		m_Fonts.Font2D = glGenLists(256);
		font = CreateFont(FontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, FontName);
		SelectObject(m_Info.hDC, font);
		wglUseFontBitmaps(m_Info.hDC, 0, 255, m_Fonts.Font2D);
	}
	//-Protected-------------------------------------------------------------------
	// Name: KillFont3D()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::KillFont3D()
	{
		glDeleteLists(m_Fonts.Font3D, 256);
	}
	//-Protected-------------------------------------------------------------------
	// Name: KillFont2D()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::KillFont2D()
	{
		glDeleteLists(m_Fonts.Font2D, 256);
	}
	//-Protected-------------------------------------------------------------------
	// Name: DrawCube()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::DrawCube()
	{
		static GLfloat Rotate = 0;

		Rotate++;
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glMatrixMode(GL_MODELVIEW);
		glTranslatef(0.0f, 0.0f, -7.0f);
		glRotatef(Rotate, 1.0f, 1.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		// Front
		glColor3f(1.0f, 0.0f, 0.0f);     // Red
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);     // Green
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 1.0f);     // Blue
		glVertex3f(1.0f, -1.0f, 1.0f);
		// Right
		glColor3f(1.0f, 0.0f, 0.0f);     // Red
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);     // Blue
		glVertex3f(1.0f, -1.0f, 1.0f);
		glColor3f(0.0f, 1.0f, 0.0f);     // Green
		glVertex3f(1.0f, -1.0f, -1.0f);
		// Back
		glColor3f(1.0f, 0.0f, 0.0f);     // Red
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);     // Green
		glVertex3f(1.0f, -1.0f, -1.0f);
		glColor3f(0.0f, 0.0f, 1.0f);     // Blue
		glVertex3f(-1.0f, -1.0f, -1.0f);
		// Left
		glColor3f(1.0f, 0.0f, 0.0f);       // Red
		glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);       // Blue
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glColor3f(0.0f, 1.0f, 0.0f);       // Green
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glEnd();   // Done drawing the pyramid
		glPopMatrix();

		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
	}
	//-Protected-------------------------------------------------------------------
	// Name: DrawImage()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::DrawImage()
	{
		float width = 19.20f;
		float height = 10.80f;
		float x = 0;
		float y = 0;
		float z = 0;

		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);

		glPushMatrix();

		glColor3f(1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, mc_OpenGLTextureLoader.m_Texture.TextureID);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z);
		glEnd();

		GLUquadric* quad;
		quad = gluNewQuadric();
		gluQuadricNormals(quad, GLU_SMOOTH);
		gluQuadricTexture(quad, GL_TRUE);
		glTranslatef(10.0f, 10.0f, 10.0f);
		glBindTexture(GL_TEXTURE_2D, mc_OpenGLTextureLoader.m_Texture.TextureID);
		gluSphere(quad, 2.0, 100, 20);

		glPopMatrix();

		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	//-Protected-------------------------------------------------------------------
	// Name: Draw3DFonts()
	//-----------------------------------------------------------------------------
	void MC_OpenGL::Draw3DFonts()
	{
		glRotatef(m_Rotate, 1.0f, 0.0f, 0.0f);
		glRotatef(m_Rotate * 1.2f, 0.0f, 1.0f, 0.0f);
		glRotatef(m_Rotate * 1.4f, 0.0f, 0.0f, 1.0f);
		glColor3f(1.0f * float(cos(m_Rotate / 20.0f)), 1.0f * float(sin(m_Rotate / 25.0f)), 1.0f - 0.5f * float(cos(m_Rotate / 17.0f)));
		Print3DScreen("Vector Network Analyzer.", -3.5f, 0.0f, 0.0f);
		m_Rotate += 0.1f;
	}
	//-Protected-------------------------------------------------------------------
	// Name: AutoLineWidth(bool Smooth)
	//-----------------------------------------------------------------------------
	void MC_OpenGL::AutoLineWidth(bool Smooth)
	{
		if (Smooth) {
			glEnable(GL_LINE_SMOOTH);
		} else {
			glDisable(GL_LINE_SMOOTH);
		}
		GLfloat fSizes[2];
		GLfloat fSizesMin;
		GLfloat fSizesMax;
		glGetFloatv(GL_LINE_WIDTH_RANGE, fSizes);
		glGetFloatv(GL_LINE_WIDTH_GRANULARITY, fSizes);
		fSizesMin = fSizes[0];
		fSizesMax = fSizes[1];
		glLineWidth(fSizesMin);
	}
};
