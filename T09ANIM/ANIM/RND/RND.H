/* FILE NAME: RND.H
 * PROGRAMMER: DI6
 * DATE: 09.06.2018
 * PURPOSE: Definitions.
 */

#ifndef __RND_H_
#define __RND_H_

#include "../../def.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* Structure representing a vertex with all attributes */
typedef struct tagdi6VERTEX
{
  VEC p;  /* Vertex position */
  VEC2 t; /* Vertex texture coordinates */
  VEC n;  /* Vertex normal */
  VEC4 c; /* Vertex color */
} di6VERTEX;

/* Primitive type */
typedef enum tagdi6PRIM_TYPE
{
  DI6_RND_PRIM_TRIMESH, DI6_RND_PRIM_GRID
} di6PRIM_TYPE;

typedef struct tagdi6PRIM
{
  di6PRIM_TYPE Type;
  INT 
    VA,
    VBuf,
    IBuf;

  INT NumOfI;

  MATR Trans;
} di6PRIM;

/* Double buffering */

VOID DI6_RndInit( HWND hWnd );
VOID DI6_RndClose( VOID );
VOID DI6_RndResize( INT w, INT h );
VOID DI6_RndStart( VOID );
VOID DI6_RndEnd( VOID );
VOID DI6_RndCopyFrame( VOID );

/* Camera and projection */
extern FLT DI6_RndProjSize,
       DI6_RndProjDist,
       DI6_RndProjFarClip;

extern MATR DI6_RndMatrView,
            DI6_RndMatrProj,
            DI6_RndMatrVP;

VOID DI6_RndProjSet( VOID );
VOID DI6_RndCamSet( VEC loc, VEC at, VEC up1 );

/* Primitives */
BOOL DI6_RndPrimCreate( di6PRIM *Pr, di6PRIM_TYPE Type, di6VERTEX *V, INT NoofV, INT *I, INT NoofI );
VOID DI6_RndPrimFree( di6PRIM *Pr );
VOID DI6_RndPrimDraw( di6PRIM *Pr, MATR World );
BOOL DI6_RndPrimLoad( di6PRIM *Pr, CHAR *Filename);

/* Pseudorandon numbers functions */
FLT DI6_Rand0_1( VOID );
FLT DI6_Rand_1_1( VOID );

#endif /* __RND_H_ */

/* End of 'RND.H' file */