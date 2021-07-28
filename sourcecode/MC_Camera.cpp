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
// File: MC_Camera.cpp: implementation of the MC_Camera class.
//-----------------------------------------------------------------------------
#include "ExternNamespace.h"

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
	// Name: MC_Camera()
	// Desc: MC_Camera class
	//-----------------------------------------------------------------------------
	MC_Camera::MC_Camera()
	{
		Init();
		viewAngle = 0.0f;
		aspect = 0.0f;
		nearDist = 0.0f;
		farDist = 0.0f;
	}
	//-Public----------------------------------------------------------------------
	// Name: ~MC_Camera()
	// Desc: ~MC_Camera Destruction class
	//-----------------------------------------------------------------------------
	MC_Camera::~MC_Camera()
	{

	}
	//-Public----------------------------------------------------------------------
	// Name: Init()
	//-----------------------------------------------------------------------------
	void MC_Camera::Init()
	{
		eye.x = 0.0f;		eye.y = 0.0f;		eye.z = 0.0f;
		look.x = 0.0f;		look.y = 0.0f;		look.z = 0.0f;
		u.x = 0.0f;			u.y = 0.0f;			u.z = 0.0f;
		v.x = 0.0f;			v.y = 0.0f;			v.z = 0.0f;
		n.x = 0.0f;			n.y = 0.0f;			n.z = 0.0f;
		up.x = 0.0f;		up.y = 0.0f;		up.z = 0.0f;
	}
	//-Public----------------------------------------------------------------------
	// Name: SetShape(double angle, double aspectRatio, double nearN, double farN)
	//-----------------------------------------------------------------------------
	void MC_Camera::SetShape(double angle, double aspectRatio, double nearN, double farN)
	{
		gluPerspective(angle, aspectRatio, nearN, farN);
	}
	//-Public----------------------------------------------------------------------
	// Name: LookAt(float eyex,float eyey,float eyez,float lookx,float looky,float lookz,float upx,float upy,float upz)
	//-----------------------------------------------------------------------------
	void MC_Camera::LookAt(float eyex, float eyey, float eyez, float lookx, float looky, float lookz, float upx, float upy, float upz)
	{
		eye = MC_Point3(eyex, eyey, eyez);
		up = MC_Vector3(upx, upy, upz);
		n = MC_Vector3(eyex - lookx, eyey - looky, eyez - lookz);
		up.cross(n, u);
		n.cross(u, v);
		n.normalize();
		v.normalize();
		u.normalize();
	}
	//-Public----------------------------------------------------------------------
	// Name: LookAt(MC_Point3 Eye,MC_Point3 Look,MC_Vector3 Up)
	//-----------------------------------------------------------------------------
	void MC_Camera::LookAt(MC_Point3 Eye, MC_Point3 Look, MC_Vector3 Up)
	{
		LookAt(Eye.x, Eye.y, Eye.z, Look.x, Look.y, Look.z, Up.x, Up.y, Up.z);
	}
	//-Public----------------------------------------------------------------------
	// Name: Roll(float angle)
	//-----------------------------------------------------------------------------
	void MC_Camera::Roll(float angle)
	{
		float csinv = (float)(cos(M_PI / 180 * angle));
		float sinv = (float)(sin(M_PI / 180 * angle));

		u.set(csinv * u.x + sinv * v.x, csinv * u.y + sinv * v.y, csinv * u.z + sinv * v.z);
		v.set(-sinv * u.x + csinv * v.x, -sinv * u.y + csinv * v.y, -sinv * u.z + csinv * v.z);
	}
	//-Public----------------------------------------------------------------------
	// Name: Pitch(float angle)
	//-----------------------------------------------------------------------------
	void MC_Camera::Pitch(float angle)
	{
		float csinv = (float)(cos(M_PI / 180 * angle));
		float sinv = (float)(sin(M_PI / 180 * angle));

		v.set(csinv * v.x + sinv * n.x, csinv * v.y + sinv * n.y, csinv * v.z + sinv * n.z);
		n.set(-sinv * v.x + csinv * n.x, -sinv * v.y + csinv * n.y, -sinv * v.z + csinv * n.z);
	}
	//-Public----------------------------------------------------------------------
	// Name: Yaw(float angle)
	//-----------------------------------------------------------------------------
	void MC_Camera::Yaw(float angle)
	{
		float csinv = (float)(cos(M_PI / 180 * angle));
		float sinv = (float)(sin(M_PI / 180 * angle));

		u.set(csinv * u.x + sinv * n.x, csinv * u.y + sinv * n.y, csinv * u.z + sinv * n.z);
		n.set(-sinv * u.x + csinv * n.x, -sinv * u.y + csinv * n.y, -sinv * u.z + csinv * n.z);
	}
	//-Public----------------------------------------------------------------------
	// Name: Slide(float Upart,float Vpart,float Npart)
	//-----------------------------------------------------------------------------
	void MC_Camera::Slide(float Upart, float Vpart, float Npart)
	{
		eye.x += Upart * u.x + Vpart * v.x + Npart * n.x;
		eye.y += Upart * u.y + Vpart * v.y + Npart * n.y;
		eye.z += Upart * u.z + Vpart * v.z + Npart * n.z;
	}
	//-Public----------------------------------------------------------------------
	// Name: SetEye(float x,float y,float z)
	//-----------------------------------------------------------------------------
	void MC_Camera::SetEye(float x, float y, float z)
	{
		eye.x = x;
		eye.y = y;
		eye.z = z;
	}
	//-Public----------------------------------------------------------------------
	// Name: SetModelViewMatrix()
	//-----------------------------------------------------------------------------
	void MC_Camera::SetModelViewMatrix()
	{
		MC_Vector3 eyet = MC_Vector3(eye.x, eye.y, eye.z);
		float   M[4][4];

		M[0][0] = u.x;			M[0][1] = v.x;			M[0][2] = n.x;			M[0][3] = 0;
		M[1][0] = u.y;			M[1][1] = v.y;			M[1][2] = n.y;			M[1][3] = 0;
		M[2][0] = u.z;			M[2][1] = v.z;			M[2][2] = n.z;			M[2][3] = 0;
		M[3][0] = -eyet.dot(u);	M[3][1] = -eyet.dot(v);	M[3][2] = -eyet.dot(n);	M[3][3] = 1;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((float*)M);
	}








	//*****************************************************************************  
	// Protected Code
	//*****************************************************************************

	//-Protected-------------------------------------------------------------------
	// Name: ()
	//-----------------------------------------------------------------------------

};
