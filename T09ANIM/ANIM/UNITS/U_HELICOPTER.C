/* FILE NAME: U_HELICOPTER.C
 * PROGRAMMER: DI6
 * DATE: 11.06.2018
 * PURPOSE: Unit helicopter.
 */

#include <stdio.h>
#include "../anim.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  di6PRIMS Helicopter;
  di6MOV
    MoveForv,
    MoveUp,
    Rotate;

  DBL
    DevAngleX,
    DevAngleXMin,
    DevAngleXMax,
    DevAngleZ,
    DevAngleZMin,
    DevAngleZMax;

  VEC Dir, Pos;

  BOOL IsLight;
} di6UNIT_HELICOPTER;

VOID DI6_RotateHel( di6UNIT_HELICOPTER *Uni )
{
  Uni->Helicopter.Trans = MatrMulMatr4(Uni->Helicopter.Trans, MatrTranslate(VecNeg(Uni->Pos)),
                          MatrRotate(-Uni->DevAngleX, VectorTransform(Uni->Dir, MatrRotateY(90))), MatrTranslate(Uni->Pos));

  /*Uni->Helicopter.Trans = MatrMulMatr4(Uni->Helicopter.Trans, MatrTranslate(VecNeg(Uni->Pos)),
                          MatrRotate(-Uni->DevAngleZ, Uni->Dir), MatrTranslate(Uni->Pos));

  Uni->Dir = VectorTransform(Uni->Dir, MatrRotate(Uni->DevAngleZ, Uni->Dir));*/

  Uni->DevAngleX = max(min((Uni->MoveForv.Speed * 2), Uni->DevAngleXMax), Uni->DevAngleXMin);

  Uni->DevAngleZ = max(min((Uni->Rotate.Speed * 0.25), Uni->DevAngleZMax), Uni->DevAngleZMin);

  Uni->Helicopter.Trans = MatrMulMatr4(Uni->Helicopter.Trans, MatrTranslate(VecNeg(Uni->Pos)),
                          MatrRotate(Uni->DevAngleX, VectorTransform(Uni->Dir, MatrRotateY(90))), MatrTranslate(Uni->Pos));

  /*Uni->Helicopter.Trans = MatrMulMatr4(Uni->Helicopter.Trans, MatrTranslate(VecNeg(Uni->Pos)),
                          MatrRotate(-Uni->DevAngleZ, Uni->Dir), MatrTranslate(Uni->Pos));

  Uni->Dir = VectorTransform(Uni->Dir, MatrRotate(Uni->DevAngleZ, Uni->Dir));*/
}

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

  Uni->Dir = VecSet(0, 0, -1);
  Uni->Pos = VectorTransform(VecSet(0, 50, 0), MatrScale(VecSetEqual(0.6)));

  Uni->Helicopter.Trans = MatrTranslate(Uni->Pos);

  Uni->MoveForv.Acceleration = 0;
  Uni->MoveForv.Speed = 0;

  Uni->MoveForv.AcEx = 2;

  Uni->MoveForv.AcMax = 3;
  Uni->MoveForv.SpeedMax = 10;
  Uni->MoveForv.SpeedMin = 0;

  Uni->MoveUp.Acceleration = 0;
  Uni->MoveUp.Speed = 0;

  Uni->MoveUp.AcEx = 1;

  Uni->MoveUp.AcMax = 2;
  Uni->MoveUp.SpeedMax = 6;
  Uni->MoveUp.SpeedMin = -6;

  Uni->Rotate.Acceleration = 0;
  Uni->Rotate.Speed = 0;

  Uni->Rotate.AcEx = 10;

  Uni->Rotate.AcMax = 15;
  Uni->Rotate.SpeedMax = 40;
  Uni->Rotate.SpeedMin = -40;

  Uni->DevAngleX = 0;
  Uni->DevAngleXMax = 30;
  Uni->DevAngleXMin = 0;

  Uni->DevAngleZ = 0;
  Uni->DevAngleZMax = 20;
  Uni->DevAngleZMin = -20;

  Uni->Camera.Loc = VecAddVec(VecSet(0, 10, 15), Uni->Pos);
  Uni->Camera.At = VecAddVec(VecSet(0, 0, -5), Uni->Pos);
  Uni->Camera.Up = VecSet(0, 1, 0);

  Uni->IsLight = FALSE;

  DI6_RndActiveCam(Uni->i);

  Ani->IsCameraFree = FALSE;
  Ani->LightPos = VecSetEqual(-1000);
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
  DBL shift, up, angle;
  VEC dif;
  MATR rot;

  /* Rotating propeller */
  Uni->Helicopter.Prims[7].Trans = MatrMulMatr3(MatrTranslate(VecNeg(VecSet(-0.05, 1.8, -1.2))), 
    MatrRotateY((1000 + fabs(Uni->MoveForv.Speed * 40) + fabs(Uni->MoveUp.Speed * 30) + fabs(Uni->Rotate.Speed * 3)) * DI6_Anim.Timer.Time),
                                                MatrTranslate(VecSet(-0.05, 1.8, -1.2)));
  Uni->Helicopter.Prims[9].Trans = MatrMulMatr3(MatrTranslate(VecNeg(VecSet(-0.05, 1.8, -1.2))),
    MatrRotateY((1000 + fabs(Uni->MoveForv.Speed * 40) + fabs(Uni->MoveUp.Speed * 30) + fabs(Uni->Rotate.Speed * 3)) * DI6_Anim.Timer.Time),
                                                MatrTranslate(VecSet(-0.05, 1.8, -1.2)));

  /* Control module */

  /* Three shifts - forwards, upwards and incline */
  shift = DI6_Shift(&Uni->MoveForv, DI6_Anim.Timer.DeltaTime, Ani->Keyboard.Keys['W'] - Ani->Keyboard.Keys['S'], DI6_Anim.IsCameraFree);
  up = DI6_Shift(&Uni->MoveUp, DI6_Anim.Timer.DeltaTime, Ani->Keyboard.Keys[VK_UP] - Ani->Keyboard.Keys[VK_DOWN], DI6_Anim.IsCameraFree);
  angle = DI6_Shift(&Uni->Rotate, DI6_Anim.Timer.DeltaTime, Ani->Keyboard.Keys['A'] - Ani->Keyboard.Keys['D'], DI6_Anim.IsCameraFree) / 2;

  /* Moving forwards */
  dif = VecAddVec(VecSet(0, up, 0), VecMulNum(Uni->Dir, shift));

  Uni->Helicopter.Trans = MatrMulMatr(Uni->Helicopter.Trans, MatrTranslate(dif));

  Uni->Pos = VecAddVec(Uni->Pos, dif);

  DI6_RotateHel(Uni);

  /* Incline */
  rot = MatrMulMatr3(MatrTranslate(VecNeg(Uni->Pos)), MatrRotateY(angle), MatrTranslate(Uni->Pos));

  Uni->Helicopter.Trans = MatrMulMatr(Uni->Helicopter.Trans, rot);
  Uni->Dir = PointTransform(Uni->Dir, MatrRotateY(angle));

  Uni->Camera.Loc = PointTransform(Uni->Camera.Loc, rot);
  Uni->Camera.At = PointTransform(Uni->Camera.At, rot);

  /* Camera follows the helicopter */
  DI6_RndCamMove(&Uni->Camera.Loc, &Uni->Camera.At, VecAddVec(VecSet(0, up, 0), VecMulNum(Uni->Dir, shift)));

  if (!Ani->IsCameraFree)
    DI6_RndCamSet(Uni->Camera.Loc, Uni->Camera.At, Uni->Camera.Up);

  if (((Ani->Keyboard.KeysClick['L']) && (Ani->Keyboard.Keys[VK_SHIFT])))
  {
    if (!Uni->IsLight)
      DI6_Anim.LightPos = PointTransform(VecSet(0, 0, -3), MatrTranslate(Uni->Pos));
    else
      DI6_Anim.LightPos = VecSetEqual(-1000);

    Uni->IsLight = !Uni->IsLight;
  }
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
  CHAR Buf[100];

  DI6_RndPrimsDraw(&Uni->Helicopter, MatrIdentity());

  glUseProgram(0);
  glDisable(GL_DEPTH_TEST);

  glLoadIdentity();
  glColor3d(1, 1, 1);
  glRasterPos2d(-0.98, 0.9);
  glListBase(102);
  glCallLists(sprintf(Buf, "FPS: %.3f, Height: %.3f, Speed: %.3f", Ani->Timer.FPS, Uni->Pos.y, Uni->MoveForv.Speed), GL_UNSIGNED_BYTE, Buf);

  glEnable(GL_DEPTH_TEST);
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