/* FILE NAME: WINDOW.C
 * PROGRAMMER: DI6
 * DATE: 11.06.2018
 * PURPOSE: Unit window.
 */

#include "../anim.h"
#include <stdio.h>

typedef struct
{
  UNIT_BASE_FIELDS;

  VEC 
    CamLoc,
    CamAt,
    CamUp;

  DBL
    Speed,
    AngleSpeed;

} di6UNIT_CTRL;

/* Control unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_COW *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitInit( di6UNIT_CTRL *Uni, di6ANIM *Ani )
{
  Uni->CamLoc = VecSet(13, 13, 13);
  Uni->CamAt = VecSetEqual(0);
  Uni->CamUp = VecSet(0, 1, 0);
  Uni->Speed = 30.0;
  Uni->AngleSpeed = 10.0;

  DI6_RndCamSet(Uni->CamLoc, Uni->CamAt, Uni->CamUp);
} /* End of 'DI6_UnitInit' function */

/* Camera unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_COW *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitResponse( di6UNIT_CTRL *Uni, di6ANIM *Ani )
{
  CHAR Buf[100];

  if (Ani->Keyboard.KeysClick['F'])
    DI6_AnimFlipFullScreen();
  else if (Ani->Keyboard.KeysClick[VK_ESCAPE])
    DI6_AnimDoExit();
  else if (Ani->Keyboard.KeysClick['P'] && (Ani->Keyboard.Keys[VK_SHIFT]))
    DI6_AnimTimePause();

  if (Ani->Joystick.ButClick[7])
    DI6_AnimTimePause();
  else if (Ani->Joystick.But[5])
    Uni->CamLoc = VecDivNum(Uni->CamLoc, 1 + Ani->Time.GlobalDeltaTime);
  else if (Ani->Joystick.But[4])
    Uni->CamLoc = VecDivNum(Uni->CamLoc, 1 - Ani->Time.GlobalDeltaTime);
  else if (Ani->Joystick.ButClick[6])
    DI6_AnimFlipFullScreen();

  Uni->CamLoc = VecDivNum(Uni->CamLoc, 1 + Ani->Time.GlobalDeltaTime * (Ani->Keyboard.Keys[VK_UP] - Ani->Keyboard.Keys[VK_DOWN]));

  Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateY(Ani->Time.GlobalDeltaTime * Uni->AngleSpeed * 10
                                            * (Ani->Keyboard.Keys[VK_LEFT] - Ani->Keyboard.Keys[VK_RIGHT])));

  Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateY(Ani->Time.DeltaTime * Uni->AngleSpeed * 5 * Ani->Joystick.x));
  Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateX(Ani->Time.DeltaTime * Uni->AngleSpeed * 5 * Ani->Joystick.y));
  Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateZ(Ani->Time.DeltaTime * Uni->AngleSpeed * 5 * Ani->Joystick.z));

  DI6_RndCamSet(Uni->CamLoc, Uni->CamAt, Uni->CamUp);

  sprintf(Buf, "FPS = %.3f", Ani->Time.FPS);
  SetWindowText(Ani->hWnd, Buf);

} /* End of 'DI6_UnitResponse' function */

/* Unit camera creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (di6UNIT *) pointer to created unit.
 */
di6UNIT * DI6_UnitCreateControl( VOID )
{
  di6UNIT_CTRL *Uni;

  if ((Uni = (di6UNIT_CTRL *)DI6_AnimUnitCreate(sizeof(di6UNIT_CTRL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DI6_UnitInit;
  Uni->Response = (VOID *)DI6_UnitResponse;

  return (di6UNIT *)Uni;
} /* End of 'VG4_UnitCreateBall' function */