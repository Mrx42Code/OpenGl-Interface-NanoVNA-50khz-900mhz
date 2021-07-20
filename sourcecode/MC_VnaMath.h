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
// File: MC_VnaMath.h: interface for the MC_VnaMath class.
// Desc: Application interface for the MC_VnaMath class.
//-----------------------------------------------------------------------------
#ifndef MC_VnaMath_H
	#define MC_VnaMath_H

#pragma once

#include <stdint.h>
#include <complex.h>
#include <complex>

#include "CommanStruct.h"

namespace NanoVNA {
	//-----------------------------------------------------------------------------
	// Const
	//-----------------------------------------------------------------------------
	#define VNA_PI								3.14159265358979323846
	#define VNA_PI2								(VNA_PI * 2.0)
	#define VNA_SPEEDOFLIGHT					299.792458
	#define VNA_CHARACTERISTIC_IMPEDANCE		50.0

	//-----------------------------------------------------------------------------
	// Struct
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	// Name: class MC_VnaMath
	// Desc: Application class
	//-----------------------------------------------------------------------------
	class MC_VnaMath
	{

		public:

		protected:

		private:

			//-----------------------------------------------------------------------------

		public:
								MC_VnaMath();
			virtual				~MC_VnaMath();
			void				GetCapacitanceInductance(char* buf, int len, VNARAWVALUES* Complex, uint32_t Frequency);
			double				Linear(VNARAWVALUES* Complex);
			double				LogMag(VNARAWVALUES* Complex);
			double				Swr(VNARAWVALUES* Complex);
			double				Resitance(VNARAWVALUES* Complex);
			double				Reactance(VNARAWVALUES* Complex);
			double				CapacitanceInductance(VNARAWVALUES* Complex, uint32_t Frequency, char* Prefix);
			double				Phase(VNARAWVALUES* Complex);
			double              mw_to_dbm(double mW);
			double              dbm_to_mw(double dBm);
			double              mw_to_rms(double mw, double res);
			double              rms_to_ptp(double rms);
			double              rms_to_ptp2(double rms);
			double              LossInFreeSpace(double FrequencyMhz, double Distance, double AntennaGain1, double AntennaGain2);
			double				db_to_swr(double dBm);
			double				Power_To_Swr(double power_fwd, double power_rvr);
			double				Power_To_Dbm(double power);
			double				Voltage_To_Power(double voltage);
			double				Power_To_Voltage(double power);
			double				logBased(double value, double base);
			_Dcomplex			Polar_To_Complex(double magnitude, double phase);
			_Dcomplex			ComplexLoadFromReflection(double magnitude, double phase);
			int					String_Value_With_Prefix(char* buf, int len, double val, const char* unit);
			void				Gamma2Resistance(char* buf, int len, VNARAWVALUES* Complex);
			void				Gamma2Reactance(char* buf, int len, VNARAWVALUES* Complex);
			void				Gamma2CapacitanceInductance(char* buf, int len, VNARAWVALUES* Complex, uint32_t Frequency);
			void				Frequency_String(char* buf, size_t len, int32_t freq);
			double				ImpedanceToCapacitance(double Impedance, double Freq);
			double				ImpedanceToInductance(double Impedance, double Freq);
			VNAVALUES_S11_Real	RealImpedance_S11(std::complex<double> s11, double freqHz, double z0);
			void				ImpedanceDisplay(std::complex<double> s11, double freqHz, double z0);
			double				SeriesCapacitanceInductance(double freq, double Z);
			double				ParallelCapacitanceInductance(double freq, double Y);
			double				Value_Scale(double val);
			const char*			Value_Unit(double val);
			double				Value_ScaleFrequency(double val);
			const char*			Value_UnitFrequency(double val);

		protected:

		private:

	};
};
//-----------------------------------------------------------------------------
#endif // MC_NanoVNA_H

