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
// File: MC_Forms.cpp: implementation of the MC_Forms class.
//
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

    //-----------------------------------------------------------------------------
    // message handlers
    //-----------------------------------------------------------------------------

    //-Public----------------------------------------------------------------------
    // Name: MC_Forms()
    // Desc: MC_Forms class
    //-----------------------------------------------------------------------------
    MC_Forms::MC_Forms()
    {
        Menu_Init();
        Colour_Init();
        FormFrequencySweep_Init();
        FormSerialPort_Init();
    }
    //-Public----------------------------------------------------------------------
    // Name: ~MC_Forms()
    // Desc: ~MC_Forms Destruction class
    //-----------------------------------------------------------------------------
    MC_Forms::~MC_Forms()
    {

    }
    //-Public----------------------------------------------------------------------
    // Name: OnCreate(HWND hWnd)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnCreate(HWND hDlg)
    {
        mc_RS232.OnCreate(hDlg);

        FormSetCheckBox(hDlg, IDM_CONNECT, m_MenuCheked.ConnectDisconnect);
        FormSetCheckBox(hDlg, IDM_AUTO_DATA_S11, m_MenuCheked.Auto_GetDataS11);
        FormSetCheckBox(hDlg, IDM_AUTO_DATA_S21, m_MenuCheked.Auto_GetDataS21);
        FormSetCheckBox(hDlg, IDM_CHARTS_2D3D, m_MenuCheked.Chart_2D3D);
        FormSetCheckBox(hDlg, IDM_CHARTS_GRID, m_MenuCheked.Chart_Grid);
        FormSetCheckBox(hDlg, IDM_CHARTS_GRIDFINE, m_MenuCheked.Chart_GridFine);
        FormSetCheckBox(hDlg, IDM_CHARTSMITH_RED, m_MenuCheked.Chart_SmithChartRed);
        FormSetCheckBox(hDlg, IDM_CHARTSMITH_BLUE, m_MenuCheked.Chart_SmithChartBlue);
        FormSetCheckBox(hDlg, IDM_CHARTSMITH_IMAGE, m_MenuCheked.Chart_SmithChartImage);
        FormSetCheckBox(hDlg, IDM_CHARTSMITH_IMAGE_REDBLUE, m_MenuCheked.Chart_SmithChartImageRedBlue);
        FormSetCheckBox(hDlg, IDM_OSCILLOSCOPE, m_MenuCheked.Chart_Oscilloscope);
        FormSetCheckBox(hDlg, IDM_OSCILLOSCOPE_S11_S21, m_MenuCheked.Chart_Oscilloscope_S11_S21);
        FormSetCheckBox(hDlg, IDM_S11_LOGMAG, m_MenuCheked.TraceS11_LogMag);
        FormSetCheckBox(hDlg, IDM_S11_LINEARMAG, m_MenuCheked.TraceS11_LinearMag);
        FormSetCheckBox(hDlg, IDM_S11_SWR, m_MenuCheked.TraceS11_SWR);
        FormSetCheckBox(hDlg, IDM_S11_RESITANCE, m_MenuCheked.TraceS11_Resitance);
        FormSetCheckBox(hDlg, IDM_S11_REACTANCE, m_MenuCheked.TraceS11_Reactance);
        FormSetCheckBox(hDlg, IDM_S11_PHASE, m_MenuCheked.TraceS11_Phase);
        FormSetCheckBox(hDlg, IDM_S21_LOGMAG, m_MenuCheked.TraceS21_LogMag);
        FormSetCheckBox(hDlg, IDM_S21_LINEARMAG, m_MenuCheked.TraceS21_LinearMag);
        FormSetCheckBox(hDlg, IDM_S21_PHASE, m_MenuCheked.TraceS21_Phase);
        FormSetCheckBox(hDlg, IDM_MARKER_RED_ENABLE, m_MenuCheked.Marker_Red_Enable);
        FormSetCheckBox(hDlg, IDM_MARKER_GREEN_ENABLE, m_MenuCheked.Marker_Green_Enable);
        FormSetCheckBox(hDlg, IDM_MARKER_BLUE_ENABLE, m_MenuCheked.Marker_Blue_Enable);
        FormSetCheckBox(hDlg, IDM_MARKER_MAX_ENABLE, m_MenuCheked.Marker_Max_Enable);
        FormSetCheckBox(hDlg, IDM_MARKER_MIN_ENABLE, m_MenuCheked.Marker_Min_Enable);
        FormSetCheckBox(hDlg, IDM_AVERAGE_FILTER, m_MenuCheked.Filter_Average);
        FormSetCheckBox(hDlg, IDM_NOISE_FILTER, m_MenuCheked.Filter_Noise);
        FormSetCheckBox(hDlg, IDM_RECALL0, true);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnDestroy()
    //-----------------------------------------------------------------------------
    void MC_Forms::OnDestroy()
    {
        mc_RS232.OnDestroy();
    }
    //-Public----------------------------------------------------------------------
    // Name: Menu_Init()
    //-----------------------------------------------------------------------------
    void MC_Forms::Menu_Init()
    {
        ZeroMemory(&m_MenuCheked, sizeof(m_MenuCheked));
        m_MenuCheked.Auto_GetDataS11 = false;
        m_MenuCheked.Auto_GetDataS21 = false;
        m_MenuCheked.Chart_2D3D = true;
        m_MenuCheked.Chart_Grid = true;
        m_MenuCheked.Chart_GridFine = true;
        m_MenuCheked.Chart_SmithChartRed = true;
        m_MenuCheked.Chart_SmithChartBlue = false;
        m_MenuCheked.Chart_SmithChartImage = false;
        m_MenuCheked.Chart_SmithChartImageRedBlue = false;
        m_MenuCheked.Chart_Oscilloscope = false;
        m_MenuCheked.Chart_Oscilloscope_S11_S21 = false;
        m_MenuCheked.TraceS11_LogMag = true;
        m_MenuCheked.TraceS11_LinearMag = false;
        m_MenuCheked.TraceS11_SWR = true;
        m_MenuCheked.TraceS11_Resitance = false;
        m_MenuCheked.TraceS11_Reactance = false;
        m_MenuCheked.TraceS11_Phase = false;
        m_MenuCheked.TraceS21_LogMag = false;
        m_MenuCheked.TraceS21_LinearMag = false;
        m_MenuCheked.TraceS21_Phase = false;
        m_MenuCheked.Marker_Red_Enable = false;
        m_MenuCheked.Marker_Green_Enable = false;
        m_MenuCheked.Marker_Blue_Enable = false;
        m_MenuCheked.Marker_Max_Enable = false;
        m_MenuCheked.Marker_Min_Enable = false;
        m_MenuCheked.Filter_Average = false;
        m_MenuCheked.Filter_Noise = false;
        m_MenuCheked.ConnectDisconnect = false;
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Open(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_FileOpen(HWND hDlg)
    {
        std::string myFile;

        myFile = Filename_Open(FORM_FILE_FILTER, hDlg);
        if (myFile.length()) {
            mc_OpenGLChart.OpenVNAData(myFile.c_str());
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_FileSave(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_FileSave(HWND hDlg)
    {
        std::string myFile;

        myFile = Filename_Save(FORM_FILE_FILTER, hDlg);
        if (myFile.length()) {
            mc_OpenGLChart.SaveVNAData(myFile.c_str());
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_ConnectDisconnect(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_ConnectDisconnect(HWND hDlg)
    {
        m_MenuCheked.ConnectDisconnect ^= true;
        m_SerialPort.Connect = m_MenuCheked.ConnectDisconnect;
        FormSetCheckBox(hDlg, IDM_CONNECT, m_MenuCheked.ConnectDisconnect);
        m_SerialPort.Update = true;
        if (mc_RS232.OnConnect() && m_MenuCheked.ConnectDisconnect) {
            m_MenuCheked.ConnectDisconnect = false;
            m_SerialPort.Connect = m_MenuCheked.ConnectDisconnect;
            FormSetCheckBox(hDlg, IDM_CONNECT, m_MenuCheked.ConnectDisconnect);
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Data_Clear(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Data_Clear(HWND hDlg)
    {
        int i;

        memset(&mc_NanoVNA.m_VNAData.Frequencies, 0, sizeof(mc_NanoVNA.m_VNAData.Frequencies));
        memset(&mc_NanoVNA.m_VNASample.Data_S11, 0, sizeof(mc_NanoVNA.m_VNASample.Data_S11));
        memset(&mc_NanoVNA.m_VNASample.Data_S21, 0, sizeof(mc_NanoVNA.m_VNASample.Data_S21));
        for (i = 0; i < VNA_MAX_x10Samples; i++) {
            mc_NanoVNA.m_VNAMultiData.MultSweep[i].VNAStartFrequency = 0;
            mc_NanoVNA.m_VNAMultiData.MultSweep[i].VNAStopFrequency = 0;
        }
        mc_NanoVNA.m_Update = true;
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_GetData_S11(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_GetData_S11(HWND hDlg)
    {
        mc_NanoVNA.m_VNAMultiData.Stop = true;
        mc_OpenGLChart.m_GetVNAData.S11 = 1;
        mc_OpenGLChart.m_GetVNAData.S21 = 0;
        mc_OpenGLChart.m_GetVNAData.Update = true;
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_GetData_S21(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_GetData_S21(HWND hDlg)
    {
        mc_NanoVNA.m_VNAMultiData.Stop = true;
        mc_OpenGLChart.m_GetVNAData.S11 = 0;
        mc_OpenGLChart.m_GetVNAData.S21 = 1;
        mc_OpenGLChart.m_GetVNAData.Update = true;
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_GetData_Type(HWND hDlg, int DataType)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_GetData_Type(HWND hDlg, int DataType)
    {
        mc_NanoVNA.m_VNAMultiData.Stop = true;
        mc_OpenGLChart.m_GetVNADataType.Type = DataType;
        mc_OpenGLChart.m_GetVNADataType.Update = true;
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_GetAutoData_S11(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_GetAutoData_S11(HWND hDlg)
    {
        m_MenuCheked.Auto_GetDataS11 ^= true;
        FormSetCheckBox(hDlg, IDM_AUTO_DATA_S11, m_MenuCheked.Auto_GetDataS11);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_GetAutoData_S21(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_GetAutoData_S21(HWND hDlg)
    {
        m_MenuCheked.Auto_GetDataS21 ^= true;
        FormSetCheckBox(hDlg, IDM_AUTO_DATA_S21, m_MenuCheked.Auto_GetDataS21);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Chart_2D3D(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Chart_2D3D(HWND hDlg)
    {
        m_MenuCheked.Chart_2D3D ^= true;
        FormSetCheckBox(hDlg, IDM_CHARTS_2D3D, m_MenuCheked.Chart_2D3D);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Chart_Grid(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Chart_Grid(HWND hDlg)
    {
        m_MenuCheked.Chart_Grid ^= true;
        FormSetCheckBox(hDlg, IDM_CHARTS_GRID, m_MenuCheked.Chart_Grid);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Chart_GridFine(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Chart_GridFine(HWND hDlg)
    {
        m_MenuCheked.Chart_GridFine ^= true;
        FormSetCheckBox(hDlg, IDM_CHARTS_GRIDFINE, m_MenuCheked.Chart_GridFine);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Chart_SmithChartRed(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Chart_SmithChartRed(HWND hDlg)
    {
        m_MenuCheked.Chart_SmithChartRed ^= true;
        FormSetCheckBox(hDlg, IDM_CHARTSMITH_RED, m_MenuCheked.Chart_SmithChartRed);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Chart_SmithChartBlue(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Chart_SmithChartBlue(HWND hDlg)
    {
        m_MenuCheked.Chart_SmithChartBlue ^= true;
        FormSetCheckBox(hDlg, IDM_CHARTSMITH_BLUE, m_MenuCheked.Chart_SmithChartBlue);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Chart_SmithChartImage(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Chart_SmithChartImage(HWND hDlg)
    {
        m_MenuCheked.Chart_SmithChartImage ^= true;
        FormSetCheckBox(hDlg, IDM_CHARTSMITH_IMAGE, m_MenuCheked.Chart_SmithChartImage);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Chart_SmithChartImageRedBlue(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Chart_SmithChartImageRedBlue(HWND hDlg)
    {
        m_MenuCheked.Chart_SmithChartImageRedBlue ^= true;
        FormSetCheckBox(hDlg, IDM_CHARTSMITH_IMAGE_REDBLUE, m_MenuCheked.Chart_SmithChartImageRedBlue);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Chart_Oscilloscope(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Chart_Oscilloscope(HWND hDlg)
    {
        m_MenuCheked.Chart_Oscilloscope ^= true;
        FormSetCheckBox(hDlg, IDM_OSCILLOSCOPE, m_MenuCheked.Chart_Oscilloscope);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Chart_Oscilloscope_S11_S21(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Chart_Oscilloscope_S11_S21(HWND hDlg)
    {
        m_MenuCheked.Chart_Oscilloscope_S11_S21 ^= true;
        FormSetCheckBox(hDlg, IDM_OSCILLOSCOPE_S11_S21, m_MenuCheked.Chart_Oscilloscope_S11_S21);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_TraceS11_LogMag(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_TraceS11_LogMag(HWND hDlg)
    {
        m_MenuCheked.TraceS11_LogMag^= true;
        FormSetCheckBox(hDlg, IDM_S11_LOGMAG, m_MenuCheked.TraceS11_LogMag);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_TraceS11_LinearMag(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_TraceS11_LinearMag(HWND hDlg)
    {
        m_MenuCheked.TraceS11_LinearMag ^= true;
        FormSetCheckBox(hDlg, IDM_S11_LINEARMAG, m_MenuCheked.TraceS11_LinearMag);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_TraceS11_SWR(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_TraceS11_SWR(HWND hDlg)
    {
        m_MenuCheked.TraceS11_SWR ^= true;
        FormSetCheckBox(hDlg, IDM_S11_SWR, m_MenuCheked.TraceS11_SWR);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_TraceS11_Resitance(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_TraceS11_Resitance(HWND hDlg)
    {
        m_MenuCheked.TraceS11_Resitance ^= true;
        FormSetCheckBox(hDlg, IDM_S11_RESITANCE, m_MenuCheked.TraceS11_Resitance);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_TraceS11_Reactance(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_TraceS11_Reactance(HWND hDlg)
    {
        m_MenuCheked.TraceS11_Reactance ^= true;
        FormSetCheckBox(hDlg, IDM_S11_REACTANCE, m_MenuCheked.TraceS11_Reactance);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_TraceS11_Phase(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_TraceS11_Phase(HWND hDlg)
    {
        m_MenuCheked.TraceS11_Phase ^= true;
        FormSetCheckBox(hDlg, IDM_S11_PHASE, m_MenuCheked.TraceS11_Phase);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_TraceS21_LogMag(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_TraceS21_LogMag(HWND hDlg)
    {
        m_MenuCheked.TraceS21_LogMag ^= true;
        FormSetCheckBox(hDlg, IDM_S21_LOGMAG, m_MenuCheked.TraceS21_LogMag);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_TraceS21_LinearMag(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_TraceS21_LinearMag(HWND hDlg)
    {
        m_MenuCheked.TraceS21_LinearMag ^= true;
        FormSetCheckBox(hDlg, IDM_S21_LINEARMAG, m_MenuCheked.TraceS21_LinearMag);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_TraceS21_Phase(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_TraceS21_Phase(HWND hDlg)
    {
        m_MenuCheked.TraceS21_Phase ^= true;
        FormSetCheckBox(hDlg, IDM_S21_PHASE, m_MenuCheked.TraceS21_Phase);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Marker_Red_Enable(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Marker_Red_Enable(HWND hDlg)
    {
        m_MenuCheked.Marker_Red_Enable ^= true;
        mc_OpenGLChart.m_Marker.Trace[1].Enable = m_MenuCheked.Marker_Red_Enable;
        FormSetCheckBox(hDlg, IDM_MARKER_RED_ENABLE, m_MenuCheked.Marker_Red_Enable);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Marker_Red_Set(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Marker_Red_Set(HWND hDlg)
    {
        m_MenuCheked.Marker_Red_Enable = true;
        mc_OpenGLChart.m_Marker.Trace[1].Enable = m_MenuCheked.Marker_Red_Enable;
        FormSetCheckBox(hDlg, IDM_MARKER_RED_ENABLE, m_MenuCheked.Marker_Red_Enable);
        mc_OpenGLChart.m_Marker.Trace[1].Index = mc_OpenGLChart.m_Marker.Trace[0].Index;
        mc_OpenGLChart.m_Marker.Trace[1].Frequencies = mc_OpenGLChart.m_Marker.Trace[0].Frequencies;
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Marker_Green_Enable(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Marker_Green_Enable(HWND hDlg)
    {
        m_MenuCheked.Marker_Green_Enable ^= true;
        mc_OpenGLChart.m_Marker.Trace[2].Enable = m_MenuCheked.Marker_Green_Enable;
        FormSetCheckBox(hDlg, IDM_MARKER_GREEN_ENABLE, m_MenuCheked.Marker_Green_Enable);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Marker_Green_Set(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Marker_Green_Set(HWND hDlg)
    {
        m_MenuCheked.Marker_Green_Enable = true;
        mc_OpenGLChart.m_Marker.Trace[2].Enable = m_MenuCheked.Marker_Green_Enable;
        FormSetCheckBox(hDlg, IDM_MARKER_GREEN_ENABLE, m_MenuCheked.Marker_Green_Enable);
        mc_OpenGLChart.m_Marker.Trace[2].Index = mc_OpenGLChart.m_Marker.Trace[0].Index;
        mc_OpenGLChart.m_Marker.Trace[2].Frequencies = mc_OpenGLChart.m_Marker.Trace[0].Frequencies;
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Marker_Blue_Enable(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Marker_Blue_Enable(HWND hDlg)
    {
        m_MenuCheked.Marker_Blue_Enable ^= true;
        mc_OpenGLChart.m_Marker.Trace[3].Enable = m_MenuCheked.Marker_Blue_Enable;
        FormSetCheckBox(hDlg, IDM_MARKER_BLUE_ENABLE, m_MenuCheked.Marker_Blue_Enable);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Marker_Blue_Set(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Marker_Blue_Set(HWND hDlg)
    {
        m_MenuCheked.Marker_Blue_Enable = true;
        mc_OpenGLChart.m_Marker.Trace[3].Enable = m_MenuCheked.Marker_Blue_Enable;
        FormSetCheckBox(hDlg, IDM_MARKER_BLUE_ENABLE, m_MenuCheked.Marker_Blue_Enable);
        mc_OpenGLChart.m_Marker.Trace[3].Index = mc_OpenGLChart.m_Marker.Trace[0].Index;
        mc_OpenGLChart.m_Marker.Trace[3].Frequencies = mc_OpenGLChart.m_Marker.Trace[0].Frequencies;
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Marker_Max_Enable(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Marker_Max_Enable(HWND hDlg)
    {
        m_MenuCheked.Marker_Max_Enable ^= true;
        mc_OpenGLChart.m_Marker.Trace[4].Enable = m_MenuCheked.Marker_Max_Enable;
        FormSetCheckBox(hDlg, IDM_MARKER_MAX_ENABLE, m_MenuCheked.Marker_Max_Enable);
        mc_OpenGLChart.Calculate_Marker_MinMax();
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Marker_Min_Enable(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Marker_Min_Enable(HWND hDlg)
    {
        m_MenuCheked.Marker_Min_Enable ^= true;
        mc_OpenGLChart.m_Marker.Trace[5].Enable = m_MenuCheked.Marker_Min_Enable;
        FormSetCheckBox(hDlg, IDM_MARKER_MIN_ENABLE, m_MenuCheked.Marker_Min_Enable);
        mc_OpenGLChart.Calculate_Marker_MinMax();
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Filter_Average(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Filter_Average(HWND hDlg)
    {
        m_MenuCheked.Filter_Average ^= true;
        FormSetCheckBox(hDlg, IDM_AVERAGE_FILTER, m_MenuCheked.Filter_Average);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Filter_Noise(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Filter_Noise(HWND hDlg)
    {
        m_MenuCheked.Filter_Noise ^= true;
        FormSetCheckBox(hDlg, IDM_NOISE_FILTER, m_MenuCheked.Filter_Noise);
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_Recall(HWND hDlg, UINT32 MenuItem)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_Recall(HWND hDlg, UINT32 MenuItem)
    {
        int RecallItem = 0;

        FormSetCheckBox(hDlg, IDM_RECALL0, false);
        FormSetCheckBox(hDlg, IDM_RECALL1, false);
        FormSetCheckBox(hDlg, IDM_RECALL2, false);
        FormSetCheckBox(hDlg, IDM_RECALL3, false);
        FormSetCheckBox(hDlg, IDM_RECALL4, false);
        FormSetCheckBox(hDlg, MenuItem, true);
        RecallItem = MenuItem - IDM_RECALL0;
        mc_NanoVNA.m_VNAMultiData.Stop = true;
        mc_OpenGLChart.m_GetVNARecall.Type = RecallItem;
        mc_OpenGLChart.m_GetVNARecall.Update = true;
    }
    //-Public----------------------------------------------------------------------
    // Name: OnMenu_ColourStettings(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::OnMenu_ColourStettings(HWND hDlg)
    {
        if (Colour_Select(hDlg, &m_Colours)) {
            mc_OpenGLChart.Calculate_Grid(&mc_OpenGLChart.m_GridData);
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: Filename_Open(const char* filter , HWND hwnd)
    //-----------------------------------------------------------------------------
    std::string MC_Forms::Filename_Open(const char* filter, HWND hwnd)
    {
        OPENFILENAMEA OpenFileDialog;
        char fileName[MAX_PATH] = "";
        string fileNameStr;

        ZeroMemory(&OpenFileDialog, sizeof(OpenFileDialog));
        ZeroMemory(&fileName, sizeof(fileName));
        fileName[0] = 0;
        OpenFileDialog.lStructSize = sizeof(OPENFILENAMEA);
        OpenFileDialog.hwndOwner = hwnd;
        OpenFileDialog.lpstrFilter = filter;
        OpenFileDialog.lpstrFile = fileName;
        OpenFileDialog.nMaxFile = MAX_PATH;
        OpenFileDialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        OpenFileDialog.lpstrDefExt = "Dtxt";
        OpenFileDialog.lpstrTitle = { "Open File" };
        if (GetOpenFileNameA(&OpenFileDialog)) {
            fileNameStr = fileName;
        }
        return fileNameStr;
    }
    //-Public----------------------------------------------------------------------
    // Name: Filename_Save(const char* filter, HWND hwnd)
    //-----------------------------------------------------------------------------
    std::string MC_Forms::Filename_Save(const char* filter, HWND hwnd)
    {
        OPENFILENAMEA SaveFileDialog;
        char fileName[MAX_PATH];
        string fileNameStr;

        ZeroMemory(&SaveFileDialog, sizeof(SaveFileDialog));
        ZeroMemory(&fileName, sizeof(fileName));
        fileName[0] = 0;
        SaveFileDialog.lStructSize = sizeof(OPENFILENAMEA);
        SaveFileDialog.hwndOwner = hwnd;
        SaveFileDialog.lpstrFilter = filter;
        SaveFileDialog.lpstrFile = fileName;
        SaveFileDialog.nMaxFile = MAX_PATH;
        SaveFileDialog.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
        SaveFileDialog.lpstrDefExt = "txt";
        SaveFileDialog.lpstrTitle = { "Save File" };
        if (GetSaveFileNameA(&SaveFileDialog)) {
            fileNameStr = fileName;
        }
        return fileNameStr;
    }
    //-Public----------------------------------------------------------------------
    // Name: FormFrequencySweep_Init()
    //-----------------------------------------------------------------------------
    void MC_Forms::FormFrequencySweep_Init()
    {
        m_FrequencySweep.StartFrequency = _T("50000");
        m_FrequencySweep.StopFrequency = _T("900000000");
        m_FrequencySweep.CenterFrequency = _T("449975000");
        m_FrequencySweep.SpanFrequency = _T("899950000");
        m_FrequencySweep.xSampleRate = _T("1");
        m_FrequencySweep.FrequencyInfo = _T("Frequency Steps 8999500 Hz");
        m_FrequencySweep.xSpinSampleRate = FORM_SPIN_MIN_POS;
        m_FrequencySweep.TotalSamples = FORM_VNA_SAMPLE_PERSACN;
        m_FrequencySweep.Update = false;

        m_FrequencySweep.Sweep.StartFrequency = 50000;
        m_FrequencySweep.Sweep.StopFrequency = 900000000;
        m_FrequencySweep.Sweep.CenterFrequency = 449975000;
        m_FrequencySweep.Sweep.SpanFrequency = 899950000;
        m_FrequencySweep.Sweep.xSampleRate = FORM_SPIN_MIN_POS;
        m_FrequencySweep.Sweep.TotalSamples = FORM_VNA_SAMPLE_PERSACN;
    }
    // -Public---------------------------------------------------------------------
    // Name: FormFrequencySweep_SetData(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::FormFrequencySweep_SetData(HWND hDlg)
    {
        HWND hDlgItem = NULL;
        std::wstring wstr;
        std::string str;

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSTART);
        wstr = m_FrequencySweep.StartFrequency;
        Edit_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSTOP);
        wstr = m_FrequencySweep.StopFrequency;
        Edit_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQCENTER);
        wstr = m_FrequencySweep.CenterFrequency;
        Edit_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSPAN);
        wstr = m_FrequencySweep.SpanFrequency;
        Edit_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_SAMPLERATE);
        wstr = m_FrequencySweep.xSampleRate;
        Edit_SetText(hDlgItem, wstr.c_str());
        str = wstring_to_utf8(wstr);
        m_FrequencySweep.xSpinSampleRate = std::atoi(str.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_TOTALSAMPLES);
        m_FrequencySweep.TotalSamples = m_FrequencySweep.xSpinSampleRate * FORM_VNA_SAMPLE_PERSACN;
        str = std::to_string(m_FrequencySweep.TotalSamples);
        wstr = utf8_to_wstring(str);
        Edit_SetText(hDlgItem, wstr.c_str());
        FormFrequencySweep_CalculateSteps(hDlg, m_FrequencySweep.xSpinSampleRate);
        m_FrequencySweep.Update = false;
    }
    // -Public---------------------------------------------------------------------
    // Name: FormFrequencySweep_GetData(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::FormFrequencySweep_GetData(HWND hDlg)
    {
        HWND hDlgItem = NULL;
        WCHAR lpString[256];
        std::wstring wstr;
        std::string str;

        memset(&lpString, 0, sizeof(lpString));

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSTART);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        m_FrequencySweep.StartFrequency = wstr;

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSTOP);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        m_FrequencySweep.StopFrequency = wstr;

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQCENTER);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        m_FrequencySweep.CenterFrequency = wstr;

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSPAN);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        m_FrequencySweep.SpanFrequency = wstr;

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_SAMPLERATE);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        m_FrequencySweep.xSampleRate = wstr;
        str = wstring_to_utf8(wstr);
        m_FrequencySweep.xSpinSampleRate = std::atoi(str.c_str());
        m_FrequencySweep.TotalSamples = m_FrequencySweep.xSpinSampleRate * FORM_VNA_SAMPLE_PERSACN;

        hDlgItem = GetDlgItem(hDlg, IDC_LABLE_FREQSTEPS);
        Static_GetText(hDlgItem, lpString, 255);
        wstr = lpString;
        m_FrequencySweep.FrequencyInfo = wstr;

        str = wstring_to_utf8(m_FrequencySweep.StartFrequency);
        m_FrequencySweep.Sweep.StartFrequency = std::atoi(str.c_str());
        str = wstring_to_utf8(m_FrequencySweep.StopFrequency);
        m_FrequencySweep.Sweep.StopFrequency = std::atoi(str.c_str());
        str = wstring_to_utf8(m_FrequencySweep.CenterFrequency);
        m_FrequencySweep.Sweep.CenterFrequency = std::atoi(str.c_str());
        str = wstring_to_utf8(m_FrequencySweep.SpanFrequency);
        m_FrequencySweep.Sweep.SpanFrequency = std::atoi(str.c_str());
        str = wstring_to_utf8(m_FrequencySweep.xSampleRate);
        m_FrequencySweep.Sweep.xSampleRate = std::atoi(str.c_str());
        m_FrequencySweep.Sweep.TotalSamples = m_FrequencySweep.TotalSamples;

        mc_OpenGLChart.m_X10Samples = float(m_FrequencySweep.Sweep.xSampleRate);
        mc_OpenGLChart.m_NumberOfSamples = (100.0f * mc_OpenGLChart.m_X10Samples) + 1.0f;
        if (mc_OpenGLChart.m_Marker.Trace[0].Index < 0) {
            mc_OpenGLChart.m_Marker.Trace[0].Index = 0;
        } else if (mc_OpenGLChart.m_Marker.Trace[0].Index > 100.0f * mc_OpenGLChart.m_X10Samples) {
            mc_OpenGLChart.m_Marker.Trace[0].Index = int(100.0f * mc_OpenGLChart.m_X10Samples);
        }
        mc_OpenGLChart.Calculate_Trace_All(&mc_OpenGLChart.m_GridData);
        m_FrequencySweep.Update = true;
    }
    //-Public----------------------------------------------------------------------
    // Name: FormFrequencySweep_CalculateSweep(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::FormFrequencySweep_CalculateSweep(HWND hDlg)
    {
        HWND hDlgItem = NULL;
        WCHAR lpString[256];
        char TmpBuffer[256];
        std::wstring wstr;
        std::string str;
        uint32_t StartFrequency;
        uint32_t StopFrequency;
        uint32_t CenterFrequency;
        uint32_t SpanFrequency;
        int32_t xSampleRate;
        int32_t StepFrequency;

        memset(&lpString, 0, sizeof(lpString));
        memset(&TmpBuffer, 0, sizeof(TmpBuffer));

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSTART);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        str = wstring_to_utf8(wstr);
        StartFrequency = std::atol(str.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSTOP);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        str = wstring_to_utf8(wstr);
        StopFrequency = std::atol(str.c_str());

        SpanFrequency = (StopFrequency - StartFrequency);
        CenterFrequency = (StartFrequency + (SpanFrequency / 2));

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQCENTER);
        str = std::to_string(CenterFrequency);
        wstr = utf8_to_wstring(str);
        Edit_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSPAN);
        str = std::to_string(SpanFrequency);
        wstr = utf8_to_wstring(str);
        Edit_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_SAMPLERATE);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        str = wstring_to_utf8(wstr);
        xSampleRate = std::atoi(str.c_str());
        if (xSampleRate < FORM_SPIN_MIN_POS) {
            xSampleRate = FORM_SPIN_MIN_POS;
        } else if(xSampleRate > FORM_SPIN_MAX_POS) {
            xSampleRate = FORM_SPIN_MAX_POS;
        }
        StepFrequency = (SpanFrequency / (xSampleRate * FORM_VNA_SAMPLE_PERSACN));
        sprintf_s(TmpBuffer, sizeof(TmpBuffer), "Frequency Steps %ld Hz", StepFrequency);
        str = TmpBuffer;
        wstr = utf8_to_wstring(str);
        hDlgItem = GetDlgItem(hDlg, IDC_LABLE_FREQSTEPS);
        Static_SetText(hDlgItem, wstr.c_str());
    }
    //-Public----------------------------------------------------------------------
    // Name: FormFrequencySweep_CalculateSpan(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::FormFrequencySweep_CalculateSpan(HWND hDlg)
    {
        HWND hDlgItem = NULL;
        WCHAR lpString[256];
        char TmpBuffer[256];
        std::wstring wstr;
        std::string str;
        uint32_t StartFrequency;
        uint32_t StopFrequency;
        uint32_t CenterFrequency;
        uint32_t SpanFrequency;
        int32_t xSampleRate;
        int32_t StepFrequency;

        memset(&lpString, 0, sizeof(lpString));
        memset(&TmpBuffer, 0, sizeof(TmpBuffer));

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQCENTER);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        str = wstring_to_utf8(wstr);
        CenterFrequency = std::atol(str.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSPAN);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        str = wstring_to_utf8(wstr);
        SpanFrequency = std::atol(str.c_str());

        StartFrequency = (CenterFrequency - (SpanFrequency / 2));
        StopFrequency = (CenterFrequency + (SpanFrequency / 2));

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSTART);
        str = std::to_string(StartFrequency);
        wstr = utf8_to_wstring(str);
        Edit_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSTOP);
        str = std::to_string(StopFrequency);
        wstr = utf8_to_wstring(str);
        Edit_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_SAMPLERATE);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        str = wstring_to_utf8(wstr);
        xSampleRate = std::atoi(str.c_str());
        if (xSampleRate < FORM_SPIN_MIN_POS) {
            xSampleRate = FORM_SPIN_MIN_POS;
        } else if (xSampleRate > FORM_SPIN_MAX_POS) {
            xSampleRate = FORM_SPIN_MAX_POS;
        }
        StepFrequency = (SpanFrequency / (xSampleRate * FORM_VNA_SAMPLE_PERSACN));
        sprintf_s(TmpBuffer, sizeof(TmpBuffer), "Frequency Steps %ld Hz", StepFrequency);
        str = TmpBuffer;
        wstr = utf8_to_wstring(str);
        hDlgItem = GetDlgItem(hDlg, IDC_LABLE_FREQSTEPS);
        Static_SetText(hDlgItem, wstr.c_str());
    }
    //-Public----------------------------------------------------------------------
    // Name: FormFrequencySweep_CalculateSteps(HWND hDlg, int Value)
    //-----------------------------------------------------------------------------
    void MC_Forms::FormFrequencySweep_CalculateSteps(HWND hDlg, int Value)
    {
        HWND hDlgItem = NULL;
        WCHAR lpString[256];
        char TmpBuffer[256];
        std::wstring wstr;
        std::string str;
        uint32_t StartFrequency;
        uint32_t StopFrequency;
        uint32_t SpanFrequency;
        int32_t xSampleRate;
        int32_t StepFrequency;
        int TotalSamples;

        memset(&lpString, 0, sizeof(lpString));
        memset(&TmpBuffer, 0, sizeof(TmpBuffer));

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSTART);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        str = wstring_to_utf8(wstr);
        StartFrequency = std::atol(str.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_FREQSTOP);
        Edit_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        str = wstring_to_utf8(wstr);
        StopFrequency = std::atol(str.c_str());
        SpanFrequency = (StopFrequency - StartFrequency);

        hDlgItem = GetDlgItem(hDlg, IDC_EDIT_SAMPLERATE);
        xSampleRate = Value;
        str = std::to_string(xSampleRate);
        wstr = utf8_to_wstring(str);
        Edit_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_TOTALSAMPLES);
        TotalSamples = xSampleRate * FORM_VNA_SAMPLE_PERSACN;
        str = std::to_string(TotalSamples);
        wstr = utf8_to_wstring(str);
        Edit_SetText(hDlgItem, wstr.c_str());

        StepFrequency = (SpanFrequency / (xSampleRate * FORM_VNA_SAMPLE_PERSACN));
        sprintf_s(TmpBuffer, sizeof(TmpBuffer), "Frequency Steps %ld Hz", StepFrequency);
        str = TmpBuffer;
        wstr = utf8_to_wstring(str);
        hDlgItem = GetDlgItem(hDlg, IDC_LABLE_FREQSTEPS);
        Static_SetText(hDlgItem, wstr.c_str());
    }
    //-Public----------------------------------------------------------------------
    // Name: FormSerialPort_Init()
    //-----------------------------------------------------------------------------
    void MC_Forms::FormSerialPort_Init()
    {
        m_SerialPort.SerialPort = _T("COM6");
        m_SerialPort.BaudRate = _T("115200");
        m_SerialPort.DataBits = _T("8");
        m_SerialPort.StopBits = _T("2");
        m_SerialPort.ParityBit = _T("None");
        m_SerialPort.RTS = 0;
        m_SerialPort.DTR = 0;
        m_SerialPort.Connect = m_MenuCheked.ConnectDisconnect;
        m_SerialPort.Update = false;
    }
    // -Public---------------------------------------------------------------------
    // Name: FormSerialPort_SetData(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::FormSerialPort_SetData(HWND hDlg)
    {
        HWND hDlgItem = NULL;
        std::wstring wstr;

        FormSerialPort_RefreshData(hDlg);
        hDlgItem = GetDlgItem(hDlg, IDC_COMBO_BAUDRATE);
        ComboBox_ResetContent(hDlgItem);

        wstr = _T("110");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("300");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("600");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("1200");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("2400");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("4800");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("9600");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("19200");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("38400");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("57600");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("115200");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("128000");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("256000");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("500000");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("921600");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("1000000");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("1500000");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("2000000");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("3000000");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = m_SerialPort.BaudRate;
        ComboBox_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_COMBO_DATABITS);
        ComboBox_ResetContent(hDlgItem);
        wstr = _T("8");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("7");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("6");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("5");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = m_SerialPort.DataBits;
        ComboBox_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_COMBO_STOPBITS);
        ComboBox_ResetContent(hDlgItem);
        wstr = _T("1");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("2");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = m_SerialPort.StopBits;
        ComboBox_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_COMBO_PARITYBIT);
        ComboBox_ResetContent(hDlgItem);
        wstr = _T("None");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("Odd");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("Even");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = m_SerialPort.ParityBit;
        ComboBox_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_CHECK_FLOW_RTS);
        Button_SetCheck(hDlgItem, m_SerialPort.RTS);

        hDlgItem = GetDlgItem(hDlg, IDC_CHECK_FLOW_DTR);
        Button_SetCheck(hDlgItem, m_SerialPort.DTR);

        m_SerialPort.Update = false;
    }
    // -Public---------------------------------------------------------------------
    // Name: FormSerialPort_GetData(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::FormSerialPort_GetData(HWND hDlg)
    {
        HWND hDlgItem = NULL;
        WCHAR lpString[256];
        std::wstring wstr;

        memset(&lpString, 0, sizeof(lpString));

        hDlgItem = GetDlgItem(hDlg, IDC_COMBO_SERIALPORT);
        ComboBox_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        m_SerialPort.SerialPort = wstr;

        hDlgItem = GetDlgItem(hDlg, IDC_COMBO_BAUDRATE);
        ComboBox_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        m_SerialPort.BaudRate = wstr;

        hDlgItem = GetDlgItem(hDlg, IDC_COMBO_DATABITS);
        ComboBox_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        m_SerialPort.DataBits = wstr;

        hDlgItem = GetDlgItem(hDlg, IDC_COMBO_STOPBITS);
        ComboBox_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        m_SerialPort.StopBits = wstr;

        hDlgItem = GetDlgItem(hDlg, IDC_COMBO_PARITYBIT);
        ComboBox_GetText(hDlgItem, lpString, 10);
        wstr = lpString;
        m_SerialPort.ParityBit = wstr;

        hDlgItem = GetDlgItem(hDlg, IDC_CHECK_FLOW_RTS);
        m_SerialPort.RTS = Button_GetCheck(hDlgItem);

        hDlgItem = GetDlgItem(hDlg, IDC_CHECK_FLOW_DTR);
        m_SerialPort.DTR = Button_GetCheck(hDlgItem);

        m_SerialPort.Update = true;
        mc_RS232.OnConnect();
    }
    // -Public---------------------------------------------------------------------
    // Name: FormSerialPort_RefreshData(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::FormSerialPort_RefreshData(HWND hDlg)
    {
        HWND hDlgItem = NULL;
        char lpTargetPath[5000];
        bool gotPort = false;
        int ItemCount = 0;
        std::string str;
        std::wstring wstr;

        memset(&lpTargetPath, 0, sizeof(lpTargetPath));

        hDlgItem = GetDlgItem(hDlg, IDC_COMBO_SERIALPORT);
        ItemCount = ComboBox_GetCount(hDlgItem);
        ComboBox_ResetContent(hDlgItem);
        for (int i = 0; i < 255; i++) {
            str = "COM" + std::to_string(i);
            DWORD test = QueryDosDeviceA(str.c_str(), lpTargetPath, 5000);
            if (test != 0) {
                wstr = utf8_to_wstring(str);
                ComboBox_AddItemData(hDlgItem, wstr.c_str());
                gotPort = true;
            }
            if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {

            }
        }
        wstr = m_SerialPort.SerialPort;
        ComboBox_SetText(hDlgItem, wstr.c_str());
    }
    // -Public---------------------------------------------------------------------
    // Name: FormCalibration_SetData(HWND hDlg)
    //-----------------------------------------------------------------------------
    void MC_Forms::FormCalibration_SetData(HWND hDlg)
    {
        HWND hDlgItem = NULL;
        std::wstring wstr;

        hDlgItem = GetDlgItem(hDlg, IDC_COMBO_CAL_SAVE);
        ComboBox_ResetContent(hDlgItem);
        wstr = _T("0");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("1");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("2");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("3");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("4");
        ComboBox_AddItemData(hDlgItem, wstr.c_str());
        wstr = _T("0");
        ComboBox_SetText(hDlgItem, wstr.c_str());

        hDlgItem = GetDlgItem(hDlg, IDC_CAL_OPEN);
        Button_Enable(hDlgItem, false);
        hDlgItem = GetDlgItem(hDlg, IDC_CAL_SHORT);
        Button_Enable(hDlgItem, false);
        hDlgItem = GetDlgItem(hDlg, IDC_CAL_LOAD);
        Button_Enable(hDlgItem, false);
        hDlgItem = GetDlgItem(hDlg, IDC_CAL_ISOLN);
        Button_Enable(hDlgItem, false);
        hDlgItem = GetDlgItem(hDlg, IDC_CAL_THRU);
        Button_Enable(hDlgItem, false);
        hDlgItem = GetDlgItem(hDlg, IDC_CAL_DONE);
        Button_Enable(hDlgItem, false);
        hDlgItem = GetDlgItem(hDlg, IDC_CAL_SAVE);
        Button_Enable(hDlgItem, false);
        hDlgItem = GetDlgItem(hDlg, IDC_COMBO_CAL_SAVE);
        ComboBox_Enable(hDlgItem, false);
        hDlgItem = GetDlgItem(hDlg, IDOK);
        Button_Enable(hDlgItem, false);
        hDlgItem = GetDlgItem(hDlg, IDCANCEL);
        ComboBox_Enable(hDlgItem, true);
    }
    // -Public---------------------------------------------------------------------
    // Name: FormCalibration_Buttons(HWND hDlg, UINT Button)
    //-----------------------------------------------------------------------------
    void MC_Forms::FormCalibration_Buttons(HWND hDlg, UINT Button)
    {
        HWND hDlgItem = NULL;
        WCHAR lpString[256];
        char Buffer[256];
        std::string str;
        std::wstring wstr;

        memset(&lpString, 0, sizeof(lpString));
        memset(&Buffer, 0, sizeof(Buffer));
        switch (Button) {
            case IDC_CAL_RESET: {
                FormCalibration_SetData(hDlg);
                mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, "cal reset\r\n");
            }
            break;
            case IDC_CAL_CALIBRATION: {
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_OPEN);
                Button_Enable(hDlgItem, true);

            }
            break;
            case IDC_CAL_OPEN: {
                mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, "cal open\r\n");
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_OPEN);
                Button_Enable(hDlgItem, false);
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_SHORT);
                Button_Enable(hDlgItem, true);
            }
            break;
            case IDC_CAL_SHORT: {
                mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, "cal short\r\n");
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_SHORT);
                Button_Enable(hDlgItem, false);
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_LOAD);
                Button_Enable(hDlgItem, true);
            }
            break;
            case IDC_CAL_LOAD: {
                mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, "cal load\r\n");
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_LOAD);
                Button_Enable(hDlgItem, false);
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_ISOLN);
                Button_Enable(hDlgItem, true);
            }
            break;
            case IDC_CAL_ISOLN: {
                mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, "cal isoln\r\n");
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_ISOLN);
                Button_Enable(hDlgItem, false);
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_THRU);
                Button_Enable(hDlgItem, true);
            }
            break;
            case IDC_CAL_THRU: {
                mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, "cal thru\r\n");
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_THRU);
                Button_Enable(hDlgItem, false);
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_DONE);
                Button_Enable(hDlgItem, true);
            }
            break;
            case IDC_CAL_DONE: {
                mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, "cal done\r\n");
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_DONE);
                Button_Enable(hDlgItem, false);
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_SAVE);
                Button_Enable(hDlgItem, true);
                hDlgItem = GetDlgItem(hDlg, IDC_COMBO_CAL_SAVE);
                ComboBox_Enable(hDlgItem, true);
            }
            break;
            case IDC_CAL_SAVE: {
                hDlgItem = GetDlgItem(hDlg, IDC_COMBO_CAL_SAVE);
                ComboBox_GetText(hDlgItem, lpString, 10);
                wstr = lpString;
                str = wstring_to_utf8(wstr);
                sprintf_s(Buffer, sizeof(Buffer), "save %s\r\n", str.c_str());
                mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, Buffer);
                hDlgItem = GetDlgItem(hDlg, IDC_CAL_SAVE);
                Button_Enable(hDlgItem, false);
                hDlgItem = GetDlgItem(hDlg, IDC_COMBO_CAL_SAVE);
                ComboBox_Enable(hDlgItem, false);
                hDlgItem = GetDlgItem(hDlg, IDCANCEL);
                ComboBox_Enable(hDlgItem, false);
                hDlgItem = GetDlgItem(hDlg, IDOK);
                Button_Enable(hDlgItem, true);
            }
            break;
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: Colour_Init()
    //-----------------------------------------------------------------------------
    void MC_Forms::Colour_Init()
    {
        int i;

        ZeroMemory(&m_Colours, sizeof(m_Colours));
        m_Colours.ColourResult = 0x00FFFFFF;

        m_Colours.Colourfloat[M_COLOUR_White] = { 1.0f, 1.0f, 1.0f };                           // White

        m_Colours.Colourfloat[M_COLOUR_Grid10] = { 0.0f, 0.5f, 0.0f };                          // Grid 10
        m_Colours.Colourfloat[M_COLOUR_Grid1] = { 0.0f, 0.15f, 0.0f };                          // Grid 1

        m_Colours.Colourfloat[M_COLOUR_S11_Swr] = { 1.0f, 0.5f, 0.0f };                         // S11_Swr
        m_Colours.Colourfloat[M_COLOUR_S11_Phase] = { 0.5f, 1.0f, 0.5f };                       // S11_Phase
        m_Colours.Colourfloat[M_COLOUR_S11_Reactance] = { 1.0f, 0.0f, 0.0f };                   // S11_Reactance
        m_Colours.Colourfloat[M_COLOUR_S11_Resitance] = { 0.0f, 1.0f, 1.0f };                   // S11_Resitance
        m_Colours.Colourfloat[M_COLOUR_S11_LogMag] = { 1.0f, 1.0f, 0.0f };                      // S11_LogMag
        m_Colours.Colourfloat[M_COLOUR_S11_LinMag] = { 1.0f, 1.0f, 0.0f };                      // S11_LinMag

        m_Colours.Colourfloat[M_COLOUR_S21_LogMag] = { 1.0f, 0.0f, 1.0f };                      // S21_LogMag
        m_Colours.Colourfloat[M_COLOUR_S21_LinMag] = { 1.0f, 0.0f, 1.0f };                      // S21_LinMag
        m_Colours.Colourfloat[M_COLOUR_S21_Phase] = { 1.0f, 0.5f, 1.0f };                       // S21_Phase

        m_Colours.Colourfloat[M_COLOUR_Smith_Red] = { 0.75f, 0.25f, 0.25f };                    // Smith_Red
        m_Colours.Colourfloat[M_COLOUR_Smith_Blue] = { 0.0f , 0.35f, 0.75f };                   // Smith_Blue
        m_Colours.Colourfloat[M_COLOUR_Smith_Trace] = { 0.0f , 1.0f , 0.0f };                   // Smith_Trace

        m_Colours.Colourfloat[M_COLOUR_Black] = { 0.0f , 0.0f , 0.0f };                         // Black

        for (i = 0; i < FORM_MAX_COLOURS; i++) {
            m_Colours.ColourDWord[i] = ConvertRGBToDword(&m_Colours.Colourfloat[i]);
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: Colour_Select(HWND hwnd, FORM_COLOURSELECT* Colours)
    //-----------------------------------------------------------------------------
    bool MC_Forms::Colour_Select(HWND hwnd, FORM_COLOURSELECT* Colours)
    {
        int i;
        bool Result = false;
        CHOOSECOLOR color;

        ZeroMemory(&color, sizeof(color));
        color.lStructSize = sizeof(CHOOSECOLOR);
        color.hwndOwner = hwnd;
        color.lpCustColors = Colours->ColourDWord;
        color.rgbResult = Colours->ColourResult;
        color.Flags = CC_RGBINIT | CC_FULLOPEN;
        if (ChooseColor(&color)) {
            Colours->ColourResult = color.rgbResult;
            for (i = 0; i < FORM_MAX_COLOURS; i++) {
                Colours->ColourDWord[i] = color.lpCustColors[i];
                Colours->Colourfloat[i] = ConvertDwordToRGB(Colours->ColourDWord[i]);
            }
            Result = true;
        }
        return Result;
    }
    //-Public----------------------------------------------------------------------
    // Name: ConvertRGBToDword(FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    COLORREF MC_Forms::ConvertRGBToDword(FORM_COLOUR* Colour)
    {
        FORM_COLOR4B  Dw_Colour;

        Dw_Colour.Dword.r = unsigned char(Colour->r * 255.0f);
        Dw_Colour.Dword.g = unsigned char(Colour->g * 255.0f);
        Dw_Colour.Dword.b = unsigned char(Colour->b * 255.0f);
        Dw_Colour.Dword.a = 0;
        return(Dw_Colour.DwColour);
    }
    //-Public----------------------------------------------------------------------
    // Name: ConvertDwordToRGB(COLORREF DwColour)
    //-----------------------------------------------------------------------------
    FORM_COLOUR MC_Forms::ConvertDwordToRGB(COLORREF DwColour)
    {
        FORM_COLOR4B  Dw_Colour;
        FORM_COLOUR Colour;

        Dw_Colour.DwColour = DwColour;
        Colour.r = float(Dw_Colour.Dword.r) / 255.0f;
        Colour.g = float(Dw_Colour.Dword.g) / 255.0f;
        Colour.b = float(Dw_Colour.Dword.b) / 255.0f;
        return(Colour);
    }
    //-Public----------------------------------------------------------------------
    // Name: utf8_to_wstring(const std::string& str) 
    //-----------------------------------------------------------------------------
    std::wstring MC_Forms::utf8_to_wstring(const std::string& str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        return myconv.from_bytes(str);
    }
    //-Public----------------------------------------------------------------------
    // Name: wstring_to_utf8(const std::wstring& str)
    //-----------------------------------------------------------------------------
    std::string MC_Forms::wstring_to_utf8(const std::wstring& str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        return myconv.to_bytes(str);
    }
    //-Public----------------------------------------------------------------------
    // Name: UpdateSweepStrings()
    //-----------------------------------------------------------------------------
    void MC_Forms::UpdateSweepStrings()
    {
        std::wstring wstr;
        std::string str;

        str = std::to_string(m_FrequencySweep.Sweep.StartFrequency);
        m_FrequencySweep.StartFrequency = utf8_to_wstring(str);
        str = std::to_string(m_FrequencySweep.Sweep.StopFrequency);
        m_FrequencySweep.StopFrequency = utf8_to_wstring(str);
        str = std::to_string(m_FrequencySweep.Sweep.CenterFrequency);
        m_FrequencySweep.CenterFrequency = utf8_to_wstring(str);
        str = std::to_string(m_FrequencySweep.Sweep.SpanFrequency);
        m_FrequencySweep.SpanFrequency = utf8_to_wstring(str);
        str = std::to_string(m_FrequencySweep.Sweep.xSampleRate);
        m_FrequencySweep.xSampleRate = utf8_to_wstring(str);
        m_FrequencySweep.xSpinSampleRate = m_FrequencySweep.Sweep.xSampleRate;
        m_FrequencySweep.TotalSamples = m_FrequencySweep.Sweep.TotalSamples;
    }








    //*****************************************************************************  
    // Protected Code
    //*****************************************************************************

    //-Protected-------------------------------------------------------------------
    // Name: 
    //-----------------------------------------------------------------------------
    //-Public----------------------------------------------------------------------
    // Name: FormSetCheckBox(HWND hDlg,UINT uIDCheckItem, bool Value)
    //-----------------------------------------------------------------------------
    void MC_Forms::FormSetCheckBox(HWND hDlg,UINT uIDCheckItem, bool Value)
    {
        if (Value) {
            CheckMenuItem(GetMenu(hDlg), uIDCheckItem, MF_CHECKED);
        } else {
            CheckMenuItem(GetMenu(hDlg), uIDCheckItem, MF_UNCHECKED);
        }
    }
};

