/* FILE NAME: U_Water.C
 * PROGRAMMER: DI6
 * DATE: 11.06.2018
 * PURPOSE: Unit walrus.
 */

#include <stdio.h>
#include "../anim.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  di6PRIMS Walrus;
} di6UNIT_Water;

/* Walrus unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_Water *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitInit( di6UNIT_Water *Uni, di6ANIM *Ani )
{
  DI6_RndPrimsLoad(&Uni->Walrus, "bin/models/penguin.g3dm");

  Uni->Walrus.Trans = MatrTranslate(VecSet(-180, 10, -130));
} /* End of 'DI6_UnitInit' function */        

/* Walrus unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_Water *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitResponse( di6UNIT_Water *Uni, di6ANIM *Ani )
{
} /* End of 'DI6_UnitResponse' function */

/* Walrus unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_Water *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitRender( di6UNIT_Water *Uni, di6ANIM *Ani )
{
  DI6_RndPrimsDraw(&Uni->Walrus, MatrScale(VecSetEqual(3)));
} /* End of 'DI6_UnitRender' function */

/* Walrus unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_Water *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitClose( di6UNIT_Water *Uni, di6ANIM *Ani )
{
  DI6_RndPrimsFree(&Uni->Walrus);
} /* End of 'DI6_UnitClose' function */

/* Unit cow creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (di6UNIT *) pointer to created unit.
 */
di6UNIT * DI6_UnitCreateWalrus( VOID )
{
  di6UNIT_Water *Uni;

  if ((Uni = (di6UNIT_Water *)DI6_AnimUnitCreate(sizeof(di6UNIT_Water))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DI6_UnitInit;
  Uni->Response = (VOID *)DI6_UnitResponse;
  Uni->Render = (VOID *)DI6_UnitRender;
  Uni->Close = (VOID *)DI6_UnitClose;

  return (di6UNIT *)Uni;
} /* End of 'DI6_UnitCreateBall' function */