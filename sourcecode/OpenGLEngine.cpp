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
// File: OpenGLEngine.cpp: implementation of the OpenGLEngine c++.
//-----------------------------------------------------------------------------
#include "ExternNamespace.h"

using namespace std;

//-Protected-------------------------------------------------------------------
// Const
//-----------------------------------------------------------------------------

//-Protected-------------------------------------------------------------------
// Struct
//-----------------------------------------------------------------------------

//*****************************************************************************  
// Public Code
//*****************************************************************************

//-----------------------------------------------------------------------------
// IMPLEMENT_DYNCREATE
//-----------------------------------------------------------------------------
HINSTANCE hInst;                                                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                                                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];                                            // the main window class name

//-----------------------------------------------------------------------------
// message handlers
//-----------------------------------------------------------------------------

//-Public----------------------------------------------------------------------
// Name: wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
//-----------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    MSG msg;
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OPENGLENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OPENGLENGINE));
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (mc_OpenGL.m_Input.Keys[VK_ESCAPE]) {
            SendMessage(mc_OpenGL.m_Info.hWnd, WM_CLOSE, 0, 0);
        }
    }
    return (int) msg.wParam;
}
//-Public----------------------------------------------------------------------
// Name: MyRegisterClass(HINSTANCE hInstance)
//-----------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OPENGLENGINE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OPENGLENGINE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}
//-Public----------------------------------------------------------------------
// Name: InitInstance(HINSTANCE hInstance, int nCmdShow)
//-----------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd) {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   SetFocus(hWnd);
   return TRUE;
}
//-Public----------------------------------------------------------------------
// Name: WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_CREATE: {
                mc_Forms.OnCreate(hWnd);
                mc_OpenGL.OnCreate(hWnd);
                SetTimer(hWnd, OPENGL_TIMER_RENDER, OPENGL_TIMER_RENDER_TICK, (TIMERPROC)NULL);
            }
            break;
        case WM_COMMAND: {
                int wmId = LOWORD(wParam);
                if (WndProcMenu(hWnd, wmId)) {
                    return DefWindowProc(hWnd, message, wParam, lParam);
                }
            }
            break;
        case WM_PAINT: { 
                mc_OpenGL.OnDrawScene();
                ValidateRect(hWnd, NULL);
            } 
            break;
        case WM_KEYDOWN:
            mc_OpenGL.OnKeys(TRUE, (DWORD)wParam);
            break;
        case WM_KEYUP:
            mc_OpenGL.OnKeys(FALSE, (DWORD)wParam);
            break;
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            mc_OpenGL.OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
            break;
        case WM_MOUSEWHEEL:
            mc_OpenGL.OnMouseWheel(HIWORD(wParam));
            break;
        case WM_SIZE:
            mc_OpenGL.OnReSize(LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_DESTROY:
        case WM_CLOSE:
            ChangeDisplaySettings(NULL, 0);
            mc_Forms.OnDestroy();
            mc_OpenGL.OnDestroy();
            PostQuitMessage(0);
            break;
        case WM_TIMER: {
            switch (wParam) {
                case  OPENGL_TIMER_RENDER:
                    InvalidateRect(hWnd, NULL, FALSE);
                    break;
                case OPENGL_TIMER_ID:
                    mc_OpenGL.OnTimer();
                    break;
                }
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
//-Public----------------------------------------------------------------------
// Name: WndProcMenu(HWND hWnd, UINT wmId)
//-----------------------------------------------------------------------------
bool WndProcMenu(HWND hWnd, UINT wmId)
{
    switch (wmId) {
        case IDM_OPEN:
            mc_Forms.OnMenu_FileOpen(hWnd);
            break;
        case IDM_SAVE:
            mc_Forms.OnMenu_FileSave(hWnd);
            break;
        case IDM_CONNECT:
            mc_Forms.OnMenu_ConnectDisconnect(hWnd);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_DATA_CLEAR:
            mc_NanoVNA.m_VNAMultiData.Stop = true;
            mc_Forms.OnMenu_Data_Clear(hWnd);
            break;
        case IDM_DATA_S11:
            mc_Forms.OnMenu_GetData_S11(hWnd);
            break;
        case IDM_DATA_S21:
            mc_Forms.OnMenu_GetData_S21(hWnd);
            break;
        case IDM_DATA_2:
            mc_Forms.OnMenu_GetData_Type(hWnd, 2);
            break;
        case IDM_DATA_3:
            mc_Forms.OnMenu_GetData_Type(hWnd, 3);
            break;
        case IDM_DATA_4:
            mc_Forms.OnMenu_GetData_Type(hWnd, 4);
            break;
        case IDM_DATA_5:
            mc_Forms.OnMenu_GetData_Type(hWnd, 5);
            break;
        case IDM_DATA_6:
            mc_Forms.OnMenu_GetData_Type(hWnd, 6);
            break;
        case IDM_AUTO_DATA_S11:
            mc_Forms.OnMenu_GetAutoData_S11(hWnd);
            break;
        case IDM_AUTO_DATA_S21:
            mc_Forms.OnMenu_GetAutoData_S21(hWnd);
            break;

        case IDM_CHARTS_2D3D:
            mc_Forms.OnMenu_Chart_2D3D(hWnd);
            break;
        case IDM_CHARTS_GRID:
            mc_Forms.OnMenu_Chart_Grid(hWnd);
            break;
        case IDM_CHARTS_GRIDFINE:
            mc_Forms.OnMenu_Chart_GridFine(hWnd);
            break;
        case IDM_CHARTSMITH_RED:
            mc_Forms.OnMenu_Chart_SmithChartRed(hWnd);
            break;
        case IDM_CHARTSMITH_BLUE:
            mc_Forms.OnMenu_Chart_SmithChartBlue(hWnd);
            break;
        case IDM_CHARTSMITH_IMAGE:
            mc_Forms.OnMenu_Chart_SmithChartImage(hWnd);
            break;
        case IDM_CHARTSMITH_IMAGE_REDBLUE:
            mc_Forms.OnMenu_Chart_SmithChartImageRedBlue(hWnd);
            break;
        case IDM_OSCILLOSCOPE:
            mc_Forms.OnMenu_Chart_Oscilloscope(hWnd);
            break;
        case IDM_OSCILLOSCOPE_S11_S21:
            mc_Forms.OnMenu_Chart_Oscilloscope_S11_S21(hWnd);
            break;

        case IDM_S11_LOGMAG:
            mc_Forms.OnMenu_TraceS11_LogMag(hWnd);
            break;
        case IDM_S11_LINEARMAG:
            mc_Forms.OnMenu_TraceS11_LinearMag(hWnd);
            break;
        case IDM_S11_SWR:
            mc_Forms.OnMenu_TraceS11_SWR(hWnd);
            break;
        case IDM_S11_RESITANCE:
            mc_Forms.OnMenu_TraceS11_Resitance(hWnd);
            break;
        case IDM_S11_REACTANCE:
            mc_Forms.OnMenu_TraceS11_Reactance(hWnd);
            break;
        case IDM_S11_PHASE:
            mc_Forms.OnMenu_TraceS11_Phase(hWnd);
            break;

        case IDM_S21_LOGMAG:
            mc_Forms.OnMenu_TraceS21_LogMag(hWnd);
            break;
        case IDM_S21_LINEARMAG:
            mc_Forms.OnMenu_TraceS21_LinearMag(hWnd);
            break;
        case IDM_S21_PHASE:
            mc_Forms.OnMenu_TraceS21_Phase(hWnd);
            break;

        case IDM_MARKER_RED_ENABLE:
            mc_Forms.OnMenu_Marker_Red_Enable(hWnd);
            break;
        case IDM_MARKER_RED_SET:
            mc_Forms.OnMenu_Marker_Red_Set(hWnd);
            break;
        case IDM_MARKER_GREEN_ENABLE:
            mc_Forms.OnMenu_Marker_Green_Enable(hWnd);
            break;
        case IDM_MARKER_GREEN_SET:
            mc_Forms.OnMenu_Marker_Green_Set(hWnd);
            break;
        case IDM_MARKER_BLUE_ENABLE:
            mc_Forms.OnMenu_Marker_Blue_Enable(hWnd);
            break;
        case IDM_MARKER_BLUE_SET:
            mc_Forms.OnMenu_Marker_Blue_Set(hWnd);
            break;
        case IDM_MARKER_MAX_ENABLE:
            mc_Forms.OnMenu_Marker_Max_Enable(hWnd);
            break;
        case IDM_MARKER_MIN_ENABLE:
            mc_Forms.OnMenu_Marker_Min_Enable(hWnd);
            break;

        case IDM_SWEEP_SETUP:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_FREQUENCYSWEEP), hWnd, FromFrequencySweep_Proc);
            break;

        case IDM_AVERAGE_FILTER:
            mc_Forms.OnMenu_Filter_Average(hWnd);
            break;
        case IDM_NOISE_FILTER:
            mc_Forms.OnMenu_Filter_Noise(hWnd);
            break;

        case IDM_SETTINGS_SERIALPORT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_SERIALPORT), hWnd, FormSerialPort_Proc);
            break;
        case IDM_RECALL0:
        case IDM_RECALL1:
        case IDM_RECALL2:
        case IDM_RECALL3:
        case IDM_RECALL4:
            mc_Forms.OnMenu_Recall(hWnd, wmId);
            break;
        case IDM_CALIBRATION:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_CALIBRATION), hWnd, FormCalibration_Proc);
            break;
        case IDM_SETTINGS_COLOUR:
            mc_Forms.OnMenu_ColourStettings(hWnd);
            break;

        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, FormAbout_Proc);
            break;

        default:
            return true;;
    }
    return false;
}
//-Public----------------------------------------------------------------------
// Name: FormAbout_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//-----------------------------------------------------------------------------
INT_PTR CALLBACK FormAbout_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    
    switch (message) {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}
