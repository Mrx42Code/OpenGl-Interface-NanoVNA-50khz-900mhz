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
// File: MC_Oscilloscope.h: interface for the MC_Oscilloscope class.
// Desc: Application interface for the MC_Oscilloscope class.
//-----------------------------------------------------------------------------
#ifndef MC_Oscilloscope_H
	#define MC_Oscilloscope_H

#pragma once

#include "CommanStruct.h"

namespace NanoVNA {
	//-----------------------------------------------------------------------------
	// Const
	//-----------------------------------------------------------------------------
	#define	OSC_MAX_x10Samples					100
	#define	OSC_SamplesPerScan					100
	#define	OSC_MAX_NumberOfSamples				(102 * OSC_MAX_x10Samples)

	#define	OSC_CHANNEL_ALL						0
	#define	OSC_CHANNEL_1						1
	#define	OSC_CHANNEL_2						2
	//-----------------------------------------------------------------------------
	// Struct
	//-----------------------------------------------------------------------------
	typedef struct _OSC_VECTOR
	{
		float				x;
		float				y;
	} OSC_VECTOR;

	typedef struct _OSCVALUES_CH
	{
		VNARAWVALUES		Complex;
		uint32_t			Frequencies;
		OSC_VECTOR			Output[2];
	} OSCVALUES_CH;

	typedef struct _OSCDATA
	{
		OSCVALUES_CH		Channel_1[OSC_MAX_NumberOfSamples];
		OSCVALUES_CH		Channel_2[OSC_MAX_NumberOfSamples];
		uint32_t			Channel_1_MaxSamples;
		uint32_t			Channel_2_MaxSamples;
	} OSCDATA;
	//-----------------------------------------------------------------------------
	// Name: class MC_Oscilloscope
	// Desc: Application class
	//-----------------------------------------------------------------------------
	class MC_Oscilloscope
	{

		public:
			OSCDATA				m_Oscilloscope;

		protected:

		private:

			//-----------------------------------------------------------------------------

		public:
								MC_Oscilloscope();
			virtual				~MC_Oscilloscope();
			void				Init();
			void				ClearValues(int Channel);
			void				AddValues(int Channel, VNARAWVALUES Values, uint32_t Frequencies);
			void				CalculateValues(int Channel, float Scale_X, float Scale_Y, float Max_X, float Max_Y, float Half_X);
			void                DrawOscilloscope(int Channel, FORM_COLOUR* Colour);

		protected:

		private:
			void				Limit(OSC_VECTOR* Vectors, float x, float y, float Max_X, float Max_Y);
			void				Draw_2DLine(float xx, float yy, float x, float y);

	};
	//-----------------------------------------------------------------------------
};
#endif // MC_Oscilloscope_H
