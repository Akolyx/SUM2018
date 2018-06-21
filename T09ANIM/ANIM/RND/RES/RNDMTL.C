/* FILE NAME   : RNDMTL.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Materials handle.
 * LAST UPDATE : 15.06.2018.
 */

#include "../../anim.h"

/* Array of materials */
di6MATERIAL DI6_RndMaterials[DI6_MAX_MATERIALS];

/* Materials array store size */
INT DI6_RndMaterialsSize;

/* Material storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DI6_RndMtlInit( VOID )
{
  di6MATERIAL defm =
  {
    "Default material",
    {0.2, 0.5, 0.2}, {0.3, 0.3, 0.3}, {0.5, 0.5, 0.5},
    13.7,
    1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    "",
    0
  };

  DI6_RndMaterialsSize = 0;
  DI6_RndMtlAdd(&defm);
} /* End of 'DI6_RndMtlInit' function */

/* Add material to stock function.
 * ARGUMENTS:
 *   - pointer to material data:
 *       di6MATERIAL *Mtl;
 * RETURNS:
 *   (INT) new material stock number.
 */
INT DI6_RndMtlAdd( di6MATERIAL *Mtl )
{
  if (DI6_RndMaterialsSize >= DI6_MAX_MATERIALS)
    return 0;
  DI6_RndMaterials[DI6_RndMaterialsSize] = *Mtl;
  return DI6_RndMaterialsSize++;
} /* End of 'DI6_RndMtlAdd' function */

/* Apply material before rendering function.
 * ARGUMENTS:
 *   - material number in materials storage array:
 *       INT MtlNo;
 * RETURNS:
 *   (INT) applyed material shader program Id.
 */
INT DI6_RndMtlApply( INT MtlNo )
{
  INT prg, loc, i;
  di6MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= DI6_RndMaterialsSize)
    MtlNo = 0;
  mtl = &DI6_RndMaterials[MtlNo];

  /* Set program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= DI6_RndShadersSize)
    prg = DI6_RndShaders[0].ProgId;
  else
    prg = DI6_RndShaders[prg].ProgId;

  glUseProgram(prg);

  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.x);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.x);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.x);
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
      glBindTexture(GL_TEXTURE_2D, DI6_RndTextures[mtl->Tex[i]].TexId);
    }
  }

  return prg;
} /* End of 'DI6_RndMtlApply' function */

/* END OF 'RNDMTL.C' FILE */
