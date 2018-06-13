/* FILE NAME: RNDBASE.C
 * PROGRAMMER: DI6
 * DATE: 09.06.2018
 * PURPOSE: Base functions.
 */

#include "../anim.h"

VOID DI6_RndInit( HWND hWnd )
{
  HDC hDC;

  DI6_Anim.hWnd = hWnd;

  hDC = GetDC(DI6_Anim.hWnd);
  DI6_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(DI6_Anim.hWnd, hDC);

  DI6_RndProjSet();
  //DI6_RndCamSet(VecSet(13, 13, 10), VecSet(0, 0, 0), VecSet(0, 1, 0));
} /* End of 'DI6_RndInit' function */

VOID DI6_RndClose( VOID )
{
  DeleteObject(DI6_Anim.hBm);
  DeleteDC(DI6_Anim.hDC);
} /* End of 'DI6_RndClose' function */

VOID DI6_RndResize( INT w, INT h )
{
  HDC hDC;

  if (DI6_Anim.hBm != NULL)
    DeleteObject(DI6_Anim.hBm);

  hDC = GetDC(DI6_Anim.hWnd);
  DI6_Anim.hBm = CreateCompatibleBitmap(hDC, w, h);
  ReleaseDC(DI6_Anim.hWnd, hDC);

  SelectObject(DI6_Anim.hDC, DI6_Anim.hBm);
 
  DI6_Anim.w = w;
  DI6_Anim.h = h;

  DI6_RndProjSet();
} /* End of 'DI6_RndResize' function */


VOID DI6_RndStart( VOID )
{
  SelectObject(DI6_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(DI6_Anim.hDC, GetStockObject(BLACK_PEN));

  SetDCBrushColor(DI6_Anim.hDC, RGB(255, 255, 255));
  Rectangle(DI6_Anim.hDC, 0, 0, DI6_Anim.w, DI6_Anim.h);

  SelectObject(DI6_Anim.hDC, GetStockObject(NULL_BRUSH));
  SelectObject(DI6_Anim.hDC, GetStockObject(DC_PEN));
  SetDCPenColor(DI6_Anim.hDC, RGB(0, 0, 0));
} /* End of 'DI6_RndStart' function */

VOID DI6_RndEnd( VOID )
{

} /* End of 'DI6_RndEnd' function */

VOID DI6_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, DI6_Anim.w, DI6_Anim.h, DI6_Anim.hDC, 0, 0, SRCCOPY);
} /* End of 'DI6_RndCopyFrame' function */

/* End of 'RNDBASE.C' file */