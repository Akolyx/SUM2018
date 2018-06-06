/* FILE NAME: T03CLOCK.C
 * PROGRAMMER: DI6
 * DATE: 05.06.2018
 * PURPOSE: WinAPI Clock.
 */

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "resource.h"

#define PI 3.14159265358979323846

/* Main window class name */
#define WND_CLASS_NAME "My window class"

/* Forward references */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
void DrawHand(HDC hDc, INT x0, INT y0, INT l, INT w, DOUBLE angle );
void DrawSecondHand(HDC hDc, INT x0, INT y0, INT l, INT w, DOUBLE angle );
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
  BITMAP bm;
  SIZE s;
  SYSTEMTIME tm;
  MINMAXINFO *minmax;
  CREATESTRUCT *cs;

  static INT w, h, len;
  static HDC hMemDC, hDCXor, hDCAnd;
  static HBITMAP hBm, hBmXor, hBmAnd;
  static HFONT hFnt;
  static char Buf[1000];
  static char* days[] = {"Воскресенье", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hDCXor = CreateCompatibleDC(hDC);
    hDCAnd = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    FlipFullScreen(hWnd);

    hFnt = CreateFont(50, 0, 0, 0, FW_THIN, TRUE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH | FF_SWISS, "");
    SelectObject(hMemDC, hFnt);

    /*hBmXor = LoadImage(NULL, "Clock_XOR.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAnd = LoadImage(NULL, "Clock_AND.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);*/

    cs = (CREATESTRUCT *)lParam;

    hBmXor = LoadImage(cs->hInstance, (CHAR *)IDB_BITMAP2, IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
    hBmAnd = LoadImage(cs->hInstance, (CHAR *)IDB_BITMAP1, IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);

    SelectObject(hDCXor, hBmXor);
    SelectObject(hDCAnd, hBmAnd);

    SetTimer(hWnd, 15,  10, NULL);
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
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));

    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, w, h);

    GetLocalTime(&tm);

    GetObject(hBmAnd, sizeof(BITMAP), &bm);

    BitBlt(hMemDC, w / 2 - bm.bmWidth / 2, h / 2 - bm.bmWidth / 2, bm.bmWidth, bm.bmHeight, hDCAnd, 0, 0, SRCAND);
    BitBlt(hMemDC, w / 2 - bm.bmWidth / 2, h / 2 - bm.bmWidth / 2, bm.bmWidth, bm.bmHeight, hDCXor, 0, 0, SRCINVERT);

    SetDCBrushColor(hMemDC, RGB(0, 0, 0));
    Ellipse(hMemDC, w / 2 - 12, h / 2 + 12, w / 2 + 12, h / 2 - 12);

    DrawSecondHand(hMemDC, w / 2 + 99, h / 2 + 60, (INT)(bm.bmWidth * 0.08), (INT)(bm.bmWidth * 0.003), tm.wSecond * PI / 30 + tm.wMilliseconds * PI / 30000);
    DrawHand(hMemDC, w / 2, h / 2, (INT)(bm.bmWidth * 0.37), (INT)(bm.bmWidth * 0.01), tm.wMinute * PI / 30 + tm.wSecond * PI / 1800);
    DrawHand(hMemDC, w / 2, h / 2, (INT)(bm.bmWidth * 0.30), (INT)(bm.bmWidth * 0.01), (tm.wHour % 12) * PI / 6 + tm.wMinute * PI / 360);

    len = sprintf(Buf, "%s, %02i.%02i.%i", days[tm.wDayOfWeek], tm.wDay, tm.wMonth, tm.wYear);

    SetTextColor(hMemDC, RGB(0, 0, 0));
    SetBkMode(hMemDC, TRANSPARENT);
    GetTextExtentPoint(hMemDC, Buf, len, &s);
    TextOut(hMemDC, w / 2 - s.cx / 2, h / 2 + bm.bmHeight / 2, Buf, len);

    len = sprintf(Buf, "%02i:%02i:%02i", tm.wHour, tm.wMinute, tm.wSecond);

    GetTextExtentPoint(hMemDC, Buf, len, &s);
    TextOut(hMemDC, w / 2 - s.cx / 2, h / 2 + bm.bmHeight / 2 + s.cy, Buf, len);

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
    DeleteObject(hBmXor);
    DeleteDC(hDCXor);
    DeleteObject(hBmAnd);
    DeleteDC(hDCAnd);

    DeleteObject(hFnt);

    PostQuitMessage(0);
    KillTimer(hWnd, 15);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* Drawing of minute and hour arrow-shaped hands
 * ARGUMENTS:
 *   - handle of paint contest:
 *       HDC hDC;
 *   - coordinates of center of the clock:
 *       INT x0, INT y0;
 *   - characteristics of the hand - length and width:
 *       INT l, INT w;
 *   - angle on which we must turn our hand (in radians):
 *       DOUBLE angle;
 * RETURNS:
 *   None.
 */
void DrawHand( HDC hDC, INT x0, INT y0, INT l, INT w, DOUBLE angle )
{
  DOUBLE mysin, mycos;
  INT i;

  POINT pts[] = {{0, l}, {-w * 3, (INT)(l * 0.87)}, {-w, (INT)(l * 0.9)}, {-w, 0}, {0, -w}, {w, 0}, {w, (INT)(l * 0.9)}, {w * 3, (INT)(l * 0.87)}};

  POINT pts1[sizeof(pts) / sizeof(pts[0])];

  mysin = sin(angle);
  mycos = cos(angle);

  for (i = 0; i < sizeof(pts) / sizeof(pts[0]); i++)
  {
    pts1[i].x = (LONG)(x0 - pts[i].x * mycos + pts[i].y * mysin);
    pts1[i].y = (LONG)(y0 - pts[i].x * mysin - pts[i].y * mycos);
  }

  SetBkMode(hDC, TRANSPARENT);

  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 0));

  Polygon(hDC, pts1, sizeof(pts) / sizeof(pts[0]));
} /* End of 'DrawHand' function */

/* Drawing of thin, red second hand
 * ARGUMENTS:
 *   - handle of paint contest:
 *       HDC hDC;
 *   - coordinates of center of the clock:
 *       INT x0, INT y0;
 *   - characteristics of the hand - length and width:
 *       INT l, INT w;
 *   - angle on which we must turn our hand (in radians):
 *       DOUBLE angle;
 * RETURNS:
 *   None.
 */
void DrawSecondHand( HDC hDC, INT x0, INT y0, INT l, INT w, DOUBLE angle )
{
  DOUBLE mysin, mycos;
  INT i;

  POINT pts[] = {{-w, 0}, {-w, l}, {w, l}, {w, 0}};

  POINT pts1[sizeof(pts) / sizeof(pts[0])];

  mysin = sin(angle);
  mycos = cos(angle);

  for (i = 0; i < sizeof(pts) / sizeof(pts[0]); i++)
  {
    pts1[i].x = (LONG)(x0 - pts[i].x * mycos + pts[i].y * mysin);
    pts1[i].y = (LONG)(y0 - pts[i].x * mysin - pts[i].y * mycos);
  }

  SetBkMode(hDC, OPAQUE);

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(255, 0, 0));

  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(255, 0, 0));

  Polygon(hDC, pts1, sizeof(pts) / sizeof(pts[0]));
} /* End of 'DrawSecondHand' function */

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