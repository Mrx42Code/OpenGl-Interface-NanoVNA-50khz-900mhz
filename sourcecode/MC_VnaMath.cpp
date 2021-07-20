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
// File: MC_VnaMath.cpp: implementation of the MC_VnaMath class.
//
//-----------------------------------------------------------------------------
#include "MC_VnaMath.h"
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
    // Name: MC_VnaMath()
    // Desc: MC_VnaMath class
    //-----------------------------------------------------------------------------
    MC_VnaMath::MC_VnaMath()
    {

    }
    //-Public----------------------------------------------------------------------
    // Name: ~MC_VnaMath()
    // Desc: ~MC_VnaMath Destruction class
    //-----------------------------------------------------------------------------
    MC_VnaMath::~MC_VnaMath()
    {

    }
    //-Public----------------------------------------------------------------------
    // Name: GetCapacitanceInductance(char* buf, int len, VNARAWVALUES* Complex, uint32_t Frequency)
    //-----------------------------------------------------------------------------
    void MC_VnaMath::GetCapacitanceInductance(char* buf, int len, VNARAWVALUES* Complex, uint32_t Frequency)
    {
        Gamma2CapacitanceInductance(buf, len, Complex, Frequency);
    }
    //-Public----------------------------------------------------------------------
    // Name: Linear(VNARAWVALUES* Complex)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::Linear(VNARAWVALUES* Complex)
    {
        return -sqrt((Complex->Value1 * Complex->Value1 + Complex->Value2 * Complex->Value2));
    }
    //-Public----------------------------------------------------------------------
    // Name: LogMag(VNARAWVALUES* Complex)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::LogMag(VNARAWVALUES* Complex)
    {
        return log10((Complex->Value1 * Complex->Value1 + Complex->Value2 * Complex->Value2)) * 10;
    }
    //-Public----------------------------------------------------------------------
    // Name: Swr(VNARAWVALUES* Complex)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::Swr(VNARAWVALUES* Complex)
    {
        double x = sqrt(Complex->Value1 * Complex->Value1 + Complex->Value2 * Complex->Value2);
        if (x >= 1.0)
            return INFINITY;

        return (1.0 + x) / (1.0 - x);
    }
    //-Public----------------------------------------------------------------------
    // Name: Resitance(VNARAWVALUES* Complex)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::Resitance(VNARAWVALUES* Complex)
    {
        double z0 = VNA_CHARACTERISTIC_IMPEDANCE;
        double d = z0 / ((1.0 - Complex->Value1) * (1.0 - Complex->Value1) + Complex->Value2 * Complex->Value2);
        double zr = ((1.0 + Complex->Value1) * (1.0 - Complex->Value1) - Complex->Value2 * Complex->Value2) * d;
        return zr;
    }
    //-Public----------------------------------------------------------------------
    // Name: Reactance(VNARAWVALUES* Complex)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::Reactance(VNARAWVALUES* Complex)
    {
        double z0 = VNA_CHARACTERISTIC_IMPEDANCE;
        double d = z0 / ((1.0 - Complex->Value1) * (1.0 - Complex->Value1) + Complex->Value2 * Complex->Value2);
        double zi = 2.0 * Complex->Value2 * d;
        return zi;
    }
    //-Public----------------------------------------------------------------------
    // Name: CapacitanceInductance(VNARAWVALUES* Complex, uint32_t Frequency, char* Prefix)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::CapacitanceInductance(VNARAWVALUES* Complex, uint32_t Frequency, char* Prefix)
    {
        bool Capacitance_Inductance = false;
        double Freq = (double)(Frequency);
        double zi = Reactance(Complex);
        double value = 0.0;
        double Returnvalue = 0.0;

        if (zi < 0.0) {                                                             // Capacitance
            value = -1.0 / ((VNA_PI2 * Freq * zi) / 1000000.0);
            Capacitance_Inductance = false;
        } else {                                                                    // Inductance
            value = zi / ((VNA_PI2 * Freq) / 1000000.0);
            Capacitance_Inductance = true;
        }
        Returnvalue = value * 1000000.0;
        if (Returnvalue >= 1000.0) {
            if (Capacitance_Inductance) {
                snprintf(Prefix, 4, "nH");
            } else {
                snprintf(Prefix, 4, "nF");
            }
            Returnvalue = value * 1000.0;
        } else {
            if (Capacitance_Inductance) {
                snprintf(Prefix, 4, "pH");
            } else {
                snprintf(Prefix, 4, "pF");
            }
        }
        return Returnvalue;
    }
    //-Public----------------------------------------------------------------------
    // Name: Phase(VNARAWVALUES* Complex)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::Phase(VNARAWVALUES* Complex)
    {
        return 2.0 * atan2(Complex->Value2, Complex->Value1) / VNA_PI * 90.0;
    }
    //-Public----------------------------------------------------------------------
    // Name: mw_to_dbm(double mW)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::mw_to_dbm(double mW)
    {
        return (10.0f * log10(mW));
    }
    //-Public----------------------------------------------------------------------
    // Name: dbm_to_mw(double dBm)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::dbm_to_mw(double dBm)
    {
        return pow(10.0, dBm / 10.0);
    }
    //-Public----------------------------------------------------------------------
    // Name: mw_to_rms(double mw,double res)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::mw_to_rms(double mw, double res)
    {
        return sqrt((mw / 1000.0) * res);
    }
    //-Public----------------------------------------------------------------------
    // Name: rms_to_ptp(double rms)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::rms_to_ptp(double rms)
    {
        return rms * sqrt(2.0);
    }
    //-Public----------------------------------------------------------------------
    // Name: rms_to_ptp2(double rms)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::rms_to_ptp2(double rms)
    {
        return rms * (sqrt(2.0) * 2.0);
    }
    //-Public----------------------------------------------------------------------
    // Name: RF_LossInFreeSpace(double FrequencyMhz,double Distance,double AntennaGain1,double AntennaGain2)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::LossInFreeSpace(double FrequencyMhz, double Distance, double AntennaGain1, double AntennaGain2)
    {
        double fspl = 0.0;
        double Db = 0.0;
        double Gain = AntennaGain1 * AntennaGain2;

        fspl = pow(((Gain * VNA_PI * Distance * FrequencyMhz) / VNA_SPEEDOFLIGHT), 2.0);
        Db = 10.0 * log10(fspl);
        printf("FrequencyMhz = %f Distance = %.1f AntennaGain1 = %.1f AntennaGain2 = %.1f db = %.3f\r\n", FrequencyMhz, Distance, AntennaGain1, AntennaGain2, Db);
        return Db;
    }
    //-Public----------------------------------------------------------------------
    // Name: rms_to_ptp2(double rms)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::db_to_swr(double dBm)
    {
        double linearFactor = pow(10.0, -1.0 * dBm / 20.0);
        if (linearFactor <= 0.0)
            return 1.0;

        return (linearFactor + 1.0) / (linearFactor - 1.0);
    }
    //-Public----------------------------------------------------------------------
    // Name: Power_To_Swr(double power_fwd, double power_rvr)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::Power_To_Swr(double power_fwd, double power_rvr)
    {
        if (power_rvr <= 0.0)
            return 1.0;

        double pwrs = sqrt(power_rvr / power_fwd);
        return (1.0 + pwrs) / (1.0 - pwrs);
    }
    //-Public----------------------------------------------------------------------
    // Name: Power_To_Dbm(double power)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::Power_To_Dbm(double power)
    {
        return 10.0 * log10(power) + 30.0;
    }
    //-Public----------------------------------------------------------------------
    // Name: Voltage_To_Power(double voltage)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::Voltage_To_Power(double voltage)
    {
        return (voltage * voltage) / VNA_CHARACTERISTIC_IMPEDANCE;
    }
    //-Public----------------------------------------------------------------------
    // Name: Power_To_Voltage(double power)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::Power_To_Voltage(double power)
    {
        return sqrt(power * VNA_CHARACTERISTIC_IMPEDANCE);
    }
    //-Public----------------------------------------------------------------------
    // Name: logBased(double value, double base)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::logBased(double value, double base)
    {
        return log10(value) / log10(base);
    }
    //-Public----------------------------------------------------------------------
    // Name: Polar_To_Complex(double magnitude, double phase)
    //-----------------------------------------------------------------------------
    _Dcomplex MC_VnaMath::Polar_To_Complex(double magnitude, double phase)
    {
        double real = magnitude * cos(phase * VNA_PI / 180.0);
        double imaginary = magnitude * sin(phase * VNA_PI / 180.0);
        _Dcomplex Result = { real ,imaginary };
        return  Result;
    }
    //-Public----------------------------------------------------------------------
    // Name: ComplexLoadFromReflection(double magnitude, double phase)
    //-----------------------------------------------------------------------------
    _Dcomplex MC_VnaMath::ComplexLoadFromReflection(double magnitude, double phase)
    {
        _Dcomplex refl = Polar_To_Complex(magnitude, phase);
        _Dcomplex loadZ;
        loadZ._Val[0] = (VNA_CHARACTERISTIC_IMPEDANCE * (1.0 + refl._Val[0])) / (1.0 - refl._Val[0]);
        loadZ._Val[1] = (VNA_CHARACTERISTIC_IMPEDANCE * (1.0 + refl._Val[1])) / (1.0 - refl._Val[1]);
        return loadZ;
    }
    //-Public----------------------------------------------------------------------
    // Name: String_Value_With_Prefix(char* buf, int len, double val, const char* unit)
    //-----------------------------------------------------------------------------
    int MC_VnaMath::String_Value_With_Prefix(char* buf, int len, double val, const char* unit)
    {
        char prefix;
        int n;
        size_t unitlen;
        size_t i;

        if (val < 0.0) {
            val = -val;
            *buf++ = '-';
            len--;
        }
        if (val < 1e-12) {
            prefix = 'f';
            val *= 1e15;
        } else if (val < 1e-9) {
            prefix = 'p';
            val *= 1e12;
        } else if (val < 1e-6) {
            prefix = 'n';
            val *= 1e9;
        } else if (val < 1e-3) {
            prefix = 'u';
            val *= 1e6;
        } else if (val < 1) {
            prefix = 'm';
            val *= 1e3;
        } else if (val < 1e3) {
            prefix = 0;
        } else if (val < 1e6) {
            prefix = 'k';
            val /= 1e3;
        } else if (val < 1e9) {
            prefix = 'M';
            val /= 1e6;
        } else {
            prefix = 'G';
            val /= 1e9;
        }
        if (val < 10.0) {
            n = snprintf(buf, len, "%.4f", val);
        } else if (val < 100.0) {
            n = snprintf(buf, len, "%.3f", val);
        } else if (val < 1000.0) {
            n = snprintf(buf, len, "%.2f", val);
        } else if (val < 10000.0) {
            n = snprintf(buf, len, "%.1f", val);
        } else {
            n = snprintf(buf, len, "%d", (int)val);
        }
        if (prefix)
            buf[n++] = prefix;

        unitlen = strlen(unit);
        if (unitlen) {
            for (i = 0; i < unitlen; i++) {
                buf[n++] = unit[i];
            }
        }
        buf[n] = '\0';
        return n;
    }
    //-Public----------------------------------------------------------------------
    // Name: Gamma2Resistance(char* buf, int len, VNARAWVALUES* Complex)
    //-----------------------------------------------------------------------------
    void MC_VnaMath::Gamma2Resistance(char* buf, int len, VNARAWVALUES* Complex)
    {
        double z0 = VNA_CHARACTERISTIC_IMPEDANCE;
        double d = z0 / ((1.0 - Complex->Value1) * (1.0 - Complex->Value1) + Complex->Value2 * Complex->Value2);
        double zr = ((1.0 + Complex->Value1) * (1.0 - Complex->Value1) - Complex->Value2 * Complex->Value2) * d;
        String_Value_With_Prefix(buf, len, zr, " Ohms");
    }
    //-Public----------------------------------------------------------------------
    // Name: Gamma2Reactance(char* buf, int len, VNARAWVALUES* Complex)
    //-----------------------------------------------------------------------------
    void MC_VnaMath::Gamma2Reactance(char* buf, int len, VNARAWVALUES* Complex)
    {
        double z0 = VNA_CHARACTERISTIC_IMPEDANCE;
        double d = z0 / ((1.0 - Complex->Value1) * (1.0 - Complex->Value1) + Complex->Value2 * Complex->Value2);
        double zi = 2.0 * Complex->Value2 * d;
        String_Value_With_Prefix(buf, len, zi, " Ohms");
    }
    //-Public----------------------------------------------------------------------
    // Name: Gamma2CapacitanceInductance(char* buf, int len, VNARAWVALUES* Complex, uint32_t Frequency)
    //-----------------------------------------------------------------------------
    void MC_VnaMath::Gamma2CapacitanceInductance(char* buf, int len, VNARAWVALUES* Complex, uint32_t Frequency)
    {
        double Freq = (double)(Frequency);
        double z0 = VNA_CHARACTERISTIC_IMPEDANCE;
        double d = z0 / ((1.0 - Complex->Value1) * (1.0 - Complex->Value1) + Complex->Value2 * Complex->Value2);
        double zr = ((1.0 + Complex->Value1) * (1.0 - Complex->Value1) - Complex->Value2 * Complex->Value2) * d;
        double zi = 2.0 * Complex->Value2 * d;
        int n;

        n = String_Value_With_Prefix(buf, len, zr, " Ohms");
        buf[n++] = ' ';
        if (zi < 0.0) {
            double Capacitance = -1.0 / ((VNA_PI2 * Freq) * zi);
            String_Value_With_Prefix(buf + n, len - n, Capacitance, "F");
        } else {
            double Inductance = zi / (VNA_PI2 * Freq);
            String_Value_With_Prefix(buf + n, len - n, Inductance, "H");
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: Frequency_String(char* buf, size_t len, int32_t freq)
    //-----------------------------------------------------------------------------
    void MC_VnaMath::Frequency_String(char* buf, size_t len, int32_t freq)
    {
        if (freq < 0) {
            freq = -freq;
            *buf++ = '-';
            len -= 1;
        }
        if (freq < 1000) {
            snprintf(buf, len, "%d Hz", (int)freq);
        } else if (freq < 1000000) {
            snprintf(buf, len, "%d.%03d kHz", (int)(freq / 1000), (int)(freq % 1000));
        } else {
            snprintf(buf, len, "%d.%03d %03d MHz", (int)(freq / 1000000), (int)((freq / 1000) % 1000), (int)(freq % 1000));
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: ImpedanceToCapacitance(double LogMag, double Freq)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::ImpedanceToCapacitance(double Impedance, double Freq)
    {
        double  Capacitance = 0.0;

        if (Freq == 0)
            return Capacitance;

        if (Impedance == 0)
            return Capacitance;

        Capacitance = -(1.0 / ((Freq * VNA_PI2) * Impedance));
        return Capacitance;
    }
    //-Public----------------------------------------------------------------------
    // Name: ImpedanceToInductance(double Impedance, double Freq)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::ImpedanceToInductance(double Impedance, double Freq)
    {
        double  Inductance = 0.0;

        if (Freq == 0)
            return Inductance;

        Inductance = (Impedance * 1.0) / (Freq * VNA_PI2);
        return Inductance;
    }

    //-Public----------------------------------------------------------------------
    // Name: RealImpedance_S11(std::complex<double> s11, double freqHz, double z0)
    //-----------------------------------------------------------------------------
    VNAVALUES_S11_Real MC_VnaMath::RealImpedance_S11(std::complex<double> s11, double freqHz, double z0)
    {
        VNAVALUES_S11_Real Real;
        std::complex<double> Z;
        std::complex<double> Y;

        Z = -z0 * (s11 + 1.0) / (s11 - 1.0);
        Y = -(s11 - 1.0) / (z0 * (s11 + 1.0));
        Real.Frequency = freqHz;
        Real.Impedance_Resitance = Z.real();
        Real.Impedance_Resitance_jx = Z.imag();
        Real.Admittance_Resitance = Y.real();
        Real.Admittance_Resitance_jx = Y.imag();
        Real.Series_Admittance_Resitance = 1.0 / Z.real();
        Real.Series_Admittance_Resitance_jx = 1.0 / Z.imag();
        Real.Parallel_Admittance_Resitance = 1.0 / Y.real();
        Real.Parallel_Admittance_Resitance_jx = 1.0 / Y.imag();
        Real.Series_Resitance = Real.Impedance_Resitance;
        Real.Series_CapacitanceInductance = SeriesCapacitanceInductance(freqHz, Z.imag());
        Real.Parallel_Resitance = Real.Parallel_Admittance_Resitance_jx;
        Real.Parallel_CapacitanceInductance = ParallelCapacitanceInductance(freqHz, Y.imag());
        return Real;
    }
    //-Public----------------------------------------------------------------------
    // Name: ImpedanceDisplay(std::complex<double> s11, double freqHz, double z0)
    //-----------------------------------------------------------------------------
    void MC_VnaMath::ImpedanceDisplay(std::complex<double> s11, double freqHz, double z0)
    {
        std::complex<double> Z = -z0 * (s11 + 1.0) / (s11 - 1.0);
        std::complex<double> Y = -(s11 - 1.0) / (z0 * (s11 + 1.0));

        printf(" impedance  %.3f %s jx%.4f\r\n", Z.real(), Z.imag() >= 0 ? "+" : "-", fabs(Z.imag()));
        printf(" impedance  %.3f jx %.4f\r\n", Z.real(), Z.imag());
        printf(" admittance %.4f %s jx%.4f\r\n", Y.real(), Y.imag() >= 0 ? "+" : "-", fabs(Y.imag()));
        printf(" s_admittance %.4f %s jx%.4f\r\n", 1.0 / Z.real(), Z.imag() >= 0 ? "+" : "-", fabs(1.0 / Z.imag()));
        printf(" p_impedance %.4f jx%.4f\r\n", 1. / Y.real(), 1.0 / Y.imag());
        double value = SeriesCapacitanceInductance(freqHz, Z.imag());
        printf(" series %.3f ohms %.2f %s%s\r\n", Z.real(), fabs(Value_Scale(value)), Value_Unit(value), value > 0.0 ? "H" : "F");
        value = ParallelCapacitanceInductance(freqHz, Y.imag());
        printf(" parallel %.3f ohms %.2f %s%s\r\n", 1.0 / Y.real(), fabs(Value_Scale(value)), Value_Unit(value), value > 0.0 ? "H" : "F");
    }
    //-Public----------------------------------------------------------------------
    // Name: SeriesCapacitanceInductance(double freq, double Z)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::SeriesCapacitanceInductance(double freq, double Z)
    {
        if (Z > 0.0) return
            Z / (VNA_PI2 * freq);

        return 1.0 / (Z * VNA_PI2 * freq);
    }
    //-Public----------------------------------------------------------------------
    // Name: ParallelCapacitanceInductance(double freq, double Y)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::ParallelCapacitanceInductance(double freq, double Y)
    {
        if (Y < 0.0)
            return -1.0 / (Y * VNA_PI2 * freq);

        return -Y / (VNA_PI2 * freq);
    }
    //-Public----------------------------------------------------------------------
    // Name: Value_Scale(double val)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::Value_Scale(double val)
    {
        double val2 = fabs(val);
        if (val2 > 1e12) return val * 1e-12;
        if (val2 > 1e9) return val * 1e-9;
        if (val2 > 1e6) return val * 1e-6;
        if (val2 > 1e3) return val * 1e-3;
        if (val2 > 1e0) return val;
        if (val2 > 1e-3) return val * 1e3;
        if (val2 > 1e-6) return val * 1e6;
        if (val2 > 1e-9) return val * 1e9;
        if (val2 > 1e-12) return val * 1e12;
        return val * 1e15;
    }
    //-Public----------------------------------------------------------------------
    // Name: Value_Unit(double val)
    //-----------------------------------------------------------------------------
    const char* MC_VnaMath::Value_Unit(double val)
    {
        val = fabs(val);
        if (val > 1e12) return "T";
        if (val > 1e9) return "G";
        if (val > 1e6) return "M";
        if (val > 1e3) return "k";
        if (val > 1e0) return "";
        if (val > 1e-3) return "m";
        if (val > 1e-6) return "u";
        if (val > 1e-9) return "n";
        if (val > 1e-12) return "p";
        return "f";
    }
    //-Public----------------------------------------------------------------------
    // Name: Value_ScaleFrequency(double val)
    //-----------------------------------------------------------------------------
    double MC_VnaMath::Value_ScaleFrequency(double val)
    {
        double val2 = fabs(val);
        if (val2 > 1e9) return val * 1e-9;
        if (val2 > 1e6) return val * 1e-6;
        if (val2 > 1e3) return val * 1e-3;
        return val;
    }
    //-Public----------------------------------------------------------------------
    // Name: Value_UnitFrequency(double val)
    //-----------------------------------------------------------------------------
    const char* MC_VnaMath::Value_UnitFrequency(double val)
    {
        val = fabs(val);
        if (val > 1e9) return "Ghz";
        if (val > 1e6) return "Mhz";
        if (val > 1e3) return "Khz";
        return "Hz";
    }










    //*****************************************************************************  
    // Protected Code
    //*****************************************************************************

    //-Public----------------------------------------------------------------------
    // Name: 
    //-----------------------------------------------------------------------------
};
