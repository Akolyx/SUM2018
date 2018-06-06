/* FILE NAME: T07CLOCK.C
 * PROGRAMMER: DI6
 * DATE: 06.06.2018
 * PURPOSE: Moving sphere using orthogonal projection.
 */

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

/* Main window class name */
#define WND_CLASS_NAME "My window class"

/* Forward references */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
void DrawSphere( HDC hDC, INT x0, INT y0, INT r, INT p, DOUBLE angle );
void FlipFullScreen( HWND hWnd );

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
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
   
  return 0;
} /* End of 'WinMain' function */

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  SYSTEMTIME tm;
  MINMAXINFO *minmax;

  static INT w, h, len;
  static HDC hMemDC;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SelectObject(hMemDC, GetStockObject(DC_PEN));

    SetTimer(hWnd, 15,  1, NULL);
    return 0;
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2 + 500;
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

    SendMessage(hWnd, WM_TIMER, 0, 0);

    return 0;
  case WM_TIMER:
    SetDCBrushColor(hMemDC, RGB(0, 0, 0));
    Rectangle(hMemDC, 0, 0, w, h);

    GetLocalTime(&tm);

    DrawSphere(hMemDC, w / 2, h / 2, (w < h ? w / 4 : h / 4), 15, tm.wSecond * PI / 30 + tm.wMilliseconds * PI / 30000);

    InvalidateRect(hWnd, NULL, TRUE);

    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_KEYDOWN:
    if (wParam == 'F')
      FlipFullScreen(hWnd);
    else if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);

    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure to close the window?", "Quit", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
      SendMessage(hWnd, WM_DESTROY, 0, 0);

    return 0;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);

    PostQuitMessage(0);
    KillTimer(hWnd, 15);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* Drawing of sphere with meridians and parallels
 * ARGUMENTS:
 *   - handle of paint contest:
 *       HDC hDC;
 *   - coordinates of center of the sphere:
 *       INT x, INT y;
 *   - radius of the sphere:
 *       INT r;
 *   - amount of slices:
 *       INT p;
 *   - turning angle:
 *       DOUBLE angle;
 * RETURNS:
 *   None.
 */
void DrawSphere( HDC hDC, INT x, INT y, INT r, INT p, DOUBLE angle )
{
  INT i, j, x1, y1;
  INT x2, y2, x3, y3;
  DOUBLE f = 0, t = angle, d = PI / p;
  static INT x0, y0;
  POINT pts[4];

  /*for (i = 0; i < p; i++)
  {  
    for (j = 0; j <= 2 * p; j++)
    {
      f += d;

      x1 = (INT)(r * sin(t) * cos(f));
      y1 = (INT)(r * sin(t) * sin(f));

      if (j != 0)
      {
        SetDCPenColor(hDC, RGB(0, 100, 255));

        MoveToEx(hDC, x + x0, y + y0, NULL);
        LineTo(hDC, x + x1, y + y1);
      }

      x0 = x1;
      y0 = y1;
    }

    t += d;
  }*/

  for (i = 0; i < p; i++)
  {  
    for (j = 0; j <= 2 * p; j++)
    {
      f += d;

      x1 = (INT)(r * sin(t) * cos(f));
      y1 = (INT)(r * sin(t) * sin(f));

      x2 = (INT)(r * sin(t + d) * cos(f));
      y2 = (INT)(r * sin(t + d) * sin(f));

      x3 = (INT)(r * sin(t + d) * cos(f - d));
      y3 = (INT)(r * sin(t + d) * sin(f - d));

      pts[0].x = x + x0, pts[0].y = y +  y0;
      pts[1].x = x + x1, pts[1].y = y +  y1;
      pts[2].x = x + x2, pts[2].y = y +  y2;
      pts[3].x = x + x3, pts[3].y = y +  y3;

      if (j != 0)
      {
        SetDCBrushColor(hDC, RGB((100 + j) % 256, (100 + j) % 256, (200 + j) % 256));

        Polygon(hDC, pts, sizeof(pts) / sizeof(pts[0]));
      }

      x0 = x1;
      y0 = y1;
    }

    t += d;
  }

  /*for (i = 0; i < p; i++)
  {  
    for (j = 0; j <= 2 * p; j++)
    {
      t += d;

      x1 = (INT)(r * sin(t) * cos(f));
      y1 = (INT)(r * sin(t) * sin(f));

      if (j != 0)
      {
        SetDCPenColor(hDC, RGB(200, 200, 0));

        MoveToEx(hDC, x + x0, y + y0, NULL);
        LineTo(hDC, x + x1, y + y1);
      }

      x0 = x1;
      y0 = y1;
    }

    f += d;
  }*/
} /* End of 'DrawSphere' function */

/* Enabling switching between window and fullscreen modes */
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;

  if (IsFullScreen)
  {
    /* Restore window size and position */
    IsFullScreen = FALSE;

    SetWindowPos(hWnd, HWND_TOP, SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top, SWP_NOOWNERZORDER);
  }
  else
  {
    RECT rc;
    HMONITOR hMon;
    MONITORINFOEX moninfo;

    /* Go to full screen mode */
    IsFullScreen = TRUE;

    /* Save window position and size */
    GetWindowRect(hWnd, &SaveRC);

    /* Obtain closest monitor info */
    hMon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP, rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top, SWP_NOOWNERZORDER);
  }
} /* End of 'FlipFullScreen' function */

/* End of 'T03CLOCK.C' file */