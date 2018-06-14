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
  DI6_RndMatrVP = MatrMulMatr(DI6_RndMatrView, DI6_RndMatrProj);
} /* End of 'DI6_RndCamset' function */