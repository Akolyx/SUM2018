#include <windows.h>
#include <stdlib.h>
#include <math.h>

#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
void DrawEye(HDC hDC, int x, int y, int r, int r1, int mx, int my);

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  HWND hWnd;
  WNDCLASS wc;
  MSG msg;

  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_INFORMATION);
  wc.hInstance = hInstance;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.lpfnWndProc = MyWindowFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME,
          "Eyes",
          WS_OVERLAPPEDWINDOW,
          CW_USEDEFAULT, CW_USEDEFAULT,
          CW_USEDEFAULT, CW_USEDEFAULT,
          NULL,
          NULL,
          hInstance,
          NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    DispatchMessage(&msg);
  }
   
  return 0;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  POINT pt;
  RECT rc;
  HDC hDC;

  INT r;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 15, 2, NULL);
    return 0;
  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    
    hDC = GetDC(hWnd);
    GetClientRect(hWnd, &rc);

    r = rc.right > rc.bottom ? rc.bottom : rc.right;

    DrawEye(hDC, rc.right / 2 - r / 4 - r / 20, rc.bottom / 2, r / 4, r / 20, pt.x, pt.y);
    DrawEye(hDC, rc.right / 2 + r / 4 + r / 20, rc.bottom / 2, r / 4, r / 20, pt.x, pt.y);
  
    ReleaseDC(hWnd, hDC);

    return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    KillTimer(hWnd, 15);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

void DrawEye(HDC hDC, int x, int y, int r, int r1, int mx, int my)
{
  DOUBLE t, x1, y1;

  t = (r - r1) / sqrt((mx - x) * (mx - x) + (my - y) * (my - y));
  x1 = x + (mx - x) * t;
  y1 = y + (my - y) * t;

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));

  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));

  Ellipse(hDC, x - r, y + r, x + r, y - r);

  SetDCPenColor(hDC, RGB(255, 255, 255));
  SetDCBrushColor(hDC, RGB(0, 0, 230));

  Ellipse(hDC, x1 - r1, y1 + r1, x1 + r1, y1 - r1);
}