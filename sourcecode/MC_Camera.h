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
// File: MC_Camera.h: interface for the MC_Camera class.
// Desc: Application interface for the MC_Camera class.
//-----------------------------------------------------------------------------
#ifndef MC_Camera_H
	#define MC_Camera_H

#pragma once

namespace NanoVNA {
	//-----------------------------------------------------------------------------
	// Const
	//-----------------------------------------------------------------------------
	#ifndef M_PI
		#define M_PI			3.14159265358979323846
	#endif
	//-----------------------------------------------------------------------------
	// Struct
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	// Name: class MC_Point3
	// Desc: Application class. The base class (MC_Point3)
	//-----------------------------------------------------------------------------
	class MC_Point3
	{

		public:
			float				x, y, z;

		protected:

		private:

			//-----------------------------------------------------------------------------

		public:
								MC_Point3() : x(0.0f), y(0.0f), z(0.0f) {}
								MC_Point3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }
		
			void 				set(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }
			void				add(const MC_Point3& point) { x += point.x; y += point.y; z += point.z; }
			void				sub(const MC_Point3& point) { x -= point.x; y -= point.y; z -= point.z; }

		protected:

		private:

	};
	//-----------------------------------------------------------------------------
	// Name: class MC_Vector3
	// Desc: Application class. The base class (MC_Vector3)
	//-----------------------------------------------------------------------------
	class MC_Vector3
	{

		public:
			float				x, y, z;

		protected:

		private:

			//-----------------------------------------------------------------------------

		public:
								MC_Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
								MC_Vector3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }

			void				set(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }
			void				add(const MC_Vector3& vec) { x += vec.x; y += vec.y; z += vec.z; }
			void				sub(const MC_Vector3& vec) { x -= vec.x; y -= vec.y; z -= vec.z; }
			void				mult(float value) { x *= value; y *= value; z *= value; }
			float				dot(const MC_Vector3& src) { return x * src.x + y * src.y + z * src.z; }
			void				cross(const MC_Vector3& src, MC_Vector3& result) { result.x = y * src.z - z * src.y; 	result.y = z * src.x - x * src.z; result.z = x * src.y - y * src.x; }
			void				normalize() { float length = x * x + y * y + z * z; float frac = 1.0f / (float)sqrt(length); x *= frac; y *= frac; z *= frac; }

		protected:

		private:

	};
	//-----------------------------------------------------------------------------
	// Name: class MC_Camera
	// Desc: Application class
	//-----------------------------------------------------------------------------
	class MC_Camera
	{
		public:
			MC_Point3			eye;
			MC_Point3			look;													// Position in world coordinates of the camera

		protected:
			MC_Vector3			u, v, n, up;											// Camera coordinate axis
			double				viewAngle, aspect, nearDist, farDist;

		private:

			//-----------------------------------------------------------------------------

		public:
								MC_Camera();
			virtual				~MC_Camera();

			void				Init();
			void				SetShape(double angle, double aspectRatio, double nearN, double farN);
			void				LookAt(MC_Point3 eye, MC_Point3 look, MC_Vector3 up);
			void				LookAt(float eyex, float eyey, float eyez, float lookx, float looky, float lookz, float upx, float upy, float upz);
			void				Roll(float angle);
			void				Pitch(float angle);
			void				Yaw(float angle);
			void				Slide(float Upart, float Vpart, float Npart);
			void				SetEye(float x, float y, float z);
			void				SetModelViewMatrix();

		protected:

		private:

	};
};
//-----------------------------------------------------------------------------
#endif // MC_Camera_H