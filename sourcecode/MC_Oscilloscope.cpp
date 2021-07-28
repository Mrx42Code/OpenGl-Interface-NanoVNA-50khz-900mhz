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
// File: MC_Oscilloscope.cpp: implementation of the MC_Oscilloscope class.
//-----------------------------------------------------------------------------
#include "ExternNamespace.h"

namespace NanoVNA {
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
	// Name: MC_Oscilloscope()
	// Desc: MC_Oscilloscope class
	//-----------------------------------------------------------------------------
	MC_Oscilloscope::MC_Oscilloscope()
	{
		memset(&m_Oscilloscope, 0, sizeof(m_Oscilloscope));
	}
	//-Public----------------------------------------------------------------------
	// Name: ~MC_Oscilloscope()
	// Desc: ~MC_Oscilloscope Destruction class
	//-----------------------------------------------------------------------------
	MC_Oscilloscope::~MC_Oscilloscope()
	{

	}
	//-Public----------------------------------------------------------------------
	// Name: Init()
	//-----------------------------------------------------------------------------
	void MC_Oscilloscope::Init()
	{

	}
	//-Public----------------------------------------------------------------------
	// Name: ClearValues(int Channel)
	//-----------------------------------------------------------------------------
	void MC_Oscilloscope::ClearValues(int Channel)
	{
		if (Channel == OSC_CHANNEL_ALL) {
			memset(&m_Oscilloscope, 0, sizeof(m_Oscilloscope));
		} else if (Channel == OSC_CHANNEL_1) {
			memset(&m_Oscilloscope.Channel_1, 0, sizeof(m_Oscilloscope.Channel_1));
			m_Oscilloscope.Channel_1_MaxSamples = 0;
		} else if (Channel == OSC_CHANNEL_2) {
			memset(&m_Oscilloscope.Channel_2, 0, sizeof(m_Oscilloscope.Channel_2));
			m_Oscilloscope.Channel_2_MaxSamples = 0;
		}
	}
	//-Public----------------------------------------------------------------------
	// Name: AddValues(int Channel, VNARAWVALUES Values, uint32_t Frequencies)
	//-----------------------------------------------------------------------------
	void MC_Oscilloscope::AddValues(int Channel, VNARAWVALUES Values, uint32_t Frequencies)
	{
		if (Channel == OSC_CHANNEL_1 && m_Oscilloscope.Channel_1_MaxSamples < OSC_MAX_NumberOfSamples) {
			m_Oscilloscope.Channel_1[m_Oscilloscope.Channel_1_MaxSamples].Complex = Values;
			m_Oscilloscope.Channel_1[m_Oscilloscope.Channel_1_MaxSamples].Frequencies = Frequencies;
			m_Oscilloscope.Channel_1_MaxSamples++;
		} else if (Channel == OSC_CHANNEL_2 && m_Oscilloscope.Channel_2_MaxSamples < OSC_MAX_NumberOfSamples) {
			m_Oscilloscope.Channel_2[m_Oscilloscope.Channel_2_MaxSamples].Complex = Values;
			m_Oscilloscope.Channel_2[m_Oscilloscope.Channel_2_MaxSamples].Frequencies = Frequencies;
			m_Oscilloscope.Channel_2_MaxSamples++;
		}
	}
	//-Public----------------------------------------------------------------------
	// Name: CalculateValues(int Channel, float Scale_X, float Scale_Y, float Max_X, float Max_Y, float Half_X)
	//-----------------------------------------------------------------------------
	void MC_Oscilloscope::CalculateValues(int Channel, float Scale_X, float Scale_Y, float Max_X, float Max_Y, float Half_X)
	{
		uint32_t i;
		float x, y;
		float Value;
		float ScaleY = (Max_Y / (m_Oscilloscope.Channel_1_MaxSamples - 1));
		float Mag = 25.0f;

		if (Channel == OSC_CHANNEL_1) {
			for (i = 0; i < m_Oscilloscope.Channel_1_MaxSamples; i++) {
				y = (float(i) * ScaleY) ;
				Value = float(m_Oscilloscope.Channel_1[i].Complex.Value1) * Mag;
				x = (Value * Scale_X) + (Half_X / 2.0f);
				Limit(&m_Oscilloscope.Channel_1[i].Output[0], x, y, Max_X, Max_Y);
				Value = float(m_Oscilloscope.Channel_1[i].Complex.Value2) * Mag;
				x = (Value * Scale_X) + (Half_X + (Half_X / 2.0f));
				Limit(&m_Oscilloscope.Channel_1[i].Output[1], x, y, Max_X, Max_Y);
			}
		} else if (Channel == OSC_CHANNEL_2) {
			for (i = 0; i < m_Oscilloscope.Channel_2_MaxSamples; i++) {
				y = (float(i) * ScaleY);
				Value = float(m_Oscilloscope.Channel_2[i].Complex.Value1) * Mag;
				x = (Value * Scale_X) + (Half_X / 2.0f);
				Limit(&m_Oscilloscope.Channel_2[i].Output[0], x, y, Max_X, Max_Y);
				Value = float(m_Oscilloscope.Channel_2[i].Complex.Value2) * Mag;
				x = (Value * Scale_X) + (Half_X + (Half_X / 2.0f));
				Limit(&m_Oscilloscope.Channel_2[i].Output[1], x, y, Max_X, Max_Y);
			}
		}
	}
	//-Public----------------------------------------------------------------------
	// Name: DrawOscilloscope(int Channel, FORM_COLOUR* Colour)
	//-----------------------------------------------------------------------------
	void MC_Oscilloscope::DrawOscilloscope(int Channel, FORM_COLOUR* Colour)
	{
		uint32_t i;
		GLfloat x, y;
		GLfloat lx, ly;

		if (!mc_Forms.m_MenuCheked.Chart_Oscilloscope) {
			return;
		}
		glColor3f(Colour->r * 0.6f, Colour->g * 0.6f, Colour->b * 0.6f);
		if (Channel == OSC_CHANNEL_1) {
			for (i = 0; i < m_Oscilloscope.Channel_1_MaxSamples; i++) {
				x = m_Oscilloscope.Channel_1[i].Output[0].x;
				y = m_Oscilloscope.Channel_1[i].Output[0].y;
				if (i == 0) {
					lx = x; ly = y;
				}
				Draw_2DLine(lx, ly, x, y);
				lx = x; ly = y;
			}
			for (i = 0; i < m_Oscilloscope.Channel_1_MaxSamples; i++) {
				x = m_Oscilloscope.Channel_1[i].Output[1].x;
				y = m_Oscilloscope.Channel_1[i].Output[1].y;
				if (i == 0) {
					lx = x; ly = y;
				}
				Draw_2DLine(lx, ly, x, y);
				lx = x; ly = y;
			}
		} else if (Channel == OSC_CHANNEL_2) {
			for (i = 0; i < m_Oscilloscope.Channel_2_MaxSamples; i++) {
				x = m_Oscilloscope.Channel_2[i].Output[0].x;
				y = m_Oscilloscope.Channel_2[i].Output[0].y;
				if (i == 0) {
					lx = x; ly = y;
				}
				Draw_2DLine(lx, ly, x, y);
				lx = x; ly = y;
			}
			for (i = 0; i < m_Oscilloscope.Channel_2_MaxSamples; i++) {
				x = m_Oscilloscope.Channel_2[i].Output[1].x;
				y = m_Oscilloscope.Channel_2[i].Output[1].y;
				if (i == 0) {
					lx = x; ly = y;
				}
				Draw_2DLine(lx, ly, x, y);
				lx = x; ly = y;
			}
		}
	}
	//*****************************************************************************  
	// Protected Code
	//*****************************************************************************

	//-Protected-------------------------------------------------------------------
	// Name: Limit(OSC_VECTOR* Vectors, float x, float y, float Max_X, float Max_Y)
	//-----------------------------------------------------------------------------
	void MC_Oscilloscope::Limit(OSC_VECTOR* Vectors, float x, float y, float Max_X, float Max_Y)
	{
		if (x < 0.0f) {
			x = 0.0f;
		}
		else if (x > Max_X) {
			x = Max_X;
		}
		if (y < 0.0f) {
			y = 0.0f;
		}
		else if (y > Max_Y) {
			y = Max_Y;
		}
		Vectors->x = x;
		Vectors->y = y;
	}
	//-Protected-------------------------------------------------------------------
	// Name: Draw_2DLine(float xx, float yy, float x, float y)
	//-----------------------------------------------------------------------------
	void MC_Oscilloscope::Draw_2DLine(float xx, float yy, float x, float y)
	{
		glBegin(GL_LINES);
			glVertex2f(xx, yy);
			glVertex2f(x, y);
		glEnd();
	}
};