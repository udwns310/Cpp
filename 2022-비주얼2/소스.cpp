#include <windows.h>
#include <tchar.h>
#include <math.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM IParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX WndClass;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("Window Title Name"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
#define BSIZE 20
double LengthPts(int x1, int y1, int x2, int y2) {
	return (sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my) {
	if (LengthPts(x, y, mx, my) < BSIZE) return TRUE;
	else return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;

	static int x, y, startX, startY, oldX, oldY;
	static BOOL Selection, Drag;
	int mx, my, endX, endY;


	switch (iMsg) {
	case WM_CREATE:
		x = y = startX = startY = oldX = oldY = 20;
		Selection = FALSE;
		Drag = FALSE;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (Selection) {
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);

			Ellipse(hdc, x - BSIZE, y - BSIZE, x + BSIZE, y + BSIZE);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);

			hPen = (HPEN)SelectObject(hdc, oldPen);
			DeleteObject(hPen);

			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY);
		}
		else
			Ellipse(hdc, x - BSIZE, y - BSIZE, x + BSIZE, y + BSIZE);
		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(x, y, mx, my)) {
			Selection = TRUE; Drag = TRUE;
		}				
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		Selection = FALSE;
		Drag = FALSE;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (Drag) {
			SetROP2(hdc, R2_XORPEN);
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY);

			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);

			oldX = endX; oldY = endY;
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_TIMER:

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}