/* FILE NAME   : RNDIMG.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Images handle.
 * LAST UPDATE : 18.06.2018.
 */

#include <string.h>

#include "../../anim.h"

/* Load image from BMP file function.
 * ARGUMENTS:
 *   - image structure to be load to:
 *       di6IMAGE *Img;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DI6_RndImgLoad( di6IMAGE *Img, CHAR *FileName )
{
  INT i;
  HBITMAP hBmFile;
  BITMAP bm;
  BITMAPINFOHEADER bmih = {0};
  HDC hMemDC1, hMemDC2;

  memset(Img, 0, sizeof(di6IMAGE));

  if ((hBmFile = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) == NULL)
    return FALSE;

  GetObject(hBmFile, sizeof(BITMAP), &bm);

  bmih.biSize = sizeof(BITMAPINFOHEADER);
  bmih.biPlanes = 1;
  bmih.biCompression = BI_RGB; /* No compression */
  bmih.biBitCount = 32;
  bmih.biWidth = Img->W = bm.bmWidth;
  bmih.biHeight = Img->H = bm.bmHeight; /* if negative - up-bottom order */
  bmih.biSizeImage = Img->W * Img->H * 4;

  Img->hBm = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS, &Img->Bits, NULL, 0);

  hMemDC1 = CreateCompatibleDC(DI6_Anim.hDC);
  SelectObject(hMemDC1, hBmFile);
  hMemDC2 = CreateCompatibleDC(DI6_Anim.hDC);
  SelectObject(hMemDC2, Img->hBm);
  BitBlt(hMemDC2, 0, 0, Img->W, Img->H, hMemDC1, 0, 0, SRCCOPY);

  DeleteDC(hMemDC1);
  DeleteDC(hMemDC2);
  DeleteObject(hBmFile);

  /* Fill alpha channel */
  for (i = 0; i < Img->W * Img->H; i++)
    Img->Bits[i] |= 0xFF000000;

  return TRUE;
} /* End of 'DI6_RndImgLoad' function */

/* Free image function.
 * ARGUMENTS:
 *   - image structure to be load to:
 *       di6IMAGE *Img;
 * RETURNS: None.
 */
VOID DI6_RndImgFree( di6IMAGE *Img )
{
  DeleteObject(Img->hBm);
  memset(Img, 0, sizeof(di6IMAGE));
} /* End of 'DI6_RndImgFree' function */

/* Upload image to OpenGL texture memory function.
 * ARGUMENTS:
 *   - image structure to be load to:
 *       di6IMAGE *Img;
 * RETURNS: None.
 */
VOID DI6_RndImgToTex( di6IMAGE *Img )
{
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img->W, Img->H,
    GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img->Bits);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
} /* End of 'DI6_RndImgToTex' function */

/* Otain image pixel color function.
 * ARGUMENTS:
 *   - image structure to be load to:
 *       di6IMAGE *Img;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS: None.
 */
DWORD DI6_RndImgGet( di6IMAGE *Img, INT X, INT Y )
{
  X = (X + Img->W) % Img->W;
  Y = (Y + Img->H) % Img->H;
  return Img->Bits[Y * Img->W + X];
} /* End of 'DI6_RndImgGet' function */

/* END OF 'RNDIMG.C' FILE */
