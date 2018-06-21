/* FILE NAME: U_WATER.C
 * PROGRAMMER: DI6
 * DATE: 19.06.2018
 * PURPOSE: Unit ground.
 */

#include "../anim.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  di6PRIM Water;
} di6UNIT_WATER;

/* Water unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_WATER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitInit( di6UNIT_WATER *Uni, di6ANIM *Ani )
{
  di6GRID G;
  di6MATERIAL mtl = {"Water material", {0.1, 0.1, 0.4}, {0.9, 0.9, 0.9}, {0.9, 0.9, 0.9}, 100, 0.5, {-1, -1, -1, -1, -1, -1, -1, -1}, "", 0};               

  DI6_RndGridCreatePlane(&G, 1000, 1000, VecSet(-500, -9.9, -500), VecSet(0, 0, 1000), VecSet(1000, 0, 0), 1, 1);

  DI6_RndPrimGridEvalNormals(&G);
  DI6_RndPrimFromGrid(&Uni->Water, &G);

  //mtl.Tex[0] = DI6_RndTexAdd("bin/textures/water/water.bmp");

  mtl.ShdNo = DI6_RndShdAdd("BIN/SHADERS/WATER");

  Uni->Water.MtlNo = DI6_RndMtlAdd(&mtl);
} /* End of 'DI6_UnitInit' function */

/* Water unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_WATER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitResponse( di6UNIT_WATER *Uni, di6ANIM *Ani )
{

} /* End of 'DI6_UnitResponse' function */

/* Water unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_HELICOPTER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitRender( di6UNIT_WATER *Uni, di6ANIM *Ani )
{
  DI6_RndPrimDraw(&Uni->Water, MatrIdentity());
} /* End of 'DI6_UnitRender' function */

/* Water unit create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (di6UNIT *) pointer to created unit
 */
di6UNIT * DI6_UnitCreateWater( VOID )
{
  di6UNIT_WATER *Uni;

  if ((Uni = (di6UNIT_WATER *)DI6_AnimUnitCreate(sizeof(di6UNIT_WATER))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DI6_UnitInit;
  Uni->Response = (VOID *)DI6_UnitResponse;
  Uni->Render = (VOID *)DI6_UnitRender;
  Uni->Close = (VOID *)DI6_UnitClose;

  return (di6UNIT *)Uni;
} /* End of 'DI6_UnitCreateBall' function */