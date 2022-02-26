#include <Windows.h>
#include <tchar.h>
#include <ctime>
#include <sstream>
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

int WINAPI _tWinMain(HINSTANCE This,		 // ���������� �������� ���������� 
HINSTANCE Prev, 	// � ����������� �������� ������ 0 
LPTSTR cmd, 		// ��������� ������ 
int mode) 		// ����� ����������� ����
{ 
HWND hWnd;	// ���������� �������� ���� ��������� 
MSG msg; 		// ��������� ��� �������� ��������� 
WNDCLASS wc; 	// ����� ����
// ����������� ������ ���� 
wc.hInstance = This; 
wc.lpszClassName = WinName; 				// ��� ������ ���� 
wc.lpfnWndProc = WndProc; 					// ������� ���� 
wc.style = CS_HREDRAW | CS_VREDRAW; 			// ����� ���� 
wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); 		// ����������� ������ 
wc.hCursor = LoadCursor(NULL, IDC_ARROW); 		// ����������� ������ 
wc.lpszMenuName = NULL; 					// ��� ���� 
wc.cbClsExtra = 0; 						// ��� �������������� ������ ������ 
wc.cbWndExtra = 0; 						// ��� �������������� ������ ���� 
wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1); 	// ���������� ���� ����� ������ 


// ����������� ������ ����
if (!RegisterClass(&wc)) return 0; 	

// �������� ���� 
hWnd = CreateWindow(WinName,			// ��� ������ ���� 
_T("������������ ���4"), 		// ��������� ���� 
WS_OVERLAPPEDWINDOW, 		// ����� ���� 
CW_USEDEFAULT,				// x 
CW_USEDEFAULT, 				// y	 ������� ���� 
CW_USEDEFAULT, 				// width 
CW_USEDEFAULT, 				// Height 
HWND_DESKTOP, 				// ���������� ������������� ���� 
NULL, 						// ��� ���� 
This, 						// ���������� ���������� 
NULL); 					// �������������� ���������� ��� 

ShowWindow(hWnd, mode); 				// �������� ����


// ���� ��������� ��������� 
while(GetMessage(&msg, NULL, 0, 0)) 
{ 
TranslateMessage(&msg); 		// ������� ���������� ����� ������� ������� 
DispatchMessage(&msg); 		// �������� ��������� ������� WndProc() 
} 
return 0;
}

// ������� ������� ���������� ������������ ��������
// � �������� ��������� �� ������� ��� ������� ����������
	
static HBRUSH hBrush1, hBrush2, hBrush3, hBrushU, hBrushD, hBrushC; 
static int sx, sy;
PAINTSTRUCT ps;
HDC hdc;
static HWND hStat, hStatH;
int count=0;
	std::wstring df;

DWORD WINAPI Cycle (LPVOID param) 
{
	
	
	while(true)
	{
	int i;
	 srand( time( 0 ) );
	 i = rand()% 3;
	 switch(i)
	 {
	 case 0:
		 hBrushC= hBrush1;
		 break;
	 case 1:
		 hBrushC= hBrush2;
		 break;
	 case 2:
		 hBrushC= hBrush3;
		 break;
	 }
	 
	 InvalidateRect(
		(HWND) param,           // ���������� ����
		NULL,  // ���������� ��������������
		TRUE          // ��������� �������
	);
	 count++;
		df = std::to_wstring(count);
		SetWindowText(hStat,df.c_str());
	 Sleep (1000);
	}
	return 0;
}
TCHAR str[512];
DWORD WINAPI  Time (LPVOID param) 
{
	while(true)
	{
	SYSTEMTIME sm;
	GetLocalTime(&sm);

	GetTimeFormat(GetThreadLocale(),LOCALE_NOUSEROVERRIDE,NULL, NULL, str,512);
	SetWindowText(hStatH,str);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{ 
	hdc = GetDC(hWnd);
	switch(message)		 // ���������� ���������
{ 
	case WM_SIZE:      
		 sx = LOWORD(lParam); // ������ ������� �������     
		 sy = HIWORD(lParam); // ������ ������� ������� 
		 break;
	case WM_CREATE: 
		  hStat = CreateWindow(TEXT("STATIC"),NULL,
    WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
    0,0,
	70,40,hWnd,(HMENU)1,NULL,NULL);
    ShowWindow(hStat, SW_SHOWNORMAL);
		 hStatH = CreateWindow(TEXT("STATIC"),NULL,
    WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
    200,300,
	700,40,hWnd,(HMENU)1,NULL,NULL);
    ShowWindow(hStatH, SW_SHOWNORMAL);
		 
		 hBrush1 = CreateSolidBrush( RGB(139,69,19));
		 hBrush2 = CreateSolidBrush( RGB(165,42,42));
		 hBrush3 = CreateSolidBrush( RGB(199,21,133));
		 hBrushD = CreateSolidBrush( RGB(255,228,225));
		 hBrushU = CreateSolidBrush( RGB(255,250,250));

		 CreateThread(NULL, 0, Cycle, hWnd, 0, NULL);   
		 CreateThread(NULL, 0, Time, hdc, 0, NULL);  
		 break; 
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		/*SelectObject(hdc, hBrushU); 
		Rectangle(hdc,0,0, sx ,sy/2);*/
		SelectObject(hdc, hBrushD); 
		Rectangle(hdc,0,sy/2, sx ,sy);
		SelectObject(hdc, hBrushC);
		Ellipse (hdc,(sx/2)-80, (sy/4)-80, (sx/2)+80, (sy/4)+80); 
		
		
		EndPaint(hWnd, &ps); 
		  /*ReleaseDC(hWnd, hdc); */
		 break; 
	case WM_DESTROY : 
		PostQuitMessage(0); 
		DeleteObject(hBrush1) ;  
		DeleteObject(hBrush2) ; 
		DeleteObject(hBrush3) ;      
break; 			// ���������� ��������� 
default : 			// ��������� ��������� �� ��������� 
return DefWindowProc(hWnd, message, wParam, lParam); 
} 
return 0;
}