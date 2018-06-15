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
  INT prg;
  di6MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= DI6_RndMaterialsSize)
    MtlNo = 0;
  mtl = &DI6_RndMaterials[MtlNo];

  /* Set program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= DI6_RndShadersSize)
    prg = 0;
  else
    prg = DI6_RndShaders[prg].ProgId;

  glUseProgram(prg);

  return prg;
} /* End of 'DI6_RndMtlApply' function */

/* END OF 'RNDMTL.C' FILE */
