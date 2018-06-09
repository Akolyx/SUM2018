/* FILE NAME: RND.H
 * PROGRAMMER: DI6
 * DATE: 09.06.2018
 * PURPOSE: Definitions.
 */

#ifndef __RND_H_
#define __RND_H_

#include "../../def.h"

typedef struct tagdi6VERTEX
{
  VEC p;
} di6VERTEX;

typedef struct tagdi6PRIM
{
  di6VERTEX *V;
  INT NumOfV;
  INT *I;
  INT NumOfI;
  MATR Trans;
} di6PRIM;

/* Double buffering */
extern HWND DI6_hWndRndFrame;
extern HDC DI6_hDCRndFrame;
extern HBITMAP DI6_hBmRndFrame;
extern INT DI6_RndFrameW, DI6_RndFrameH;

VOID DI6_RndInit( HWND hWnd );
VOID DI6_RndClose( VOID );
VOID DI6_RndResize( INT w, INT h );
VOID DI6_RndStart( VOID );
VOID DI6_RndEnd( VOID );
VOID DI6_RndCopyFrame( HDC hDC );

/* Camera and projection */
extern DBL DI6_RndProjSize,
       DI6_RndProjDist,
       DI6_RndProjFarClip;

extern MATR DI6_RndMatrView,
            DI6_RndMatrProj,
            DI6_RndMatrVP;

VOID DI6_RndProjSet( VOID );
VOID DI6_RndCamSet( VEC loc, VEC at, VEC up1 );

/* Primitives */
BOOL DI6_RndPrimCreate( di6PRIM *Pr, INT NoofV, INT NoofI );
VOID DI6_RndPrimFree( di6PRIM *Pr );
VOID DI6_RndPrimDraw( di6PRIM *Pr, MATR World );
BOOL DI6_RndPrimLoad( di6PRIM *Pr, CHAR *Filename);

#endif /* __RND_H_ */

/* End of 'RND.H' file */