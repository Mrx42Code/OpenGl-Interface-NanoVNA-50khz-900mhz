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
// File: MC_OpenGLSmithChart.cpp: implementation of the MC_OpenGLSmithChart class.
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
    const GLfloat m_CircleLeSize[SMITH_MAXCIRCLE]   = { 2.381f ,4.5455f ,8.333f ,10.0f ,12.5f ,16.666f ,17.857f ,19.231f ,20.8335f ,22.7275f ,
                                                        25.0f ,26.316f ,27.778f ,29.412f ,31.25f ,33.333f ,35.7145f ,38.4615f ,41.666f ,45.4545f ,50.0f };

    const char*   m_CircleLeText[SMITH_MAXCIRCLE]   = { NULL ,"10.0" ,"5.0" ,NULL ,NULL ,"2.0" ,NULL ,NULL ,NULL ,NULL ,
                                                        "1.0" ,NULL ,NULL ,NULL ,NULL ,"0.5" ,NULL ,NULL ,"0.2" ,NULL ,"0.0" };

    const GLfloat m_ArcSize[SMITH_MAXARC]           = { 2.5f ,5.0f ,10.0f ,12.4f ,16.6f ,25.0f ,27.7f ,31.2f ,35.6f ,41.6f ,
                                                        50.0f ,55.5f ,62.5f ,71.5f ,83.0f ,100.0f ,125.0f ,166.6f ,250.0f ,500.0f };

    const GLfloat m_ArcLeftAngleCap[SMITH_MAXARC]   = { 185.5f ,191.0f ,202.5f ,207.5f ,216.5f ,233.0f ,237.5f ,243.5f ,250.5f ,259.0f ,
                                                        269.5f ,270.0f ,282.5f ,290.0f ,297.5f ,306.5f ,316.0f ,326.5f ,337.25f ,348.0f };

    const GLfloat m_ArcLeftAngleInd[SMITH_MAXARC]   = { 354.5f ,339.0f ,337.5f ,332.5f ,323.5f ,307.0f ,303.5f ,296.5f ,289.5f ,281.0f ,
                                                        270.5f ,270.0f ,257.5f ,250.0f ,242.5f ,233.5f ,224.0f ,213.5f ,202.5f ,192.0f };

    const GLfloat m_ArcRightAngleCap[SMITH_MAXARC]  = { 175.0f ,169.0f ,157.5f ,152.5f ,143.5f ,127.0f ,122.5f ,116.5f ,109.5f ,101.0f ,
                                                        90.5f ,90.0f ,77.5f ,70.0f ,62.5f ,53.5f ,44.0f ,33.5f ,22.75f ,27.0f };

    const GLfloat m_ArcRightAngleInd[SMITH_MAXARC]  = { 5.5f ,21.0f ,22.5f ,27.5f ,36.5f ,53.0f ,56.5f ,63.5f ,70.5f ,79.0f ,
                                                        89.5f ,90.0f ,102.5f ,110.0f ,117.5f ,126.5f ,136.0f ,146.5f ,157.5f ,155.0f };

    //-----------------------------------------------------------------------------
    // message handlers
    //-----------------------------------------------------------------------------

    //-Public----------------------------------------------------------------------
    // Name: MC_OpenGLSmithChart()
    // Desc: MC_OpenGLSmithChart class
    //-----------------------------------------------------------------------------
    MC_OpenGLSmithChart::MC_OpenGLSmithChart()
    {
        m_Enable = false;
        m_transparen = false;
    }
    //-Public----------------------------------------------------------------------
    // Name: ~GL_Skybox()
    // Desc: ~GL_Skybox Destruction class
    //-----------------------------------------------------------------------------
    MC_OpenGLSmithChart::~MC_OpenGLSmithChart()
    {
        UnLoad();
    }
    //-Public----------------------------------------------------------------------
    // Name: Init()
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::Init()
    {
        mc_OpenGLTextureSmithChart.LoadTextureFromDisk("Chart\\Smithchart4.bmp");
    }
    //-Public----------------------------------------------------------------------
    // Name: UnLoad()
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::UnLoad()
    {
        mc_OpenGLTextureSmithChart.FreeTexture();
    }
    //-Public----------------------------------------------------------------------
    // Name: DrawChart(OPENGL_GRIDDATA* GridData, OPENGL_MARKERS* Marker)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::DrawChart(OPENGL_GRIDDATA* GridData, OPENGL_MARKERS* Marker)
    {
        DrawChart_TopeLine(GridData, Marker->Trace[0].Index, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_White]);
        Translate3D(GridData);
        DrawChart_Smith_Red(GridData);
        DrawChart_Smith_Blue(GridData);
        DrawChart_TraceS11_Smith(GridData, Marker->Trace[0].Index, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Trace]);
        if (mc_Forms.m_MenuCheked.Chart_SmithChartImage) {
            DrawChart_Image(GridData);
        }
    }










    //*****************************************************************************  
    // Protected Code
    //*****************************************************************************

    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_Image(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::DrawChart_Image(OPENGL_GRIDDATA* GridData)
    {
        GLfloat OffSet_y = 0.0f;
        GLfloat OffSet_Z = 0.001f;

        if (m_Enable == false)
            return;

        OffSet_y = GetOffSet_Y(GridData);
        if (!mc_Forms.m_MenuCheked.Chart_2D3D) {
            if (GL_GRID_MAXCOLS == 10) {
                OffSet_Z = -0.001f;
            }
        }
        DrawChartImage(GridData, GridData->ScaleHalf_X, GridData->ScaleHalf_Y + OffSet_y, OffSet_Z, GridData->ScaleFull_X, GridData->ScaleFull_Y, 0.0f);
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_Smith_Red(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::DrawChart_Smith_Red(OPENGL_GRIDDATA* GridData)
    {
        FORM_COLOUR Colour;
        GLfloat OffSet_y;
        int i;

        if (!mc_Forms.m_MenuCheked.Chart_SmithChartRed) {
            return;
        }
        GridData->SmithChartRedBlue = false;
        for (i = 0; i < SMITH_MAXCIRCLE; i++) {
            if (m_CircleLeText[i] == NULL) {
                Colour.r = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red].r * 0.5f;
                Colour.g = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red].g * 0.5f;
                Colour.b = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red].b * 0.5f;
            } else {
                Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red];
            }
            DrawCircleLeft(GridData, GridData->HalfSize, GridData->Size - m_CircleLeSize[i], m_CircleLeSize[i], 180, &Colour, m_CircleLeText[i]);
        }
        for (i = 0; i < SMITH_MAXARC; i++) {
            if (m_CircleLeText[i] == NULL) {
                Colour.r = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red].r * 0.5f;
                Colour.g = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red].g * 0.5f;
                Colour.b = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red].b * 0.5f;
            } else {
                Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red];
            }
            DrawArcLeft(GridData, 50.0f + m_ArcSize[i], 100.0f, m_ArcSize[i], m_ArcLeftAngleCap[i], 360.0f, 1440, false, &Colour, m_CircleLeText[i]);
            DrawArcLeft(GridData, 50.0f - m_ArcSize[i], 100.0f, m_ArcSize[i], 180.0f, m_ArcLeftAngleInd[i], 1440, false, &Colour, m_CircleLeText[i]);
        }
        Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red];
        glColor3f(Colour.r, Colour.g, Colour.b);
        OffSet_y = GetOffSet_Y(GridData);
        Draw_2DLine(GridData->ScaleHalf_X, 0.0f + OffSet_y, GridData->ScaleHalf_X, GridData->ScaleFull_Y + OffSet_y);    //0
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_Smith_Blue(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::DrawChart_Smith_Blue(OPENGL_GRIDDATA* GridData)
    {
        FORM_COLOUR Colour;
        GLfloat OffSet_y;
        int i;

        if (!mc_Forms.m_MenuCheked.Chart_SmithChartBlue) {
            return;
        }
        GridData->SmithChartRedBlue = true;
        for (i = 0; i < SMITH_MAXCIRCLE; i++) {
            if (m_CircleLeText[i] == NULL) {
                Colour.r = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue].r * 0.5f;
                Colour.g = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue].g * 0.5f;
                Colour.b = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue].b * 0.5f;
            } else {
                Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue];
            }
            DrawCircleRight(GridData, GridData->HalfSize, m_CircleLeSize[i], m_CircleLeSize[i], 180, &Colour, m_CircleLeText[i]);
        }
        for (i = 0; i < SMITH_MAXARC; i++) {
            if (m_CircleLeText[i] == NULL) {
                Colour.r = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue].r * 0.5f;
                Colour.g = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue].g * 0.5f;
                Colour.b = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue].b * 0.5f;
            } else {
                Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue];
            }
            DrawArcRight(GridData, 50.0f + m_ArcSize[i], 0.0f, m_ArcSize[i], 0.0f, m_ArcRightAngleCap[i], 1440, false, &Colour, m_CircleLeText[i]);
            DrawArcRight(GridData, 50.0f - m_ArcSize[i], 0.0f, m_ArcSize[i], m_ArcRightAngleInd[i], 180.0f, 1440, false, &Colour, m_CircleLeText[i]);
        }
        Colour = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue];
        glColor3f(Colour.r, Colour.g, Colour.b);
        OffSet_y = GetOffSet_Y(GridData);
        Draw_2DLine(GridData->ScaleHalf_X, 0.0f + OffSet_y, GridData->ScaleHalf_X, GridData->ScaleFull_Y + OffSet_y); //0
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TopeLine(OPENGL_GRIDDATA* GridData, int MarkerIndex, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::DrawChart_TopeLine(OPENGL_GRIDDATA* GridData, int MarkerIndex, FORM_COLOUR* Colour)
    {
        GLfloat xx, yy;
        GLfloat xxt;
        char szBuf[64];

        ZeroMemory(&szBuf, sizeof(szBuf));
        xx = -0.2f * GridData->ScaleX;
        xxt = -0.4f * GridData->ScaleX;

        if (mc_Forms.m_MenuCheked.Chart_SmithChartRed == false && mc_Forms.m_MenuCheked.Chart_SmithChartBlue == false && mc_Forms.m_MenuCheked.Chart_SmithChartImage == false) {

        } else {
            if (mc_Forms.m_MenuCheked.Chart_SmithChartRed || (mc_Forms.m_MenuCheked.Chart_SmithChartImage == true && mc_Forms.m_MenuCheked.Chart_SmithChartImageRedBlue == false)) {
                if (mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Reactance <= 0.0) {
                    sprintf_s(szBuf, sizeof(szBuf), "%.6f Ohms >< %.6f Ohms Capacitance", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Resitance, mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Reactance);
                } else {
                    sprintf_s(szBuf, sizeof(szBuf), "%.6f Ohms >< %.6f Ohms Inductance", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Resitance, mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Reactance);
                }
                yy = GridData->ScaleFull_Y * 1.05f;
                Draw_3DText(szBuf, xx, yy, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red]);

                if (mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Reactance <= 0.0) {
                    sprintf_s(szBuf, sizeof(szBuf), "%.6f Resitance >< %.6f Capacitance", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Resitance / VNA_CHARACTERISTIC_IMPEDANCE, mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Reactance / VNA_CHARACTERISTIC_IMPEDANCE);
                } else {
                    sprintf_s(szBuf, sizeof(szBuf), "%.6f Resitance >< %.6f Inductance", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Resitance / VNA_CHARACTERISTIC_IMPEDANCE, mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Reactance / VNA_CHARACTERISTIC_IMPEDANCE);
                }
                yy = GridData->ScaleFull_Y * 1.45f;
                Draw_3DText(szBuf, xx, yy, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red]);
            }

            if (mc_Forms.m_MenuCheked.Chart_SmithChartBlue || (mc_Forms.m_MenuCheked.Chart_SmithChartImage == true && mc_Forms.m_MenuCheked.Chart_SmithChartImageRedBlue == true)) {
                if (mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Reactance <= 0.0) {
                    sprintf_s(szBuf, sizeof(szBuf), "%.6f Ohms >< %.6f Ohms Capacitance", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Resitance, mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Reactance);
                } else {
                    sprintf_s(szBuf, sizeof(szBuf), "%.6f Ohms >< %.6f Ohms Inductance", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Resitance, mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Reactance);
                }
                yy = GridData->ScaleFull_Y * 1.05f;
                Draw_3DText(szBuf, xxt, yy, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue]);

                if (mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Reactance <= 0.0) {
                    sprintf_s(szBuf, sizeof(szBuf), "%.6f Resitance >< %.6f Capacitance", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Resitance / VNA_CHARACTERISTIC_IMPEDANCE, mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Reactance / VNA_CHARACTERISTIC_IMPEDANCE);
                } else {
                    sprintf_s(szBuf, sizeof(szBuf), "%.6f Resitance >< %.6f Inductance", mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Resitance / VNA_CHARACTERISTIC_IMPEDANCE, mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Reactance / VNA_CHARACTERISTIC_IMPEDANCE);
                }
                yy = GridData->ScaleFull_Y * 1.45f;
                Draw_3DText(szBuf, xxt, yy, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue]);
            }

            if (mc_Forms.m_MenuCheked.Chart_SmithChartRed || (mc_Forms.m_MenuCheked.Chart_SmithChartImage == true && mc_Forms.m_MenuCheked.Chart_SmithChartImageRedBlue == false)) {
                ZeroMemory(&szBuf, sizeof(szBuf));
                if (mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Reactance <= 0.0) {
                    //mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Reactance = -55.0; // = 204pF
                    //mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies = 14200000;
                    double  Reactance = mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Reactance;
                    double  freq_Pi2 = (VNA_PI2 * (double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies) / 1.0));
                    double  Capacitance = -(1.0 / (freq_Pi2 * Reactance));
                    sprintf_s(szBuf, sizeof(szBuf), "%.6f %sF Capacitance >< %.4f %s", mc_VnaMath.Value_Scale(Capacitance), mc_VnaMath.Value_Unit(Capacitance), mc_VnaMath.Value_ScaleFrequency(double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies)), mc_VnaMath.Value_UnitFrequency(double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies)));
                } else {
                    //mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Reactance = 42.0; // = 471nH
                    //mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies = 14200000;
                    double  Reactance = mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Reactance;
                    double  freq_Pi2 = (VNA_PI2 * (double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies) / 1.0));
                    double  Inductance = (Reactance / freq_Pi2);
                    sprintf_s(szBuf, sizeof(szBuf), "%.6f %sH Inductance >< %.4f %s", mc_VnaMath.Value_Scale(Inductance), mc_VnaMath.Value_Unit(Inductance), mc_VnaMath.Value_ScaleFrequency(double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies)), mc_VnaMath.Value_UnitFrequency(double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies)));
                }
                yy = GridData->ScaleFull_Y * 1.8f;
                Draw_3DText(szBuf, xx, yy, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red]);
            }

            if (mc_Forms.m_MenuCheked.Chart_SmithChartBlue || (mc_Forms.m_MenuCheked.Chart_SmithChartImage == true && mc_Forms.m_MenuCheked.Chart_SmithChartImageRedBlue == true)) {
                ZeroMemory(&szBuf, sizeof(szBuf));
                if (mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Reactance <= 0.0) {
                    //mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Reactance = -55.0; // = 204pF
                    //mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies = 14200000;
                    double  Reactance = mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Reactance;
                    double  freq_Pi2 = (VNA_PI2 * (double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies) / 1.0));
                    double  Capacitance = -(1.0 / (freq_Pi2 * Reactance));
                    sprintf_s(szBuf, sizeof(szBuf), "%.6f %sF Capacitance >< %.4f %s", mc_VnaMath.Value_Scale(Capacitance), mc_VnaMath.Value_Unit(Capacitance), mc_VnaMath.Value_ScaleFrequency(double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies)), mc_VnaMath.Value_UnitFrequency(double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies)));
                } else {
                    //mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Reactance = 42.0; // = 471nH
                    //mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies = 14200000;
                    double  Reactance = mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Admittance_Reactance;
                    double  freq_Pi2 = (VNA_PI2 * (double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies) / 1.0));
                    double  Inductance = (Reactance / freq_Pi2);
                    sprintf_s(szBuf, sizeof(szBuf), "%.6f %sH Inductance >< %.4f %s", mc_VnaMath.Value_Scale(Inductance), mc_VnaMath.Value_Unit(Inductance), mc_VnaMath.Value_ScaleFrequency(double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies)), mc_VnaMath.Value_UnitFrequency(double(mc_NanoVNA.m_VNASample.Data_S11[MarkerIndex].Frequencies)));
                }
                yy = GridData->ScaleFull_Y * 1.8f;
                Draw_3DText(szBuf, xxt, yy, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue]);
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_TraceS11_Smith(OPENGL_GRIDDATA* GridData, int MarkerIndex, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::DrawChart_TraceS11_Smith(OPENGL_GRIDDATA* GridData, int MarkerIndex, FORM_COLOUR* Colour)
    {
        int i;
        int MarkerItem;
        GLfloat x, y;
        GLfloat lx, ly;
        GLfloat OffSet_y;
        char szBuf[64];
        OPENGL_VECTOR Vectors;

        OffSet_y = GetOffSet_Y(GridData);
        if (mc_Forms.m_MenuCheked.Chart_SmithChartRed == false && mc_Forms.m_MenuCheked.Chart_SmithChartBlue == false && mc_Forms.m_MenuCheked.Chart_SmithChartImage == false) {
            return;
        }
        ZeroMemory(&szBuf, sizeof(szBuf));
        glColor3f(Colour->r * 0.6f, Colour->g * 0.6f, Colour->b * 0.6f);
        for (i = 0; i < mc_OpenGLChart.m_NumberOfSamples; i++) {
            x = -(GLfloat(mc_NanoVNA.m_VNASample.Data_S11[i].Complex.Value2) * GridData->ScaleHalf_X) + GridData->ScaleHalf_X;
            y = (GLfloat(mc_NanoVNA.m_VNASample.Data_S11[i].Complex.Value1) * GridData->ScaleHalf_Y) + GridData->ScaleHalf_Y;
            DrawChart_Limit(GridData, &Vectors, x, y);
            x = Vectors.x;
            y = Vectors.y;

            if (i == 0) {
                lx = x; ly = y;
            }
            Draw_2DLine(lx, ly + OffSet_y, x, y + OffSet_y);
            lx = x; ly = y;
        }
        for (MarkerItem = 0; MarkerItem < GL_MAX_MARKER; MarkerItem++) {
            if (mc_OpenGLChart.m_Marker.Trace[MarkerItem].Enable) {
                x = -(GLfloat(mc_NanoVNA.m_VNASample.Data_S11[mc_OpenGLChart.m_Marker.Trace[MarkerItem].Index].Complex.Value2) * GridData->ScaleHalf_X) + GridData->ScaleHalf_X;
                y = (GLfloat(mc_NanoVNA.m_VNASample.Data_S11[mc_OpenGLChart.m_Marker.Trace[MarkerItem].Index].Complex.Value1) * GridData->ScaleHalf_Y) + GridData->ScaleHalf_Y;
                if (mc_NanoVNA.m_VNASample.Data_S11[mc_OpenGLChart.m_Marker.Trace[MarkerItem].Index].Reactance <= 0.0) {
                    sprintf_s(szBuf, sizeof(szBuf), "%.3f Res %.3f Cap", mc_NanoVNA.m_VNASample.Data_S11[mc_OpenGLChart.m_Marker.Trace[MarkerItem].Index].Resitance / VNA_CHARACTERISTIC_IMPEDANCE, mc_NanoVNA.m_VNASample.Data_S11[mc_OpenGLChart.m_Marker.Trace[MarkerItem].Index].Reactance / VNA_CHARACTERISTIC_IMPEDANCE);
                } else {
                    sprintf_s(szBuf, sizeof(szBuf), "%.3f Res %.3f Ind", mc_NanoVNA.m_VNASample.Data_S11[mc_OpenGLChart.m_Marker.Trace[MarkerItem].Index].Resitance / VNA_CHARACTERISTIC_IMPEDANCE, mc_NanoVNA.m_VNASample.Data_S11[mc_OpenGLChart.m_Marker.Trace[MarkerItem].Index].Reactance / VNA_CHARACTERISTIC_IMPEDANCE);
                }
                if (mc_OpenGLChart.m_Marker.Trace[MarkerItem].Index < mc_OpenGLChart.m_NumberOfSamples) {
                    Draw_3DMarker(szBuf, x, y + OffSet_y, 0.0f, Colour, MarkerItem);
                }
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChartImage(OPENGL_GRIDDATA* GridData, float x,float y,float z,float width,float height,float length)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::DrawChartImage(OPENGL_GRIDDATA* GridData, float x, float y, float z, float width, float height, float length)
    {
        FORM_COLOUR Colour;
        x = x - width / 2.0f;
        y = y - height / 2.0f;
        z = z - length / 2.0f;

        if (mc_Forms.m_MenuCheked.Chart_SmithChartImageRedBlue) {
            Colour.r = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue].r * 0.5f;
            Colour.g = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue].g * 0.5f;
            Colour.b = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Blue].b * 0.5f;
        } else {
            Colour.r = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red].r * 0.5f;
            Colour.g = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red].g * 0.5f;
            Colour.b = mc_Forms.m_Colours.Colourfloat[M_COLOUR_Smith_Red].b * 0.5f;
        }
        glColor3f(Colour.r, Colour.g, Colour.b);
        glPushMatrix();
        glDisable(GL_LIGHTING);
        glCullFace(GL_BACK);
        if (m_transparen) {
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_TEXTURE_2D);
        }
        glBindTexture(GL_TEXTURE_2D, mc_OpenGLTextureSmithChart.m_Texture.TextureID);
        glBegin(GL_QUADS);
        if (mc_Forms.m_MenuCheked.Chart_SmithChartImageRedBlue) {
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z);
        }
        else {
            glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
        }
        glEnd();
        if (m_transparen) {

            glDisable(GL_TEXTURE_2D);
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
            glDepthMask(GL_TRUE);
        }
        glCullFace(GL_FRONT);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawCircleRight(OPENGL_GRIDDATA* GridData, GLfloat cx, GLfloat cy, GLfloat radius, int num_segments, FORM_COLOUR* Colour, const char* Label)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::DrawCircleRight(OPENGL_GRIDDATA* GridData, GLfloat cx, GLfloat cy, GLfloat radius, int num_segments, FORM_COLOUR* Colour, const char* Label)
    {
        GLfloat theta;
        GLfloat angle;
        GLfloat x;
        GLfloat y;
        GLfloat OffSet_y;
        GLfloat OffSetTxt_y = 0.0f;
        double Value = 0.0;;
        char szBuf[64];

        OffSet_y = GetOffSet_Y(GridData);
        glColor3f(Colour->r, Colour->g, Colour->b);
        glBegin(GL_LINE_LOOP);
        for (int ii = 0; ii < num_segments; ii++) {
            theta = GL_PI2f * (float(ii) / float(num_segments));
            x = ((radius * -cosf(theta)) + cx) * GridData->Scale_X;
            y = ((radius * sinf(theta)) + cy) * GridData->Scale_Y;
            if (x >= 0.0f && y >= 0.0f && x <= GridData->ScaleFull_X && y <= GridData->ScaleFull_Y) {
                glColor3f(Colour->r, Colour->g, Colour->b);
                glVertex2f(x, y + OffSet_y);
            } else {
                glColor3f(0.0f, 0.0f, 0.0f);
                glVertex2f(x, y + OffSet_y);
            }
        }
        glEnd();
        if (Label != NULL) {
            cy = cy - 2.1f;
            for (int ii = 0; ii < num_segments; ii++) {
                theta = GL_PI2f * (float(ii) / float(num_segments));
                angle = (360.0f / GL_PI2f) * theta;
                x = ((radius * -cosf(theta)) + cx) * GridData->Scale_X;
                y = ((radius * sinf(theta)) + cy) * GridData->Scale_Y;
                if (angle == 90.0f && Label != NULL) {
                    Value = atof(Label);
                    if (Value == 0.0) {
                        OffSetTxt_y = 0.0225f + 0.01f;
                    } else {
                        OffSetTxt_y = 0.0225f;
                    }
                    sprintf_s(szBuf, sizeof(szBuf), "%s", Label);
                    Draw_3DText(szBuf, x, y + OffSet_y + OffSetTxt_y, 0.0f, Colour);
                }
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawCircleLeft(OPENGL_GRIDDATA* GridData, GLfloat cx, GLfloat cy, GLfloat radius, int num_segments, FORM_COLOUR* Colour, const char* Label)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::DrawCircleLeft(OPENGL_GRIDDATA* GridData, GLfloat cx, GLfloat cy, GLfloat radius, int num_segments, FORM_COLOUR* Colour, const char* Label)
    {
        GLfloat theta;
        GLfloat angle;
        GLfloat x;
        GLfloat y;
        GLfloat OffSet_y;
        GLfloat OffSetTxt_y = 0.0f;
        double Value = 0.0;;
        char szBuf[64];

        OffSet_y = GetOffSet_Y(GridData);
        glColor3f(Colour->r, Colour->g, Colour->b);
        glBegin(GL_LINE_LOOP);
        for (int ii = 0; ii < num_segments; ii++) {
            theta = GL_PI2f * (float(ii) / float(num_segments));
            x = ((radius * -cosf(theta)) + cx) * GridData->Scale_X;
            y = ((radius * sinf(theta)) + cy) * GridData->Scale_Y;
            if (x >= 0.0f && y >= 0.0f && x <= GridData->ScaleFull_X && y <= GridData->ScaleFull_Y) {
                glColor3f(Colour->r, Colour->g, Colour->b);
                glVertex2f(x, y + OffSet_y);
            } else {
                glColor3f(0.0f, 0.0f, 0.0f);
                glVertex2f(x, y + OffSet_y);
            }
        }
        glEnd();
        if (Label != NULL) {
            cy = cy - 2.1f;
            for (int ii = 0; ii < num_segments; ii++) {
                theta = GL_PI2f * (float(ii) / float(num_segments));
                angle = (360.0f / GL_PI2f) * theta;
                x = ((radius * -cosf(theta)) + cx) * GridData->Scale_X;
                y = ((radius * sinf(theta)) + cy) * GridData->Scale_Y;
                if (angle == 270.0f && Label != NULL) {
                    Value = atof(Label);
                    if (Value == 0.0) {
                        OffSetTxt_y = 0.0f - 0.01f;
                    } else {
                        OffSetTxt_y = 0.0f;
                    }
                    sprintf_s(szBuf, sizeof(szBuf), "%s", Label);
                    Draw_3DText(szBuf, x, y + OffSet_y + OffSetTxt_y, 0.0f, Colour);
                }
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawArcLeft(OPENGL_GRIDDATA* GridData, GLfloat cx, GLfloat cy, GLfloat radius, float start_angle, float arc_angle, int num_segments, bool Debug, FORM_COLOUR* Colour, const char* Label)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::DrawArcLeft(OPENGL_GRIDDATA* GridData, GLfloat cx, GLfloat cy, GLfloat radius, GLfloat start_angle, GLfloat arc_angle, int num_segments, bool Debug, FORM_COLOUR* Colour, const char* Label)
    {
        int ii;
        GLfloat theta;
        GLfloat x;
        GLfloat y;
        GLfloat xx = 0.0f;
        GLfloat yy = 0.0f;
        GLfloat angle;
        int Labelangle;
        GLfloat OffSet_y;
        char szBuf[64];

        ZeroMemory(&szBuf, sizeof(szBuf));
        OffSet_y = GetOffSet_Y(GridData);
        GLfloat Start = (360.0f - arc_angle - 1.0f) * (GLfloat(num_segments) / 360.0f);
        GLfloat End = (360.0f - start_angle + 1.0f) * (GLfloat(num_segments) / 360.0f);
        glColor3f(Colour->r, Colour->g, Colour->b);
        for (ii = int(Start); ii < int(End); ii++) {
            theta = GL_PI2f - (GL_PI2f * (GLfloat(ii) / GLfloat(num_segments)));
            angle = (360.0f / GL_PI2f) * theta;
            x = ((radius * -cosf(theta)) + cx) * GridData->Scale_X;
            y = ((radius * sinf(theta)) + cy) * GridData->Scale_Y;
            if (x >= 0.0f && y >= 0.0f && x <= GridData->ScaleFull_X && y <= GridData->ScaleFull_Y) {
                if (angle >= start_angle && angle <= arc_angle) {
                    if (xx == 0.0f && yy == 0.0f) {
                        xx = x;
                        yy = y + OffSet_y;
                    }
                    glBegin(GL_LINES);
                    glVertex2f(xx, yy);
                    glVertex2f(x, y + OffSet_y);
                    glEnd();
                    xx = x;
                    yy = y + OffSet_y;
                }
            }
        }
        if (Label != NULL) {
            Labelangle = 0;
            if (start_angle != 180.0f) {
                Labelangle = int(End) - 1;
            } else if (arc_angle != 360.0f) {
                Labelangle = int(Start);
            }
            for (ii = int(Start); ii < int(End); ii++) {
                theta = GL_PI2f - (GL_PI2f * (GLfloat(ii) / GLfloat(num_segments)));
                angle = (360.0f / GL_PI2f) * theta;
                x = (((radius * -cosf(theta)) + cx) * GridData->Scale_X);
                y = (((radius * sinf(theta)) + cy) * GridData->Scale_Y);
                if (ii == Labelangle) {
                    sprintf_s(szBuf, sizeof(szBuf), "%s", Label);
                    Draw_3DText(szBuf, x, y + OffSet_y, 0.0f, Colour);
                }
            }
        }
        if (Debug) {
            for (ii = int(Start); ii < int(End); ii++) {
                theta = GL_PI2f - (GL_PI2f * (GLfloat(ii) / GLfloat(num_segments)));
                angle = (360.0f / GL_PI2f) * theta;
                x = (((radius * -cosf(theta)) + cx) * GridData->Scale_X);
                y = (((radius * sinf(theta)) + cy) * GridData->Scale_Y);
                sprintf_s(szBuf, sizeof(szBuf), "< %.3f", angle);
                Draw_3DText(szBuf, x, y + OffSet_y, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_White]);

            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawArcRight(OPENGL_GRIDDATA* GridData, GLfloat cx, GLfloat cy, GLfloat radius, float start_angle, float arc_angle, int num_segments, bool Debug, FORM_COLOUR* Colour, const char* Label)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::DrawArcRight(OPENGL_GRIDDATA* GridData, GLfloat cx, GLfloat cy, GLfloat radius, GLfloat start_angle, GLfloat arc_angle, int num_segments, bool Debug, FORM_COLOUR* Colour, const char* Label)
    {
        int ii;
        GLfloat theta;
        GLfloat x;
        GLfloat y;
        GLfloat xx = 0.0f;
        GLfloat yy = 0.0f;
        GLfloat angle;
        int Labelangle;
        GLfloat OffSet_y;
        char szBuf[64];

        ZeroMemory(&szBuf, sizeof(szBuf));
        OffSet_y = GetOffSet_Y(GridData);
        GLfloat Start = (360.0f - arc_angle - 1.0f) * (GLfloat(num_segments) / 360.0f);
        GLfloat End = (360.0f - start_angle + 1.0f) * (GLfloat(num_segments) / 360.0f);
        glColor3f(Colour->r, Colour->g, Colour->b);
        for (ii = int(Start); ii < int(End); ii++) {
            theta = GL_PI2f - (GL_PI2f * (GLfloat(ii) / GLfloat(num_segments)));
            angle = (360.0f / GL_PI2f) * theta;
            x = ((radius * -cosf(theta)) + cx) * GridData->Scale_X;
            y = ((radius * sinf(theta)) + cy) * GridData->Scale_Y;
            if (x >= 0.0f && y >= 0.0f && x <= GridData->ScaleFull_X && y <= GridData->ScaleFull_Y) {
                if (angle >= start_angle && angle <= arc_angle) {
                    if (xx == 0.0f && yy == 0.0f) {
                        xx = x;
                        yy = y + OffSet_y;
                    }
                    glBegin(GL_LINES);
                    glVertex2f(xx, yy);
                    glVertex2f(x, y + OffSet_y);
                    glEnd();
                    xx = x;
                    yy = y + OffSet_y;
                }
            }
        }
        glEnd();
        if (Label != NULL) {
            Labelangle = 0;
            if (start_angle != 0.0f) {
                Labelangle = int(End) - 1;
            } else if (arc_angle != 180.0f) {
                Labelangle = int(Start);
            }
            for (ii = int(Start); ii < int(End); ii++) {
                theta = GL_PI2f - (GL_PI2f * (GLfloat(ii) / GLfloat(num_segments)));
                angle = (360.0f / GL_PI2f) * theta;
                x = (((radius * -cosf(theta)) + cx) * GridData->Scale_X);
                y = (((radius * sinf(theta)) + cy) * GridData->Scale_Y);
                if (ii == Labelangle) {
                    sprintf_s(szBuf, sizeof(szBuf), "%s", Label);
                    Draw_3DText(szBuf, x, y + OffSet_y, 0.0f, Colour);
                }
            }
        }
        if (Debug) {
            for (ii = int(Start); ii < int(End); ii++) {
                theta = GL_PI2f - (GL_PI2f * (GLfloat(ii) / GLfloat(num_segments)));
                angle = (360.0f / GL_PI2f) * theta;
                x = (((radius * -cosf(theta)) + cx) * GridData->Scale_X);
                y = (((radius * sinf(theta)) + cy) * GridData->Scale_Y);
                sprintf_s(szBuf, sizeof(szBuf), "< %.3f", angle);
                Draw_3DText(szBuf, x, y + OffSet_y, 0.0f, &mc_Forms.m_Colours.Colourfloat[M_COLOUR_White]);
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: Draw_2DLine(GLfloat xx, GLfloat yy, GLfloat x, GLfloat y)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::Draw_2DLine(GLfloat xx, GLfloat yy, GLfloat x, GLfloat y)
    {
        glBegin(GL_LINES);
        glVertex2f(xx, yy);
        glVertex2f(x, y);
        glEnd();
    }
    //-Protected-------------------------------------------------------------------
    // Name: Draw_3DText(char* Buffer, GLfloat x, GLfloat y, GLfloat z, FORM_COLOUR* Colour)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::Draw_3DText(char* Buffer, GLfloat x, GLfloat y, GLfloat z, FORM_COLOUR* Colour)
    {
        glColor3f(Colour->r, Colour->g, Colour->b);
        glPushMatrix();
        glVertex3f(0.0f, 0.0f, 0.0f);
        glTranslatef(x, y, z);
        Draw_3DString(Buffer);
        glPopMatrix();
    }
    //-Protected-------------------------------------------------------------------
    // Name: Draw_3DMarker(char* Buffer, GLfloat x, GLfloat y, GLfloat z, FORM_COLOUR* Colour int MarkerIndex)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::Draw_3DMarker(char* Buffer, GLfloat x, GLfloat y, GLfloat z, FORM_COLOUR* Colour, int MarkerIndex)
    {
        char szBuf[64];

        ZeroMemory(&szBuf, sizeof(szBuf));
        glColor3f(mc_OpenGLChart.m_Marker.Trace[MarkerIndex].Colour.r, mc_OpenGLChart.m_Marker.Trace[MarkerIndex].Colour.g, mc_OpenGLChart.m_Marker.Trace[MarkerIndex].Colour.b);
        glPushMatrix();
        glVertex3f(0.0f, 0.0f, 0.0f);
        glTranslatef(x, y, z);
        sprintf_s(szBuf, sizeof(szBuf), "<");
        Draw_3DString(szBuf);
        sprintf_s(szBuf, sizeof(szBuf), "-");
        Draw_3DString(szBuf);
        glPopMatrix();

        glColor3f(Colour->r, Colour->g, Colour->b);
        sprintf_s(szBuf, sizeof(szBuf), "   %s", Buffer);
        glPushMatrix();
        glVertex3f(0.0f, 0.0f, 0.0f);
        glTranslatef(x, y, z);
        Draw_3DString(szBuf);
        glPopMatrix();
    }
    //-Protected-------------------------------------------------------------------
    // Name: Draw_3DString(char* Buffer)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::Draw_3DString(char* Buffer)
    {
        glRasterPos3f(0.0f, 0.0f, 0.0f);
        glListBase(mc_OpenGL.m_Fonts.Font2D);
        glCallLists(GLsizei(strlen(Buffer)), GL_UNSIGNED_BYTE, Buffer);
    }
    //-Protected-------------------------------------------------------------------
    // Name: GetOffSet_Y(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    GLfloat MC_OpenGLSmithChart::GetOffSet_Y(OPENGL_GRIDDATA* GridData)
    {
        GLfloat OffSet_y = 0.0f;

        if (!mc_Forms.m_MenuCheked.Chart_2D3D) {
            if (GL_GRID_MAXCOLS == 20) {
                OffSet_y = 0.05f + (GridData->ScaleHalf_Y * 2.0f);
            } else if (GL_GRID_MAXCOLS == 10) {
                OffSet_y = 0.1f + (GridData->ScaleHalf_Y * 2.0f);
            } else {
                OffSet_y = 0.05f + (GridData->ScaleHalf_Y * 2.0f);
            }
        }
        return OffSet_y;
    }
    //-Protected-------------------------------------------------------------------
    // Name: Translate3D(OPENGL_GRIDDATA* GridData)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::Translate3D(OPENGL_GRIDDATA* GridData)
    {
        if (mc_Forms.m_MenuCheked.Chart_2D3D) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glRotatef(270.0f, 0.0f, 0.0f, 1.0f);
            if (GL_GRID_MAXCOLS == 20) {
                glTranslatef(-(GridData->ScaleHalf_X + 0.02f), -(GridData->ScaleHalf_Y - 0.05f), 0.0f);
            } else if (GL_GRID_MAXCOLS == 10) {
                glTranslatef(-(GridData->ScaleHalf_X + 0.02f), +0.1f, 0.0f);
            } else {
                glTranslatef(-(GridData->ScaleHalf_X + 0.02f), -(GridData->ScaleHalf_Y - 0.05f), 0.0f);
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: DrawChart_Limit(OPENGL_GRIDDATA* GridData, OPENGL_VECTOR* Vectors, GLfloat x, GLfloat y)
    //-----------------------------------------------------------------------------
    void MC_OpenGLSmithChart::DrawChart_Limit(OPENGL_GRIDDATA* GridData, OPENGL_VECTOR* Vectors, GLfloat x, GLfloat y)
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
}