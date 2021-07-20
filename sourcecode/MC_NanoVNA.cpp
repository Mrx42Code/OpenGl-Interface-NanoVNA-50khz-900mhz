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
// File: MC_NanoVNA.cpp: implementation of the MC_NanoVNA class.
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
    const char* m_Commands[MSG_MAX_Commands] = { "Commands", "help", "exit", "info", "echo", "systime", "threads", "reset", "freq", "offset", "time", "dac", "saveconfig", "clearconfig", "data", "dump",
                                                 "frequencies", "port", "stat", "gain", "power", "sweep", "test", "touchcal", "touchtest", "pause", "resume", "cal", "save", "recall", "trace", "marker", "edelay" };

    //-----------------------------------------------------------------------------
    // message handlers
    //-----------------------------------------------------------------------------

    //-Public----------------------------------------------------------------------
    // Name: MC_NanoVNA()
    // Desc: MC_NanoVNA class
    //-----------------------------------------------------------------------------
    MC_NanoVNA::MC_NanoVNA()
    {
        int i = 0;

        memset(&m_VNAMultiData, 0, sizeof(m_VNAMultiData));
        memset(&m_VNAData, 0, sizeof(m_VNAData));
        memset(&m_VNASample, 0, sizeof(m_VNASample));
        m_SweepDone = false;
        m_String.Line.clear();
        m_String.Line.erase();
        for (i = 0; i < MSG_MAXITEMS; i++) {
            m_String.Items[i].clear();
            m_String.Items[i].erase();
        }
        m_String.ItemCount = 0;
        m_String.Mode = MSG_Commands;
        m_String.SubMode = 0;

        m_X10Samples = 1;
        m_NumberOfSamples = VNA_SamplesPerScan;
        m_Update = false;
    }
    //-Public----------------------------------------------------------------------
    // Name: ~MC_NanoVNA()
    // Desc: ~MC_NanoVNA Destruction class
    //-----------------------------------------------------------------------------
    MC_NanoVNA::~MC_NanoVNA()
    {

    }
    //-Public----------------------------------------------------------------------
    // Name: GetCapacitanceInductance(char* buf, int len, VNARAWVALUES* Complex, uint32_t Frequency)
    //-----------------------------------------------------------------------------
    void MC_NanoVNA::GetCapacitanceInductance(char* buf, int len, VNARAWVALUES* Complex, uint32_t Frequency)
    {
        mc_VnaMath.Gamma2CapacitanceInductance(buf, len, Complex, Frequency);
    }
    //-Public----------------------------------------------------------------------
    // Name: Calculate_MultiSweep(uint32_t StartFrequency, uint32_t StopFrequency, int NumberOfSweeps)
    //-----------------------------------------------------------------------------
    void  MC_NanoVNA::Calculate_MultiSweep(uint32_t StartFrequency, uint32_t StopFrequency, int NumberOfSweeps)
    {
        int i;
        uint32_t SpanFrequency = (StopFrequency - StartFrequency);
        uint32_t SpanStepsFrequency = (SpanFrequency / NumberOfSweeps);
        uint32_t SpanStartFrequency;
        uint32_t SpanStopFrequency;

        m_VNAMultiData.NumberOfMultiSweep = NumberOfSweeps;
        for (i = 0; i < NumberOfSweeps; i++) {
            SpanStartFrequency = StartFrequency + (SpanStepsFrequency * i);
            SpanStopFrequency = SpanStartFrequency + SpanStepsFrequency;
            m_VNAMultiData.MultSweep[i].StartFrequency = SpanStartFrequency;
            m_VNAMultiData.MultSweep[i].StopFrequency = SpanStopFrequency;
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: DoRecall(int DataType)
    //-----------------------------------------------------------------------------
    void  MC_NanoVNA::DoRecall(int DataType)
    {
        char Buffer[256];

        memset(&Buffer, 0, sizeof(Buffer));
        sprintf_s(Buffer, sizeof(Buffer), "recall %ld \r\n", DataType);
        mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, Buffer);
        Sleep(1200);
    }
    //-Public----------------------------------------------------------------------
    // Name: DoMultiSweep(int S11 , int S21)
    //-----------------------------------------------------------------------------
    void  MC_NanoVNA::DoMultiSweep(int S11, int S21)
    {
        int i;
        char Buffer[256];

        memset(&Buffer, 0, sizeof(Buffer));
        m_VNAMultiData.Stop = false;
        for (i = 0; i < m_VNAMultiData.NumberOfMultiSweep; i++) {
            if (m_VNAMultiData.Stop) {
                break;
            }
            m_VNAMultiData.MultiSweepIndex = i;
            sprintf_s(Buffer, sizeof(Buffer), "sweep %ld %ld\r\n", m_VNAMultiData.MultSweep[i].StartFrequency, m_VNAMultiData.MultSweep[i].StopFrequency);
            printf("%s", Buffer);
            mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, Buffer);
            Sleep(500);
            if (m_VNAMultiData.MultSweep[i].VNAStartFrequency != m_VNAMultiData.MultSweep[i].StartFrequency || m_VNAMultiData.MultSweep[i].VNAStopFrequency != m_VNAMultiData.MultSweep[i].StopFrequency) {
                m_VNAMultiData.MultSweep[i].VNAStartFrequency = m_VNAMultiData.MultSweep[i].StartFrequency;
                m_VNAMultiData.MultSweep[i].VNAStopFrequency = m_VNAMultiData.MultSweep[i].StopFrequency;
                mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, "frequencies\r\n");
                Sleep(1200);
            }
            if (S11) {
                mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, "data 0\r\n");
                Sleep(1200);
            }
            if (S21) {
                mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, "data 1\r\n");
                Sleep(1200);
            }
        }
        m_SweepDone = true;
    }
    //-Public----------------------------------------------------------------------
    // Name: DoMultiSweepDataType(int DataType)
    //-----------------------------------------------------------------------------
    void MC_NanoVNA::DoMultiSweepDataType(int DataType)
    {
        int i;
        char Buffer[256];

        memset(&Buffer, 0, sizeof(Buffer));
        m_VNAMultiData.Stop = false;
        for (i = 0; i < m_VNAMultiData.NumberOfMultiSweep; i++) {
            if (m_VNAMultiData.Stop) {
                break;
            }
            m_VNAMultiData.MultiSweepIndex = i;
            sprintf_s(Buffer, sizeof(Buffer), "sweep %ld %ld\r\n", m_VNAMultiData.MultSweep[i].StartFrequency, m_VNAMultiData.MultSweep[i].StopFrequency);
            printf("%s", Buffer);
            mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, Buffer);
            Sleep(500);
            if (m_VNAMultiData.MultSweep[i].VNAStartFrequency != m_VNAMultiData.MultSweep[i].StartFrequency || m_VNAMultiData.MultSweep[i].VNAStopFrequency != m_VNAMultiData.MultSweep[i].StopFrequency) {
                m_VNAMultiData.MultSweep[i].VNAStartFrequency = m_VNAMultiData.MultSweep[i].StartFrequency;
                m_VNAMultiData.MultSweep[i].VNAStopFrequency = m_VNAMultiData.MultSweep[i].StopFrequency;
                mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, "frequencies\r\n");
                Sleep(1200);
            }
            sprintf_s(Buffer, sizeof(Buffer), "data %d\r\n", DataType);
            mc_RS232.SendText(mc_RS232.m_info.ComPortNumber, Buffer);
            Sleep(1200);
        }
        m_SweepDone = true;
    }











    //*****************************************************************************  
    // Protected Code
    //*****************************************************************************

    //-Protected-------------------------------------------------------------------
    // Name: DecodeData(char* LineData, int LineDatalen)
    //-----------------------------------------------------------------------------
    bool MC_NanoVNA::DecodeData(char* LineData, int LineDatalen)
    {
        int i = 0;
        int Index;
        VNARAWVALUES  Complex;
        VNAVALUES_S11 Data_S11;
        VNAVALUES_S21 Data_S21;
        char buf1[256];
        char buf2[256];

        m_String.Line.clear();
        m_String.Line.erase();
        for (i = 0; i < MSG_MAXITEMS; i++) {
            m_String.Items[i].clear();
            m_String.Items[i].erase();
        }
        m_String.ItemCount = 0;
        m_String.Line = LineData;
        if (GetItemData(&m_String)) {
            if (m_String.ItemCount >= 2) {
                if (m_String.Items[0].compare("ch>") == 0) {
                    for (i = 0; i < MSG_MAX_Commands; i++) {
                        if (m_String.Items[1].compare(m_Commands[i]) == 0) {
                            m_String.Mode = i;
                            m_String.SubMode = 0;
                            switch (m_String.Mode) {
                                case MSG_sweep: {
                                    //memset(&m_VNAData.Sweep, 0, sizeof(m_VNAData.Sweep));
                                    printf("Sweep\r\n");
                                }break;
                                case MSG_frequencies: {
                                    //memset(&m_VNAData.Frequencies, 0, sizeof(m_VNAData.Frequencies));
                                    printf("Frequencies\r\n");
                                }break;
                                case MSG_data: {
                                    m_String.SubMode = atoi(m_String.Items[2].c_str());
                                    if (m_String.SubMode == 1) {
                                        //memset(&m_VNAData.Complex_S21, 0, sizeof(m_VNAData.Complex_S21));
                                    } else {
                                        //memset(&m_VNAData.Complex_S11, 0, sizeof(m_VNAData.Complex_S11));
                                    }
                                    printf("Data %d\r\n", m_String.SubMode);
                                }break;
                                case MSG_marker: {
                                    memset(&m_VNAData.Marker, 0, sizeof(m_VNAData.Marker));
                                    printf("Marker\r\n");
                                }break;
                            }
                            m_VNAData.SamplesNumber = 0;
                        }
                    }
                    return true;
                }
            }
            switch (m_String.Mode) {
                case MSG_sweep: {
                    if (m_String.ItemCount == 3) {
                        m_VNAData.Sweep.StartFrequency = atol(m_String.Items[0].c_str());
                        m_VNAData.Sweep.StopFrequency = atol(m_String.Items[1].c_str());
                        m_VNAData.Sweep.NumberOfsamples = atoi(m_String.Items[2].c_str());
                        m_VNASample.Sweep = m_VNAData.Sweep;
                        memset(&buf1, 0, sizeof(buf1));
                        memset(&buf2, 0, sizeof(buf2));
                        mc_VnaMath.Frequency_String(buf1, sizeof(buf1), m_VNAData.Sweep.StartFrequency);
                        mc_VnaMath.Frequency_String(buf2, sizeof(buf2), m_VNAData.Sweep.StopFrequency);
                        m_Update = true;
                    }
                }break;
                case MSG_frequencies: {
                    if (m_String.ItemCount == 1) {
                        Index = m_VNAData.SamplesNumber + (m_VNAMultiData.MultiSweepIndex * VNA_SamplesPerScan);
                        if (Index < VNA_MAX_NumberOfSamples) {
                            m_VNAData.Frequencies[Index] = atol(m_String.Items[0].c_str());
                            memset(&buf1, 0, sizeof(buf1));
                            mc_VnaMath.Frequency_String(buf1, sizeof(buf1), m_VNAData.Frequencies[Index]);
                            if (m_VNAData.SamplesNumber == VNA_SamplesPerScan) {
                                m_Update = true;
                            }
                            m_VNAData.SamplesNumber++;
                        }
                    }
                }break;
                case MSG_data: {
                    if (m_String.ItemCount == 2) {
                        Index = m_VNAData.SamplesNumber + (m_VNAMultiData.MultiSweepIndex * VNA_SamplesPerScan);
                        if (Index < VNA_MAX_NumberOfSamples) {
                            if (m_String.SubMode == 1) {
                                Complex.Value1 = atof(m_String.Items[0].c_str());
                                Complex.Value2 = atof(m_String.Items[1].c_str());
                                if (mc_Forms.m_MenuCheked.Filter_Average) {
                                    if (abs(Complex.Value1) > 1.0 || abs(Complex.Value2) > 1.0) {
                                        Complex = m_VNAData.Average_Complex_S21[Index];
                                    }
                                    m_VNAData.LastComplex_S21[Index] = Complex;
                                    m_VNAData.Average_Complex_S21[Index].Value1 = (m_VNAData.Average_Complex_S21[Index].Value1 * VNA_AVR_HI) + (Complex.Value1 * VNA_AVR_LO);
                                    m_VNAData.Average_Complex_S21[Index].Value2 = (m_VNAData.Average_Complex_S21[Index].Value2 * VNA_AVR_HI) + (Complex.Value2 * VNA_AVR_LO);
                                    m_VNAData.Complex_S21[Index] = m_VNAData.Average_Complex_S21[Index];
                                } else {
                                    m_VNAData.Complex_S21[Index] = Complex;
                                }
                                Data_S21 = GetNanoVNA_Data_S21(&m_VNAData.Complex_S21[Index], m_VNAData.Frequencies[Index]);
                                m_VNASample.Data_S21[Index] = InsertionSortS21(mc_Forms.m_MenuCheked.Filter_Noise, &Data_S21, Index, VNA_MAX_NumberOfSamples, VNA_FILTERAMOUNT);
                                memset(&buf1, 0, sizeof(buf1));
                                memset(&buf2, 0, sizeof(buf2));
                                mc_VnaMath.Frequency_String(buf1, sizeof(buf1), Data_S21.Frequencies);
                            } else {
                                Complex.Value1 = atof(m_String.Items[0].c_str());
                                Complex.Value2 = atof(m_String.Items[1].c_str());
                                if (mc_Forms.m_MenuCheked.Filter_Average) {
                                    if (abs(Complex.Value1) > 1.0 || abs(Complex.Value2) > 1.0) {
                                        Complex = m_VNAData.Average_Complex_S11[Index];
                                    }
                                    m_VNAData.LastComplex_S11[Index] = Complex;
                                    m_VNAData.Average_Complex_S11[Index].Value1 = (m_VNAData.Average_Complex_S11[Index].Value1 * VNA_AVR_HI) + (Complex.Value1 * VNA_AVR_LO);
                                    m_VNAData.Average_Complex_S11[Index].Value2 = (m_VNAData.Average_Complex_S11[Index].Value2 * VNA_AVR_HI) + (Complex.Value2 * VNA_AVR_LO);
                                    m_VNAData.Complex_S11[Index] = m_VNAData.Average_Complex_S11[Index];
                                } else {
                                    m_VNAData.Complex_S11[Index] = Complex;
                                }
                                Data_S11 = GetNanoVNA_Data_S11(&m_VNAData.Complex_S11[Index], m_VNAData.Frequencies[Index]);
                                m_VNASample.Data_S11[Index] = InsertionSortS11(mc_Forms.m_MenuCheked.Filter_Noise, &Data_S11, Index, VNA_MAX_NumberOfSamples, VNA_FILTERAMOUNT);
                                memset(&buf1, 0, sizeof(buf1));
                                memset(&buf2, 0, sizeof(buf2));
                                mc_VnaMath.Frequency_String(buf1, sizeof(buf1), Data_S11.Frequencies);
                                mc_VnaMath.Gamma2CapacitanceInductance(buf2, sizeof(buf2), &Data_S11.Complex, Data_S11.Frequencies);
                            }
                            if (m_VNAData.SamplesNumber == VNA_SamplesPerScan) {
                                m_Update = true;
                            }
                            m_VNAData.SamplesNumber++;
                        }
                    }
                }break;
                case MSG_marker: {
                    if (m_String.ItemCount == 3) {
                        if (m_VNAData.SamplesNumber < VNA_MAX_NumberOfMakers) {
                            m_VNAData.Marker[m_VNAData.SamplesNumber].ID = atoi(m_String.Items[0].c_str());
                            m_VNAData.Marker[m_VNAData.SamplesNumber].Index = atoi(m_String.Items[1].c_str());
                            m_VNAData.Marker[m_VNAData.SamplesNumber].Frequencies = atol(m_String.Items[2].c_str());
                            m_VNASample.Marker[m_VNAData.SamplesNumber] = m_VNAData.Marker[m_VNAData.SamplesNumber];
                            m_VNAData.SamplesNumber++;
                        }
                    }
                }break;
            }
        } else {
            return false;
        }
        return true;
    }
    //-Protected-------------------------------------------------------------------
    // Name: LoadDecodeData(char* LineData, int LineDatalen)
    //-----------------------------------------------------------------------------
    bool MC_NanoVNA::LoadDecodeData(char* LineData, int LineDatalen)
    {
        int i = 0;
        VNARAWVALUES  Complex;
        VNAVALUES_S11 Data_S11;
        VNAVALUES_S21 Data_S21;
        char buf1[256];
        char buf2[256];

        m_String.Line.clear();
        m_String.Line.erase();
        for (i = 0; i < MSG_MAXITEMS; i++) {
            m_String.Items[i].clear();
            m_String.Items[i].erase();
        }
        m_String.ItemCount = 0;
        m_String.Line = LineData;
        if (GetItemData(&m_String)) {
            if (m_String.ItemCount >= 2) {
                if (m_String.Items[0].compare("ch>") == 0) {
                    for (i = 0; i < MSG_MAX_Commands; i++) {
                        if (m_String.Items[1].compare(m_Commands[i]) == 0) {
                            m_String.Mode = i;
                            m_String.SubMode = 0;
                            switch (m_String.Mode) {
                                case MSG_sweep: {
                                    memset(&m_VNAData.Sweep, 0, sizeof(m_VNAData.Sweep));
                                    printf("Sweep\r\n");
                                }break;
                                case MSG_frequencies: {
                                    memset(&m_VNAData.Frequencies, 0, sizeof(m_VNAData.Frequencies));
                                    printf("Frequencies\r\n");
                                }break;
                                case MSG_data: {
                                    m_String.SubMode = atoi(m_String.Items[2].c_str());
                                    if (m_String.SubMode == 1) {
                                        memset(&m_VNAData.Complex_S21, 0, sizeof(m_VNAData.Complex_S21));
                                    } else {
                                        memset(&m_VNAData.Complex_S11, 0, sizeof(m_VNAData.Complex_S11));
                                    }
                                    printf("Data %d\r\n", m_String.SubMode);
                                }break;
                                case MSG_marker: {
                                    memset(&m_VNAData.Marker, 0, sizeof(m_VNAData.Marker));
                                    printf("Marker\r\n");
                                }break;
                            }
                            m_VNAData.SamplesNumber = 0;
                        }
                    }
                    return true;
                }
            }
            switch (m_String.Mode) {
                case MSG_sweep: {
                    if (m_String.ItemCount == 3) {
                        m_VNAData.Sweep.StartFrequency = atol(m_String.Items[0].c_str());
                        m_VNAData.Sweep.StopFrequency = atol(m_String.Items[1].c_str());
                        m_VNAData.Sweep.NumberOfsamples = atoi(m_String.Items[2].c_str());
                        m_VNASample.Sweep = m_VNAData.Sweep;
                        memset(&buf1, 0, sizeof(buf1));
                        memset(&buf2, 0, sizeof(buf2));
                        mc_VnaMath.Frequency_String(buf1, sizeof(buf1), m_VNAData.Sweep.StartFrequency);
                        mc_VnaMath.Frequency_String(buf2, sizeof(buf2), m_VNAData.Sweep.StopFrequency);
                        m_Update = true;
                    }
                }break;
                case MSG_frequencies: {
                    if (m_String.ItemCount == 1) {
                        if (m_VNAData.SamplesNumber < VNA_MAX_NumberOfSamples) {
                            m_VNAData.Frequencies[m_VNAData.SamplesNumber] = atol(m_String.Items[0].c_str());
                            memset(&buf1, 0, sizeof(buf1));
                            mc_VnaMath.Frequency_String(buf1, sizeof(buf1), m_VNAData.Frequencies[m_VNAData.SamplesNumber]);
                            m_VNAData.SamplesNumber++;
                        }
                    }
                }break;
                case MSG_data: {
                    if (m_String.ItemCount == 2) {
                        if (m_VNAData.SamplesNumber < VNA_MAX_NumberOfSamples) {
                            if (m_String.SubMode == 1) {
                                Complex.Value1 = atof(m_String.Items[0].c_str());
                                Complex.Value2 = atof(m_String.Items[1].c_str());
                                if (mc_Forms.m_MenuCheked.Filter_Average) {
                                    if (abs(Complex.Value1) > 1.0 || abs(Complex.Value2) > 1.0) {
                                        Complex = m_VNAData.Average_Complex_S21[m_VNAData.SamplesNumber];
                                    }
                                    m_VNAData.LastComplex_S21[m_VNAData.SamplesNumber] = Complex;
                                    m_VNAData.Average_Complex_S21[m_VNAData.SamplesNumber].Value1 = (m_VNAData.Average_Complex_S21[m_VNAData.SamplesNumber].Value1 * VNA_AVR_HI) + (Complex.Value1 * VNA_AVR_LO);
                                    m_VNAData.Average_Complex_S21[m_VNAData.SamplesNumber].Value2 = (m_VNAData.Average_Complex_S21[m_VNAData.SamplesNumber].Value2 * VNA_AVR_HI) + (Complex.Value2 * VNA_AVR_LO);
                                    m_VNAData.Complex_S21[m_VNAData.SamplesNumber] = m_VNAData.Average_Complex_S21[m_VNAData.SamplesNumber];
                                } else {
                                    m_VNAData.Complex_S21[m_VNAData.SamplesNumber] = Complex;
                                }
                                Data_S21 = GetNanoVNA_Data_S21(&m_VNAData.Complex_S21[m_VNAData.SamplesNumber], m_VNAData.Frequencies[m_VNAData.SamplesNumber]);
                                m_VNASample.Data_S21[m_VNAData.SamplesNumber] = InsertionSortS21(mc_Forms.m_MenuCheked.Filter_Noise, &Data_S21, m_VNAData.SamplesNumber, VNA_MAX_NumberOfSamples, VNA_FILTERAMOUNT);
                                memset(&buf1, 0, sizeof(buf1));
                                memset(&buf2, 0, sizeof(buf2));
                                mc_VnaMath.Frequency_String(buf1, sizeof(buf1), Data_S21.Frequencies);
                            } else {
                                Complex.Value1 = atof(m_String.Items[0].c_str());
                                Complex.Value2 = atof(m_String.Items[1].c_str());
                                if (mc_Forms.m_MenuCheked.Filter_Average) {
                                    if (abs(Complex.Value1) > 1.0 || abs(Complex.Value2) > 1.0) {
                                        Complex = m_VNAData.Average_Complex_S11[m_VNAData.SamplesNumber];
                                    }
                                    m_VNAData.LastComplex_S11[m_VNAData.SamplesNumber] = Complex;
                                    m_VNAData.Average_Complex_S11[m_VNAData.SamplesNumber].Value1 = (m_VNAData.Average_Complex_S11[m_VNAData.SamplesNumber].Value1 * VNA_AVR_HI) + (Complex.Value1 * VNA_AVR_LO);
                                    m_VNAData.Average_Complex_S11[m_VNAData.SamplesNumber].Value2 = (m_VNAData.Average_Complex_S11[m_VNAData.SamplesNumber].Value2 * VNA_AVR_HI) + (Complex.Value2 * VNA_AVR_LO);
                                    m_VNAData.Complex_S11[m_VNAData.SamplesNumber] = m_VNAData.Average_Complex_S11[m_VNAData.SamplesNumber];
                                } else {
                                    m_VNAData.Complex_S11[m_VNAData.SamplesNumber] = Complex;
                                }
                                Data_S11 = GetNanoVNA_Data_S11(&m_VNAData.Complex_S11[m_VNAData.SamplesNumber], m_VNAData.Frequencies[m_VNAData.SamplesNumber]);
                                m_VNASample.Data_S11[m_VNAData.SamplesNumber] = InsertionSortS11(mc_Forms.m_MenuCheked.Filter_Noise, &Data_S11, m_VNAData.SamplesNumber, VNA_MAX_NumberOfSamples, VNA_FILTERAMOUNT);
                                memset(&buf1, 0, sizeof(buf1));
                                memset(&buf2, 0, sizeof(buf2));
                                mc_VnaMath.Frequency_String(buf1, sizeof(buf1), Data_S11.Frequencies);
                                mc_VnaMath.Gamma2CapacitanceInductance(buf2, sizeof(buf2), &Data_S11.Complex, Data_S11.Frequencies);
                            }
                            m_X10Samples = float(m_VNAData.SamplesNumber / VNA_SamplesPerScan);
                            m_NumberOfSamples = (float(VNA_SamplesPerScan) * m_X10Samples) + 1.0f;
                            m_VNAData.SamplesNumber++;
                        }
                    }
                }break;
                case MSG_marker: {
                    if (m_String.ItemCount == 3) {
                        if (m_VNAData.SamplesNumber < VNA_MAX_NumberOfMakers) {
                            m_VNAData.Marker[m_VNAData.SamplesNumber].ID = atoi(m_String.Items[0].c_str());
                            m_VNAData.Marker[m_VNAData.SamplesNumber].Index = atoi(m_String.Items[1].c_str());
                            m_VNAData.Marker[m_VNAData.SamplesNumber].Frequencies = atol(m_String.Items[2].c_str());
                            m_VNASample.Marker[m_VNAData.SamplesNumber] = m_VNAData.Marker[m_VNAData.SamplesNumber];
                            m_VNAData.SamplesNumber++;
                        }
                    }
                }break;
            }
        } else {
            return false;
        }
        return true;
    }
    //-Protected-------------------------------------------------------------------
    // Name: GetItemData(STRINGDATA* Data)
    //-----------------------------------------------------------------------------
    bool MC_NanoVNA::GetItemData(STRINGDATA* Data)
    {
        size_t i = 0;
        int Index = 0;
        int cp = 0;
        char ch = 0;
        string Tmp;

        for (i = 0; i < Data->Line.length(); i++) {
            ch = Data->Line.at(i);
            if (ch == ' ' || ch == 0x0A || ch == 0x0D) {
                if (Tmp.length() > 0) {
                    Data->Items[Index] = Tmp;
                    Tmp.clear();
                    Tmp.erase();
                    Index++;
                    Data->ItemCount = Index;
                }
            } else {
                Tmp.append(1, ch);
            }
            if (Index >= MSG_MAXITEMS) {
                return false;
            }

        }
        return true;
    }
    //-Protected-------------------------------------------------------------------
    // Name: GetNanoVNA_Data_S11(VNARAWVALUES* Complex, uint32_t Frequencies);
    //-----------------------------------------------------------------------------
    VNAVALUES_S11 MC_NanoVNA::GetNanoVNA_Data_S11(VNARAWVALUES* Complex, uint32_t Frequencies)
    {
        VNAVALUES_S11 Data_S11;
        std::complex<double> s11;
        VNARAWVALUES Complex_Admittance;

        memset(&Data_S11, 0, sizeof(Data_S11));
        Data_S11.Complex = *Complex;
        Data_S11.Frequencies = Frequencies;
        Data_S11.Rv_Linear = mc_VnaMath.Linear(Complex);
        Data_S11.Fw_Linear = 1.0 + Data_S11.Rv_Linear;
        Data_S11.Rv_Power = mc_VnaMath.dbm_to_mw(mc_VnaMath.LogMag(Complex));
        Data_S11.Fw_Power = 1.0 - Data_S11.Rv_Power;
        Data_S11.Rv_LogMag = mc_VnaMath.LogMag(Complex);
        Data_S11.Fw_LogMag = mc_VnaMath.mw_to_dbm(Data_S11.Fw_Power);
        Data_S11.SWR = mc_VnaMath.Swr(Complex);
        Data_S11.Resitance = mc_VnaMath.Resitance(Complex);
        Data_S11.Reactance = mc_VnaMath.Reactance(Complex);

        Complex_Admittance.Value1 = -Complex->Value1;
        Complex_Admittance.Value2 = Complex->Value2;
        Data_S11.Admittance_Resitance = mc_VnaMath.Resitance(&Complex_Admittance);
        Data_S11.Admittance_Reactance = mc_VnaMath.Reactance(&Complex_Admittance);

        Data_S11.CapacitanceInductance = mc_VnaMath.CapacitanceInductance(Complex, Frequencies, Data_S11.CapacitanceInductancePrefix);
        Data_S11.Phase = mc_VnaMath.Phase(Complex);
        s11._Val[0] = Complex->Value1;
        s11._Val[1] = Complex->Value2;
        Data_S11.Real = mc_VnaMath.RealImpedance_S11(s11, Data_S11.Frequencies, VNA_CHARACTERISTIC_IMPEDANCE);
        return(Data_S11);
    }
    //-Protected-------------------------------------------------------------------
    // Name: GetNanoVNA_Data_S21(VNARAWVALUES* Complex, uint32_t Frequencies);
    //-----------------------------------------------------------------------------
    VNAVALUES_S21 MC_NanoVNA::GetNanoVNA_Data_S21(VNARAWVALUES* Complex, uint32_t Frequencies)
    {
        VNAVALUES_S21 Data_S21;

        memset(&Data_S21, 0, sizeof(Data_S21));
        Data_S21.Complex = *Complex;
        Data_S21.Frequencies = Frequencies;
        Data_S21.Rv_Linear = mc_VnaMath.Linear(Complex);
        Data_S21.Rv_LogMag = mc_VnaMath.LogMag(Complex);
        Data_S21.Phase = mc_VnaMath.Phase(Complex);
        return(Data_S21);
    }
    //-Protected-------------------------------------------------------------------
    // Name: nsertionSortS11(bool Filter, VNAVALUES_S11* Data_In ,int Index, int NumberOfSamples, int Amount)
    //-----------------------------------------------------------------------------
    VNAVALUES_S11 MC_NanoVNA::InsertionSortS11(bool Filter, VNAVALUES_S11* Data_In, int Index, int NumberOfSamples, int Amount)
    {
        int j;
        VNAVALUES_S11 AvrValue;
        VNAVALUES_S11 ReturnValue;
        int Count;

        AvrValue = *Data_In;
        ReturnValue = AvrValue;
        if (Filter) {
            Count = 1;
            for (j = 1; j < Amount; j++) {
                if ((Index - j) >= 0 && (Index - j) < NumberOfSamples) {
                    AvrValue.Rv_LogMag += m_VNASample.Data_S11[Index - j].Rv_LogMag;
                    AvrValue.Rv_Linear += m_VNASample.Data_S11[Index - j].Rv_Linear;
                    Count++;
                }
            }
            //double DifValue = ReturnValue.Rv_LogMag - (AvrValue.Rv_LogMag / double(Count));
            //ReturnValue.Rv_LogMag = (AvrValue.Rv_LogMag / double(Count)) + (DifValue * 0.1);

            ReturnValue.Rv_LogMag = AvrValue.Rv_LogMag / double(Count);
            ReturnValue.Rv_Linear = AvrValue.Rv_Linear / double(Count);
        }
        return(ReturnValue);
    }
    //-Protected-------------------------------------------------------------------
    // Name: InsertionSortS21(bool Filter, VNAVALUES_S21* Data_In, int Index, int NumberOfSamples, int Amount)
    //-----------------------------------------------------------------------------
    VNAVALUES_S21 MC_NanoVNA::InsertionSortS21(bool Filter, VNAVALUES_S21* Data_In, int Index, int NumberOfSamples, int Amount)
    {
        int j;
        VNAVALUES_S21 AvrValue;
        VNAVALUES_S21 ReturnValue;
        int Count;

        AvrValue = *Data_In;
        ReturnValue = AvrValue;
        if (Filter) {
            Count = 1;
            for (j = 1; j < Amount; j++) {
                if ((Index - j) >= 0 && (Index - j) < NumberOfSamples) {
                    AvrValue.Rv_LogMag += m_VNASample.Data_S21[Index - j].Rv_LogMag;
                    AvrValue.Rv_Linear += m_VNASample.Data_S21[Index - j].Rv_Linear;
                    Count++;
                }
            }
            //double DifValue = ReturnValue.Rv_LogMag - (AvrValue.Rv_LogMag / double(Count));
            //ReturnValue.Rv_LogMag = (AvrValue.Rv_LogMag / double(Count)) + (DifValue * 0.1);

            ReturnValue.Rv_LogMag = AvrValue.Rv_LogMag / double(Count);
            ReturnValue.Rv_Linear = AvrValue.Rv_Linear / double(Count);
        }
        return(ReturnValue);
    }
    //-Protected-------------------------------------------------------------------
    // Name: InsertionSortComplex_S11(bool Filter, VNARAWVALUES* Data_In, int Index, int NumberOfSamples, int Amount)
    //-----------------------------------------------------------------------------
    VNARAWVALUES MC_NanoVNA::InsertionSortComplex_S11(bool Filter, VNARAWVALUES* Data_In, int Index, int NumberOfSamples, int Amount)
    {
        int j;
        VNARAWVALUES AvrValue;
        VNARAWVALUES ReturnValue;
        int Count;

        AvrValue = *Data_In;
        ReturnValue = AvrValue;
        if (Filter) {
            Count = 1;
            for (j = 1; j < Amount; j++) {
                if ((Index - j) >= 0 && (Index - j) < NumberOfSamples) {
                    AvrValue.Value1 += m_VNAData.Complex_S11[Index - j].Value1;
                    AvrValue.Value2 += m_VNAData.Complex_S11[Index - j].Value2;
                    Count++;
                }
            }
            ReturnValue.Value1 = AvrValue.Value1 / double(Count);
            ReturnValue.Value2 = AvrValue.Value2 / double(Count);
        }
        return(ReturnValue);
    }
    //-Protected-------------------------------------------------------------------
    // Name: InsertionSortComplex_S21(bool Filter, VNARAWVALUES* Data_In, int Index, int NumberOfSamples, int Amount)
    //-----------------------------------------------------------------------------
    VNARAWVALUES MC_NanoVNA::InsertionSortComplex_S21(bool Filter, VNARAWVALUES* Data_In, int Index, int NumberOfSamples, int Amount)
    {
        int j;
        VNARAWVALUES AvrValue;
        VNARAWVALUES ReturnValue;
        int Count;

        AvrValue = *Data_In;
        ReturnValue = AvrValue;
        if (Filter) {
            Count = 1;
            for (j = 1; j < Amount; j++) {
                if ((Index - j) >= 0 && (Index - j) < NumberOfSamples) {
                    AvrValue.Value1 += m_VNAData.Complex_S21[Index - j].Value1;
                    AvrValue.Value2 += m_VNAData.Complex_S21[Index - j].Value2;
                    Count++;
                }
            }
            ReturnValue.Value1 = AvrValue.Value1 / double(Count);
            ReturnValue.Value2 = AvrValue.Value2 / double(Count);
        }
        return(ReturnValue);
    }
};