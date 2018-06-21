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

  FLT
    Speed,
    AngleSpeed;

  BOOL IsLine;
} di6UNIT_CTRL;

/* Control unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_HELICOPTER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitInit( di6UNIT_CTRL *Uni, di6ANIM *Ani )
{
  Uni->Camera.Loc = VecSet(0, 50, 15);
  Uni->Camera.At = VecSet(0, 0, -5);
  Uni->Camera.Up = VecSet(0, 1, 0);
  Uni->Speed = 20.0;
  Uni->AngleSpeed = 100.0;
} /* End of 'DI6_UnitInit' function */

/* Camera unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_HELICOPTER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitResponse( di6UNIT_CTRL *Uni, di6ANIM *Ani )
{
  VEC dir, dir90;

  if (((Ani->Keyboard.KeysClick['F']) && (Ani->Keyboard.Keys[VK_SHIFT])) || (Ani->Keyboard.KeysClick[VK_F11]))
    DI6_AnimFlipFullScreen();
  else if (Ani->Keyboard.KeysClick[VK_ESCAPE])
    DI6_AnimDoExit();
  else if (Ani->Keyboard.KeysClick['C'])
  {
    if (!Ani->IsCameraFree)
      DI6_RndActiveCam(Uni->i);
    else
      DI6_RndActiveCam(0);

    Ani->IsCameraFree = !Ani->IsCameraFree;
  }
  else if ((Ani->Keyboard.KeysClick['P'] && (Ani->Keyboard.Keys[VK_SHIFT])))
  {
    DI6_AnimTimePause();
  }
  else if ((Ani->Keyboard.KeysClick['W'] && (Ani->Keyboard.Keys[VK_CONTROL])))
  {
    if (Uni->IsLine == FALSE)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    Uni->IsLine = !Uni->IsLine;
  }

  if (Ani->IsCameraFree)
  {
    /* Calculating direction vector and rotated direction vector */
    dir = DI6_Anim.Camera.Dir;
    dir90 = VectorTransform(dir, MatrRotateY(90));

    /* Motion forwards, backwards, rightwards and leftwards using WASD */
    Uni->Camera.At = VecAddVec(VecMulNum(VecNormalize(VecSet(dir.x, 0, dir.z)), 
      Ani->Timer.GlobalDeltaTime * Uni->Speed * (Ani->Keyboard.Keys['W'] - Ani->Keyboard.Keys['S'])), Uni->Camera.At);

    Uni->Camera.Loc = VecAddVec(VecMulNum(VecNormalize(VecSet(dir.x, 0, dir.z)), 
      Ani->Timer.GlobalDeltaTime * Uni->Speed * (Ani->Keyboard.Keys['W'] - Ani->Keyboard.Keys['S'])), Uni->Camera.Loc);

    Uni->Camera.At = VecAddVec(VecMulNum(VecNormalize(VecSet(dir90.x, 0, dir90.z)), 
      Ani->Timer.GlobalDeltaTime * Uni->Speed * (Ani->Keyboard.Keys['A'] - Ani->Keyboard.Keys['D'])), Uni->Camera.At);

    Uni->Camera.Loc = VecAddVec(VecMulNum(VecNormalize(VecSet(dir90.x, 0, dir90.z)),
      Ani->Timer.GlobalDeltaTime * Uni->Speed * (Ani->Keyboard.Keys['A'] - Ani->Keyboard.Keys['D'])), Uni->Camera.Loc);

    /* Moving upwards using PageUp and PageDown */
    Uni->Camera.At = VecAddVec(VecMulNum(VecSet(0, 1, 0), 
      Ani->Timer.GlobalDeltaTime * Uni->Speed * (Ani->Keyboard.Keys[VK_PRIOR] - Ani->Keyboard.Keys[VK_NEXT])), Uni->Camera.At);

    Uni->Camera.Loc = VecAddVec(VecMulNum(VecSet(0, 1, 0),
      Ani->Timer.GlobalDeltaTime * Uni->Speed * (Ani->Keyboard.Keys[VK_PRIOR] - Ani->Keyboard.Keys[VK_NEXT])), Uni->Camera.Loc);

    /* Recalculating direction vector */
    dir = VecSubVec(Uni->Camera.At, Uni->Camera.Loc);

    /* Motion towards and backwards along direction vector using arrows (up and down) */
    Uni->Camera.At = VecAddVec(VecMulNum(VecNormalize(dir), 
      Ani->Timer.GlobalDeltaTime * Uni->Speed * (Ani->Keyboard.Keys[VK_UP] - Ani->Keyboard.Keys[VK_DOWN])), Uni->Camera.At);

    Uni->Camera.Loc = VecAddVec(VecMulNum(VecNormalize(dir), 
      Ani->Timer.GlobalDeltaTime * Uni->Speed * (Ani->Keyboard.Keys[VK_UP] - Ani->Keyboard.Keys[VK_DOWN])), Uni->Camera.Loc);

    /* Rotating along Y axis using arrows (left and right) */
    Uni->Camera.Loc = PointTransform(Uni->Camera.Loc, MatrMulMatr3(MatrTranslate(VecSet(-Uni->Camera.At.x, 0, -Uni->Camera.At.z)),
                                              MatrRotateY(Ani->Timer.GlobalDeltaTime * Uni->AngleSpeed
                                              * (Ani->Keyboard.Keys[VK_RIGHT] - Ani->Keyboard.Keys[VK_LEFT])),
                                              MatrTranslate(VecSet(Uni->Camera.At.x, 0, Uni->Camera.At.z))));

    /* Motion towards and backwards along direction vector using mouse wheel */
    Uni->Camera.Loc = VecAddVec(VecMulNum(VecNormalize(dir), 
      Ani->Timer.GlobalDeltaTime * Uni->Speed * 10 * Ani->Mouse.dz), Uni->Camera.Loc);

    /* Rotating along X and Z axes using mouse */
    Uni->Camera.Loc = PointTransform(Uni->Camera.Loc, MatrMulMatr3(MatrTranslate(VecSet(-Uni->Camera.At.x, 0, -Uni->Camera.At.z)),
                                              MatrRotateX(Ani->Timer.GlobalDeltaTime * Uni->AngleSpeed
                                              * Ani->Keyboard.Keys[VK_LBUTTON] * -Ani->Mouse.dx),
                                              MatrTranslate(VecSet(Uni->Camera.At.x, 0, Uni->Camera.At.z))));

    Uni->Camera.Loc = PointTransform(Uni->Camera.Loc, MatrMulMatr3(MatrTranslate(VecSet(0, -Uni->Camera.At.y, -Uni->Camera.At.z)),
                                              MatrRotateZ(Ani->Timer.GlobalDeltaTime * Uni->AngleSpeed
                                              * Ani->Keyboard.Keys[VK_LBUTTON] * -Ani->Mouse.dy),
                                              MatrTranslate(VecSet(0, Uni->Camera.At.y, Uni->Camera.At.z))));

    DI6_RndCamSet(Uni->Camera.Loc, Uni->Camera.At, Uni->Camera.Up);
  }
} /* End of 'DI6_UnitResponse' function */

/* Control unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_HELICOPTER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitRender( di6UNIT_CTRL *Uni, di6ANIM *Ani )
{
} /* End of 'DI6_UnitRender' function */

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
  Uni->Render = (VOID *)DI6_UnitRender;

  return (di6UNIT *)Uni;
} /* End of 'DI6_UnitCreateBall' function */