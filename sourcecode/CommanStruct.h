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
// File: CommanStruct.h: interface for the CommanStruct.h.
//-----------------------------------------------------------------------------
#ifndef CommanStruct_H
	#define CommanStruct_H

#pragma once

#include <windows.h>

//-Public----------------------------------------------------------------------
// Const
//-----------------------------------------------------------------------------
#define FORM_MAX_COLOURS            16

//-Public----------------------------------------------------------------------
// Struct
//-----------------------------------------------------------------------------
typedef union _FORM_COLOR4B
{
	COLORREF            DwColour;
	struct _Dword
	{
		unsigned char   r;
		unsigned char   g;
		unsigned char   b;
		unsigned char   a;
	} Dword;
} FORM_COLOR4B;

typedef struct _FORM_COLOUR
{
	float				r;
	float				g;
	float				b;
} FORM_COLOUR;

typedef struct _FORM_COLOURSELECT
{
	FORM_COLOUR			Colourfloat[FORM_MAX_COLOURS];
	COLORREF            ColourDWord[FORM_MAX_COLOURS];
	COLORREF            ColourResult;
} FORM_COLOURSELECT;

typedef struct _VNARAWVALUES
{
	double				Value1;
	double				Value2;
} VNARAWVALUES;

typedef struct _VNAVALUES_S11_Real
{
	double				Frequency;
	double				Impedance_Resitance;
	double				Impedance_Resitance_jx;
	double				Admittance_Resitance;
	double				Admittance_Resitance_jx;
	double				Series_Admittance_Resitance;
	double				Series_Admittance_Resitance_jx;
	double				Parallel_Admittance_Resitance;
	double				Parallel_Admittance_Resitance_jx;
	double				Series_Resitance;
	double				Series_CapacitanceInductance;
	double				Parallel_Resitance;
	double				Parallel_CapacitanceInductance;
} VNAVALUES_S11_Real;

//-Public----------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#endif // CommanStruct_H
