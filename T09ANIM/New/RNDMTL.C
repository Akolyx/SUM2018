/* FILE NAME   : RNDMTL.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Materials handle.
 * PROGRAMMER  : Vitaly A. Galinsky.
 * LAST UPDATE : 11.06.2018.
 * NOTE        : Module prefix 'VG4'.
 */

#include "../../anim.h"

/* Array of materials */
vg4MATERIAL VG4_RndMaterials[VG4_MAX_MATERIALS];
/* Materials array store size */
INT VG4_RndMaterialsSize;

/* Material storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_RndMtlInit( VOID )
{
  vg4MATERIAL defm =
  {
    "Default material",
    {0.2, 0.2, 0.2}, {0.8, 0.8, 0.8}, {0.3, 0.3, 0.3}, 24,
    1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    "",
    0
  };

  VG4_RndMaterialsSize = 0;
  VG4_RndMtlAdd(&defm);
} /* End of 'VG4_RndMtlInit' function */

/* Add material to stock function.
 * ARGUMENTS:
 *   - pointer to material data:
 *       vg4MATERIAL *Mtl;
 * RETURNS:
 *   (INT) new material stock number.
 */
INT VG4_RndMtlAdd( vg4MATERIAL *Mtl )
{
  if (VG4_RndMaterialsSize >= VG4_MAX_MATERIALS)
    return 0;
  VG4_RndMaterials[VG4_RndMaterialsSize] = *Mtl;
  return VG4_RndMaterialsSize++;
} /* End of 'VG4_RndMtlAdd' function */

/* Apply material before rendering function.
 * ARGUMENTS:
 *   - material number in materials storage array:
 *       INT MtlNo;
 * RETURNS:
 *   (INT) applyed material shader program Id.
 */
INT VG4_RndMtlApply( INT MtlNo )
{
  INT i, prg, loc;
  vg4MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= VG4_RndMaterialsSize)
    MtlNo = 0;
  mtl = &VG4_RndMaterials[MtlNo];

  /* Set program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= VG4_RndShadersSize)
    prg = VG4_RndShaders[0].ProgId;
  else
    prg = VG4_RndShaders[prg].ProgId;

  glUseProgram(prg);
  if ((loc = glGetUniformLocation(prg, "Time")) != -1)
    glUniform1f(loc, VG4_Anim.Time);
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, VG4_Anim.GlobalTime);

  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);

  for (i = 0; i < 8; i++)
  {
    CHAR Buf[100] = "IsTexture0";

    Buf[9] = '0' + i;
    if ((loc = glGetUniformLocation(prg, Buf)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, VG4_RndTextures[mtl->Tex[i]].TexId);
    }
  }

  return prg;
} /* End of 'VG4_RndMtlApply' function */

/* END OF 'RNDMTL.C' FILE */
