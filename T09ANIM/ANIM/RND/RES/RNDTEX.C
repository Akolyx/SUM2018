/* FILE NAME   : RNDTEX.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Textures handle.
 * LAST UPDATE : 18.06.2018.
 */

#include <stdio.h>
#include <string.h>

#include "../../anim.h"

/* Array of textures */
di6TEXTURE DI6_RndTextures[DI6_MAX_TEXTURES];
/* Textures array store size */
INT DI6_RndTexturesSize;

/* Texture storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DI6_RndTexInit( VOID )
{
  DI6_RndTexturesSize = 0;
} /* End of 'DI6_RndTexInit' function */

/* Texture storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DI6_RndTexClose( VOID )
{
  INT i;

  for (i = 0; i < DI6_RndTexturesSize; i++)
    glDeleteTextures(1, &DI6_RndTextures[i].TexId);
  DI6_RndTexturesSize = 0;
} /* End of 'DI6_RndTexClose' function */

/* Add texture from image to stock from file function.
 * ARGUMENTS:
 *   - image name:
 *       CHAR *Name;
 *   - image size:
 *       INT W, H;
 *   - image pixels data:
 *       DWORD *Bits;
 * RETURNS:
 *   (INT) new texture stock number.
 */
INT DI6_RndTexAddImg( CHAR *Name, INT W, INT H, DWORD *Bits )
{
  if (DI6_RndTexturesSize >= DI6_MAX_TEXTURES)
    return -1;

  /* Allocate texture space */
  glGenTextures(1, &DI6_RndTextures[DI6_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, DI6_RndTextures[DI6_RndTexturesSize].TexId);

  /* Upload texture */
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, W, H, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Bits);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  strncpy(DI6_RndTextures[DI6_RndTexturesSize].Name, Name, DI6_STR_MAX - 1);
  DI6_RndTextures[DI6_RndTexturesSize].W = W;
  DI6_RndTextures[DI6_RndTexturesSize].H = H;

  glBindTexture(GL_TEXTURE_2D, 0);

  return DI6_RndTexturesSize++;
} /* End of 'DI6_RndTexAddImg' function */

/* Add texture to stock from file function.
 * ARGUMENTS:
 *   - texture file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) new texture stock number (-1 if error occured).
 */
INT DI6_RndTexAdd( CHAR *FileName )
{
  INT n;
  di6IMAGE Img;

  if (!DI6_RndImgLoad(&Img, FileName))
    return -1;
  n = DI6_RndTexAddImg(FileName, Img.W, Img.H, Img.Bits);
  DI6_RndImgFree(&Img);
  return n;
} /* End of 'DI6_RndTexAdd' function */

/* END OF 'RNDTEX.C' FILE */
