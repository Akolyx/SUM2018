/* FILE NAME: ANIM.C
 * PROGRAMMER: DI6
 * DATE: 11.06.2018
 * PURPOSE: Animation functions.
 */

#include "anim.h"

di6ANIM DI6_Anim;

VOID DI6_AnimInit( HWND hWnd )
{
  DI6_RndInit(hWnd);
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
VOID DI6_AnimCopyFrame( HDC hDC )
{
  DI6_RndCopyFrame(hDC);
}
VOID DI6_AnimRender( VOID )
{
  INT i;

  for (i = 0; i < DI6_Anim.NumOfUnits; i++)
    DI6_Anim.Units[i]->Response(DI6_Anim.Units[i], &DI6_Anim);

  DI6_RndStart();

  for (i = 0; i < DI6_Anim.NumOfUnits; i++)
    DI6_Anim.Units[i]->Render(DI6_Anim.Units[i], &DI6_Anim);

  DI6_RndEnd();
}
VOID DI6_AnimUnitAdd( di6UNIT *Uni )
{
  if (DI6_Anim.NumOfUnits < di6_MAX_UNITS)
  {  
    DI6_Anim.Units[DI6_Anim.NumOfUnits++] = Uni;
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
} /* End of 'VG4_AnimUnitCreate' function */

/* Enabling switching between window and fullscreen modes */
VOID DI6_AnimFlipFullScreen( HWND hWnd )
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

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitInit( di6UNIT *Uni, di6ANIM *Ani )
{
} /* End of 'di6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitClose( di6UNIT *Uni, di6ANIM *Ani )
{
} /* End of 'di6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitResponse( di6UNIT *Uni, di6ANIM *Ani )
{
} /* End of 'di6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitRender( di6UNIT *Uni, di6ANIM *Ani )
{
} /* End of 'di6_UnitRender' function */