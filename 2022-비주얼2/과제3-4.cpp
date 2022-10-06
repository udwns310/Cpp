#include <windows.h>
#include <tchar.h>
#define MAX_X 400
#define MIN_X 0
#define MAX_Y 400
#define MIN_Y 0

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC   hdc;
    PAINTSTRUCT ps;
    static int  x, y;     // x 와 y 의 기본 좌표를 변수로 둠.
    static RECT rectView;    // 측정하려는 윈도우 핸들과 크기를 저장할 RECT 구조체 변수.
    static int r, b;
    static int headX = 60, headY = 20;
    static int tailX = 20, tailY = 20;
    static int Direction = 1;
    static bool isGoing = false;
    HPEN hPen, oldPen;

    switch (iMsg)
    {
    case WM_CREATE:
        x = 20; y = 20;    // x 축으로 20 만큼 y 축으로 20 만큼의 기본 좌표를 지정, 즉 좌표값 (20,20)
        break;
    case WM_PAINT:
        GetClientRect(hwnd, &rectView);
        hdc = BeginPaint(hwnd, &ps);
        r = rectView.right - rectView.right % 40;
        b = rectView.bottom - rectView.bottom % 40;
        Rectangle(hdc, 0, 0, r, b);
        hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //머리
        oldPen = (HPEN)SelectObject(hdc, hPen);
        Ellipse(hdc, headX - 20, headY - 20, headX + 20, headY + 20);
        SelectObject(hdc, oldPen);

        hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); //꼬리
        oldPen = (HPEN)SelectObject(hdc, hPen);
        Ellipse(hdc, tailX - 20, tailY - 20, tailX + 20, tailY + 20);
        SelectObject(hdc, oldPen);

        DeleteObject(hPen);
        EndPaint(hwnd, &ps);
        break;
    case WM_KEYDOWN:
		InvalidateRgn(hwnd, NULL, TRUE);
        if (wParam == VK_RIGHT && (tailX + 60) <= r)
        {
            headX = tailX + 40;
            headY = tailY;
            Direction = 1;
        }
        else if (wParam == VK_LEFT && (tailX - 60) >= 0)
        {
            headX = tailX - 40;
            headY = tailY;
            Direction = 3;
        }
        else if (wParam == VK_UP && (tailY - 60) >= 0)
        {
            headX = tailX;
            headY = tailY - 40;
            Direction = 4;
        }
        else if (wParam == VK_DOWN && (tailY + 60) <= b)
        {
            headX = tailX;
            headY = tailY + 40;
            Direction = 2;
        }
        if (wParam == VK_RETURN)
        {
            if (isGoing) isGoing = false;
            else isGoing = true;

            if (isGoing)
                SetTimer(hwnd, 1, 10, NULL);
            else
                KillTimer(hwnd, 1);
            InvalidateRgn(hwnd, NULL, TRUE);
            break;
        }
        InvalidateRgn(hwnd, NULL, TRUE);
        break;
	case WM_TIMER:
        if (Direction == 1 && (headX + 20) < r) //오른
        {
            headX += 2;
            tailX = headX - 40;
        }
        else if (Direction == 3 && (headX - 20) > 0) //왼
        {
            headX -= 2;
            tailX = headX + 40;
        }
        else if (Direction == 4 && (headY - 20) > 0) //위
        {
            headY -= 2;
            tailY = headY + 40;
        }
        else if (Direction == 2 && (headY + 20) < b) //아래
        {
            headY += 2;
            tailY = headY - 40;
        }
        InvalidateRgn(hwnd, NULL, TRUE);
        break;
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        break;
    }
    return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

