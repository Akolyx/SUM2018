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
  INT MtlNo;

  INT NumInArray;

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
BOOL DI6_RndPrimCreate( di6PRIM *Pr, di6PRIM_TYPE Type, di6VERTEX *V, INT NoofV, INT *I, INT NoofI, INT NumInArray );
VOID DI6_RndPrimFree( di6PRIM *Pr );
VOID DI6_RndPrimDraw( di6PRIM *Pr, MATR World );
BOOL DI6_RndPrimLoad( di6PRIM *Pr, CHAR *Filename);

/* Pseudorandon numbers functions */
FLT DI6_Rand0_1( VOID );
FLT DI6_Rand_1_1( VOID );

/***
** Shader module
***/

/* Shader program store type */

#define DI6_STR_MAX 300
#define DI6_MAX_SHADERS 30

typedef struct tagdi6SHADER
{
  CHAR Name[DI6_STR_MAX]; /* Shader filename prefix */
  INT ProgId;             /* Shader program Id */
} di6SHADER;

/* Load shader program from .GLSL files function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) load shader program Id.
 */
INT DI6_RndShdLoad( CHAR *ShaderFileNamePrefix );

/* Unload shader program from memory files function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETURNS: None.
 */
VOID DI6_RndShdDelete( INT ProgId );

/* Array of shaders */
extern di6SHADER DI6_RndShaders[DI6_MAX_SHADERS];

/* Shaders array store size */
extern INT DI6_RndShadersSize;

/* Shader storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DI6_RndShdInit( VOID );

/* Shader storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DI6_RndShdClose( VOID );

/* Add shader to stock from file function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) new shader stock number.
 */
INT DI6_RndShdAdd( CHAR *ShaderFileNamePrefix );

/* Update from file all load shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DI6_RndShdUpdate( VOID );

/***
** Materials module
***/

#define DI6_MAX_MATERIALS 30

typedef struct tagdi6MATERIAL
{
  CHAR Name[DI6_STR_MAX]; /* Material name */
  INT ShdNo;              /* Shader number in shader array */

  /* to be continue */
} di6MATERIAL;

/* Array of materials */
extern di6MATERIAL DI6_RndMaterials[DI6_MAX_MATERIALS];

/* Materials array store size */
extern INT DI6_RndMaterialsSize;

/* Material storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DI6_RndMtlInit( VOID );

/* Add material to stock function.
 * ARGUMENTS:
 *   - pointer to material data:
 *       di6MATERIAL *Mtl;
 * RETURNS:
 *   (INT) 
 ew material stock number.
 */
INT DI6_RndMtlAdd( di6MATERIAL *Mtl );

/* Apply material before rendering function.
 * ARGUMENTS:
 *   - material number in materials storage array:
 *       INT MtlNo;
 * RETURNS:
 *   (INT) applyed material shader program Id.
 */
INT DI6_RndMtlApply( INT MtlNo );

/***
 * Array of primitives handle
 ***/

/* Array of primitives representation type */
typedef struct tagdi6PRIMS
{
  di6PRIM *Prims; /* Primitive array */
  INT NumOfPrims; /* Primitive array size */
  MATR Trans;     /* All primitives transformation matrix */
} di6PRIMS;

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       di6PRIMS *Prs;
 *   - number of primitives:
 *       INT NoofP;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DI6_RndPrimsCreate( di6PRIMS *Prs, INT NoofP );

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       di6PRIMS *Prs;
 * RETURNS: None.
 */
VOID DI6_RndPrimsFree( di6PRIMS *Prs );

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       di6PRIMS *Prs;
 *   - world transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID DI6_RndPrimsDraw( di6PRIMS *Prs, MATR World );

/* Load array of primitives from G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       di6PRIMS *Prs;
 *   - filename to load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DI6_RndPrimsLoad( di6PRIMS *Prs, CHAR *FileName );

#endif /* __RND_H_ */

/* End of 'RND.H' file */