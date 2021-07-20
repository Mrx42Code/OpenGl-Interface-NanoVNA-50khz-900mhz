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
// File: MC_OpenGLChart.cpp: implementation of the MC_OpenGLChart class.
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
    char m_ComPortStr[] = "COM1";
    char m_ComMode[] = { '8','N','2',0 };
    int m_ComBaudRate = 115200;
    const char* m_MarkerText[GL_MAX_MARKER] = { "Main", "Red", "Green", "Blue", "Max", "Min" };

    //-----------------------------------------------------------------------------
    // message handlers
    //-----------------------------------------------------------------------------

    //-Public----------------------------------------------------------------------
    // Name: MC_OpenGLChart()
    // Desc: MC_OpenGLChart class
    //-----------------------------------------------------------------------------
    MC_OpenGLChart::MC_OpenGLChart()
    {
        ZeroMemory(&m_StringBuffer, sizeof(m_StringBuffer));
        ZeroMemory(&m_GridInfo, sizeof(m_GridInfo));
        ZeroMemory(&m_GridData, sizeof(m_GridData));
        ZeroMemory(&m_Trace, sizeof(m_Trace));
        ZeroMemory(&m_Marker, sizeof(m_Marker));
        ZeroMemory(&m_GetVNAData, sizeof(m_GetVNAData));
        ZeroMemory(&m_GetVNADataType, sizeof(m_GetVNADataType));
        ZeroMemory(&m_GetVNARecall, sizeof(m_GetVNARecall));
        m_X10Samples = 1.0f;
        m_NumberOfSamples = (100.0f * m_X10Samples) + 1.0f;
        m_Marker.Trace[0].Enable = 1;
        m_Marker.Trace[0].Index = 0;
        m_Marker.Trace[0].Colour = { 1.0f, 1.0f, 1.0f };
        m_Marker.Trace[1].Colour = { 1.0f, 0.0f, 0.0f };
        m_Marker.Trace[2].Colour = { 0.0f, 1.0f, 0.0f };
        m_Marker.Trace[3].Colour = { 0.0f, 0.5f, 1.0f };
        m_Marker.Trace[4].Colour = { 0.0f, 1.0f, 1.0f };
        m_Marker.Trace[5].Colour = { 0.0f, 1.0f, 1.0f };
        mc_NanoVNA.m_Update = false;
        Init();
    }
    //-Public----------------------------------------------------------------------
    // Name: ~MC_OpenGLChart()
    // Desc: ~MC_OpenGLChart Destruction class
    //-----------------------------------------------------------------------------
    MC_OpenGLChart::~MC_OpenGLChart()
    {

    }
    //-Public----------------------------------------------------------------------
    // Name: Init()
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Init()
    {
    
    }
    //-Public----------------------------------------------------------------------
    // Name: Tick()
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Tick()
    {
        if (mc_RS232.m_info.ConnectionStatus) {
            if (m_GetVNARecall.Update) {
                m_GetVNARecall.Update = false;
                GetVNADataRecall(m_GetVNARecall.Type);
            }
            if (mc_Forms.m_MenuCheked.Auto_GetDataS11 || mc_Forms.m_MenuCheked.Auto_GetDataS21) {
                GetVNAData(mc_Forms.m_MenuCheked.Auto_GetDataS11, mc_Forms.m_MenuCheked.Auto_GetDataS21);
            }
            if (m_GetVNADataType.Update) {
                m_GetVNADataType.Update = false;
                GetVNADataVarType(m_GetVNADataType.Type);
            }
            if (m_GetVNAData.Update) {
                m_GetVNAData.Update = false;
                GetVNAData(m_GetVNAData.S11, m_GetVNAData.S21);
            }
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: DrawChart()
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart()
    {
        if (mc_Forms.m_MenuCheked.Chart_2D3D) {
            DrawChart_Scale(&m_GridData, GL_GRID_MAXROWS, GL_GRID_MAXCOLS, 0.17f, 0.17f * 0.5f);
        } else {
            DrawChart_Scale(&m_GridData, GL_GRID_MAXROWS, GL_GRID_MAXCOLS, 0.17f, 0.17f);
        }
        if (mc_NanoVNA.m_SweepDone) {
            mc_NanoVNA.m_SweepDone = false;
        }
        if (mc_NanoVNA.m_Update) {
            Calculate_Trace_All(&m_GridData);
            mc_NanoVNA.m_Update = false;
        }
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);
    
        DrawChart_Grid();
        DrawChart_TopeLine(&m_GridData, m_Marker.Trace[0].Index, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_White]);
        if (mc_Forms.m_MenuCheked.Chart_Oscilloscope_S11_S21) {
            mc_Oscilloscope.DrawOscilloscope(OSC_CHANNEL_2, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Resitance]);
        } else {
            mc_Oscilloscope.DrawOscilloscope(OSC_CHANNEL_1, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_White]);
        }
        DrawChart_TraceS11_Frequencies(&m_GridData, m_Marker.Trace[0].Index, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_White]);
        DrawChart_TraceS11_Swr(&m_Marker, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Swr]);
        DrawChart_TraceS11_Phase(&m_Marker, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Phase]);
        DrawChart_TraceS11_Reactance(&m_Marker, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Reactance]);
        DrawChart_TraceS11_Resitance(&m_Marker, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Resitance]);
        DrawChart_TraceS11_LogMag(&m_Marker, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_LogMag]);
        DrawChart_TraceS11_LinMag(&m_Marker, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_LinMag]);
        DrawChart_TraceS21_LogMag(&m_Marker, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S21_LogMag]);
        DrawChart_TraceS21_LinMag(&m_Marker, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S21_LinMag]);
        DrawChart_TraceS21_Phase(&m_Marker, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S21_Phase]);
        mc_OpenGLSmithChart.DrawChart(&m_GridData, &m_Marker);

        glEnable(GL_CULL_FACE);
        glEnable(GL_LIGHTING);

        DrawSceneRasterText();

    //    DrawCube();
    }
    //-Public----------------------------------------------------------------------
    // Name: DrawScreenRasterText()
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawSceneRasterText()
    {
        int Marker = 0;
        int MarkerItem;
        float OffSet = mc_OpenGL.m_ViewPort.height - 20.0f;

        for (MarkerItem = 0; MarkerItem < GL_MAX_MARKER; MarkerItem++) {
            if (m_Marker.Trace[MarkerItem].Enable) {
                Draw_MarkerText("Marker", &OffSet, MarkerItem, m_Marker.Trace[MarkerItem].Index);
            }
        }

    }
    //-Public----------------------------------------------------------------------
    // Name: DrawCube()
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawCube()
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
    //-Public----------------------------------------------------------------------
    // Name: OnCreate()
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::OnCreate()
    {
        mc_OpenGLSmithChart.Init();
        mc_OpenGLSmithChart.m_Enable = true;
    }
    //-Public----------------------------------------------------------------------
    // Name: FormCallBack(int control)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::FormCallBack(int control)
    {
        int i;
        bool gotPort = false;
        std::string str;
        DWORD test = 0;
        std::string StrPort = m_ComPortStr;

        switch (control) {
            case M_AverageClear: {
                for (i = 0; i < VNA_MAX_NumberOfSamples; i++) {
                    mc_NanoVNA.m_VNAData.Average_Complex_S11[i] = mc_NanoVNA.m_VNAData.LastComplex_S11[i];
                    mc_NanoVNA.m_VNAData.Average_Complex_S21[i] = mc_NanoVNA.m_VNAData.LastComplex_S21[i];
                }
            } break;
            default: {                                                                           } break;
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: GetVNADataRecall(int DataType)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::GetVNADataRecall(int DataType)
    {
        mc_NanoVNA.DoRecall(DataType);
    }
    //-Public----------------------------------------------------------------------
    // Name: GetVNAData(int S11, int S21)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::GetVNAData(int S11, int S21)
    {
    //    glutSetWindowTitle("VNA OpenGL Engine Version 4.0.0 Working");
        m_NumberOfSamples = (100.0f * m_X10Samples) + 1.0f;
        mc_NanoVNA.Calculate_MultiSweep(mc_Forms.m_FrequencySweep.Sweep.StartFrequency, mc_Forms.m_FrequencySweep.Sweep.StopFrequency, int(mc_Forms.m_FrequencySweep.Sweep.xSampleRate));
        mc_NanoVNA.DoMultiSweep(S11, S21);
    //    glutSetWindowTitle("VNA OpenGL Engine Version 4.0.0 Done");
    }
    //-Public----------------------------------------------------------------------
    // Name: GetVNADataVarType(int DataType)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::GetVNADataVarType(int DataType)
    {
    //    glutSetWindowTitle("VNA OpenGL Engine Version 4.0.0 Working");
        m_NumberOfSamples = (100.0f * m_X10Samples) + 1.0f;
        mc_NanoVNA.Calculate_MultiSweep(mc_Forms.m_FrequencySweep.Sweep.StartFrequency, mc_Forms.m_FrequencySweep.Sweep.StopFrequency, int(mc_Forms.m_FrequencySweep.Sweep.xSampleRate));
        mc_NanoVNA.DoMultiSweepDataType(DataType);
    //    glutSetWindowTitle("VNA OpenGL Engine Version 4.0.0 Done");
    }
    //-Public----------------------------------------------------------------------
    // Name: OpenVNAData(const char* FileName)
    //-----------------------------------------------------------------------------
    int MC_OpenGLChart::OpenVNAData(const char* FileName)
    {
        FILE* pFile = NULL;
        errno_t err = 0;
        char buff[256];
        int bufflen;
        int i;

        ZeroMemory(&buff, sizeof(buff));
        if (strlen(FileName)) {
            err = fopen_s(&pFile, FileName, "r");
            if (err == 0) {
                if (pFile != NULL) {
                    mc_Forms.m_MenuCheked.Filter_Average = false;
    //                FormSetCheckBox(hDlg, IDM_AVERAGE_FILTER, m_MenuCheked.Filter_Average);
                    for (i = 1; i < GL_MAX_MARKER; i++) {
                        m_Marker.Trace[i].Enable = false;
                        mc_NanoVNA.m_VNASample.Marker[i].ID = 0;
                        mc_NanoVNA.m_VNASample.Marker[i].Index = 0;
                        mc_NanoVNA.m_VNASample.Marker[i].Frequencies = 0;
                    }
                    while (fgets(buff, 256, pFile) != NULL) {
                        bufflen = int(strlen(buff));
                        if (bufflen) {
                            mc_NanoVNA.LoadDecodeData(buff, bufflen);
                        }
                    }
                    fclose(pFile);
                    mc_Forms.m_FrequencySweep.Sweep.StartFrequency = mc_NanoVNA.m_VNAData.Sweep.StartFrequency;
                    mc_Forms.m_FrequencySweep.Sweep.StopFrequency = mc_NanoVNA.m_VNAData.Sweep.StopFrequency;
                    mc_Forms.m_FrequencySweep.Sweep.CenterFrequency = ((mc_Forms.m_FrequencySweep.Sweep.StopFrequency - mc_Forms.m_FrequencySweep.Sweep.StartFrequency) / 2) + mc_Forms.m_FrequencySweep.Sweep.StartFrequency;
                    mc_Forms.m_FrequencySweep.Sweep.SpanFrequency = (mc_Forms.m_FrequencySweep.Sweep.StopFrequency - mc_Forms.m_FrequencySweep.Sweep.StartFrequency);
                    mc_Forms.m_FrequencySweep.Sweep.xSampleRate  = (int)mc_NanoVNA.m_X10Samples;
                    mc_Forms.m_FrequencySweep.Sweep.TotalSamples = mc_Forms.m_FrequencySweep.Sweep.xSampleRate * FORM_VNA_SAMPLE_PERSACN;
                    mc_Forms.UpdateSweepStrings();
                    for (i = 0; i < GL_MAX_MARKER; i++) {
                        if (mc_NanoVNA.m_VNASample.Marker[i].ID < GL_MAX_MARKER) {
                            m_Marker.Trace[mc_NanoVNA.m_VNASample.Marker[i].ID].Index = mc_NanoVNA.m_VNASample.Marker[i].Index;
                            m_Marker.Trace[mc_NanoVNA.m_VNASample.Marker[i].ID].Frequencies = mc_NanoVNA.m_VNASample.Marker[i].Frequencies;
                            m_Marker.Trace[mc_NanoVNA.m_VNASample.Marker[i].ID].Enable = true;
                        }
                    }
                    m_X10Samples = float(mc_Forms.m_FrequencySweep.Sweep.xSampleRate);
                    m_NumberOfSamples = (100.0f * m_X10Samples) + 1.0f;
                    Calculate_Trace_All(&m_GridData);
                    Calculate_Trace_All(&m_GridData);
                    mc_NanoVNA.m_Update = true;
                }
            } else {
                printf("The file %s was not open\r\n", FileName);
            }
        }
        return err;
    }
    //-Public----------------------------------------------------------------------
    // Name: SaveVNAData(const char* FileName)
    //-----------------------------------------------------------------------------
    int MC_OpenGLChart::SaveVNAData(const char* FileName)
    {
        FILE* pFile;
        errno_t err = 0;
        char buff[256];
        int i;

        ZeroMemory(&buff, sizeof(buff));
        if (strlen(FileName)) {
            err = fopen_s(&pFile, FileName, "w");
            if (err == 0) {
                sprintf_s(buff, sizeof(buff), "ch> sweep\n");
                fputs(buff, pFile);
                sprintf_s(buff, sizeof(buff), "%ld %ld %d\n", mc_Forms.m_FrequencySweep.Sweep.StartFrequency , mc_Forms.m_FrequencySweep.Sweep.StopFrequency, (mc_Forms.m_FrequencySweep.Sweep.TotalSamples) + 1);
                fputs(buff, pFile);
                sprintf_s(buff, sizeof(buff), "ch> marker\n");
                fputs(buff, pFile);
                for (i = 0; i < GL_MAX_MARKER; i++) {
                    if (m_Marker.Trace[i].Enable) {
                        sprintf_s(buff, sizeof(buff), "%d %d %ld\n", i, m_Marker.Trace[i].Index, m_Marker.Trace[i].Frequencies);
                        fputs(buff, pFile);
                    }
                }
                sprintf_s(buff, sizeof(buff), "ch> frequencies\n");
                fputs(buff, pFile);
                for (i = 0; i < m_NumberOfSamples; i++) {
                    sprintf_s(buff, sizeof(buff), "%ld\n", mc_NanoVNA.m_VNASample.Data_S11[i].Frequencies);
                    fputs(buff, pFile);
                }
                sprintf_s(buff, sizeof(buff), "ch> data 0\n");
                fputs(buff, pFile);
                for (i = 0; i < m_NumberOfSamples; i++) {
                    sprintf_s(buff, sizeof(buff), "%.9f %.9f\n", mc_NanoVNA.m_VNASample.Data_S11[i].Complex.Value1, mc_NanoVNA.m_VNASample.Data_S11[i].Complex.Value2);
                    fputs(buff, pFile);
                }
                sprintf_s(buff, sizeof(buff), "ch> data 1\n");
                fputs(buff, pFile);
                for (i = 0; i < m_NumberOfSamples; i++) {
                    sprintf_s(buff, sizeof(buff), "%.9f %.9f\n", mc_NanoVNA.m_VNASample.Data_S21[i].Complex.Value1, mc_NanoVNA.m_VNASample.Data_S11[i].Complex.Value2);
                    fputs(buff, pFile);
                }
                fclose(pFile);
            } else {
                printf("The file %s was not Save\r\n", FileName);
            }
        }
        return err;
    }
    //-Public----------------------------------------------------------------------
    // Name: Calculate_Grid(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_Grid(OPENGL_GRIDDATA* GridData)
    {
        GLfloat x, y;
        GLfloat xx, yy;

        ZeroMemory(&m_GridInfo, sizeof(m_GridInfo));
        for (x = 0; x <= GridData->Rows; x++) {
            if (x < GridData->Rows) {
                if (mc_Forms.m_MenuCheked.Chart_GridFine) {
                    m_GridInfo.RowsFine.Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Grid1];
                    m_GridInfo.RowsFine.FixedLen.Start = 0 * GridData->ScaleY;
                    m_GridInfo.RowsFine.FixedLen.End = GridData->Cols * GridData->ScaleY;
                    for (xx = 1; xx < 10; xx++) {
                        m_GridInfo.RowsFine.OffSet[m_GridInfo.RowsFine.Count] = (x + (xx / 10.0f)) * GridData->ScaleX;
                        m_GridInfo.RowsFine.Count++;
                    }
                }
            }
            m_GridInfo.Rows.Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Grid10];
            m_GridInfo.Rows.FixedLen.Start = -0.2f * GridData->ScaleY;
            m_GridInfo.Rows.FixedLen.End = GridData->Cols * GridData->ScaleY;
            m_GridInfo.Rows.OffSet[m_GridInfo.Rows.Count] = x * GridData->ScaleX;
            m_GridInfo.Rows.Count++;
        }
        for (y = 0; y <= GridData->Cols; y++) {
            if (y < GridData->Cols) {
                m_GridInfo.ColsFine.Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Grid1];
                m_GridInfo.ColsFine.FixedLen.Start = 0 * GridData->ScaleX;
                m_GridInfo.ColsFine.FixedLen.End = (GridData->Rows + 0.2f) * GridData->ScaleX;
                for (yy = 1; yy < 10; yy++) {
                    m_GridInfo.ColsFine.OffSet[m_GridInfo.ColsFine.Count] = (y + (yy / 10.0f)) * GridData->ScaleY;
                    m_GridInfo.ColsFine.Count++;
                }
            }
            m_GridInfo.Cols.Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Grid10];
            m_GridInfo.Cols.FixedLen.Start = 0 * GridData->ScaleX;
            m_GridInfo.Cols.FixedLen.End = (GridData->Rows + 0.4f) * GridData->ScaleX;
            m_GridInfo.Cols.OffSet[m_GridInfo.Cols.Count] = y * GridData->ScaleY;
            m_GridInfo.Cols.Count++;
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: Calculate_Trace_All(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_Trace_All(OPENGL_GRIDDATA* GridData)
    {
        Calculate_TraceS11_LogMag(GridData);
        Calculate_TraceS11_LinMag(GridData);
        Calculate_TraceS11_Swr(GridData);
        Calculate_TraceS11_Resitance(GridData);
        Calculate_TraceS11_Reactance(GridData);
        Calculate_TraceS11_Phase(GridData);
        Calculate_TraceS11_Smith(GridData);
        Calculate_TraceS21_LogMag(GridData);
        Calculate_TraceS21_LinMag(GridData);
        Calculate_TraceS21_Phase(GridData);
        Calculate_Marker_MinMax();
        Calculate_TraceS11_Oscilloscope(GridData);
    }
    //-Public----------------------------------------------------------------------
    // Name: Calculate_Marker_MinMax()
    //-----------------------------------------------------------------------------
    void  MC_OpenGLChart::Calculate_Marker_MinMax()
    {
        int i;
        int ValueMin_Index = 0;
        int ValueMax_Index = 0;
        double ValueTmp = 0.0;
        double ValueMin = 0.0;
        double ValueMax = -100.0;

        for (i = 0; i < m_NumberOfSamples; i++) {
            ValueTmp = mc_NanoVNA.m_VNASample.Data_S11[i].Rv_LogMag;
            if (ValueTmp > ValueMax) {
                ValueMax = ValueTmp;
                ValueMax_Index = i;
            }
            if (ValueTmp < ValueMin) {
                ValueMin = ValueTmp;
                ValueMin_Index = i;
            }
        }
        if (m_Marker.Trace[GL_MARKER_MAX_VALUE].Enable) {
            m_Marker.Trace[GL_MARKER_MAX_VALUE].Index = ValueMax_Index;
        }
        if (m_Marker.Trace[GL_MARKER_MIN_VALUE].Enable) {
            m_Marker.Trace[GL_MARKER_MIN_VALUE].Index = ValueMin_Index;
        }
    }









    //*****************************************************************************  
    // Protected Code
    //*****************************************************************************


    //-Protected-------------------------------------------------------------------
    // Name: Calculate_TraceS11_Swr(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_TraceS11_Swr(OPENGL_GRIDDATA* GridData)
    {
        int i;
        GLfloat x, y;
        GLfloat Value;

        ZeroMemory(&m_Trace.S11_Swr, sizeof(m_Trace.S11_Swr));
        for (i = 0; i < m_NumberOfSamples; i++) {
            y = (GLfloat(i) * GridData->Scale_Y) / m_X10Samples;
            if (mc_NanoVNA.m_VNASample.Data_S11[i].SWR == 0.0) {
                mc_NanoVNA.m_VNASample.Data_S11[i].SWR = 1.0;
            }
            Value = ((GLfloat)(((mc_NanoVNA.m_VNASample.Data_S11[i].SWR - 1.0) * 0.2)) * GridData->Size);
            x = (-Value * GridData->Scale_X) + GridData->ScaleFull_X;
            DrawChart_Limit(GridData, &m_Trace.S11_Swr[i], x, y);
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: Calculate_TraceS11_Phase(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_TraceS11_Phase(OPENGL_GRIDDATA* GridData)
    {
        int i;
        GLfloat x, y;
        GLfloat Value;

        ZeroMemory(&m_Trace.S11_Phase, sizeof(m_Trace.S11_Phase));
        for (i = 0; i < m_NumberOfSamples; i++) {
            y = (GLfloat(i) * GridData->Scale_Y) / m_X10Samples;
            Value = ((GLfloat)(-mc_NanoVNA.m_VNASample.Data_S11[i].Phase) * (GridData->HalfSize / 180.0f)) + GridData->HalfSize;
            x = Value * GridData->Scale_X;
            DrawChart_Limit(GridData, &m_Trace.S11_Phase[i], x, y);
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: Calculate_TraceS11_Reactance(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_TraceS11_Reactance(OPENGL_GRIDDATA* GridData)
    {
        int i;
        GLfloat x, y;

        ZeroMemory(&m_Trace.S11_Reactance, sizeof(m_Trace.S11_Reactance));
        for (i = 0; i < m_NumberOfSamples; i++) {
            y = (GLfloat(i) * GridData->Scale_Y) / m_X10Samples;
            x = (GLfloat)((-mc_NanoVNA.m_VNASample.Data_S11[i].Reactance) * GridData->Scale_X) + GridData->ScaleHalf_X;
            DrawChart_Limit(GridData, &m_Trace.S11_Reactance[i], x, y);
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: Calculate_TraceS11_Resitance(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_TraceS11_Resitance(OPENGL_GRIDDATA* GridData)
    {
        int i;
        GLfloat x, y;
        GLfloat Value;

        ZeroMemory(&m_Trace.S11_Resitance, sizeof(m_Trace.S11_Resitance));
        for (i = 0; i < m_NumberOfSamples; i++) {
            y = (GLfloat(i) * GridData->Scale_Y) / m_X10Samples;
            Value = (GLfloat)((mc_NanoVNA.m_VNASample.Data_S11[i].Resitance - VNA_CHARACTERISTIC_IMPEDANCE) / (mc_NanoVNA.m_VNASample.Data_S11[i].Resitance + VNA_CHARACTERISTIC_IMPEDANCE));
            x = ((-Value) + GridData->ScaleHalf_X);
            DrawChart_Limit(GridData, &m_Trace.S11_Resitance[i], x, y);
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: Calculate_TraceS11_LogMag(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_TraceS11_LogMag(OPENGL_GRIDDATA* GridData)
    {
        int i;
        GLfloat x, y;

        ZeroMemory(&m_Trace.S11_LogMag, sizeof(m_Trace.S11_LogMag));
        for (i = 0; i < m_NumberOfSamples; i++) {
            y = (GLfloat(i) * GridData->Scale_Y) / m_X10Samples;
            x = (GLfloat)(-mc_NanoVNA.m_VNASample.Data_S11[i].Rv_LogMag) * GridData->Scale_X;
            DrawChart_Limit(GridData, &m_Trace.S11_LogMag[i], x, y);
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: Calculate_TraceS11_LinMag(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_TraceS11_LinMag(OPENGL_GRIDDATA* GridData)
    {
        int i;
        GLfloat x, y;

        ZeroMemory(&m_Trace.S11_LinMag, sizeof(m_Trace.S11_LinMag));
        for (i = 0; i < m_NumberOfSamples; i++) {
            y = (GLfloat(i) * GridData->Scale_Y) / m_X10Samples;
            x = ((GLfloat)((1.0 + mc_NanoVNA.m_VNASample.Data_S11[i].Rv_Linear) * 100.0) * GridData->Scale_X);
            DrawChart_Limit(GridData, &m_Trace.S11_LinMag[i], x, y);
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: Calculate_TraceS21_LogMag(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_TraceS21_LogMag(OPENGL_GRIDDATA* GridData)
    {
        int i;
        GLfloat x, y;

        ZeroMemory(&m_Trace.S21_LogMag, sizeof(m_Trace.S21_LogMag));
        for (i = 0; i < m_NumberOfSamples; i++) {
            y = (GLfloat(i) * GridData->Scale_Y) / m_X10Samples;
            x = (GLfloat)(-mc_NanoVNA.m_VNASample.Data_S21[i].Rv_LogMag) * GridData->Scale_X;
            DrawChart_Limit(GridData, &m_Trace.S21_LogMag[i], x, y);
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: Calculate_TraceS21_LinMag(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_TraceS21_LinMag(OPENGL_GRIDDATA* GridData)
    {
        int i;
        GLfloat x, y;

        ZeroMemory(&m_Trace.S21_LinMag, sizeof(m_Trace.S21_LinMag));
        for (i = 0; i < m_NumberOfSamples; i++) {
            y = (GLfloat(i) * GridData->Scale_Y) / m_X10Samples;
            x = ((GLfloat)((1.0 + mc_NanoVNA.m_VNASample.Data_S21[i].Rv_Linear) * 100.0) * GridData->Scale_X);
            DrawChart_Limit(GridData, &m_Trace.S21_LinMag[i], x, y);
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: Calculate_TraceS21_Phase(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_TraceS21_Phase(OPENGL_GRIDDATA* GridData)
    {
        int i;
        GLfloat x, y;
        GLfloat Value;

        ZeroMemory(&m_Trace.S21_Phase, sizeof(m_Trace.S21_Phase));
        for (i = 0; i < m_NumberOfSamples; i++) {
            y = (GLfloat(i) * GridData->Scale_Y) / m_X10Samples;
            Value = ((GLfloat)(-mc_NanoVNA.m_VNASample.Data_S21[i].Phase) * (GridData->HalfSize / 180.0f)) + GridData->HalfSize;
            x = Value * GridData->Scale_X;
            DrawChart_Limit(GridData, &m_Trace.S21_Phase[i], x, y);
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: Calculate_TraceS11_Smith(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_TraceS11_Smith(OPENGL_GRIDDATA* GridData)
    {
        int i;
        GLfloat x, y;
        GLfloat Value_x;
        GLfloat Value_y;

        ZeroMemory(&m_Trace.S11_Smith, sizeof(m_Trace.S11_Smith));
        for (i = 0; i < m_NumberOfSamples; i++) {
            Value_x = (GLfloat)(mc_NanoVNA.m_VNASample.Data_S11[i].Reactance * 0.5);
            x = (-Value_x * GridData->Scale_X) + GridData->ScaleHalf_X;
            Value_y = (GLfloat)((mc_NanoVNA.m_VNASample.Data_S11[i].Resitance - VNA_CHARACTERISTIC_IMPEDANCE) / (mc_NanoVNA.m_VNASample.Data_S11[i].Resitance + VNA_CHARACTERISTIC_IMPEDANCE));
            y = (((Value_y * GridData->Size) / 2.0f) * GridData->Scale_Y) + GridData->ScaleHalf_Y;
            DrawChart_Limit(GridData, &m_Trace.S11_Smith[i], x, y);
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: Calculate_TraceS11_Oscilloscope(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Calculate_TraceS11_Oscilloscope(OPENGL_GRIDDATA* GridData)
    {
        int i;

        mc_Oscilloscope.ClearValues(OSC_CHANNEL_ALL);
        for (i = 0; i < m_NumberOfSamples; i++) {
            mc_Oscilloscope.AddValues(OSC_CHANNEL_1, mc_NanoVNA.m_VNASample.Data_S11[i].Complex, mc_NanoVNA.m_VNASample.Data_S11[i].Frequencies);
            mc_Oscilloscope.AddValues(OSC_CHANNEL_2, mc_NanoVNA.m_VNASample.Data_S21[i].Complex, mc_NanoVNA.m_VNASample.Data_S21[i].Frequencies);
        }
        mc_Oscilloscope.CalculateValues(OSC_CHANNEL_1, GridData->Scale_X, GridData->Scale_Y, GridData->ScaleFull_X, GridData->ScaleFull_Y, GridData->ScaleHalf_X);
        mc_Oscilloscope.CalculateValues(OSC_CHANNEL_2, GridData->Scale_X, GridData->Scale_Y, GridData->ScaleFull_X, GridData->ScaleFull_Y, GridData->ScaleHalf_X);
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_Grid()
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_Grid()
    {
        int Index;
        GLfloat TxtSy;
        int DataIndex;
        char szBuf[64];

        if (!mc_Forms.m_MenuCheked.Chart_Grid) {
            return;
        }
        ZeroMemory(&szBuf, sizeof(szBuf));
        TxtSy = -0.6f * m_GridData.ScaleY;
        if (mc_Forms.m_MenuCheked.Chart_GridFine) {
            glColor3f(m_GridInfo.RowsFine.Colour.r, m_GridInfo.RowsFine.Colour.g, m_GridInfo.RowsFine.Colour.b);
            for (Index = 0; Index < m_GridInfo.RowsFine.Count; Index++) {
                Draw_2DLine(m_GridInfo.RowsFine.OffSet[Index], m_GridInfo.RowsFine.FixedLen.Start, m_GridInfo.RowsFine.OffSet[Index], m_GridInfo.RowsFine.FixedLen.End);
            }
            glColor3f(m_GridInfo.ColsFine.Colour.r, m_GridInfo.ColsFine.Colour.g, m_GridInfo.ColsFine.Colour.b);
            for (Index = 0; Index < m_GridInfo.ColsFine.Count; Index++) {
                Draw_2DLine(m_GridInfo.ColsFine.FixedLen.Start, m_GridInfo.ColsFine.OffSet[Index], m_GridInfo.ColsFine.FixedLen.End, m_GridInfo.ColsFine.OffSet[Index]);
            }
        }
        for (Index = 0; Index < m_GridInfo.Rows.Count; Index++) {
            glColor3f(m_GridInfo.Rows.Colour.r, m_GridInfo.Rows.Colour.g, m_GridInfo.Rows.Colour.b);
            Draw_2DLine(m_GridInfo.Rows.OffSet[Index], m_GridInfo.Rows.FixedLen.Start, m_GridInfo.Rows.OffSet[Index], m_GridInfo.Rows.FixedLen.End);
            sprintf_s(szBuf, sizeof(szBuf), "%d dbm", int(-((100.0f / m_GridData.Rows) * Index)));
            Draw_3DText(szBuf, m_GridInfo.Rows.OffSet[Index], TxtSy, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_White]);
        }
        for (Index = 0; Index < m_GridInfo.Cols.Count; Index++) {
            glColor3f(m_GridInfo.Cols.Colour.r, m_GridInfo.Cols.Colour.g, m_GridInfo.Cols.Colour.b);
            Draw_2DLine(m_GridInfo.Cols.FixedLen.Start, m_GridInfo.Cols.OffSet[Index], m_GridInfo.Cols.FixedLen.End, m_GridInfo.Cols.OffSet[Index]);
            DataIndex = (int)(((100.0f / m_GridData.Cols) * Index) * m_X10Samples);
            sprintf_s(szBuf, sizeof(szBuf), "%.3f %s", mc_VnaMath.Value_ScaleFrequency(double(mc_NanoVNA.m_VNAData.Frequencies[DataIndex])), mc_VnaMath.Value_UnitFrequency(double(mc_NanoVNA.m_VNAData.Frequencies[DataIndex])));
            Draw_3DText(szBuf, m_GridInfo.Cols.FixedLen.End, m_GridInfo.Cols.OffSet[Index], 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_White]);
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TraceS11_Frequencies(OPENGL_GRIDDATA* GridData, int MarkerIndex, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_TraceS11_Frequencies(OPENGL_GRIDDATA* GridData, int MarkerIndex, FORM_COLOUR* Colour)
    {
        int MarkerItem = 0;
        GLfloat x, y;
        GLfloat lx, ly;
        char szBuf[64];
        double Frequency;

        ZeroMemory(&szBuf, sizeof(szBuf));
        Frequency = mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies;
        y = (GLfloat(MarkerIndex) * GridData->Scale_Y) / m_X10Samples;
        x = (GridData->Size + 2.0f) * GridData->Scale_X;
        lx = 0.0f; ly = y;
        glLineStipple(1, 0x9999);
        glEnable(GL_LINE_STIPPLE);
        glColor3f(Colour->r * 0.6f, Colour->g * 0.6f, Colour->b * 0.6f);
        Draw_2DLine(lx, ly, x, y);
        glDisable(GL_LINE_STIPPLE);
        sprintf_s(szBuf, sizeof(szBuf), "%.4f %s", mc_VnaMath.Value_ScaleFrequency(Frequency), mc_VnaMath.Value_UnitFrequency(Frequency));
        m_Marker.Trace[0].Frequencies = mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies;
        Draw_3DMarker(szBuf, x, y, 0.0f, Colour, MarkerItem);
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TraceS11_Swr(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_TraceS11_Swr(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    {
        int i;
        int MarkerItem;
        GLfloat x, y;
        GLfloat lx, ly;
        char szBuf[64];

        if (!mc_Forms.m_MenuCheked.TraceS11_SWR) {
            return;
        }
        ZeroMemory(&szBuf, sizeof(szBuf));
        glColor3f(Colour->r * 0.6f, Colour->g * 0.6f, Colour->b * 0.6f);
        for (i = 0; i < m_NumberOfSamples; i++) {
            x = m_Trace.S11_Swr[i].x;
            y = m_Trace.S11_Swr[i].y;
            if (i == 0) {
                lx = x; ly = y;
            }
            Draw_2DLine(lx, ly, x, y);
            lx = x; ly = y;
        }
        for (MarkerItem = 0; MarkerItem < GL_MAX_MARKER; MarkerItem++) {
            if (Marker->Trace[MarkerItem].Enable) {
                 x = m_Trace.S11_Swr[Marker->Trace[MarkerItem].Index].x;
                 y = m_Trace.S11_Swr[Marker->Trace[MarkerItem].Index].y;
                 sprintf_s(szBuf, sizeof(szBuf), "%.4f Swr", mc_NanoVNA.m_VNASample.Data_S11[Marker->Trace[MarkerItem].Index].SWR);
                 if (Marker->Trace[MarkerItem].Index < m_NumberOfSamples) {
                     Draw_3DMarker(szBuf, x, y, 0.0f, Colour, MarkerItem);
                 }
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TraceS11_Phase(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_TraceS11_Phase(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    {
        int i;
        int MarkerItem;
        GLfloat x, y;
        GLfloat lx, ly;
        char szBuf[64];

        if (!mc_Forms.m_MenuCheked.TraceS11_Phase) {
            return;
        }
        ZeroMemory(&szBuf, sizeof(szBuf));
        glColor3f(Colour->r * 0.6f, Colour->g * 0.6f, Colour->b * 0.6f);
        for (i = 0; i < m_NumberOfSamples; i++) {
            x = m_Trace.S11_Phase[i].x;
            y = m_Trace.S11_Phase[i].y;
            if (i == 0) {
                lx = x; ly = y;
            }
            Draw_2DLine(lx, ly, x, y);
            lx = x; ly = y;
        }
        for (MarkerItem = 0; MarkerItem < GL_MAX_MARKER; MarkerItem++) {
            if (Marker->Trace[MarkerItem].Enable) {
                x = m_Trace.S11_Phase[Marker->Trace[MarkerItem].Index].x;
                y = m_Trace.S11_Phase[Marker->Trace[MarkerItem].Index].y;
                sprintf_s(szBuf, sizeof(szBuf), "%.3f Pha", mc_NanoVNA.m_VNASample.Data_S11[Marker->Trace[MarkerItem].Index].Phase);
                if (Marker->Trace[MarkerItem].Index < m_NumberOfSamples) {
                    Draw_3DMarker(szBuf, x, y, 0.0f, Colour, MarkerItem);
                }
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TraceS11_Reactance(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_TraceS11_Reactance(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    {
        int i;
        int MarkerItem;
        GLfloat x, y;
        GLfloat lx, ly;
        char szBuf[64];

        if (!mc_Forms.m_MenuCheked.TraceS11_Reactance) {
            return;
        }
        ZeroMemory(&szBuf, sizeof(szBuf));
        glColor3f(Colour->r * 0.6f, Colour->g * 0.6f, Colour->b * 0.6f);
        for (i = 0; i < m_NumberOfSamples; i++) {
            x = m_Trace.S11_Reactance[i].x;
            y = m_Trace.S11_Reactance[i].y;
            if (i == 0) {
                lx = x; ly = y;
            }
            Draw_2DLine(lx, ly, x, y);
            lx = x; ly = y;
        }
        for (MarkerItem = 0; MarkerItem < GL_MAX_MARKER; MarkerItem++) {
            if (Marker->Trace[MarkerItem].Enable) {
                x = m_Trace.S11_Reactance[Marker->Trace[MarkerItem].Index].x;
                y = m_Trace.S11_Reactance[Marker->Trace[MarkerItem].Index].y;
                if (mc_NanoVNA.m_VNASample.Data_S11[Marker->Trace[MarkerItem].Index].Reactance <= 0.0) {
                    sprintf_s(szBuf, sizeof(szBuf), "%.3f Cap", mc_NanoVNA.m_VNASample.Data_S11[Marker->Trace[MarkerItem].Index].Reactance);
                } else {
                    sprintf_s(szBuf, sizeof(szBuf), "%.3f Ind", mc_NanoVNA.m_VNASample.Data_S11[Marker->Trace[MarkerItem].Index].Reactance);
                }
                if (Marker->Trace[MarkerItem].Index < m_NumberOfSamples) {
                    Draw_3DMarker(szBuf, x, y, 0.0f, Colour, MarkerItem);
                }
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TraceS11_Resitance(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_TraceS11_Resitance(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    {
        int i;
        int MarkerItem;
        GLfloat x, y;
        GLfloat lx, ly;
        char szBuf[64];

        if (!mc_Forms.m_MenuCheked.TraceS11_Resitance) {
            return;
        }
        ZeroMemory(&szBuf, sizeof(szBuf));
        glColor3f(Colour->r * 0.6f, Colour->g * 0.6f, Colour->b * 0.6f);
        for (i = 0; i < m_NumberOfSamples; i++) {
            x = m_Trace.S11_Resitance[i].x;
            y = m_Trace.S11_Resitance[i].y;
            if (i == 0) {
                lx = x; ly = y;
            }
            Draw_2DLine(lx, ly, x, y);
            lx = x; ly = y;
        }
        for (MarkerItem = 0; MarkerItem < GL_MAX_MARKER; MarkerItem++) {
            if (Marker->Trace[MarkerItem].Enable) {
                x = m_Trace.S11_Resitance[Marker->Trace[MarkerItem].Index].x;
                y = m_Trace.S11_Resitance[Marker->Trace[MarkerItem].Index].y;
                sprintf_s(szBuf, sizeof(szBuf), "%.3f Ohms", mc_NanoVNA.m_VNASample.Data_S11[Marker->Trace[MarkerItem].Index].Resitance);
                if (Marker->Trace[MarkerItem].Index < m_NumberOfSamples) {
                    Draw_3DMarker(szBuf, x, y, 0.0f, Colour, MarkerItem);
                }
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TraceS11_LogMag(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_TraceS11_LogMag(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    {
        int i;
        int MarkerItem;
        GLfloat x, y;
        GLfloat lx, ly;
        char szBuf[64];

        if (!mc_Forms.m_MenuCheked.TraceS11_LogMag) {
            return;
        }
        ZeroMemory(&szBuf, sizeof(szBuf));
        glColor3f(Colour->r * 0.6f, Colour->g * 0.6f, Colour->b * 0.6f);
        for (i = 0; i < m_NumberOfSamples; i++) {
            x = m_Trace.S11_LogMag[i].x;
            y = m_Trace.S11_LogMag[i].y;
            if (i == 0) {
                lx = x; ly = y;
            }
            Draw_2DLine(lx, ly, x, y);
            lx = x; ly = y;
        }
        for (MarkerItem = 0; MarkerItem < GL_MAX_MARKER; MarkerItem++) {
            if (Marker->Trace[MarkerItem].Enable) {
                x = m_Trace.S11_LogMag[Marker->Trace[MarkerItem].Index].x;
                y = m_Trace.S11_LogMag[Marker->Trace[MarkerItem].Index].y;
                sprintf_s(szBuf, sizeof(szBuf), "%.3f dbm", mc_NanoVNA.m_VNASample.Data_S11[Marker->Trace[MarkerItem].Index].Rv_LogMag);
                if (Marker->Trace[MarkerItem].Index < m_NumberOfSamples) {
                    Draw_3DMarker(szBuf, x, y, 0.0f, Colour, MarkerItem);
                }
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TraceS11_LinMag(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_TraceS11_LinMag(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    {
        int i;
        int MarkerItem;
        GLfloat x, y;
        GLfloat lx, ly;
        char szBuf[64];

        if (!mc_Forms.m_MenuCheked.TraceS11_LinearMag) {
            return;
        }
        ZeroMemory(&szBuf, sizeof(szBuf));
        glColor3f(Colour->r * 0.6f, Colour->g * 0.6f, Colour->b * 0.6f);
        for (i = 0; i < m_NumberOfSamples; i++) {
            x = m_Trace.S11_LinMag[i].x;
            y = m_Trace.S11_LinMag[i].y;
            if (i == 0) {
                lx = x; ly = y;
            }
            Draw_2DLine(lx, ly, x, y);
            lx = x; ly = y;
        }
        for (MarkerItem = 0; MarkerItem < GL_MAX_MARKER; MarkerItem++) {
            if (Marker->Trace[MarkerItem].Enable) {
                x = m_Trace.S11_LinMag[Marker->Trace[MarkerItem].Index].x;
                y = m_Trace.S11_LinMag[Marker->Trace[MarkerItem].Index].y;
                sprintf_s(szBuf, sizeof(szBuf), "%.3f lin", mc_NanoVNA.m_VNASample.Data_S11[Marker->Trace[MarkerItem].Index].Rv_Linear);
                if (Marker->Trace[MarkerItem].Index < m_NumberOfSamples) {
                    Draw_3DMarker(szBuf, x, y, 0.0f, Colour, MarkerItem);
                }
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TraceS21_LogMag(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_TraceS21_LogMag(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    {
        int i;
        int MarkerItem;
        GLfloat x, y;
        GLfloat lx, ly;
        char szBuf[64];

        if (!mc_Forms.m_MenuCheked.TraceS21_LogMag) {
            return;
        }
        ZeroMemory(&szBuf, sizeof(szBuf));
        glColor3f(Colour->r * 0.6f, Colour->g * 0.6f, Colour->b * 0.6f);
        for (i = 0; i < m_NumberOfSamples; i++) {
            x = m_Trace.S21_LogMag[i].x;
            y = m_Trace.S21_LogMag[i].y;
            if (i == 0) {
                lx = x; ly = y;
            }
            Draw_2DLine(lx, ly, x, y);
            lx = x; ly = y;
        }
        for (MarkerItem = 0; MarkerItem < GL_MAX_MARKER; MarkerItem++) {
            if (Marker->Trace[MarkerItem].Enable) {
                x = m_Trace.S21_LogMag[Marker->Trace[MarkerItem].Index].x;
                y = m_Trace.S21_LogMag[Marker->Trace[MarkerItem].Index].y;
                sprintf_s(szBuf, sizeof(szBuf), "%.3f dbm", mc_NanoVNA.m_VNASample.Data_S21[Marker->Trace[MarkerItem].Index].Rv_LogMag);
                if (Marker->Trace[MarkerItem].Index < m_NumberOfSamples) {
                    Draw_3DMarker(szBuf, x, y, 0.0f, Colour, MarkerItem);
                }
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TraceS21_LinMag(OPENGL_MARKERS* Marker,FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_TraceS21_LinMag(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    {
        int i;
        int MarkerItem;
        GLfloat x, y;
        GLfloat lx, ly;
        char szBuf[64];

        if (!mc_Forms.m_MenuCheked.TraceS21_LinearMag) {
            return;
        }
        ZeroMemory(&szBuf, sizeof(szBuf));
        glColor3f(Colour->r * 0.6f, Colour->g * 0.6f, Colour->b * 0.6f);
        for (i = 0; i < m_NumberOfSamples; i++) {
            x = m_Trace.S21_LinMag[i].x;
            y = m_Trace.S21_LinMag[i].y;
            if (i == 0) {
                lx = x; ly = y;
            }
            Draw_2DLine(lx, ly, x, y);
            lx = x; ly = y;
        }
        for (MarkerItem = 0; MarkerItem < GL_MAX_MARKER; MarkerItem++) {
            if (Marker->Trace[MarkerItem].Enable) {
                x = m_Trace.S21_LinMag[Marker->Trace[MarkerItem].Index].x;
                y = m_Trace.S21_LinMag[Marker->Trace[MarkerItem].Index].y;
                sprintf_s(szBuf, sizeof(szBuf), "%.3f lin", mc_NanoVNA.m_VNASample.Data_S21[Marker->Trace[MarkerItem].Index].Rv_Linear);
                if (Marker->Trace[MarkerItem].Index < m_NumberOfSamples) {
                    Draw_3DMarker(szBuf, x, y, 0.0f, Colour, MarkerItem);
                }
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TraceS21_Phase(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_TraceS21_Phase(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour)
    {
        int i;
        int MarkerItem;
        GLfloat x, y;
        GLfloat lx, ly;
        char szBuf[64];

        if (!mc_Forms.m_MenuCheked.TraceS21_Phase) {
            return;
        }
        ZeroMemory(&szBuf, sizeof(szBuf));
        glColor3f(Colour->r * 0.6f, Colour->g * 0.6f, Colour->b * 0.6f);
        for (i = 0; i < m_NumberOfSamples; i++) {
            x = m_Trace.S21_Phase[i].x;
            y = m_Trace.S21_Phase[i].y;
            if (i == 0) {
                lx = x; ly = y;
            }
            Draw_2DLine(lx, ly, x, y);
            lx = x; ly = y;
        }
        for (MarkerItem = 0; MarkerItem < GL_MAX_MARKER; MarkerItem++) {
            if (Marker->Trace[MarkerItem].Enable) {
                x = m_Trace.S21_Phase[Marker->Trace[MarkerItem].Index].x;
                y = m_Trace.S21_Phase[Marker->Trace[MarkerItem].Index].y;
                sprintf_s(szBuf, sizeof(szBuf), "%.3f Pha", mc_NanoVNA.m_VNASample.Data_S21[Marker->Trace[MarkerItem].Index].Phase);
                if (Marker->Trace[MarkerItem].Index < m_NumberOfSamples) {
                    Draw_3DMarker(szBuf, x, y, 0.0f, Colour, MarkerItem);
                }
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TopeLine(OPENGL_GRIDDATA* GridData, int MarkerIndex, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_TopeLine(OPENGL_GRIDDATA* GridData, int MarkerIndex, FORM_COLOUR* Colour)
    {
        GLfloat xx, yy;
        GLfloat xxt;
        char szBuf[64];
        int Samples;
        double Steps;

        ZeroMemory(&szBuf, sizeof(szBuf));
        Samples = (int)((m_NumberOfSamples - 1.0f) * 0.01f) * 100;
        Steps = double(mc_Forms.m_FrequencySweep.Sweep.StopFrequency - mc_Forms.m_FrequencySweep.Sweep.StartFrequency) / double(Samples);
        if (Steps < 100.0) {
            Steps = 100.0;
        }
        xx = -0.2f * GridData->ScaleX;
        xxt = -0.4f * GridData->ScaleX;
    
        yy = 0 * GridData->ScaleY;
        sprintf_s(szBuf, sizeof(szBuf), "Sweep %.4f %s to  %.4f %s", mc_VnaMath.Value_ScaleFrequency(double(mc_Forms.m_FrequencySweep.Sweep.StartFrequency)), mc_VnaMath.Value_UnitFrequency(double(mc_Forms.m_FrequencySweep.Sweep.StartFrequency)), mc_VnaMath.Value_ScaleFrequency(double(mc_Forms.m_FrequencySweep.Sweep.StopFrequency)), mc_VnaMath.Value_UnitFrequency(double(mc_Forms.m_FrequencySweep.Sweep.StopFrequency)));
        Draw_3DText(szBuf, xxt, yy, 0.0f, Colour);

        yy = 0 * GridData->ScaleY;
        sprintf_s(szBuf, sizeof(szBuf), "Steps %.4f %s", mc_VnaMath.Value_ScaleFrequency(Steps), mc_VnaMath.Value_UnitFrequency(Steps));
        Draw_3DText(szBuf, xx, yy, 0.0f, Colour);

        yy = GridData->ScaleFull_Y * 0.25f;
        sprintf_s(szBuf, sizeof(szBuf), "Frequency %.4f %s", mc_VnaMath.Value_ScaleFrequency(double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies)), mc_VnaMath.Value_UnitFrequency(double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies)));
        Draw_3DText(szBuf, xx, yy, 0.0f, Colour);

        yy = GridData->ScaleFull_Y * 0.25f;
        sprintf_s(szBuf, sizeof(szBuf), "Samples x %d ", Samples);
        Draw_3DText(szBuf, xxt, yy, 0.0f, Colour);

        yy = GridData->ScaleFull_Y * 0.4f;
        mc_NanoVNA.GetCapacitanceInductance(szBuf, sizeof(szBuf), &mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Complex, mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies);
        Draw_3DText(szBuf, xx, yy, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Resitance]);

        yy = GridData->ScaleFull_Y * 0.6f;
        sprintf_s(szBuf, sizeof(szBuf), "%.3f jx Ohms", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Real.Impedance_Resitance_jx);
        Draw_3DText(szBuf, xx, yy, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Reactance]);

        yy = GridData->ScaleFull_Y * 0.6f;
        sprintf_s(szBuf, sizeof(szBuf), "%.3f Phase", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Phase);
        Draw_3DText(szBuf, xxt, yy, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Phase]);

        yy = GridData->ScaleFull_Y * 0.725f;
        sprintf_s(szBuf, sizeof(szBuf), "Rv Mag %.9f dbm", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Rv_LogMag);
        Draw_3DText(szBuf, xx, yy, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_LogMag]);

        yy = GridData->ScaleFull_Y * 0.725f;
        sprintf_s(szBuf, sizeof(szBuf), "Fw Pow %.9f mw", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Fw_Power);
        Draw_3DText(szBuf, xxt, yy, 0.0f, Colour);

        yy = GridData->ScaleFull_Y * 0.925f;
        sprintf_s(szBuf, sizeof(szBuf), "Swr %.4f", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].SWR);
        Draw_3DText(szBuf, xx, yy, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Swr]);
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_Scale(OPENGL_GRIDDATA* GridData, GLfloat Rows, GLfloat Cols, GLfloat ScaleX, GLfloat ScaleY)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_Scale(OPENGL_GRIDDATA* GridData, GLfloat Rows, GLfloat Cols, GLfloat ScaleX, GLfloat ScaleY)
    {
        static bool Last2D3D = false;

        if (Last2D3D != mc_Forms.m_MenuCheked.Chart_2D3D) {
            if (!mc_Forms.m_MenuCheked.Chart_2D3D) {
                mc_OpenGL.OnReSize3d2d(mc_OpenGL.m_ViewPort.width, mc_OpenGL.m_ViewPort.height);    // Enable 3d
            }
            GridData->Size = 100.0f;
            GridData->Rows = Rows;
            GridData->Cols = Cols;
            GridData->HalfSize = GridData->Size * 0.5f;
            GridData->ScaleX = ScaleX;
            GridData->ScaleY = ScaleY;
            GridData->Scale_X = (Rows / GridData->Size) * GridData->ScaleX;
            GridData->Scale_Y = (Cols / GridData->Size) * GridData->ScaleY;
            GridData->ScaleHalf_X = GridData->Scale_X * (GridData->Size * 0.5f);
            GridData->ScaleFull_X = GridData->Scale_X * GridData->Size;
            GridData->ScaleHalf_Y = GridData->Scale_Y * (GridData->Size * 0.5f);
            GridData->ScaleFull_Y = GridData->Scale_Y * GridData->Size;
            Calculate_Grid(GridData);
            Calculate_Trace_All(&m_GridData);
            Last2D3D = mc_Forms.m_MenuCheked.Chart_2D3D;
        }
        Translate3d(GridData);
    }
    //-Protected-------------------------------------------------------------------
    // Name: Draw_2DLine(GLfloat xx, GLfloat yy, GLfloat x, GLfloat y)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Draw_2DLine(GLfloat xx, GLfloat yy, GLfloat x, GLfloat y)
    {
        glBegin(GL_LINES);
            glVertex2f(xx, yy);
            glVertex2f(x, y);
        glEnd();
    }
    //-Protected-------------------------------------------------------------------
    // Name: Draw_3DText(char* Buffer, GLfloat x, GLfloat y, GLfloat z, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Draw_3DText(char* Buffer, GLfloat x, GLfloat y, GLfloat z, FORM_COLOUR* Colour)
    {
        glColor3f(Colour->r, Colour->g, Colour->b);
        glPushMatrix();
            glVertex3f(0.0f, 0.0f, 0.0f);
            glTranslatef(x, y, z);
            Draw_3DString(Buffer);
        glPopMatrix();
    }
    //-Protected-------------------------------------------------------------------
    // Name: Draw_3DMarker(char* Buffer, GLfloat x, GLfloat y, GLfloat z,FORM_COLOUR* Colour, int MarkerIndex)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Draw_3DMarker(char* Buffer, GLfloat x, GLfloat y, GLfloat z, FORM_COLOUR* Colour, int MarkerIndex)
    {
        char szBuf[64];

        ZeroMemory(&szBuf, sizeof(szBuf));
        glColor3f(m_Marker.Trace[MarkerIndex].Colour.r, m_Marker.Trace[MarkerIndex].Colour.g, m_Marker.Trace[MarkerIndex].Colour.b);
        glPushMatrix();
            glVertex3f(0.0f, 0.0f, 0.0f);
            glTranslatef(x, y, z);
            sprintf_s(szBuf, sizeof(szBuf), "<");
            Draw_3DString(szBuf);
            sprintf_s(szBuf, sizeof(szBuf), "-");
            Draw_3DString(szBuf);
        glPopMatrix();
        glColor3f(Colour->r, Colour->g, Colour->b);
        if (MarkerIndex == GL_MARKER_MAX_VALUE) {
            sprintf_s(szBuf, sizeof(szBuf), "   Max %s", Buffer);
        } else if (MarkerIndex == GL_MARKER_MIN_VALUE) {
            sprintf_s(szBuf, sizeof(szBuf), "   Min %s", Buffer);
        } else {
            sprintf_s(szBuf, sizeof(szBuf), "   %s", Buffer);
        }
        glPushMatrix();
            glVertex3f(0.0f, 0.0f, 0.0f);
            glTranslatef(x, y, z);
            Draw_3DString(szBuf);
        glPopMatrix();
    }
    //-Protected-------------------------------------------------------------------
    // Name: Draw_3DString(char* Buffer)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Draw_3DString(char* Buffer)
    {
        glRasterPos3f(0.0f, 0.0f, 0.0f);
        glListBase(mc_OpenGL.m_Fonts.Font2D);
        glCallLists(GLsizei(strlen(Buffer)), GL_UNSIGNED_BYTE, Buffer);
    }
    //-Protected-------------------------------------------------------------------
    // Name: Translate3d(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Translate3d(OPENGL_GRIDDATA* GridData)
    {
        if (mc_Forms.m_MenuCheked.Chart_2D3D) {                                       // Disable 3d Start
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glRotatef(270.0f, 0.0f, 0.0f, 1.0f);

            if (GL_GRID_MAXCOLS == 20) {
                glTranslatef(-(GridData->ScaleHalf_X + 0.02f), -(GridData->ScaleHalf_Y - 0.06f), 0.0f);
            } else if (GL_GRID_MAXCOLS == 10) {
                glTranslatef(-(GridData->ScaleHalf_X + 0.02f), -(GridData->ScaleFull_Y - 0.06f), 0.0f);
            } else {
                glTranslatef(-(GridData->ScaleHalf_X + 0.02f), -(GridData->ScaleHalf_Y - 0.06f), 0.0f);
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_Limit(OPENGL_GRIDDATA* GridData, OPENGL_VECTOR* Vectors, GLfloat x, GLfloat y)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::DrawChart_Limit(OPENGL_GRIDDATA* GridData, OPENGL_VECTOR* Vectors, GLfloat x, GLfloat y)
    {
        if (x < 0.0f) {
            x = 0.0f;
        } else if (x > GridData->ScaleFull_X) {
            x = GridData->ScaleFull_X;
        }
        if (y < 0.0f) {
            y = 0.0f;
        } else if (y > GridData->ScaleFull_Y) {
            y = GridData->ScaleFull_Y;
        }
        Vectors->x = x;
        Vectors->y = y;
    }
    // -Protected------------------------------------------------------------------
    // Name: Draw_MarkerText(const char* Header, float* OffSet, int Marker, int MarkerIndex)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::Draw_MarkerText(const char* Header, float* OffSet, int Marker, int MarkerIndex)
    {
        char szBuf[256];
        FORM_COLOUR Colour;

        ZeroMemory(&szBuf, sizeof(szBuf));
        mc_OpenGL.BeginRasterText();

        glColor3f(m_Marker.Trace[Marker].Colour.r, m_Marker.Trace[Marker].Colour.g, m_Marker.Trace[Marker].Colour.b);
        sprintf_s(szBuf, sizeof(szBuf), "S11 %s %s", Header, m_MarkerText[Marker]);
        *OffSet -= 15.0f;
        mc_OpenGL.Print2DRaster(szBuf, 0.0f, *OffSet);

        glColor3f(1.0f, 1.0f, 1.0f);

        sprintf_s(szBuf, sizeof(szBuf), "Freq %.4f %s", mc_VnaMath.Value_ScaleFrequency(double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies)), mc_VnaMath.Value_UnitFrequency(double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies)));
        *OffSet -= 15.0f;
        mc_OpenGL.Print2DRaster(szBuf, 0.0f, *OffSet);

        sprintf_s(szBuf, sizeof(szBuf), "LogMag %.3f dbm", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Rv_LogMag);
        Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_LogMag];
        glColor3f(Colour.r, Colour.g, Colour.b);
        *OffSet -= 15.0f;
        mc_OpenGL.Print2DRaster(szBuf, 0.0f, *OffSet);

        sprintf_s(szBuf, sizeof(szBuf), "Swr %.4f", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].SWR);
        Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Swr];
        glColor3f(Colour.r, Colour.g, Colour.b);
        *OffSet -= 15.0f;
        mc_OpenGL.Print2DRaster(szBuf, 0.0f, *OffSet);

        mc_NanoVNA.GetCapacitanceInductance(szBuf, sizeof(szBuf), &mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Complex, mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies);
        Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Resitance];
        glColor3f(Colour.r, Colour.g, Colour.b);
        *OffSet -= 15.0f;
        mc_OpenGL.Print2DRaster(szBuf, 0.0f, *OffSet);

        sprintf_s(szBuf, sizeof(szBuf), "%.3f jx Ohms", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Real.Impedance_Resitance_jx);
        Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Reactance];
        glColor3f(Colour.r, Colour.g, Colour.b);
        *OffSet -= 15.0f;
        mc_OpenGL.Print2DRaster(szBuf, 0.0f, *OffSet);

        sprintf_s(szBuf, sizeof(szBuf), "%.3f Phase", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Phase);
        Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_S11_Phase];
        glColor3f(Colour.r, Colour.g, Colour.b);
        *OffSet -= 15.0f;
        mc_OpenGL.Print2DRaster(szBuf, 0.0f, *OffSet);

        glColor3f(m_Marker.Trace[Marker].Colour.r, m_Marker.Trace[Marker].Colour.g, m_Marker.Trace[Marker].Colour.b);
        sprintf_s(szBuf, sizeof(szBuf), "S21 %s", Header);
        *OffSet -= 15.0f;
        mc_OpenGL.Print2DRaster(szBuf, 0.0f, *OffSet);

        glColor3f(1.0f, 1.0f, 1.0f);

        sprintf_s(szBuf, sizeof(szBuf), "LogMag %.3f dbm", mc_NanoVNA.m_VNASample.Data_S21[MarkerIndex].Rv_LogMag);
        Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_S21_LogMag];
        glColor3f(Colour.r, Colour.g, Colour.b);
        *OffSet -= 15.0f;
        mc_OpenGL.Print2DRaster(szBuf, 0.0f, *OffSet);

        sprintf_s(szBuf, sizeof(szBuf), "%.3f Phase", mc_NanoVNA.m_VNASample.Data_S21[MarkerIndex].Phase);
        Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_S21_Phase];
        glColor3f(Colour.r, Colour.g, Colour.b);
        *OffSet -= 15.0f;
        mc_OpenGL.Print2DRaster(szBuf, 0.0f, *OffSet);


        *OffSet -= 15.0f;
        mc_OpenGL.EndRasterText();
    }
    //-Protected-------------------------------------------------------------------
    // Name: CartesianScale(GLfloat re, GLfloat im, GLfloat* xp, GLfloat* yp, GLfloat scale, GLfloat Width, GLfloat Height)
    //-----------------------------------------------------------------------------
    void MC_OpenGLChart::CartesianScale(GLfloat re, GLfloat im, GLfloat* xp, GLfloat* yp, GLfloat scale, GLfloat Width, GLfloat Height)
    {
        GLfloat Radius_x = (Width / 2.0f);
        GLfloat Radius_y = (Height / 2.0f);
        GLfloat x = (re * Radius_x * scale);
        GLfloat y = (im * Radius_y * scale);
        if (x < -Radius_x)
            x = -Radius_x;

        if (y < -Radius_y)
            y = -Radius_y;

        if (x > Radius_x)
            x = Radius_x;

        if (y > Radius_y)
            y = Radius_y;

        *xp = Width / 2.0f + x;
        *yp = Height / 2.0f - y;
    }
    // -Protected------------------------------------------------------------------
    // Name: GetPointPosition(double real, double imag, double xCenter, double yCenter, double minDimension)
    //-----------------------------------------------------------------------------
    OPENGL_VECTOR MC_OpenGLChart::GetPointPosition(double real, double imag, double xCenter, double yCenter, double minDimension)
    {
        OPENGL_VECTOR Vec;

        double realRadius = 1.0 / (1.0 + real) * (minDimension / 2.0);
        double realCenterX = xCenter + ((real / (1.0 + real)) * (minDimension / 2.0));
        double realCenterY = yCenter;
        double imagRadius = (1.0 / abs(imag)) * (minDimension / 2.0);
        double imagCenterX = xCenter + (minDimension / 2.0);
        double imagCenterY = imag > 0 ? yCenter - imagRadius : yCenter + imagRadius;
        double r0 = sqrt(pow(imagCenterX - realCenterX, 2) + pow(imagCenterY - realCenterY, 2));
        double angle = atan2(realCenterY - imagCenterY, realCenterX - imagCenterX);
        double arccos = acos((pow(imagRadius, 2) - pow(realRadius, 2)) / pow(r0, 2));
        double phi = imag > 0 ? 0.5 * arccos + angle : -0.5 * arccos + angle;
        Vec.x = GLfloat(imag == 0.0 ? realCenterX - realRadius : (cos(phi) * imagRadius) + imagCenterX);
        Vec.y = GLfloat(imag == 0.0 ? yCenter : (sin(phi) * imagRadius) + imagCenterY);
        return Vec;
    }












    //*****************************************************************************  
    // Callback Code
    //*****************************************************************************

    //-Public----------------------------------------------------------------------
    // Name: 
    //-----------------------------------------------------------------------------

};
