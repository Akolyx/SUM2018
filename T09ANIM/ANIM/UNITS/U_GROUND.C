/* FILE NAME: U_GROUND.C
 * PROGRAMMER: DI6
 * DATE: 19.06.2018
 * PURPOSE: Unit ground.
 */

#include "../anim.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  di6PRIM Gr;
} di6UNIT_GROUND;

/* Sky unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_GROUND *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitInit( di6UNIT_GROUND *Uni, di6ANIM *Ani )
{
  INT i;
  di6GRID G;
  di6MATERIAL mtl = {"Ground material", {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {1, 1, 1}, 100, 1, {-1, -1, -1, -1, -1, -1, -1, -1}, "", 0};               

  DI6_RndGridCreateHeightField(&G, "bin/textures/ground/hf0.bmp", VecSet(-500, 0, -500), VecSet(0, 0, 1000), VecSet(1000, 0, 0), 10);

  for (i = 0; i < G.W * G.H; i++)
  {
    if (G.V[i].p.z > 0)
      G.V[i].p.y += (G.V[i].p.z) / 5;

    if (G.V[i].p.x > 100)
      G.V[i].p.y += (G.V[i].p.x - 100) / 3;
  }

  DI6_RndPrimGridEvalNormals(&G);
  DI6_RndPrimFromGrid(&Uni->Gr, &G);

  mtl.Tex[0] = DI6_RndTexAdd("bin/textures/ground/soil.bmp");
  mtl.Tex[1] = DI6_RndTexAdd("bin/textures/ground/grass.bmp");
  mtl.Tex[2] = DI6_RndTexAdd("bin/textures/ground/moss.bmp");
  mtl.Tex[3] = DI6_RndTexAdd("bin/textures/sky/water.bmp");
  mtl.Tex[4] = DI6_RndTexAdd("bin/textures/ground/mask_grass.bmp");
  mtl.Tex[5] = DI6_RndTexAdd("bin/textures/ground/mask_moss.bmp");
  mtl.Tex[6] = DI6_RndTexAdd("bin/textures/ground/mask_water.bmp");

  mtl.ShdNo = DI6_RndShdAdd("BIN/SHADERS/GROUND");

  Uni->Gr.MtlNo = DI6_RndMtlAdd(&mtl);
} /* End of 'DI6_UnitInit' function */

/* Sky unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_GROUND *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitResponse( di6UNIT_GROUND *Uni, di6ANIM *Ani )
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
static VOID DI6_UnitRender( di6UNIT_GROUND *Uni, di6ANIM *Ani )
{
  DI6_RndPrimDraw(&Uni->Gr, MatrIdentity());
} /* End of 'DI6_UnitRender' function */

/* Sky unit create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (di6UNIT *) pointer to created unit
 */
di6UNIT * DI6_UnitCreateGround( VOID )
{
  di6UNIT_GROUND *Uni;

  if ((Uni = (di6UNIT_GROUND *)DI6_AnimUnitCreate(sizeof(di6UNIT_GROUND))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DI6_UnitInit;
  Uni->Response = (VOID *)DI6_UnitResponse;
  Uni->Render = (VOID *)DI6_UnitRender;
  Uni->Close = (VOID *)DI6_UnitClose;

  return (di6UNIT *)Uni;
} /* End of 'DI6_UnitCreateBall' function */