/* FILE NAME: ANIM.C
 * PROGRAMMER: DI6
 * DATE: 11.06.2018
 * PURPOSE: Animation functions.
 */

#include "anim.h"

di6ANIM DI6_Anim;

VOID DI6_AnimInit( HWND hWnd )
{
  srand(130);

  DI6_RndInit(hWnd);
  DI6_TimerInit();
}
VOID DI6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < DI6_Anim.NumOfUnits; i++)
  {
    DI6_Anim.Units[i]->Close(DI6_Anim.Units[i], &DI6_Anim);
    free(DI6_Anim.Units[i]);
  }
  DI6_Anim.NumOfUnits = 0;
  DI6_RndClose();
}
VOID DI6_AnimResize( INT w, INT h )
{
  DI6_RndResize(w, h);
}
VOID DI6_AnimCopyFrame( VOID )
{
  DI6_RndCopyFrame();
}
VOID DI6_AnimRender( VOID )
{
  INT i;

  DI6_TimerResponse();

  DI6_AnimInputResponse();

  for (i = 0; i < DI6_Anim.NumOfUnits; i++)
    DI6_Anim.Units[i]->Response(DI6_Anim.Units[i], &DI6_Anim);

  DI6_RndStart();

  for (i = 0; i < DI6_Anim.NumOfUnits; i++)
    DI6_Anim.Units[i]->Render(DI6_Anim.Units[i], &DI6_Anim);

  DI6_RndEnd();
}
VOID DI6_AnimUnitAdd( di6UNIT *Uni )
{
  if (DI6_Anim.NumOfUnits < DI6_MAX_UNITS)
  {  
    DI6_Anim.Units[DI6_Anim.NumOfUnits] = Uni;
    Uni->i = DI6_Anim.NumOfUnits++;
    Uni->Init(Uni, &DI6_Anim);
  }
}

di6UNIT * DI6_AnimUnitCreate( INT Size )
{
  di6UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(di6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = DI6_UnitInit;
  Uni->Close = DI6_UnitClose;
  Uni->Response = DI6_UnitResponse;
  Uni->Render = DI6_UnitRender;
  return Uni;
} /* End of 'DI6_AnimUnitCreate' function */

/* Enabling switching between window and fullscreen modes */
VOID DI6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;
                                            
  if (IsFullScreen)
  {
    /* Restore window size and position */
    IsFullScreen = FALSE;

    SetWindowPos(DI6_Anim.hWnd, HWND_TOP, SaveRC.left, SaveRC.top,
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
    GetWindowRect(DI6_Anim.hWnd, &SaveRC);

    /* Obtain closest monitor info */
    hMon = MonitorFromWindow(DI6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(DI6_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(DI6_Anim.hWnd, HWND_TOP, rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top, SWP_NOOWNERZORDER);
  }
} /* End of 'FlipFullScreen' function */

VOID DI6_AnimDoExit ( VOID )
{
  static BOOL IsExit = FALSE;

  if (IsExit)
    return;

  IsExit = TRUE;
  SendMessage(DI6_Anim.hWnd, WM_CLOSE, 0, 0);
  IsExit = FALSE;
}
