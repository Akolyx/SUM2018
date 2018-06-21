/* FILE NAME   : RNDGRID.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Grid topology handle.
 * LAST UPDATE : 19.06.2018.
 */

#include <string.h>

#include "../anim.h"

/* Convert grid to primitive function.
 * ARGUMENTS:
 *   - primitive to be constructed:
 *       di6PRIM *Pr;
 *   - source grid:
 *       di6GRID *G;
 * RETURNS: None.
 */
VOID DI6_RndPrimFromGrid( di6PRIM *Pr, di6GRID *G )
{
  INT i, j, k, v, *Ind;

  memset(Pr, 0, sizeof(di6PRIM));

  /* Index array allocation */
  if ((Ind = malloc(((G->H - 1) * (G->W * 2 + 1) - 1) * sizeof(INT))) == NULL)
    return;
  /* Index array fill */
  for (i = 0, k = 0, v = 0; i < G->H - 1; i++)
  {
    for (j = 0; j < G->W; j++)
    {
      Ind[k++] = v + G->W;
      Ind[k++] = v++;
    }
    if (i < G->H - 2)
      Ind[k++] = -1;
  }
  /* Create primitive */
  DI6_RndPrimCreate(Pr, DI6_RND_PRIM_GRID, G->V, G->W * G->H, Ind, (G->H - 1) * (G->W * 2 + 1) - 1, 0, VecSetEqual(0), VecSetEqual(0)); 
  free(Ind);
} /* End of 'DI6_RndPrimfFromGrid' function */

/* Grid creation function.
 * ARGUMENTS:
 *   - grid to be create:
 *       di6GRID *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DI6_RndGridCreate( di6GRID *G, INT W, INT H )
{
  INT i, j, v;

  memset(G, 0, sizeof(di6GRID));
  if ((G->V = malloc(W * H * sizeof(di6VERTEX))) == NULL)
    return FALSE;
  /* Setup all vertices */
  for (i = 0, v = 0; i < H; i++)
    for (j = 0; j < W; j++, v++)
    {
      G->V[v].p = VecSet(i, 0, j);
      G->V[v].t = Vec2Set(j / (W - 1.0), i / (H - 1.0));
      G->V[v].n = VecSet(0, 1, 0);
      G->V[v].c = Vec4Set(1, 1, 1, 1);
    }
  G->W = W;
  G->H = H;
  return TRUE;
} /* End of 'DI6_RndGridCreate' function */

/* Grid free function.
 * ARGUMENTS:
 *   - grid to free:
 *       di6GRID *G;
 * RETURNS: None.
 */
VOID DI6_RndGridFree( di6GRID *G )
{
  if (G->V != NULL)
    free(G->V);
  memset(G, 0, sizeof(di6GRID));
} /* End of 'DI6_RndGridFree' function */

/* Grid normal evaluation function.
 * ARGUMENTS:
 *   - grid for recalculate normals:
 *       di6GRID *G;
 * RETURNS: None.
 */
VOID DI6_RndPrimGridEvalNormals( di6GRID *G )
{
  INT i, j, k;

  /* Set all vertex normals to (0, 0, 0) */
  for (i = 0, k = 0; i < G->H; i++)
    for (j = 0; j < G->W; j++)
      G->V[k++].n = VecSet(0, 0, 0);

  /* Evaluate all triangles normals */
  for (i = 0, k = 0; i < G->H - 1; i++)
    for (j = 0; j < G->W - 1; j++, k++)
    {
      di6VERTEX
        *p00 = &G->V[k],
        *p01 = &G->V[k + 1],
        *p10 = &G->V[k + G->W],
        *p11 = &G->V[k + G->W + 1];
      VEC N;

      N = VecNormalize(VecCrossVec(VecSubVec(p00->p, p10->p), VecSubVec(p11->p, p10->p)));
      p00->n = VecAddVec(p00->n, N);
      p10->n = VecAddVec(p10->n, N);
      p11->n = VecAddVec(p11->n, N);

      N = VecNormalize(VecCrossVec(VecSubVec(p11->p, p01->p), VecSubVec(p00->p, p01->p)));
      p00->n = VecAddVec(p00->n, N);
      p01->n = VecAddVec(p01->n, N);
      p11->n = VecAddVec(p11->n, N);
    }

  /* Normalize all vertex normals */
  for (i = 0; i < G->H * G->W; i++)
    G->V[i].n = VecNormalize(G->V[i].n);
} /* End of 'DI6_RndPrimGridEvalNormals' function */

