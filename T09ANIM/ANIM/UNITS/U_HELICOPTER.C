/* FILE NAME: U_HELICOPTER.C
 * PROGRAMMER: DI6
 * DATE: 11.06.2018
 * PURPOSE: Unit helicopter.
 */

#include "../anim.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  di6PRIMS Helicopter;
  FLT RandomDet;
  BOOL IsLine;
} di6UNIT_HELICOPTER;

/* Helicopter unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_HELICOPTER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitInit( di6UNIT_HELICOPTER *Uni, di6ANIM *Ani )
{
  DI6_RndPrimsLoad(&Uni->Helicopter, "bin/models/wa-heli.g3dm");
  Uni->Helicopter.Trans = MatrIdentity();
  Uni->RandomDet = rand() % 4579;
  Uni->IsLine = FALSE;
} /* End of 'DI6_UnitInit' function */

/* Helicopter unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_HELICOPTER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitResponse( di6UNIT_HELICOPTER *Uni, di6ANIM *Ani )
{
  if ((Ani->Keyboard.KeysClick['L'] && (Ani->Keyboard.Keys[VK_SHIFT])))
  {
    if (Uni->IsLine == FALSE)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    Uni->IsLine = !Uni->IsLine;
  }

  Uni->Helicopter.Prims[7].Trans = MatrMulMatr3(MatrTranslate(VecNeg(VecSet(-0.05, 1.8, -1.2))), MatrRotateY(980 * DI6_Anim.Timer.Time),
                                        MatrTranslate(VecSet(-0.05, 1.8, -1.2)));
  Uni->Helicopter.Prims[9].Trans = MatrMulMatr3(MatrTranslate(VecNeg(VecSet(-0.05, 1.8, -1.2))), MatrRotateY(980 * DI6_Anim.Timer.Time),
                                        MatrTranslate(VecSet(-0.05, 1.8, -1.2)));

  Uni->Helicopter.Trans = MatrTranslate(VecSet(0, 0, -10 * DI6_Anim.Timer.Time));

  //DI6_RndCamSet(Uni->CamLoc, Uni->CamAt, Uni->CamUp);
} /* End of 'DI6_UnitResponse' function */

/* Helicopter unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_HELICOPTER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitRender( di6UNIT_HELICOPTER *Uni, di6ANIM *Ani )
{
  srand(Uni->RandomDet);

  DI6_RndPrimsDraw(&Uni->Helicopter, MatrScale(VecSetEqual(1)));
} /* End of 'DI6_UnitRender' function */

/* Helicopter unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_HELICOPTER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitClose( di6UNIT_HELICOPTER *Uni, di6ANIM *Ani )
{
  DI6_RndPrimsFree(&Uni->Helicopter);
} /* End of 'DI6_UnitClose' function */

/* Unit cow creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (di6UNIT *) pointer to created unit.
 */
di6UNIT * DI6_UnitCreateHelicopter( VOID )
{
  di6UNIT_HELICOPTER *Uni;

  if ((Uni = (di6UNIT_HELICOPTER *)DI6_AnimUnitCreate(sizeof(di6UNIT_HELICOPTER))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DI6_UnitInit;
  Uni->Response = (VOID *)DI6_UnitResponse;
  Uni->Render = (VOID *)DI6_UnitRender;
  Uni->Close = (VOID *)DI6_UnitClose;

  return (di6UNIT *)Uni;
} /* End of 'DI6_UnitCreateBall' function */