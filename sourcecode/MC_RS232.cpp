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
// File: MC_RS232.cpp: implementation of the MC_RS232 class.
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
    const char* m_comports[RS232_MAXPORTs] = {  "\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",  "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
                                                "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12", "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16",
                                                "\\\\.\\COM17", "\\\\.\\COM18", "\\\\.\\COM19", "\\\\.\\COM20", "\\\\.\\COM21", "\\\\.\\COM22", "\\\\.\\COM23", "\\\\.\\COM24",
                                                "\\\\.\\COM25", "\\\\.\\COM26", "\\\\.\\COM27", "\\\\.\\COM28", "\\\\.\\COM29", "\\\\.\\COM30", "\\\\.\\COM31", "\\\\.\\COM32" };

    //-----------------------------------------------------------------------------
    // message handlers
    //-----------------------------------------------------------------------------

    //-Public----------------------------------------------------------------------
    // Name: MC_RS232()
    // Desc: MC_RS232 class
    //-----------------------------------------------------------------------------
    MC_RS232::MC_RS232()
    {
        memset(&m_Thread_Status, 0, sizeof(m_Thread_Status));
        memset(&m_portHandle, 0, sizeof(m_portHandle));
        memset(&m_ModeStr, 0, sizeof(m_ModeStr));
        memset(&m_info, 0, sizeof(m_info));
        memset(&m_ComBuffer, 0, sizeof(m_ComBuffer));
        memset(&m_ComData, 0, sizeof(m_ComData));
        m_ConnectionDead = false;
        m_hWnd = NULL;
    }
    //-Public----------------------------------------------------------------------
    // Name: ~MC_RS232()
    // Desc: ~MC_RS232 Destruction class
    //-----------------------------------------------------------------------------
    MC_RS232::~MC_RS232()
    {

    }
    //-Public----------------------------------------------------------------------
    // Name: OnCreate(HWND hWnd)
    //-----------------------------------------------------------------------------
    void MC_RS232::OnCreate(HWND hWnd)
    {
        m_hWnd = hWnd;
        Thread_Create();
    }
    //-Public----------------------------------------------------------------------
    // Name: OnDestroy()
    //-----------------------------------------------------------------------------
    void MC_RS232::OnDestroy()
    {
        if (m_info.ConnectionStatus) {
            CloseComport(m_info.ComPortNumber);
        }
        Thread_Stop();
    }
    //-Public----------------------------------------------------------------------
    // Name: OnConnect()
    //-----------------------------------------------------------------------------
    bool  MC_RS232::OnConnect()
    {
        std::string strport;
        std::string strBaudRate;
        std::string strmode;
        std::string strtmp;
        int BaudRate = 115200;
        int DataBits = 8;
        int StopBits = 2;
        char Mode[16];

        memset(&Mode, 0, sizeof(Mode));
        if (m_info.ConnectionStatus) {
            CloseComport(m_info.ComPortNumber);
            m_info.ConnectionStatus = false;
        }
        if (mc_Forms.m_SerialPort.Connect) {
            strport = wstring_to_utf8(mc_Forms.m_SerialPort.SerialPort);
            strBaudRate = wstring_to_utf8(mc_Forms.m_SerialPort.BaudRate);
            BaudRate = std::atoi(strBaudRate.c_str());
            strtmp = wstring_to_utf8(mc_Forms.m_SerialPort.DataBits);
            DataBits = std::atoi(strtmp.c_str());
            strtmp = wstring_to_utf8(mc_Forms.m_SerialPort.StopBits);
            StopBits = std::atoi(strtmp.c_str());
            strtmp = wstring_to_utf8(mc_Forms.m_SerialPort.ParityBit);
            strtmp = strtmp.at(1);
            sprintf_s(Mode, sizeof(Mode), "%d%s%d", DataBits, strtmp.c_str(), StopBits);
            strmode = Mode;
            if (Connect(strport.c_str(), BaudRate, strmode.c_str())) {
                MessageBox(m_hWnd, _T("Can't Connect To Port"), _T("Error"), MB_OK | MB_ICONERROR);
                return true;
            }
        }
        return false;
    }
    //-Public----------------------------------------------------------------------
    // Name: Connect(const char* ComPortStr, int BaudRate, const char* ComMode)
    //-----------------------------------------------------------------------------
    bool MC_RS232::Connect(const char* ComPortStr, int BaudRate, const char* ComMode)
    {
        m_info.ComPortNumber = GetComPortNumber(ComPortStr);                        // /dev/ttyS0 (COM1 on windows)
        if (OpenComport(m_info.ComPortNumber, BaudRate, ComMode, 0)) {
            m_ConnectionDead = false;
            m_info.ConnectionStatus = false;
            return true;
        }
        m_ConnectionDead = true;
        m_info.ConnectionStatus = true;
        SendText(m_info.ComPortNumber, "\r\n");
        return false;
    }
    //-Public----------------------------------------------------------------------
    // Name: OpenComport(int comport_number, int baudrate, const char* mode, int flowctrl)
    //-----------------------------------------------------------------------------
    int MC_RS232::OpenComport(int comport_number, int baudrate, const char* mode, int flowctrl)
    {
        DCB port_settings;
        COMMTIMEOUTS Cptimeouts;

        memset(&port_settings, 0, sizeof(port_settings));
        memset(&Cptimeouts, 0, sizeof(Cptimeouts));
        memset(&m_ModeStr, 0, sizeof(m_ModeStr));
        if ((comport_number >= RS232_MAXPORTs) || (comport_number < 0)) {
            return(1);
        }
        switch (baudrate) {
            case     110:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=110");               break;
            case     300:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=300");               break;
            case     600:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=600");               break;
            case    1200:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=1200");              break;
            case    2400:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=2400");              break;
            case    4800:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=4800");              break;
            case    9600:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=9600");              break;
            case   19200:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=19200");             break;
            case   38400:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=38400");             break;
            case   57600:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=57600");             break;
            case  115200:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=115200");            break;
            case  128000:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=128000");            break;
            case  256000:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=256000");            break;
            case  500000:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=500000");            break;
            case  921600:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=921600");            break;
            case 1000000:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=1000000");           break;
            case 1500000:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=1500000");           break;
            case 2000000:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=2000000");           break;
            case 3000000:   strcpy_s(m_ModeStr, sizeof(m_ModeStr), "baud=3000000");           break;
            default:                                                                          break;
        }
        if (strlen(mode) != 3) {
            return(1);
        }
        switch (mode[0]) {
            case '8':   strcat_s(m_ModeStr, sizeof(m_ModeStr), " data=8");    break;
            case '7':   strcat_s(m_ModeStr, sizeof(m_ModeStr), " data=7");    break;
            case '6':   strcat_s(m_ModeStr, sizeof(m_ModeStr), " data=6");    break;
            case '5':   strcat_s(m_ModeStr, sizeof(m_ModeStr), " data=5");    break;
            default:    return(1);                                            break;
        }
        switch (mode[1]) {
            case 'N':
            case 'n':   strcat_s(m_ModeStr, sizeof(m_ModeStr), " parity=n");  break;
            case 'E':
            case 'e':   strcat_s(m_ModeStr, sizeof(m_ModeStr), " parity=e");  break;
            case 'O':
            case 'o':   strcat_s(m_ModeStr, sizeof(m_ModeStr), " parity=o");  break;
            default:    return(1);                                            break;
        }
        switch (mode[2])
        {
            case '1':   strcat_s(m_ModeStr, sizeof(m_ModeStr), " stop=1");    break;
            case '2':   strcat_s(m_ModeStr, sizeof(m_ModeStr), " stop=2");    break;
            default:    return(1);                                            break;
        }
        if (flowctrl) {
            strcat_s(m_ModeStr, sizeof(m_ModeStr), " xon=off to=off odsr=off dtr=on rts=off");
        } else {
            strcat_s(m_ModeStr, sizeof(m_ModeStr), " xon=off to=off odsr=off dtr=on rts=on");
        }
        m_portHandle[comport_number] = CreateFileA(m_comports[comport_number], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (m_portHandle[comport_number] == INVALID_HANDLE_VALUE) {
            return(1);
        }
        port_settings.DCBlength = sizeof(port_settings);
        if (!BuildCommDCBA(m_ModeStr, &port_settings)) {
            CloseHandle(m_portHandle[comport_number]);
            return(1);
        }
        if (flowctrl) {
            port_settings.fOutxCtsFlow = TRUE;
            port_settings.fRtsControl = RTS_CONTROL_HANDSHAKE;
        }
        if (!SetCommState(m_portHandle[comport_number], &port_settings)) {
            CloseHandle(m_portHandle[comport_number]);
            return(1);
        }
        Cptimeouts.ReadIntervalTimeout = MAXDWORD;
        Cptimeouts.ReadTotalTimeoutMultiplier = 0;
        Cptimeouts.ReadTotalTimeoutConstant = 0;
        Cptimeouts.WriteTotalTimeoutMultiplier = 0;
        Cptimeouts.WriteTotalTimeoutConstant = 0;
        if (!SetCommTimeouts(m_portHandle[comport_number], &Cptimeouts)) {
            CloseHandle(m_portHandle[comport_number]);
            return(1);
        }
        return(0);
    }
    //-Public----------------------------------------------------------------------
    // Name: PollComport(int comport_number, unsigned char* buf, int size)
    //-----------------------------------------------------------------------------
    int MC_RS232::PollComport(int comport_number, unsigned char* buf, int size)
    {
        int n = 0;
        bool nError = false;
        DWORD ComError = 0;

        if (m_info.ConnectionStatus == false) {
            return(0);
        }
        nError = ReadFile(m_portHandle[comport_number], buf, size - 1, (LPDWORD)((void*)&n), NULL);
        if (nError) {
            return(n);
        } else {
            ComError = GetLastError();
            if (ComError != ERROR_IO_PENDING) {
                return(-1);
            }
            return(0);
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: SendByte(int comport_number, unsigned char byte)
    //-----------------------------------------------------------------------------
    int MC_RS232::SendByte(int comport_number, unsigned char byte)
    {
        int n = 0;
        bool nError = false;

        nError = WriteFile(m_portHandle[comport_number], &byte, 1, (LPDWORD)((void*)&n), NULL);
        if (nError) {
            if (n < 0)
                return(1);

            return(0);
        } else {
            return(1);
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: SendBuffer(int comport_number, unsigned char* buf, int size)
    //-----------------------------------------------------------------------------
    int MC_RS232::SendBuffer(int comport_number, unsigned char* buf, int size)
    {
        int n = 0;

        if (WriteFile(m_portHandle[comport_number], buf, size, (LPDWORD)((void*)&n), NULL)) {
            return(n);
        }
        return(-1);
    }
    //-Public----------------------------------------------------------------------
    // Name: SendText(int comport_number, const char* text)
    //-----------------------------------------------------------------------------
    void MC_RS232::SendText(int comport_number, const char* text)
    {
        while (*text != 0)
            SendByte(comport_number, *(text++));

    }
    //-Public----------------------------------------------------------------------
    // Name: CloseComport(int comport_number)
    //-----------------------------------------------------------------------------
    void MC_RS232::CloseComport(int comport_number)
    {
        m_info.ConnectionStatus = false;
        CloseHandle(m_portHandle[comport_number]);
    }
    //-Public----------------------------------------------------------------------
    // Name: IsDCDEnabled(int comport_number)
    //-----------------------------------------------------------------------------
    int MC_RS232::IsDCDEnabled(int comport_number)
    {
        int status;

        GetCommModemStatus(m_portHandle[comport_number], (LPDWORD)((void*)&status));
        if (status & MS_RLSD_ON)
            return(1);
        else
            return(0);

    }
    //-Public----------------------------------------------------------------------
    // Name: IsRINGEnabled(int comport_number)
    //-----------------------------------------------------------------------------
    int MC_RS232::IsRINGEnabled(int comport_number)
    {
        int status;

        GetCommModemStatus(m_portHandle[comport_number], (LPDWORD)((void*)&status));
        if (status & MS_RING_ON)
            return(1);
        else
            return(0);

    }
    //-Public----------------------------------------------------------------------
    // Name: IsCTSEnabled(int comport_number)
    //-----------------------------------------------------------------------------
    int MC_RS232::IsCTSEnabled(int comport_number)
    {
        int status;

        GetCommModemStatus(m_portHandle[comport_number], (LPDWORD)((void*)&status));
        if (status & MS_CTS_ON)
            return(1);
        else
            return(0);

    }
    //-Public----------------------------------------------------------------------
    // Name: IsDSREnabled(int comport_number)
    //-----------------------------------------------------------------------------
    int MC_RS232::IsDSREnabled(int comport_number)
    {
        int status;

        GetCommModemStatus(m_portHandle[comport_number], (LPDWORD)((void*)&status));
        if (status & MS_DSR_ON)
            return(1);
        else
            return(0);

    }
    //-Public----------------------------------------------------------------------
    // Name: EnableDTR(int comport_number)
    //-----------------------------------------------------------------------------
    void MC_RS232::EnableDTR(int comport_number)
    {
        EscapeCommFunction(m_portHandle[comport_number], SETDTR);
    }
    //-Public----------------------------------------------------------------------
    // Name: DisableDTR(int comport_number)
    //-----------------------------------------------------------------------------
    void MC_RS232::DisableDTR(int comport_number)
    {
        EscapeCommFunction(m_portHandle[comport_number], CLRDTR);
    }
    //-Public----------------------------------------------------------------------
    // Name: EnableRTS(int comport_number)
    //-----------------------------------------------------------------------------
    void MC_RS232::EnableRTS(int comport_number)
    {
        EscapeCommFunction(m_portHandle[comport_number], SETRTS);
    }
    //-Public----------------------------------------------------------------------
    // Name: DisableRTS(int comport_number)
    //-----------------------------------------------------------------------------
    void MC_RS232::DisableRTS(int comport_number)
    {
        EscapeCommFunction(m_portHandle[comport_number], CLRRTS);
    }
    //-Public----------------------------------------------------------------------
    // Name: FlushRX(int comport_number)
    //-----------------------------------------------------------------------------
    void MC_RS232::FlushRX(int comport_number)
    {
        PurgeComm(m_portHandle[comport_number], PURGE_RXCLEAR | PURGE_RXABORT);
    }
    //-Public----------------------------------------------------------------------
    // Name: FlushTX(int comport_number)
    //-----------------------------------------------------------------------------
    void MC_RS232::FlushTX(int comport_number)
    {
        PurgeComm(m_portHandle[comport_number], PURGE_TXCLEAR | PURGE_TXABORT);
    }
    //-Public----------------------------------------------------------------------
    // Name: FlushRXTX(int comport_number)
    //-----------------------------------------------------------------------------
    void MC_RS232::FlushRXTX(int comport_number)
    {
        PurgeComm(m_portHandle[comport_number], PURGE_RXCLEAR | PURGE_RXABORT);
        PurgeComm(m_portHandle[comport_number], PURGE_TXCLEAR | PURGE_TXABORT);
    }
    //-Public----------------------------------------------------------------------
    // Name: GetComPortNumber(const char* devname)
    //-----------------------------------------------------------------------------
    int MC_RS232::GetComPortNumber(const char* devname)
    {
        int i;
        char str[32];

        strcpy_s(str, sizeof(str), "\\\\.\\");
        strncat_s(str, sizeof(str), devname, 16);
        str[31] = 0;
        for (i = 0; i < RS232_MAXPORTs; i++) {
            if (!strcmp(m_comports[i], str)) {
                return i;
            }
        }
        return -1;
    }
    //-Public----------------------------------------------------------------------
    // Name: ListComPort()
    //-----------------------------------------------------------------------------
    bool MC_RS232::ListComPort()
    {
        char lpTargetPath[5000];
        bool gotPort = false;

        for (int i = 0; i < 255; i++) {
            std::string str = "COM" + std::to_string(i);
            DWORD test = QueryDosDeviceA(str.c_str(), lpTargetPath, 5000);
            if (test != 0) {
                //           printf("%s: %s\r\n", str.c_str(), lpTargetPath);
                gotPort = true;
            }
            if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {

            }
        }
        return gotPort;
    }
    //-Public----------------------------------------------------------------------
    // Name: Thread_Create()
    //-----------------------------------------------------------------------------
    void MC_RS232::Thread_Create()
    {
        m_Thread_Status.Exit = false;
        m_Thread_Status.Running = false;
        mc_Thread_Rs232 = std::thread(&MC_RS232::Thread_CallBack_Rs232, this, 0);
        //	Sleep(10);
        if (mc_Thread_Rs232.joinable()) {
            //       m_Thread_OpenGL.join();
        }
    }
    //-Public----------------------------------------------------------------------
    // Name: Thread_Stop()
    //-----------------------------------------------------------------------------
    void MC_RS232::Thread_Stop()
    {
        bool TimeOut = false;
        ULONGLONG	nTick = GetTickCount64();
        ULONGLONG   nLastTick = nTick;

        m_Thread_Status.Exit = true;
        while (m_Thread_Status.Running == true && TimeOut == false) {
            if (nTick - nLastTick > RS232_THREADTIMEOUT) {
                TimeOut = true;
                MessageBox(m_hWnd, _T("Thread_Stop Timeout"), _T("Error"), MB_OK | MB_ICONERROR);
            }
            Sleep(1);
        }
    }













    //*****************************************************************************  
    // Protected Code
    //*****************************************************************************

    //-Protected-------------------------------------------------------------------
    // Name: Thread_CallBack_Rs232(int MultiThread_ID)
    //-----------------------------------------------------------------------------
    void MC_RS232::Thread_CallBack_Rs232(int MultiThread_ID)
    {
        m_Thread_Status.Running = true;
        while (m_Thread_Status.Exit == false) {
            Sleep(5);
            Thread_Rs232_Receive(MultiThread_ID);
        }
        m_Thread_Status.Running = false;
        mc_Thread_Rs232.detach();
    }
    //-Protected-------------------------------------------------------------------
    // Name: Thread_Rs232_Receive(int MultiThread_ID,bool)
    //-----------------------------------------------------------------------------
    void MC_RS232::Thread_Rs232_Receive(int MultiThread_ID)
    {
        uint64_t SystemTick64 = 0;
        uint64_t TimeOutTick64 = 0;
        //    std::string Data;

        Sleep(5);
        while (m_Thread_Status.Exit == false) {
            if (m_ConnectionDead) {
                m_ConnectionDead = false;
            }
            Sleep(1);
            m_info.Rx.DataLen = PollComport(m_info.ComPortNumber, m_info.Rx.Data, sizeof(m_info.Rx.Data));
            if (m_info.Rx.DataLen > 0) {
                m_info.Rx.Data[m_info.Rx.DataLen] = 0;                              // always put a "null" at the end of a string
                ReceiveData(&m_info.Rx);
            }
            SystemTick64 = GetTickCount64();
            if (SystemTick64 > TimeOutTick64) {
                TimeOutTick64 = SystemTick64 + 1000;
            }
        }
        if (!m_ConnectionDead) {
            m_ConnectionDead = true;
        }
        if (m_info.ConnectionStatus) {
            CloseComport(m_info.ComPortNumber);
            m_info.ConnectionStatus = false;
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: ReceiveData(RS232_Buffer* Receive)
    //-----------------------------------------------------------------------------
    void MC_RS232::ReceiveData(RS232_Buffer* Receive)
    {
        int i;
        long DataLen = 0;

        m_ComBuffer.RxDatalen = Receive->DataLen;
        memcpy_s(&m_ComBuffer.RxData, sizeof(m_ComBuffer.RxData), Receive->Data, Receive->DataLen);
        m_ComBuffer.RxData[Receive->DataLen] = 0;
        for (i = 0; i < m_ComBuffer.RxDatalen; i++) {
            if (m_ComBuffer.Data_ep < RS232_RINGBUFFERSIZE) {
                m_ComBuffer.Data_Ring[m_ComBuffer.Data_ep] = m_ComBuffer.RxData[i];
            }
            m_ComBuffer.Data_ep++;
            if (m_ComBuffer.Data_ep >= RS232_RINGBUFFERSIZE) {
                m_ComBuffer.Data_ep = 0;
            }
        }
        if (m_ComBuffer.Data_cp == m_ComBuffer.Data_ep) {
            DataLen = 0;
        } else if (m_ComBuffer.Data_cp < m_ComBuffer.Data_ep) {
            DataLen = m_ComBuffer.Data_ep - m_ComBuffer.Data_cp;
        } else if (m_ComBuffer.Data_cp > m_ComBuffer.Data_ep) {
            DataLen = (m_ComBuffer.Data_ep + RS232_RINGBUFFERSIZE) - m_ComBuffer.Data_cp;
        } if (DataLen > 0) {
            if (DataLen < RS232_RINGBUFFERSIZE) {
                do {

                } while (CommsBufferRemoveRxMsgArray());
            }
        }
    }
    //-Protected-------------------------------------------------------------------
    // Name: CommsBufferRemoveRxMsgArray()
    //-----------------------------------------------------------------------------
    bool MC_RS232::CommsBufferRemoveRxMsgArray()
    {
        unsigned char Data;
        unsigned char Data_0D;
        long DataLen, LFFound, i;
        long tmpcp;

        DataLen = 0;
        if (m_ComBuffer.Data_cp == m_ComBuffer.Data_ep) {
            DataLen = 0;
        } else if (m_ComBuffer.Data_cp < m_ComBuffer.Data_ep) {
            DataLen = m_ComBuffer.Data_ep - m_ComBuffer.Data_cp;
        } else if (m_ComBuffer.Data_cp > m_ComBuffer.Data_ep) {
            DataLen = m_ComBuffer.Data_ep + RS232_RINGBUFFERSIZE - m_ComBuffer.Data_cp;
        }
        if (DataLen > 0) {
            LFFound = 0;
            Data_0D = 0;
            for (i = 0; i < DataLen; i++) {
                tmpcp = i + m_ComBuffer.Data_cp;
                if (tmpcp >= RS232_RINGBUFFERSIZE)
                    tmpcp = (tmpcp - RS232_RINGBUFFERSIZE);

                Data = m_ComBuffer.Data_Ring[tmpcp];
                m_ComData.Data[i] = Data;
                if (Data_0D == 0x0D && Data == 0x0A) {
                    LFFound = i + 1;
                    m_ComData.len = LFFound;
                    m_ComBuffer.Data_cp = tmpcp + 1;
                    if (m_ComBuffer.Data_cp >= RS232_RINGBUFFERSIZE)
                        m_ComBuffer.Data_cp = m_ComBuffer.Data_cp - RS232_RINGBUFFERSIZE;

                    break;
                }
                Data_0D = Data;
            }
            m_ComData.Data[LFFound] = 0;
            if (LFFound > 0) {
                if (mc_NanoVNA.DecodeData((char*)&m_ComData.Data, m_ComData.len)) {

                } else {

                }
                return true;
            }
        }
        return false;
    }
    //-Protected-------------------------------------------------------------------
    // Name: utf8_to_wstring(const std::string& str) 
    //-----------------------------------------------------------------------------
    std::wstring MC_RS232::utf8_to_wstring(const std::string& str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        return myconv.from_bytes(str);
    }
    //-Protected-------------------------------------------------------------------
    // Name: wstring_to_utf8(const std::wstring& str)
    //-----------------------------------------------------------------------------
    std::string MC_RS232::wstring_to_utf8(const std::wstring& str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        return myconv.to_bytes(str);
    }
};