/* FILE NAME   : RNDTEX.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Textures handle.
 * PROGRAMMER  : Vitaly A. Galinsky.
 * LAST UPDATE : 11.06.2018.
 * NOTE        : Module prefix 'VG4'.
 */

#include <stdio.h>
#include <string.h>

#include "../../anim.h"

/* Array of textures */
vg4TEXTURE VG4_RndTextures[VG4_MAX_TEXTURES];
/* Shadres array store size */
INT VG4_RndTexturesSize;

/* Texture storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_RndTexInit( VOID )
{
  VG4_RndTexturesSize = 0;
} /* End of 'VG4_RndTexInit' function */

/* Texture storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_RndTexClose( VOID )
{
  INT i;

  for (i = 0; i < VG4_RndTexturesSize; i++)
    glDeleteTextures(1, &VG4_RndTextures[i].TexId);
  VG4_RndTexturesSize = 0;
} /* End of 'VG4_RndTexClose' function */

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
INT VG4_RndTexAddImg( CHAR *Name, INT W, INT H, DWORD *Bits )
{
  if (VG4_RndTexturesSize >= VG4_MAX_TEXTURES)
    return -1;
  /* Allocate texture space */
  glGenTextures(1, &VG4_RndTextures[VG4_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, VG4_RndTextures[VG4_RndTexturesSize].TexId);

  /* Upload texture */
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, W, H, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Bits);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  strncpy(VG4_RndTextures[VG4_RndTexturesSize].Name, Name, VG4_STR_MAX - 1);
  VG4_RndTextures[VG4_RndTexturesSize].W = W;
  VG4_RndTextures[VG4_RndTexturesSize].H = H;

  glBindTexture(GL_TEXTURE_2D, 0);

  return VG4_RndTexturesSize++;
} /* End of 'VG4_RndTexAddImg' function */

/* Add texture to stock from file function.
 * ARGUMENTS:
 *   - texture file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) new texture stock number (-1 if error occured).
 */
INT VG4_RndTexAdd( CHAR *FileName )
{
  INT n;
  vg4IMAGE Img;

  if (!VG4_RndImgLoad(&Img, FileName))
    return -1;
  n = VG4_RndTexAddImg(FileName, Img.W, Img.H, Img.Bits);
  VG4_RndImgFree(&Img);
  return n;
} /* End of 'VG4_RndTexAdd' function */

/* END OF 'RNDTEX.C' FILE */
