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
// File: MC_OpenGLChart.h: interface for the MC_OpenGLChart class.
// Desc: Application interface for the MC_OpenGLChart class.
//-----------------------------------------------------------------------------
#ifndef MC_OpenGLChart_H
	#define MC_OpenGL_H

#pragma once

#include "CommanStruct.h"

#include <gl\gl.h>
#include <gl\glu.h>

namespace NanoVNA {
    //-----------------------------------------------------------------------------
    // Const
    //-----------------------------------------------------------------------------
    #define GL_3D2D_VIEW		    true

    #define GL_GRID_MAXROWS		    10
    #define GL_GRID_MAXCOLS		    10
    #define GL_GRID_ROWFINELINES	(GL_GRID_MAXROWS * 10)
    #define GL_GRID_COLFINELINES	(GL_GRID_MAXCOLS * 10)

    #define	GL_GRID_X10OfSamples	100
    #define	GL_GRID_NumberOfSamples	(102 * GL_GRID_X10OfSamples)

    #define GL_PI					3.14159265358979323846
    #define GL_PI2					(GL_PI * 2.0)

    #define GL_PIf					float(GL_PI)
    #define GL_PI2f					float(GL_PI2)

    #define GL_MAX_MARKER           6

    #define GL_MARKER_MAX_VALUE     4
    #define GL_MARKER_MIN_VALUE     5
    //-----------------------------------------------------------------------------
    // Struct
    //-----------------------------------------------------------------------------
    enum {
        M_DATA0, M_DATA1, M_GRID , M_GRIDFINE , M_SMITH , M_S11_LogMag , M_S21_LogMag , M_S11_Swr, M_S11_Resitance , M_S11_Reactance, M_S11_Phase, M_S21_Phase,
        M_SWEEPUPDATE, M_SPANUPDATE , M_Refresh, M_Connect , M_AverageClear , M_OpenFile, M_SaveFile , M_Marker1 , M_Marker2, M_Marker3, M_FreqSegment, M_Clear, M_SelectColour
    };

    typedef struct _OPENGL_VECTOR
    {
        GLfloat             x;
        GLfloat             y;
    } OPENGL_VECTOR;

    typedef struct _OPENGL_VECTORS
    {
        GLfloat             Start;
        GLfloat             End;
    } OPENGL_VECTORS;

    typedef struct _OPENGL_GRIDROWFINE
    {
        FORM_COLOUR         Colour;
        OPENGL_VECTORS      FixedLen;
        GLfloat             OffSet[GL_GRID_ROWFINELINES + 1];
        int                 Count;
    } OPENGL_GRIDROWFINE;

    typedef struct _OPENGL_GRIDCOLFINE
    {
        FORM_COLOUR         Colour;
        OPENGL_VECTORS      FixedLen;
        GLfloat             OffSet[GL_GRID_COLFINELINES + 1];
        int                 Count;
    } OPENGL_GRIDCOLFINE;

    typedef struct _OPENGL_GRIDROW
    {
        FORM_COLOUR         Colour;
        OPENGL_VECTORS      FixedLen;
        GLfloat             OffSet[GL_GRID_MAXROWS + 1];
        int                 Count;
    } OPENGL_GRIDROW;

    typedef struct _OPENGL_GRIDCOL
    {
        FORM_COLOUR         Colour;
        OPENGL_VECTORS      FixedLen;
        GLfloat             OffSet[GL_GRID_MAXCOLS + 1];
        int                 Count;
    } OPENGL_GRIDCOL;

    typedef struct _OPENGL_GRIDINFO
    {
        OPENGL_GRIDROW      Rows;
        OPENGL_GRIDCOL      Cols;
        OPENGL_GRIDROWFINE  RowsFine;
        OPENGL_GRIDCOLFINE  ColsFine;
    } OPENGL_GRIDINFO;

    typedef struct _OPENGL_TRACEINFO
    {
        OPENGL_VECTOR       S11_Swr[GL_GRID_NumberOfSamples];
        OPENGL_VECTOR       S11_Phase[GL_GRID_NumberOfSamples];
        OPENGL_VECTOR       S11_Reactance[GL_GRID_NumberOfSamples];
        OPENGL_VECTOR       S11_Resitance[GL_GRID_NumberOfSamples];
        OPENGL_VECTOR       S11_LogMag[GL_GRID_NumberOfSamples];
        OPENGL_VECTOR       S11_LinMag[GL_GRID_NumberOfSamples];
        OPENGL_VECTOR       S21_LogMag[GL_GRID_NumberOfSamples];
        OPENGL_VECTOR       S21_LinMag[GL_GRID_NumberOfSamples];
        OPENGL_VECTOR       S21_Phase[GL_GRID_NumberOfSamples];
        OPENGL_VECTOR       S11_Smith[GL_GRID_NumberOfSamples];
    } OPENGL_TRACEINFO;

    typedef struct _OPENGL_GRIDDATA
    {
        GLfloat             Size;
        GLfloat             Rows;
        GLfloat             Cols;
        GLfloat             HalfSize;
        GLfloat             ScaleX;
        GLfloat             ScaleY;
        GLfloat             Scale_X;
        GLfloat             Scale_Y;
        GLfloat             ScaleHalf_X;
        GLfloat             ScaleFull_X;
        GLfloat             ScaleHalf_Y;
        GLfloat             ScaleFull_Y;
        bool                SmithChartRedBlue;
    } OPENGL_GRIDDATA;

    typedef struct _OPENGL_MARKERCOLOUR
    {
        GLfloat             r;
        GLfloat             g;
        GLfloat             b;
    } OPENGL_MARKERCOLOUR;