/* Sphere grid creation function.
 * ARGUMENTS:
 *   - grid to create:
 *       di6GRID *G;
 *   - grid size:
 *       INT W, H;
 *   - sphere center:
 *       VEC C;
 *   - sphere radius:
 *       FLT R;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DI6_RndGridCreateSphere( di6GRID *G, INT W, INT H, VEC C, FLT R )
{
  INT i, j, v;

  if (!DI6_RndGridCreate(G, W, H))
    return FALSE;
  /* Setup all vertices */
  for (i = 0, v = 0; i < H; i++)
  {
    DBL theta = (H - 1 - i) * PI / (H - 1), sit = sin(theta), cot = cos(theta);

    for (j = 0; j < W; j++, v++)
    {
      DBL
        phi = j * 2 * PI / (W - 1), sip = sin(phi), cop = cos(phi),
        x = sit * sip,
        y = cot,
        z = sit * cop;

      G->V[v].p = VecSet(C.x + x * R, C.y + y * R, C.z + z * R);
      G->V[v].n = VecSet(x, y, z);
    }
  }
  return TRUE;
} /* End of 'DI6_RndGridCreateSphere' function */

/* Plane grid creation function.
 * ARGUMENTS:
 *   - grid to create:
 *       di6GRID *G;
 *   - grid size:
 *       INT W, H;
 *   - plane corner and directional vectors:
 *       VEC P, Du, Dv;
 *   - plane texture scale factors:
 *       FLT Tx, Ty;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DI6_RndGridCreatePlane( di6GRID *G, INT W, INT H, VEC P, VEC Du, VEC Dv, FLT Tx, FLT Ty )
{
  INT i, j, v;
  VEC N;

  if (!DI6_RndGridCreate(G, W, H))
    return FALSE;

  N = VecNormalize(VecCrossVec(Du, Dv));

  /* Setup all vertices */
  for (i = 0, v = 0; i < H; i++)
  {
    DBL sy = i / (H - 1.0);

    for (j = 0; j < W; j++, v++)
    {
      DBL sx = j / (W - 1.0);

      G->V[v].p = VecAddVec(P,
        VecAddVec(VecMulNum(Du, sx), VecMulNum(Dv, sy)));
      G->V[v].n = N;
      G->V[v].t.x *= Tx;
      G->V[v].t.y *= Ty;
    }
  }
  return TRUE;
} /* End of 'DI6_RndGridCreatePlane' function */

/* Height field grid creation function.
 * ARGUMENTS:
 *   - grid to create:
 *       di6GRID *G;
 *   - height field image file name:
 *       CHAR *HeightFieldFileName;
 *   - plane corner and directional vectors:
 *       VEC P, Du, Dv;
 *   - height scale factor:
 *       FLT Scale;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DI6_RndGridCreateHeightField( di6GRID *G, CHAR *HeightFieldFileName, VEC P, VEC Du, VEC Dv, FLT Scale )
{
  INT i, j, v;
  di6IMAGE Img;
  VEC N;

  memset(G, 0, sizeof(di6GRID));
  if (!DI6_RndImgLoad(&Img, HeightFieldFileName))
    return FALSE;

  if (!DI6_RndGridCreatePlane(G, Img.W, Img.H, P, Du, Dv, 1, 1))
  {
    DI6_RndImgFree(&Img);
    return FALSE;
  }

  N = G->V[0].n;

  /* Setup all vertices */
  for (i = 0, v = 0; i < G->H; i++)
  {
    for (j = 0; j < G->W; j++, v++)
    {
      DWORD col = DI6_RndImgGet(&Img, j, i);

      G->V[v].p = VecAddVec(G->V[v].p,
        VecMulNum(N, (((col >> 8) & 0xFF) / 255.0 * 2 - 1) * Scale));
    }
  }
  DI6_RndImgFree(&Img);
  DI6_RndPrimGridEvalNormals(G);
  return TRUE;
} /* End of 'DI6_RndGridCreateHeightField' function */

/* END OF 'RNDGRID.C' FILE */
