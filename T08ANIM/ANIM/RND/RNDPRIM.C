/* FILE NAME: RNDPRIM.C
 * PROGRAMMER: DI6
 * DATE: 09.06.2018
 * PURPOSE: Stuff to work with primitives.
 */

#include "rnd.h"
#include <stdio.h>

BOOL DI6_RndPrimCreate( di6PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  memset(Pr, 0, sizeof(di6PRIM));

  size = sizeof(di6VERTEX) * NoofV + sizeof(INT) * NoofI;

  Pr->V = malloc(size);
  if (Pr->V == NULL)
    return FALSE;

  memset(Pr->V, 0, size);

  Pr->I = (INT *)(Pr->V + NoofV);

  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
 
  Pr->Trans = MatrIdentity();

  return TRUE;
} /* End of 'DI6_RndPrimCreate' function */

VOID DI6_RndPrimFree( di6PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);

  memset(Pr, 0, sizeof(di6PRIM));
} /* End of 'DI6_RndPrimFree' function */

VOID DI6_RndPrimDraw( di6PRIM *Pr, MATR World )
{
  INT i;
  POINT *pnts;
  MATR M = MatrMulMatr(MatrMulMatr(Pr->Trans, World), DI6_RndMatrVP);

  pnts = malloc(sizeof(POINT) * Pr->NumOfV);
  if (pnts == NULL)
    return;

  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr4x4(Pr->V[i].p, M);

    pnts[i].x = (LONG)((p.x + 1) * DI6_RndFrameW / 2);
    pnts[i].y = (LONG)((-p.y + 1) * DI6_RndFrameH / 2);
  }

  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    POINT p[3];

    p[0] = pnts[Pr->I[i]];
    p[1] = pnts[Pr->I[i + 1]];
    p[2] = pnts[Pr->I[i + 2]];
    Polygon(DI6_hDCRndFrame, p, 3);
  }

  free(pnts);
} /* End of 'DI6_RndPrimDraw' function */

BOOL DI6_RndPrimLoad( di6PRIM *Pr, CHAR *Filename)
{
  INT nv, nf;
  FILE *F;
  CHAR Buf[1000];

  memset(Pr, 0, sizeof(di6PRIM));

  if ((F = fopen(Filename, "r")) == NULL)
    return FALSE;

  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;

  if (!DI6_RndPrimCreate(Pr, nv, nf))
  {
    fclose(F);
    return FALSE;
  }

  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Pr->V[nv++].p = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
      sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
      sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
      sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3);
      Pr->I[nf++] = n1 - 1;
      Pr->I[nf++] = n2 - 1;
      Pr->I[nf++] = n3 - 1;
    }

  fclose(F);

  return TRUE;
} /* End of 'DI6_RndPrimLoad' function */

/* End of 'RNDPRIM.C' file */