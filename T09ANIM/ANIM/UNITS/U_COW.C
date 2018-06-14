/* FILE NAME: U_COW.C
 * PROGRAMMER: DI6
 * DATE: 11.06.2018
 * PURPOSE: Unit cow.
 */

#include "../anim.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  di6PRIM Cow;
  FLT RandomDet;
} di6UNIT_COW;

/* Cow unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_COW *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitInit( di6UNIT_COW *Uni, di6ANIM *Ani )
{
  DI6_RndPrimLoad(&Uni->Cow, "cow.obj");
  Uni->Pos = VecSet(60 * DI6_Rand_1_1(), 0, 60 * DI6_Rand_1_1());
  Uni->RandomDet = rand() % 2345;
} /* End of 'DI6_UnitInit' function */

/* Cow unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_COW *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitResponse( di6UNIT_COW *Uni, di6ANIM *Ani )
{
} /* End of 'DI6_UnitResponse' function */

/* Cow unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_COW *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitRender( di6UNIT_COW *Uni, di6ANIM *Ani )
{
  srand(Uni->RandomDet);

  DI6_RndPrimDraw(&Uni->Cow, MatrMulMatr3(MatrScale(VecSet(0.4, 0.4, 0.4)), MatrTranslate(Uni->Pos),
                             MatrTranslate(VecSet(0, 5 * fabs(sin(10 * DI6_Rand0_1() * Ani->Timer.Time)), 0))));
} /* End of 'DI6_UnitRender' function */

/* Cow unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_COW *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitClose( di6UNIT_COW *Uni, di6ANIM *Ani )
{
  DI6_RndPrimFree(&Uni->Cow);
} /* End of 'DI6_UnitClose' function */

/* Unit cow creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (di6UNIT *) pointer to created unit.
 */
di6UNIT * DI6_UnitCreateCow( VOID )
{
  di6UNIT_COW *Uni;

  if ((Uni = (di6UNIT_COW *)DI6_AnimUnitCreate(sizeof(di6UNIT_COW))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DI6_UnitInit;
  Uni->Response = (VOID *)DI6_UnitResponse;
  Uni->Render = (VOID *)DI6_UnitRender;
  Uni->Close = (VOID *)DI6_UnitClose;

  return (di6UNIT *)Uni;
} /* End of 'VG4_UnitCreateBall' function */