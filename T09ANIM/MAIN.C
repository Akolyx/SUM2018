/* FILE NAME: MAIN.C
 * PROGRAMMER: DI6
 * DATE: 11.06.2018
 * PURPOSE: Animation.
 */

#include "unit.h"

/* Main window class name */
#define WND_CLASS_NAME "My window class"

/* Forward references */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT DI6_MouseWheel;

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  INT i;
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
          "Animation",
          WS_OVERLAPPEDWINDOW,
          CW_USEDEFAULT, CW_USEDEFAULT,
          CW_USEDEFAULT, CW_USEDEFAULT,
          NULL,
          NULL,
          hInstance,
          NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  SetDbgMemHooks();

  /* Adding units */
  DI6_AnimUnitAdd(DI6_UnitCreateGround());
  DI6_AnimUnitAdd(DI6_UnitCreateSky());
  DI6_AnimUnitAdd(DI6_UnitCreateWater());

  DI6_AnimUnitAdd(DI6_UnitCreateControl());

  for (i = 0; i < 1; i++)
    DI6_AnimUnitAdd(DI6_UnitCreateWalrus());

  DI6_AnimUnitAdd(DI6_UnitCreateHelicopter());

  /* Message cycle */
  while (TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      else
        DispatchMessage(&msg);
    }
    else
    {
      DI6_AnimRender();
      DI6_AnimCopyFrame();
    }
  } 
  return 0;
} /* End of 'WinMain' function */

/* Window response function
 * ARGUMENTS:
 *   - window handle
 *       HWND hWnd;
 *   - constant representing a message:
 *       UINT Msg;
 *   - w and l parameters:
 *       WPARAM wParam;
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT)
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *minmax;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2 + 500;
    return 0;
  case WM_CREATE:
    DI6_AnimInit(hWnd);            

    SetTimer(hWnd, 15,  1, NULL);
    return 0;
  case WM_SIZE:              
    DI6_AnimResize(LOWORD(lParam), HIWORD(lParam));
    DI6_AnimRender();

    return 0;
  case WM_TIMER:
    DI6_AnimRender();   

    DI6_AnimCopyFrame();

    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_MOUSEWHEEL:
    DI6_MouseWheel += (SHORT)HIWORD(wParam) / WHEEL_DELTA;

    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure to close the window?", "Quit", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
      SendMessage(hWnd, WM_DESTROY, 0, 0);

    return 0;
  case WM_DESTROY:
    DI6_AnimClose();

    PostQuitMessage(0);
    KillTimer(hWnd, 15);
    return 0;          
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* End of 'MAIN.C' file */