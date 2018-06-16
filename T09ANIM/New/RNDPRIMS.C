/* FILE NAME   : RNDPRIMS.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Primitives array support.
 * PROGRAMMER  : Vitaly A. Galinsky.
 * LAST UPDATE : 15.06.2018.
 * NOTE        : Module prefix 'VG4'.
 */

#include <stdio.h>
#include <string.h>

#include "../anim.h"

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       vg4PRIMS *Prs;
 *   - number of primitives:
 *       INT NoofP;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL VG4_RndPrimsCreate( vg4PRIMS *Prs, INT NoofP )
{
  memset(Prs, 0, sizeof(vg4PRIMS));
  if ((Prs->Prims = malloc(sizeof(vg4PRIM) * NoofP)) == NULL)
    return FALSE;
  memset(Prs->Prims, 0, sizeof(vg4PRIM) * NoofP);
  Prs->NumOfPrims = NoofP;
  Prs->Trans = MatrIdentity();
  return TRUE;
} /* End of 'VG4_RndPrimsCreate' function */  

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       vg4PRIMS *Prs;
 * RETURNS: None.
 */
VOID VG4_RndPrimsFree( vg4PRIMS *Prs )
{
  if (Prs->Prims != NULL)
    free(Prs->Prims);
  memset(Prs, 0, sizeof(vg4PRIMS));
} /* End of 'VG4_RndPrimsFree' function */  

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       vg4PRIMS *Prs;
 *   - world transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID VG4_RndPrimsDraw( vg4PRIMS *Prs, MATR World )
{
  INT i;

  World = MatrMulMatr(Prs->Trans, World);
  for (i = 0; i < Prs->NumOfPrims; i++)
    VG4_RndPrimDraw(&Prs->Prims[i], World);
} /* End of 'VG4_RndPrimsDraw' function */  

/* Load array of primitives from G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       vg4PRIMS *Prs;
 *   - filename to load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL VG4_RndPrimsLoad( vg4PRIMS *Prs, CHAR *FileName )
{
  FILE *F;
  INT flen, NoofP, NoofM, NoofT, p, m, t, first_mtl_no, first_tex_no;
  DWORD Sign;
  BYTE *mem, *ptr;

  memset(Prs, 0, sizeof(vg4PRIMS));

  /* Load the whole file into memory */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
  fseek(F, 0, SEEK_END);
  flen = ftell(F); /* measure file length */
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
  if (!VG4_RndPrimsCreate(Prs, NoofP))
  {
    free(mem);
    return FALSE;
  }

  /* Read all primitives */
  for (p = 0; p < NoofP; p++)
  {
    /* Read one primitive */
    INT NoofV, NoofI, MtlNo;
    vg4VERTEX *V;
    INT *I;

    NoofV = *(INT *)ptr;
    ptr += 4;
    NoofI = *(INT *)ptr;
    ptr += 4;
    MtlNo = *(INT *)ptr;
    ptr += 4;

    V = (vg4VERTEX *)ptr;
    ptr += sizeof(vg4VERTEX) * NoofV;
    I = (INT *)ptr;
    ptr += sizeof(INT) * NoofI;

    /* Add a new primitive */
    VG4_RndPrimCreate(&Prs->Prims[p], VG4_RND_PRIM_TRIMESH,
      V, NoofV, I, NoofI);
  }

  /* Store first material number */
  first_mtl_no = VG4_RndMaterialsSize;

  /* Read all materials */
  for (m = 0; m < NoofM; m++)
  {
    /* Read one material */
    VG4_RndMtlAdd((vg4MATERIAL *)ptr);
    ptr += sizeof(vg4MATERIAL);
  }

  /* Update material numbers in primitives */
  for (p = 0; p < NoofP; p++)
    Prs->Prims[p].MtlNo += first_mtl_no;

  /* Store first texture number */
  first_tex_no = VG4_RndTexturesSize;

  /* Read all textures */
  for (t = 0; t < NoofT; t++)
  {
    /* Read one texture */
    vg4TEXTURE *tex = (vg4TEXTURE *)ptr;

    ptr += sizeof(vg4TEXTURE);
    VG4_RndTexAddImg(tex->Name, tex->W, tex->H, (DWORD *)ptr);
    ptr += 4 * tex->W * tex->H;
  }

  /* Update texture numbers in materials */
  for (m = 0; m < NoofM; m++)
  {
    INT i;
    vg4MATERIAL *mtl = &VG4_RndMaterials[first_mtl_no + m];
    
    for (i = 0; i < 8; i++)
      if (mtl->Tex[i] != -1)
        mtl->Tex[i] += first_tex_no;
  }

  free(mem);
  return TRUE;
} /* End of 'VG4_RndPrimsLoad' function */  

/* END OF 'RNDPRIMS.C' FILE */
