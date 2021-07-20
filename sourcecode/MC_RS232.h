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
// File: MC_RS232.h: interface for the MC_RS232 class.
// Desc: Application interface for the MC_RS232 class.
//-----------------------------------------------------------------------------
#ifndef MC_RS232e_H
	#define MC_RS232_H

#pragma once

namespace NanoVNA {
	//-----------------------------------------------------------------------------
	// Const
	//-----------------------------------------------------------------------------
	#define RS232_MAXPORTs						32
	#define RS232_MODESTRLEN					128
	#define RS232_BUFFERSIZE					8192
	#define RS232_THREADTIMEOUT					5000

	#define	RS232_RINGBUFFERSIZE				(8192 * 16)

	//-----------------------------------------------------------------------------
	// Struct
	//-----------------------------------------------------------------------------
	typedef struct _RS232_Buffer
	{
		unsigned char		Data[RS232_BUFFERSIZE];
		int					DataLen;	
	} RS232_Buffer;

	typedef struct _RS232_Info
	{
		int					ComPortNumber;
		bool				ConnectionStatus;
		RS232_Buffer		Tx;
		RS232_Buffer		Rx;
	} RS232_Info;

	typedef struct _RAWCOMMSDATA
	{
		unsigned char		RxData[RS232_BUFFERSIZE];
		long				RxDatalen;
		unsigned char		Data_Ring[RS232_RINGBUFFERSIZE];
		long				Data_cp;
		long				Data_ep;
	} RAWCOMMSDATA;

	typedef struct _COMMSDATA
	{
		unsigned char		Data[RS232_RINGBUFFERSIZE];
		long				len;
	} COMMSDATA;

	typedef struct _RS232_THREAD
	{
		bool				Exit;
		bool				Running;
	} RS232_THREAD;

	//-----------------------------------------------------------------------------
	// Name: class MC_RS232
	// Desc: Application class
	//-----------------------------------------------------------------------------
	class MC_RS232
	{

		public:
			RS232_THREAD		m_Thread_Status;
			bool				m_ConnectionDead;
			RS232_Info			m_info;
			HWND				m_hWnd;
			RAWCOMMSDATA		m_ComBuffer;
			COMMSDATA			m_ComData;

		protected:
			std::thread			mc_Thread_Rs232;
			HANDLE				m_portHandle[RS232_MAXPORTs];
			char				m_ModeStr[RS232_MODESTRLEN];

		private:

			//-----------------------------------------------------------------------------

		public:
								MC_RS232();
			virtual				~MC_RS232();
			void				OnCreate(HWND hWnd);
			void				OnDestroy();
			bool				OnConnect();
			bool				Connect(const char* ComPortStr, int BaudRate, const char* ComMode);
			int					OpenComport(int comport_number, int baudrate, const char* mode, int flowctrl);
			int					PollComport(int comport_number, unsigned char* buf, int size);
			int					SendByte(int comport_number, unsigned char byte);
			int					SendBuffer(int comport_number, unsigned char* buf, int size);
			void				SendText(int comport_number, const char* text);
			void				CloseComport(int comport_number);
			int					IsDCDEnabled(int comport_number);
			int					IsRINGEnabled(int comport_number);
			int					IsCTSEnabled(int comport_number);
			int					IsDSREnabled(int comport_number);
			void				EnableDTR(int comport_number);
			void				DisableDTR(int comport_number);
			void				EnableRTS(int comport_number);
			void				DisableRTS(int comport_number);
			void				FlushRX(int comport_number);
			void				FlushTX(int comport_number);
			void				FlushRXTX(int comport_number);
			int					GetComPortNumber(const char* devname);
			bool				ListComPort();
			void				Thread_Create();
			void				Thread_Stop();

		protected:
			void				Thread_CallBack_Rs232(int MultiThread_ID);
			void				Thread_Rs232_Receive(int MultiThread_ID);
			void				ReceiveData(RS232_Buffer* Receive);
			bool				CommsBufferRemoveRxMsgArray();
			std::wstring		utf8_to_wstring(const std::string& str);
			std::string			wstring_to_utf8(const std::wstring& str);

		private:

	};
};
//-----------------------------------------------------------------------------
#endif // MC_RS232_H