// -Public---------------------------------------------------------------------
// Name: FormSerialPort_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//-----------------------------------------------------------------------------
INT_PTR CALLBACK FormSerialPort_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
        case WM_INITDIALOG:
            mc_Forms.FormSerialPort_SetData(hDlg);
            return (INT_PTR)TRUE;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            } else if (LOWORD(wParam) == IDOK) {
                HWND hwndTest = GetFocus();
                HWND hDlgItem_IDOK = GetDlgItem(hDlg, IDOK);
                if (hwndTest == hDlgItem_IDOK) {
                    mc_Forms.FormSerialPort_GetData(hDlg);
                    EndDialog(hDlg, LOWORD(wParam));
                    return (INT_PTR)TRUE;
                } else {
                    PostMessage(hDlg, WM_NEXTDLGCTL, 0, 0L);
                    return TRUE;
                }
            }
            if (LOWORD(wParam) == IDC_IDREFRESH) {
                mc_Forms.FormSerialPort_RefreshData(hDlg);
            }
            break;
    }
    return (INT_PTR)FALSE;
}
// -Public---------------------------------------------------------------------
// Name: FormCalibration_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//-----------------------------------------------------------------------------
INT_PTR CALLBACK FormCalibration_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
        case WM_INITDIALOG:
            mc_Forms.FormCalibration_SetData(hDlg);
            return (INT_PTR)TRUE;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }  else if (LOWORD(wParam) == IDOK) {
                HWND hwndTest = GetFocus();
                HWND hDlgItem_IDOK = GetDlgItem(hDlg, IDOK);
                if (hwndTest == hDlgItem_IDOK) {
                    EndDialog(hDlg, LOWORD(wParam));
                    return (INT_PTR)TRUE;
                } else {
                    PostMessage(hDlg, WM_NEXTDLGCTL, 0, 0L);
                    return TRUE;
                }
            }
            switch (LOWORD(wParam)) {
                case IDC_CAL_RESET:
                case IDC_CAL_CALIBRATION:
                case IDC_CAL_OPEN:
                case IDC_CAL_SHORT:
                case IDC_CAL_LOAD:
                case IDC_CAL_ISOLN:
                case IDC_CAL_THRU:
                case IDC_CAL_DONE:
                case IDC_CAL_SAVE:
                case IDC_COMBO_CAL_SAVE:
                    mc_Forms.FormCalibration_Buttons(hDlg, LOWORD(wParam));
                    break;
            }
            break;
    }
    return (INT_PTR)FALSE;
}
// -Public---------------------------------------------------------------------
// Name: FromFrequencySweep_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//-----------------------------------------------------------------------------
INT_PTR CALLBACK FromFrequencySweep_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
        case WM_INITDIALOG:
            SendMessageW(GetDlgItem(hDlg, IDC_SPIN_SAMPLRATE), UDM_SETRANGE, 0, MAKELPARAM(FORM_SPIN_MAX_POS, FORM_SPIN_MIN_POS));
            mc_Forms.FormFrequencySweep_SetData(hDlg);
            return (INT_PTR)TRUE;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            } else if (LOWORD(wParam) == IDOK) {
                HWND hwndTest = GetFocus();
                HWND hDlgItem_IDOK = GetDlgItem(hDlg, IDOK);
                if (hwndTest == hDlgItem_IDOK) {
                    mc_Forms.FormFrequencySweep_GetData(hDlg);
                    mc_NanoVNA.m_VNAMultiData.Stop = true;
                    EndDialog(hDlg, LOWORD(wParam));
                    return (INT_PTR)TRUE;
                } else {
                    PostMessage(hDlg, WM_NEXTDLGCTL, 0, 0L);
                    return TRUE;
                }
            }
            if (LOWORD(wParam) == IDC_BUTTON_FREQCAL1) {
                mc_Forms.FormFrequencySweep_CalculateSweep(hDlg);
            }
            if (LOWORD(wParam) == IDC_BUTTON_FREQCAL2) {
                mc_Forms.FormFrequencySweep_CalculateSpan(hDlg);
            }
            break;
        case WM_NOTIFY:
            if (LOWORD(wParam) == IDC_SPIN_SAMPLRATE) {
                LPNMUPDOWN lpnmud = (LPNMUPDOWN)lParam;
                int Value = lpnmud->iPos + lpnmud->iDelta;
                if (Value < FORM_SPIN_MIN_POS) {
                    Value = FORM_SPIN_MIN_POS;
                } else if (Value > FORM_SPIN_MAX_POS) {
                    Value = FORM_SPIN_MAX_POS;
                }
                mc_Forms.FormFrequencySweep_CalculateSteps(hDlg, Value);
            }
    }
    return (INT_PTR)FALSE;
}
