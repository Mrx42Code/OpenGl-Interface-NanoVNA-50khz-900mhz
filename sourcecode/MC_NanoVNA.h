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
// File: MC_NanoVNA.h: interface for the MC_NanoVNA class.
// Desc: Application interface for the MC_NanoVNA class.
//-----------------------------------------------------------------------------
#ifndef MC_NanoVNA_H
	#define MC_NanoVNA_H

#pragma once

namespace NanoVNA {
	//-----------------------------------------------------------------------------
	// Const
	//-----------------------------------------------------------------------------
	#define MSG_MAXITEMS						50

	#define	MSG_Commands						0
	#define	MSG_help							1
	#define	MSG_exit							2
	#define	MSG_info							3
	#define	MSG_echo							4
	#define	MSG_systime							5
	#define	MSG_threads							6
	#define	MSG_reset							7
	#define	MSG_freq							8
	#define	MSG_offset							9
	#define	MSG_time							10
	#define	MSG_dac								11
	#define	MSG_saveconfig						12
	#define	MSG_clearconfig						13
	#define	MSG_data							14
	#define	MSG_dump							15
	#define	MSG_frequencies						16
	#define	MSG_port							17
	#define	MSG_stat							18
	#define	MSG_gain							19
	#define	MSG_power							20
	#define	MSG_sweep							21
	#define	MSG_test							22
	#define	MSG_touchcal						23
	#define	MSG_touchtest						24
	#define	MSG_pause							25
	#define	MSG_resume							26
	#define	MSG_cal								27
	#define	MSG_save							28
	#define	MSG_recall							29
	#define	MSG_trace							30
	#define	MSG_marker							31
	#define	MSG_edelay							32

	#define	MSG_MAX_Commands					33

	#define	VNA_MAX_x10Samples					100
	#define	VNA_SamplesPerScan					100
	#define	VNA_MAX_NumberOfSamples				(102 * VNA_MAX_x10Samples)
	#define	VNA_MAX_NumberOfMakers				6
	#define VNA_STARTFREQ						50000
	#define VNA_STOPFREQ						900000000

	#define VNA_FILTERAMOUNT					5
	#define VNA_AVR_LO							0.1
	#define VNA_AVR_HI							(1.0 - VNA_AVR_LO)  
	//-----------------------------------------------------------------------------
	// Struct
	//-----------------------------------------------------------------------------
	typedef struct _STRINGDATA
	{
		std::string          Line;
		std::string          Items[MSG_MAXITEMS];
		int					 ItemCount = 0;
		int					 Mode = MSG_Commands;
		int					 SubMode = 0;
	} STRINGDATA;

	typedef struct _VNASWEEP
	{
		uint32_t			StartFrequency;
		uint32_t			StopFrequency;
		uint32_t			VNAStartFrequency;
		uint32_t			VNAStopFrequency;
		uint16_t			NumberOfsamples;
	} VNASWEEP;

	typedef struct _VNAVALUES_S11
	{
		VNARAWVALUES		Complex;
		uint32_t			Frequencies;				
		double				Fw_Linear;
		double				Rv_Linear;
		double				Fw_LogMag;
		double				Rv_LogMag;
		double				Fw_Power;
		double				Rv_Power;
		double				SWR;
		double				Resitance;
		double				Reactance;
		double				Admittance_Resitance;
		double				Admittance_Reactance;
		double				CapacitanceInductance;
		char				CapacitanceInductancePrefix[8];
		double				Phase;
		VNAVALUES_S11_Real	Real;
	} VNAVALUES_S11;

	typedef struct _VNAVALUES_S21
	{
		VNARAWVALUES		Complex;
		uint32_t			Frequencies;
		double				Rv_Linear;
		double				Rv_LogMag;
		double				Phase;
	} VNAVALUES_S21;

	typedef struct _VNAVALUES_MARKER
	{
		int					ID;
		int					Index;
		uint32_t			Frequencies;
	} VNAVALUES_MARKER;

