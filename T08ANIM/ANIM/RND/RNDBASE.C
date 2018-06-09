/* FILE NAME: RNDBASE.C
 * PROGRAMMER: DI6
 * DATE: 09.06.2018
 * PURPOSE: Base functions.
 */

#include "rnd.h"

HWND DI6_hWndRndFrame;
HDC DI6_hDCRndFrame;
HBITMAP DI6_hBmRndFrame;
INT DI6_RndFrameW, DI6_RndFrameH;

VOID DI6_RndInit( HWND hWnd )
{
  HDC hDC;

  DI6_hWndRndFrame = hWnd;

  hDC = GetDC(DI6_hWndRndFrame);
  DI6_hDCRndFrame = CreateCompatibleDC(hDC);
  ReleaseDC(DI6_hWndRndFrame, hDC);
} /* End of 'DI6_RndInit' function */

VOID DI6_RndClose( VOID )
{
  DeleteObject(DI6_hBmRndFrame);
  DeleteDC(DI6_hDCRndFrame);
} /* End of 'DI6_RndClose' function */

VOID DI6_RndResize( INT w, INT h )
{
  HDC hDC;

  if (DI6_hBmRndFrame != NULL)
    DeleteObject(DI6_hBmRndFrame);

  hDC = GetDC(DI6_hWndRndFrame);
  DI6_hBmRndFrame = CreateCompatibleBitmap(hDC, w, h);
  ReleaseDC(DI6_hWndRndFrame, hDC);

  SelectObject(DI6_hDCRndFrame, DI6_hBmRndFrame);
 
  DI6_RndFrameW = w;
  DI6_RndFrameH = h;

  DI6_RndProjSet();
} /* End of 'DI6_RndResize' function */

VOID DI6_RndStart( VOID )
{
  SelectObject(DI6_hDCRndFrame, GetStockObject(DC_BRUSH));
  SelectObject(DI6_hDCRndFrame, GetStockObject(DC_PEN));

  SetDCBrushColor(DI6_hDCRndFrame, RGB(255, 255, 0));
  Rectangle(DI6_hDCRndFrame, 0, 0, DI6_RndFrameW, DI6_RndFrameH);

  SetDCBrushColor(DI6_hDCRndFrame, RGB(0, 0, 255));

} /* End of 'DI6_RndStart' function */

VOID DI6_RndEnd( VOID )
{
} /* End of 'DI6_RndEnd' function */

VOID DI6_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, DI6_RndFrameW, DI6_RndFrameH, DI6_hDCRndFrame, 0, 0, SRCCOPY);
} /* End of 'DI6_RndCopyFrame' function */

/* End of 'RNDBASE.C' file */