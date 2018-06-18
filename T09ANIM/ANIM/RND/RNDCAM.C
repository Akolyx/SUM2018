/* FILE NAME: RNDCAM.C
 * PROGRAMMER: DI6
 * DATE: 09.06.2018
 * PURPOSE: Functions to work with camera and projection.
 */

#include "../anim.h"

FLT DI6_RndProjSize = 1, DI6_RndProjDist = 1, DI6_RndProjFarClip = 300;

MATR DI6_RndMatrView, DI6_RndMatrProj, DI6_RndMatrVP;

VOID DI6_RndProjSet( VOID )
{
  FLT ratio_x, ratio_y;

  ratio_x = ratio_y = DI6_RndProjSize / 2;
  if (DI6_Anim.w > DI6_Anim.h) 
    ratio_x *= (FLT)DI6_Anim.w / DI6_Anim.h;
  else
    ratio_y *= (FLT)DI6_Anim.h / DI6_Anim.w;

  DI6_RndMatrProj = MatrFrustum(-ratio_x, ratio_x, -ratio_y, ratio_y, DI6_RndProjDist, DI6_RndProjFarClip);
  DI6_RndMatrVP = MatrMulMatr(DI6_RndMatrView, DI6_RndMatrProj);
} /* End of 'DI6_RndProject' function */

VOID DI6_RndCamSet( VEC loc, VEC at, VEC up1 )
{
  DI6_RndMatrView = MatrView(loc, at, up1);

  DI6_Anim.Camera.Loc = loc;
  DI6_Anim.Camera.At = at;
  DI6_Anim.Camera.Dir = VecSet(-DI6_RndMatrView.M[0][2], -DI6_RndMatrView.M[1][2], -DI6_RndMatrView.M[2][2]);
  DI6_Anim.Camera.Right = VecSet(-DI6_RndMatrView.M[0][0], -DI6_RndMatrView.M[1][0], -DI6_RndMatrView.M[2][0]);
  DI6_Anim.Camera.Up = VecSet(-DI6_RndMatrView.M[0][1], -DI6_RndMatrView.M[1][1], -DI6_RndMatrView.M[2][1]);

  DI6_RndMatrVP = MatrMulMatr(DI6_RndMatrView, DI6_RndMatrProj);
} /* End of 'DI6_RndCamset' function */