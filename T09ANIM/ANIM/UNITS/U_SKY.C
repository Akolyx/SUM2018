/* FILE NAME: U_SKY.C
 * PROGRAMMER: DI6
 * DATE: 19.06.2018
 * PURPOSE: Unit ground.
 */

#include "../anim.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  di6PRIM Sky;
} di6UNIT_SKY;

/* Sky unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_SKY *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitInit( di6UNIT_SKY *Uni, di6ANIM *Ani )
{
  di6GRID G;
  di6MATERIAL mtl = {"Sky material", {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.2, 0.2, 0.2}, 100, 1, {-1, -1, -1, -1, -1, -1, -1, -1}, "", 0};               

  DI6_RndGridCreateSphere(&G, 400, 200, VecSet(0, -50, 0), 500);

  DI6_RndPrimFromGrid(&Uni->Sky, &G);

  mtl.Tex[0] = DI6_RndTexAdd("bin/textures/sky/sky.bmp");

  mtl.ShdNo = DI6_RndShdAdd("BIN/SHADERS/SKY");

  Uni->Sky.MtlNo = DI6_RndMtlAdd(&mtl);
} /* End of 'DI6_UnitInit' function */

/* Sky unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_SKY *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitResponse( di6UNIT_SKY *Uni, di6ANIM *Ani )
{

} /* End of 'DI6_UnitResponse' function */

/* Sky unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_HELICOPTER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitRender( di6UNIT_SKY *Uni, di6ANIM *Ani )
{
  DI6_RndPrimDraw(&Uni->Sky, MatrIdentity());
} /* End of 'DI6_UnitRender' function */

/* Sky unit create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (di6UNIT *) pointer to created unit
 */
di6UNIT * DI6_UnitCreateSky( VOID )
{
  di6UNIT_SKY *Uni;

  if ((Uni = (di6UNIT_SKY *)DI6_AnimUnitCreate(sizeof(di6UNIT_SKY))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DI6_UnitInit;
  Uni->Response = (VOID *)DI6_UnitResponse;
  Uni->Render = (VOID *)DI6_UnitRender;
  Uni->Close = (VOID *)DI6_UnitClose;

  return (di6UNIT *)Uni;
} /* End of 'DI6_UnitCreateBall' function */