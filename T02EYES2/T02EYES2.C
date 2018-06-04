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
  HDC hDC;
  PAINTSTRUCT ps;

  static INT w, h, rad = 50;
  static INT mode = 0;
  static HDC hMemDC;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SetTimer(hWnd, 15,  30, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    if (hBm != NULL)
      DeleteObject(hBm);

    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);

    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_SPACE)
      mode = (mode + 1) % 3;
    return 0;
  case WM_TIMER:

    InvalidateRect(hWnd, NULL, TRUE);

    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    
    SetDCBrushColor(hMemDC, RGB(100, 0, 0));
    Rectangle(hMemDC, 0, 0, w, h);

    if (mode == 0)
    {
      rad = w > h ? h : w;
      DrawEye(hMemDC, w / 2 - rad / 4 - rad / 20, h / 2, rad / 4, rad / 20, pt.x, pt.y);
      DrawEye(hMemDC, w / 2 + rad / 4 + rad / 20, h / 2, rad / 4, rad / 20, pt.x, pt.y);
    }
    else if (mode == 1)
    {
      INT i;

      srand(1567);        

      for (i = 0; i < 30; i++)
      {
         DrawEye(hMemDC, rand() % 2000, rand() % 1200, 45 + rand() % 20, 15 + rand() % 10, pt.x, pt.y); 
      }
    }
    else
    {
      INT i, j;

      for (i = 50 + 1; i < 2000; i += (2 * 50 + 10))
        for (j = 50 + 1; j < 1200; j += (2 * 50 + 10))
          DrawEye(hMemDC, i, j, 50, 20, pt.x, pt.y);
    }

    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);

    PostQuitMessage(0);
    KillTimer(hWnd, 15);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

void DrawEye(HDC hDC, INT x, INT y, INT r, INT r1, INT mx, INT my)
{
  DOUBLE t, dr, x1, y1;

  dr = r - r1;
  t = sqrt((mx - x) * (mx - x) + (my - y) * (my - y));

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));

  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));

  Ellipse(hDC, x - r, y + r, x + r, y - r);

  SetDCPenColor(hDC, RGB(255, 255, 255));
  SetDCBrushColor(hDC, RGB(0, 0, 230));

  if (t < dr)
  {  
    x1 = mx;
    y1 = my;
  }
  else
  {
    x1 = x + (mx - x) * dr / t;
    y1 = y + (my - y) * dr / t;
  }

  Ellipse(hDC, (INT)(x1 - r1), (INT)(y1 + r1), (INT)(x1 + r1), (INT)(y1 - r1));
}