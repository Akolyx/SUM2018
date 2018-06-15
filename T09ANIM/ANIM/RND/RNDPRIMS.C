/* FILE NAME   : RNDPRIMS.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Primitives array support.
 * LAST UPDATE : 15.06.2018.
 */

#include <stdio.h>
#include <string.h>

#include "../anim.h"

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       di6PRIMS *Prs;
 *   - number of primitives:
 *       INT NoofP;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DI6_RndPrimsCreate( di6PRIMS *Prs, INT NoofP )
{
  memset(Prs, 0, sizeof(di6PRIMS));

  if ((Prs->Prims = malloc(sizeof(di6PRIM) * NoofP)) == NULL)
    return FALSE;

  memset(Prs->Prims, 0, sizeof(di6PRIM) * NoofP);

  Prs->NumOfPrims = NoofP;

  Prs->Trans = MatrIdentity();

  return TRUE;
} /* End of 'DI6_RndPrimsCreate' function */  

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       di6PRIMS *Prs;
 * RETURNS: None.
 */
VOID DI6_RndPrimsFree( di6PRIMS *Prs )
{
  if (Prs->Prims != NULL)
    free(Prs->Prims);

  memset(Prs, 0, sizeof(di6PRIMS));
} /* End of 'DI6_RndPrimsFree' function */  

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       di6PRIMS *Prs;
 *   - world transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID DI6_RndPrimsDraw( di6PRIMS *Prs, MATR World )
{
  INT i;

  World = MatrMulMatr(Prs->Trans, World);

  for (i = 0; i < Prs->NumOfPrims; i++)
    DI6_RndPrimDraw(&Prs->Prims[i], World);
} /* End of 'DI6_RndPrimsDraw' function */  

/* Load array of primitives from G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       di6PRIMS *Prs;
 *   - filename to load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DI6_RndPrimsLoad( di6PRIMS *Prs, CHAR *FileName )
{
  FILE *F;
  INT flen, NoofP, NoofM, NoofT, p;
  DWORD Sign;
  BYTE *mem, *ptr;

  memset(Prs, 0, sizeof(di6PRIMS));

  /* Load the whole file into memory */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  /* Measure file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F); 

  rewind(F);

  /* Allocate memory */
  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }

  /* Read data */
  fread(mem, 1, flen, F);
  fclose(F);

  /* Setup file pointer to file data start */
  ptr = mem;

  /* Read signature */
  Sign = *(DWORD *)ptr;
  ptr += 4;

  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    return FALSE;
  }

  /* Read sizes */
  NoofP = *(INT *)ptr;
  ptr += 4;

  NoofM = *(INT *)ptr;
  ptr += 4;

  NoofT = *(INT *)ptr;
  ptr += 4;

  /* Create primitives array */
  if (!DI6_RndPrimsCreate(Prs, NoofP))
  {
    free(mem);
    return FALSE;
  }

  /* Read all primitives */
  for (p = 0; p < NoofP; p++)
  {
    /* Read one primitive */
    INT NoofV, NoofI, MtlNo;
    di6VERTEX *V;
    INT *I;

    NoofV = *(INT *)ptr;
    ptr += 4;

    NoofI = *(INT *)ptr;
    ptr += 4;

    MtlNo = *(INT *)ptr;
    ptr += 4;

    V = (di6VERTEX *)ptr;
    ptr += sizeof(di6VERTEX) * NoofV;

    I = (INT *)ptr;
    ptr += sizeof(INT) * NoofI;

    /* Add a new primitive */
    DI6_RndPrimCreate(&Prs->Prims[p], DI6_RND_PRIM_TRIMESH, V, NoofV, I, NoofI, p);
  }

  free(mem);
  return TRUE;
} /* End of 'DI6_RndPrimsLoad' function */  

/* END OF 'RNDPRIMS.C' FILE */