    typedef struct _OPENGL_MARKER
    {
        int                 Index;
        int                 Enable;
        uint32_t			Frequencies;
        OPENGL_MARKERCOLOUR Colour;
    } OPENGL_MARKER;

    typedef struct _OPENGL_MARKERS
    {
        OPENGL_MARKER       Trace[GL_MAX_MARKER];
    } OPENGL_MARKERS;

    typedef struct _OPENGL_DATAUPDATE
    {
        bool                Update;
        int                 S11;
        int                 S21;
    } OPENGL_DATAUPDATE;

    typedef struct _OPENGL_DATATYPEUPDATE
    {
        bool                Update;
        int                 Type;
    } OPENGL_DATATYPEUPDATE;

    //-Public----------------------------------------------------------------------
    // Callback
    //-----------------------------------------------------------------------------
    static void             Form_CallBack(int control);
    static void             FormIdle_CallBack();

    //-----------------------------------------------------------------------------
    // Name: class MC_OpenGLChart
    // Desc: Application class
    //-----------------------------------------------------------------------------
    class MC_OpenGLChart
    {

        public:
            char                m_StringBuffer[256];
            float               m_X10Samples;
            float               m_NumberOfSamples;
            OPENGL_DATAUPDATE   m_GetVNAData;
            OPENGL_DATATYPEUPDATE m_GetVNADataType;
            OPENGL_DATATYPEUPDATE m_GetVNARecall;
            OPENGL_MARKERS      m_Marker;
            OPENGL_GRIDINFO     m_GridInfo;
            OPENGL_GRIDDATA     m_GridData;
            OPENGL_TRACEINFO    m_Trace;

        protected:

        private:

            //-----------------------------------------------------------------------------

        public:
                                MC_OpenGLChart();
            virtual				~MC_OpenGLChart();
            void                Init();
            void                Tick();
            void                DrawChart();
            void                DrawSceneRasterText();
            void                DrawCube();
            void                OnCreate();
            void                FormCallBack(int control);
            void                GetVNADataRecall(int DataType);
            void                GetVNAData(int S11, int S21);
            void                GetVNADataVarType(int DataType);
            int                 OpenVNAData(const char* FileName);
            int                 SaveVNAData(const char* FileName);
            void                Calculate_Grid(OPENGL_GRIDDATA* GridData);
            void                Calculate_Trace_All(OPENGL_GRIDDATA* GridData);
            void                Calculate_Marker_MinMax();

        protected:
            void                Calculate_TraceS11_Swr(OPENGL_GRIDDATA* GridData);
            void                Calculate_TraceS11_Phase(OPENGL_GRIDDATA* GridData);
            void                Calculate_TraceS11_Reactance(OPENGL_GRIDDATA* GridData);
            void                Calculate_TraceS11_Resitance(OPENGL_GRIDDATA* GridData);
            void                Calculate_TraceS11_LogMag(OPENGL_GRIDDATA* GridData);
            void                Calculate_TraceS11_LinMag(OPENGL_GRIDDATA* GridData);
            void                Calculate_TraceS21_LogMag(OPENGL_GRIDDATA* GridData);
            void                Calculate_TraceS21_LinMag(OPENGL_GRIDDATA* GridData);
            void                Calculate_TraceS21_Phase(OPENGL_GRIDDATA* GridData);
            void                Calculate_TraceS11_Smith(OPENGL_GRIDDATA* GridData);
            void                Calculate_TraceS11_Oscilloscope(OPENGL_GRIDDATA* GridData);
            void                DrawChart_Grid();
            void                DrawChart_TraceS11_Frequencies(OPENGL_GRIDDATA* GridData, int MarkerIndex, FORM_COLOUR* Colour);
            void                DrawChart_TraceS11_Swr(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour);
            void                DrawChart_TraceS11_Phase(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour);
            void                DrawChart_TraceS11_Reactance(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour);
            void                DrawChart_TraceS11_Resitance(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour);
            void                DrawChart_TraceS11_LogMag(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour);
            void                DrawChart_TraceS11_LinMag(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour);
            void                DrawChart_TraceS21_LogMag(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour);
            void                DrawChart_TraceS21_LinMag(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour);
            void                DrawChart_TraceS21_Phase(OPENGL_MARKERS* Marker, FORM_COLOUR* Colour);
            void                DrawChart_TopeLine(OPENGL_GRIDDATA* GridData, int MarkerIndex, FORM_COLOUR* Colour);
            void                DrawChart_Scale(OPENGL_GRIDDATA* GridData, GLfloat Rows, GLfloat Cols, GLfloat ScaleX, GLfloat ScaleY);
            void                Draw_2DLine(GLfloat xx, GLfloat yy, GLfloat x, GLfloat y);
            void                Draw_3DText(char* Buffer, GLfloat x, GLfloat y, GLfloat z, FORM_COLOUR* Colour);
            void                Draw_3DMarker(char* Buffer, GLfloat x, GLfloat y, GLfloat z, FORM_COLOUR* Colour, int MarkerIndex);
            void                Draw_3DString(char* Buffer);
            void                Draw_MarkerText(const char* Header, float* OffSet, int Marker, int MarkerIndex);
            void                Translate3d(OPENGL_GRIDDATA* GridData);
            void                DrawChart_Limit(OPENGL_GRIDDATA* GridData, OPENGL_VECTOR* Vectors, GLfloat x, GLfloat y);
            void                CartesianScale(GLfloat re, GLfloat im, GLfloat* xp, GLfloat* yp, GLfloat scale, GLfloat Width, GLfloat Height);
            OPENGL_VECTOR       GetPointPosition(double real, double imag, double xCenter, double yCenter, double minDimension);

        private:

    };
};
//-----------------------------------------------------------------------------
#endif // MC_OpenGLChart_H
