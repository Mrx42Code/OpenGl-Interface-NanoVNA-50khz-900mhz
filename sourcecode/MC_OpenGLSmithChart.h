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
// File: MC_OpenGLSmithChart.h: interface for the MC_OpenGLSmithChart class.
// Desc: Application interface for the MC_OpenGLSmithChart class.
//-----------------------------------------------------------------------------
#ifndef MC_OpenGLSmithChart_H
	#define MC_OpenGLSmithChart_H

#pragma once

#include <gl\gl.h>
#include <gl\glu.h>

namespace NanoVNA {
	//-----------------------------------------------------------------------------
	// Const
	//-----------------------------------------------------------------------------
	#define	SMITH_MAXCIRCLE		21
	#define	SMITH_MAXARC		(SMITH_MAXCIRCLE - 1)
	//-----------------------------------------------------------------------------
	// Struct
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	// Name: class MC_OpenGLSmithChart
	// Desc: Application class
	//-----------------------------------------------------------------------------
	class MC_OpenGLSmithChart
	{

		public:
			bool				m_Enable;
			bool				m_transparen;

		protected:

		private:

			//-----------------------------------------------------------------------------

		public:
								MC_OpenGLSmithChart();
			virtual				~MC_OpenGLSmithChart();
			void				Init();
			void				UnLoad();
			void				DrawChart(OPENGL_GRIDDATA* GridData, OPENGL_MARKERS* Marker);

		protected:
			void				DrawChart_Image(OPENGL_GRIDDATA* GridData);
			void                DrawChart_Smith_Red(OPENGL_GRIDDATA* GridData);
			void                DrawChart_Smith_Blue(OPENGL_GRIDDATA* GridData);
			void                DrawChart_TopeLine(OPENGL_GRIDDATA* GridData, int MarkerIndex, FORM_COLOUR* Colour);
			void				DrawChart_TraceS11_Smith(OPENGL_GRIDDATA* GridData, int MarkerIndex, FORM_COLOUR* Colour);
			void				DrawChartImage(OPENGL_GRIDDATA* GridData, float x, float y, float z, float width, float height, float length);
			void                DrawCircleRight(OPENGL_GRIDDATA* GridData, GLfloat cx, GLfloat cy, GLfloat radius, int num_segments, FORM_COLOUR* Colour, const char* Label);
			void                DrawCircleLeft(OPENGL_GRIDDATA* GridData, GLfloat cx, GLfloat cy, GLfloat radius, int num_segments, FORM_COLOUR* Colour, const char* Label);
			void                DrawArcRight(OPENGL_GRIDDATA* GridData, GLfloat cx, GLfloat cy, GLfloat radius, GLfloat start_angle, GLfloat arc_angle, int num_segments, bool Debug, FORM_COLOUR* Colour, const char* Label);
			void                DrawArcLeft(OPENGL_GRIDDATA* GridData, GLfloat cx, GLfloat cy, GLfloat radius, GLfloat start_angle, GLfloat arc_angle, int num_segments, bool Debug, FORM_COLOUR* Colour, const char* Label);
			void                Draw_2DLine(GLfloat xx, GLfloat yy, GLfloat x, GLfloat y);
			void				Draw_3DText(char* Buffer, GLfloat x, GLfloat y, GLfloat z, FORM_COLOUR* Colour);
			void				Draw_3DMarker(char* Buffer, GLfloat x, GLfloat y, GLfloat z, FORM_COLOUR* Colour, int MarkerIndex);
			void                Draw_3DString(char* Buffer);
			GLfloat				GetOffSet_Y(OPENGL_GRIDDATA* GridData);
			void				Translate3D(OPENGL_GRIDDATA* GridData);
			void				DrawChart_Limit(OPENGL_GRIDDATA* GridData, OPENGL_VECTOR* Vectors, GLfloat x, GLfloat y);

		private:

	};
};
//-----------------------------------------------------------------------------
#endif // MC_OpenGLSmithChart_H	
