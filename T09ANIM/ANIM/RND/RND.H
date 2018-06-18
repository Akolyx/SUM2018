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

  VEC min;
  VEC max;

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
BOOL DI6_RndPrimCreate( di6PRIM *Pr, di6PRIM_TYPE Type, di6VERTEX *V, INT NoofV, INT *I, INT NoofI, INT NumInArray, VEC min, VEC max );
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

#define DI6_MAX_MATERIALS 300

typedef struct tagdi6MATERIAL
{
  CHAR Name[DI6_STR_MAX]; /* Material name */

  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;         /* Ambient, diffuse, specular coefficients */
  FLT Ph;                 /* Phong power coefficient */
  FLT Trans;              /* Transparency factor */
  INT Tex[8];             /* Texture references from texture table */

  /* Shader string */
  CHAR ShaderStr[DI6_STR_MAX];

  INT ShdNo;              /* Shader number in shader array */
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

/***
* Image handle
***/

/* Image representation type */
typedef struct tagdi6IMAGE
{
  INT W, H;    /* Image size in pixels */
  DWORD *Bits; /* Image pixels: pixel(X,Y) = Bits[Y * W + X] */

  HBITMAP hBm; /* WinAPI bitmap handle */
} di6IMAGE;

/* Load image from BMP file function.
 * ARGUMENTS:
 *   - image structure to be load to:
 *       di6IMAGE *Img;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DI6_RndImgLoad( di6IMAGE *Img, CHAR *FileName );

/* Free image function.
 * ARGUMENTS:
 *   - image structure to be load to:
 *       di6IMAGE *Img;
 * RETURNS: None.
 */
VOID DI6_RndImgFree( di6IMAGE *Img );

/* Upload image to OpenGL texture memory function.
 * ARGUMENTS:
 *   - image structure to be load to:
 *       di6IMAGE *Img;
 * RETURNS: None.
 */
VOID DI6_RndImgToTex( di6IMAGE *Img );

/* Otain image pixel color function.
 * ARGUMENTS:
 *   - image structure to be load to:
 *       di6IMAGE *Img;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS: None.
 */
DWORD DI6_RndImgGet( di6IMAGE *Img, INT X, INT Y );

/***
 * Texture handle
 ***/

/* Texture representation type */
typedef struct tagdi6TEXTURE
{
  CHAR Name[DI6_STR_MAX]; /* Texture name */
  INT W, H;                          /* Texture size in pixels */
  UINT TexId;                        /* OpenGL texture Id */ 
} di6TEXTURE;

/* Texture stock maximum size */
#define DI6_MAX_TEXTURES 30
/* Array of textures */
extern di6TEXTURE DI6_RndTextures[DI6_MAX_TEXTURES];
/* Shadres array store size */
extern INT DI6_RndTexturesSize;

/* Texture storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DI6_RndTexInit( VOID );

/* Texture storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DI6_RndTexClose( VOID );

/* Add texture from image to stock from file function.
 * ARGUMENTS:
 *   - image name:
 *       CHAR *Name;
 *   - image size:
 *       INT W, H;
 *   - image pixels data:
 *       DWORD *Bits;
 * RETURNS:
 *   (INT) new texture stock number.
 */
INT DI6_RndTexAddImg( CHAR *Name, INT W, INT H, DWORD *Bits );

/* Add texture to stock from file function.
 * ARGUMENTS:
 *   - texture file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) new texture stock number (-1 if error occured).
 */
INT DI6_RndTexAdd( CHAR *FileName );

#endif /* __RND_H_ */

/* End of 'RND.H' file */