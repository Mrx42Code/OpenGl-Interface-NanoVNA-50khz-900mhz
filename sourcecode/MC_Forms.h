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
// File: MC_Forms.h: interface for the MC_Forms class.
// Desc: Application interface for the MC_Forms class.
//-----------------------------------------------------------------------------
#ifndef MC_Forms_H
	#define MC_Forms_H

#pragma once

namespace NanoVNA {
	//-----------------------------------------------------------------------------
	// Const
	//-----------------------------------------------------------------------------
	#define FORM_VNA_SAMPLE_PERSACN		100
	#define FORM_SPIN_MAX_POS			100
	#define FORM_SPIN_MIN_POS			1
	#define FORM_FILE_FILTER			"Text Files (*.txt)\0*txt\0All Files (*.*)\0*.*\0"
	//-----------------------------------------------------------------------------
	// Struct
	//-----------------------------------------------------------------------------
	enum {
		M_COLOUR_White, M_COLOUR_Grid10, M_COLOUR_Grid1, M_COLOUR_S11_Swr, M_COLOUR_S11_Phase, M_COLOUR_S11_Reactance, M_COLOUR_S11_Resitance, M_COLOUR_S11_LogMag, M_COLOUR_S11_LinMag,
		M_COLOUR_S21_LogMag, M_COLOUR_S21_LinMag, M_COLOUR_S21_Phase, M_COLOUR_Smith_Red, M_COLOUR_Smith_Blue, M_COLOUR_Smith_Trace, M_COLOUR_Black
	};

	typedef struct _SerialPort_Info
	{
		std::wstring		SerialPort;
		std::wstring		BaudRate;
		std::wstring		DataBits;
		std::wstring		StopBits;
		std::wstring		ParityBit;
		int					RTS = 0;
		int					DTR = 0;
		bool				Connect = false;
		bool				Update = false;
	} SerialPort_Info;

	typedef struct _Sweep_Info
	{
		uint32_t			StartFrequency = 0;
		uint32_t			StopFrequency = 0;
		uint32_t			CenterFrequency = 0;
		uint32_t			SpanFrequency = 0;
		uint32_t			xSampleRate = 0;
		uint32_t			TotalSamples = 0;
	} Sweep_Info;

	typedef struct _FrequencySweep_Info
	{
		std::wstring		StartFrequency;
		std::wstring		StopFrequency;
		std::wstring		CenterFrequency;
		std::wstring		SpanFrequency;
		std::wstring		xSampleRate;
		std::wstring		FrequencyInfo;
		uint32_t			xSpinSampleRate = FORM_SPIN_MIN_POS;
		uint32_t			TotalSamples = FORM_VNA_SAMPLE_PERSACN;
		Sweep_Info			Sweep;
		bool				Update = false;
	} FrequencySweep_Info;

	typedef struct _FORM_MENUCHECKED
	{
		bool				Auto_GetDataS11;
		bool				Auto_GetDataS21;
		bool				Chart_2D3D;
		bool				Chart_Grid;
		bool				Chart_GridFine;
		bool				Chart_SmithChartRed;
		bool				Chart_SmithChartBlue;
		bool				Chart_SmithChartImage;
		bool				Chart_SmithChartImageRedBlue;
		bool				Chart_Oscilloscope;
		bool				Chart_Oscilloscope_S11_S21;
		bool				TraceS11_LogMag;
		bool				TraceS11_LinearMag;
		bool				TraceS11_SWR;
		bool				TraceS11_Resitance;
		bool				TraceS11_Reactance;
		bool				TraceS11_Phase;
		bool				TraceS21_LogMag;
		bool				TraceS21_LinearMag;
		bool				TraceS21_Phase;
		bool				Marker_Red_Enable;
		bool				Marker_Green_Enable;
		bool				Marker_Blue_Enable;
		bool				Marker_Max_Enable;
		bool				Marker_Min_Enable;
		bool				Filter_Average;
		bool				Filter_Noise;
		bool				ConnectDisconnect;
	} FORM_MENUCHECKED;

	//-----------------------------------------------------------------------------
	// Name: class MC_Forms
	// Desc: Application class
	//-----------------------------------------------------------------------------
	class MC_Forms
	{

		public:
			SerialPort_Info		m_SerialPort;
			FrequencySweep_Info m_FrequencySweep;
			FORM_COLOURSELECT	m_Colours;
			FORM_MENUCHECKED	m_MenuCheked;

		protected:

		private:

			//-----------------------------------------------------------------------------

