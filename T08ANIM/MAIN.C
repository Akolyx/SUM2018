/* FILE NAME: MAIN.C
 * PROGRAMMER: DI6
 * DATE: 08.06.2018
 * PURPOSE: Animation.
 */

#include "VEC.H"
#include <time.h>

#define p 15

VEC m[p + 1][2 * p];

/* Main window class name */
#define WND_CLASS_NAME "My window class"

/* Forward references */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
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

    ModelSphere();

    SetTimer(hWnd, 15,  30, NULL);
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
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, w, h);

    DrawSphere(hMemDC, w, h, w / 2, h, (w < h ? w / 4 : h / 4));

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

/* Modeling a unit sphere
 * ARGUMENTS:
 *   None;
 * RETURNS:
 *   None;
 */
VOID ModelSphere()
{
  INT i, j;
  DBL f = 0, t = 0, d = PI / p;
  DBL sint, cost;

  for (i = 0; i <= p; i++)
  { 
    sint = sin(t);
    cost = cos(t);

    for (j = 0; j < 2 * p; j++)
    {
      f += d;

      m[i][j] = VecSet(sint * sin(f), cost, sint * cos(f));
    }

    t += d;
  }
}

/* Drawing of sphere
 * ARGUMENTS:
 *   - handle of paint contest:
 *       HDC hDC;
 *   - coordinates of center of the sphere:
 *       INT x, INT y;
 *   - radius of the sphere:
 *       INT r;
 * RETURNS:
 *   None.
 */
VOID DrawSphere( HDC hDC, INT w, INT h, INT x, INT y, INT r)
{
  INT i, j, k;
  POINT pts[p + 1][2 * p];
  DBL sq = 0.0, ProjDist = 1, ProjW = 1, ProjH = 1;
  DBL t = (DBL)clock() / CLOCKS_PER_SEC;
  VEC res, pr;
  MATR m1;

  for (i = 0; i <= p; i++)
    for (j = 0; j < 2 * p; j++)
    {
      m1 = MatrMulMatr(MatrView(VecSet(0, 5, 5), VecSet(0, 5, 0), VecSet(0, 1, 0)), MatrFrustum(-5, 5, -5, 5, 1, 50));
      //m1 = MatrMulMatr(m1, MatrMulMatr(MatrTranslate(VecSet(0, 0 * -fabs(sin(t)) / 2, 0)), MatrRotateX(90 * t)));

      m1 = MatrMulMatr(m1, MatrRotateX(5 * t));

      res = VecMulMatr4x4(m[i][j], m1);

      pr.x = res.x * ProjDist / (-res.z);
      pr.y = res.y * ProjDist / (-res.z);

      pts[i][j].x = (LONG)(x + (w / ProjW) * pr.x + ProjW / 2);
      pts[i][j].y = (LONG)(y + ProjH / 2 - (h / ProjH) * pr.y);

    }

  for (i = 0; i < p; i++)
  {
    POINT pnts[4];

    for (j = 0; j < 2 * p - 1; j++)
    {
      //SetPixel(hDC, pts[i][j].x, pts[i][j].y, RGB(0, 0, 0));

      pnts[0] = pts[i][j];
      pnts[1] = pts[i][j + 1];
      pnts[2] = pts[i + 1][j + 1];
      pnts[3] = pts[i + 1][j];

      for (k = 0; k < 4; k++)
        sq += (pnts[k].x - pnts[(k + 1) % 4].x) * (pnts[k].y + pnts[(k + 1) % 4].y);

      //if (sq > 0)
      //{
        SetDCPenColor(hDC, RGB(0, 0, 0));
        Polygon(hDC, pnts, 4);
      //}
      /*else
      {
        SetDCPenColor(hDC, RGB(200, 200, 200));
        Polygon(hDC, pnts, 4);
      }*/
    }

    pnts[0] = pts[i][2 * p - 1];
    pnts[1] = pts[i][0];
    pnts[2] = pts[0][0];
    pnts[3] = pts[0][2 * p - 1];

    for (k = 0; k < 4; k++)
      sq += (pnts[k].x - pnts[(k + 1) % 4].x) * (pnts[k].y + pnts[(k + 1) % 4].y);

    //if (sq > 0)
    //{
      SetDCPenColor(hDC, RGB(0, 0, 0));
      Polygon(hDC, pnts, 4);
    //}
  }
} /* End of 'DrawSphere' function */

/* End of 'MAIN.C' file */