	typedef struct _VNADATA
	{
		VNASWEEP			Sweep;
		uint32_t			Frequencies[VNA_MAX_NumberOfSamples];
		VNARAWVALUES		Complex_S11[VNA_MAX_NumberOfSamples];
		VNARAWVALUES		LastComplex_S11[VNA_MAX_NumberOfSamples];
		VNARAWVALUES		Complex_S21[VNA_MAX_NumberOfSamples];
		VNARAWVALUES		LastComplex_S21[VNA_MAX_NumberOfSamples];
		bool				Use_Average;
		VNARAWVALUES		Average_Complex_S11[VNA_MAX_NumberOfSamples];
		VNARAWVALUES		Average_Complex_S21[VNA_MAX_NumberOfSamples];
		VNAVALUES_MARKER	Marker[VNA_MAX_NumberOfMakers];
		uint16_t			SamplesNumber;
	} VNADATA;

	typedef struct _VNASAMPLE
	{
		VNASWEEP			Sweep;
		VNAVALUES_S11		Data_S11[VNA_MAX_NumberOfSamples];
		VNAVALUES_S21		Data_S21[VNA_MAX_NumberOfSamples];
		VNAVALUES_MARKER	Marker[VNA_MAX_NumberOfMakers];
	} VNASAMPLE;

	typedef struct _VNAMULTISWEEP
	{
		int					NumberOfMultiSweep;
		int					MultiSweepIndex;
		VNASWEEP			MultSweep[VNA_MAX_x10Samples];
		bool				Stop;
	} VNAMULTISWEEP;

	//-----------------------------------------------------------------------------
	// Name: class MC_NanoVNA
	// Desc: Application class
	//-----------------------------------------------------------------------------
	class MC_NanoVNA
	{

		public:
			VNAMULTISWEEP		m_VNAMultiData;
			VNADATA				m_VNAData;
			VNASAMPLE			m_VNASample;
			STRINGDATA			m_String;

			float				m_X10Samples;
			float				m_NumberOfSamples;
			bool				m_SweepDone;
			bool				m_Update;

		protected:

		private:

			//-----------------------------------------------------------------------------

		public:
								MC_NanoVNA();
			virtual				~MC_NanoVNA();
			bool				DecodeData(char* LineData, int LineDatalen);
			bool				LoadDecodeData(char* LineData, int LineDatalen);
			void				GetCapacitanceInductance(char* buf, int len, VNARAWVALUES* Complex, uint32_t Frequency);
			void				Calculate_MultiSweep(uint32_t StartFrequency, uint32_t StopFrequency, int NumberOfSweeps);
			void				DoRecall(int DataType);
			void				DoMultiSweep(int S11, int S21);
			void				DoMultiSweepDataType(int DataType);

		protected:
			bool				GetItemData(STRINGDATA* Data);
			VNAVALUES_S11		GetNanoVNA_Data_S11(VNARAWVALUES* Complex, uint32_t Frequencies);
			VNAVALUES_S21		GetNanoVNA_Data_S21(VNARAWVALUES* Complex, uint32_t Frequencies);
			VNAVALUES_S11		InsertionSortS11(bool Filter, VNAVALUES_S11* Data_In, int Index, int NumberOfSamples, int Amount);
			VNAVALUES_S21		InsertionSortS21(bool Filter, VNAVALUES_S21* Data_In, int Index, int NumberOfSamples, int Amount);
			VNARAWVALUES		InsertionSortComplex_S11(bool Filter, VNARAWVALUES* Data_In, int Index, int NumberOfSamples, int Amount);
			VNARAWVALUES		InsertionSortComplex_S21(bool Filter, VNARAWVALUES* Data_In, int Index, int NumberOfSamples, int Amount);

		private:

	};
};
//-----------------------------------------------------------------------------
#endif // MC_NanoVNA_H