		public:
								MC_Forms();
			virtual				~MC_Forms();
			void				OnCreate(HWND hDlg);
			void				OnDestroy();
			void				Menu_Init();
			void				OnMenu_FileOpen(HWND hDlg);
			void				OnMenu_FileSave(HWND hDlg);
			void				OnMenu_ColourStettings(HWND hDlg);
			void				OnMenu_Data_Clear(HWND hDlg);
			void				OnMenu_GetData_S11(HWND hDlg);
			void				OnMenu_GetData_S21(HWND hDlg);
			void				OnMenu_GetData_Type(HWND hDlg, int DataType);
			void				OnMenu_GetAutoData_S11(HWND hDlg);
			void				OnMenu_GetAutoData_S21(HWND hDlg);
			void				OnMenu_Chart_2D3D(HWND hDlg);
			void				OnMenu_Chart_Grid(HWND hDlg);
			void				OnMenu_Chart_GridFine(HWND hDlg);
			void				OnMenu_Chart_SmithChartRed(HWND hDlg);
			void				OnMenu_Chart_SmithChartBlue(HWND hDlg);
			void				OnMenu_Chart_SmithChartImage(HWND hDlg);
			void				OnMenu_Chart_SmithChartImageRedBlue(HWND hDlg);
			void				OnMenu_Chart_Oscilloscope(HWND hDlg);
			void				OnMenu_Chart_Oscilloscope_S11_S21(HWND hDlg);
			void				OnMenu_TraceS11_LogMag(HWND hDlg);
			void				OnMenu_TraceS11_LinearMag(HWND hDlg);
			void				OnMenu_TraceS11_SWR(HWND hDlg);
			void				OnMenu_TraceS11_Resitance(HWND hDlg);
			void				OnMenu_TraceS11_Reactance(HWND hDlg);
			void				OnMenu_TraceS11_Phase(HWND hDlg);
			void				OnMenu_TraceS21_LogMag(HWND hDlg);
			void				OnMenu_TraceS21_LinearMag(HWND hDlg);
			void				OnMenu_TraceS21_Phase(HWND hDlg);
			void				OnMenu_Marker_Red_Enable(HWND hDlg);
			void				OnMenu_Marker_Red_Set(HWND hDlg);
			void				OnMenu_Marker_Green_Enable(HWND hDlg);
			void				OnMenu_Marker_Green_Set(HWND hDlg);
			void				OnMenu_Marker_Blue_Enable(HWND hDlg);
			void				OnMenu_Marker_Blue_Set(HWND hDlg);
			void				OnMenu_Marker_Max_Enable(HWND hDlg);
			void				OnMenu_Marker_Min_Enable(HWND hDlg);
			void				OnMenu_Filter_Average(HWND hDlg);
			void				OnMenu_Filter_Noise(HWND hDlg);
			void				OnMenu_Recall(HWND hDlg, UINT32 MenuItem);
			void				OnMenu_ConnectDisconnect(HWND hDlg);
			std::string			Filename_Open(const char* filter, HWND hwnd);
			std::string			Filename_Save(const char* filter, HWND hwnd);
			void				FormFrequencySweep_Init();
			void				FormFrequencySweep_SetData(HWND hDlg);
			void				FormFrequencySweep_GetData(HWND hDlg);
			void				FormFrequencySweep_CalculateSweep(HWND hDlg);
			void				FormFrequencySweep_CalculateSpan(HWND hDlg);
			void				FormFrequencySweep_CalculateSteps(HWND hDlg, int Value);
			void				FormSerialPort_Init();
			void				FormSerialPort_SetData(HWND hDlg);
			void				FormSerialPort_GetData(HWND hDlg);
			void				FormSerialPort_RefreshData(HWND hDlg);
			void				FormCalibration_SetData(HWND hDlg);
			void				FormCalibration_Buttons(HWND hDlg, UINT Button);
			void				Colour_Init();
			bool				Colour_Select(HWND hwnd, FORM_COLOURSELECT* Colours);
			COLORREF			ConvertRGBToDword(FORM_COLOUR* Colour);
			FORM_COLOUR			ConvertDwordToRGB(COLORREF DwColour);
			std::wstring		utf8_to_wstring(const std::string& str);
			std::string			wstring_to_utf8(const std::wstring& str);
			void				UpdateSweepStrings();

		protected:
			void				FormSetCheckBox(HWND hDlg, UINT uIDCheckItem, bool Value);

		private:

	};
};
//-----------------------------------------------------------------------------
#endif // MC_Forms
