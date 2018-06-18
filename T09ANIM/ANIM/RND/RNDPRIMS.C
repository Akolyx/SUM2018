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

  World = MatrMulMatr(World, Prs->Trans);

  /* Draw non-transparent primitives */
  for (i = 0; i < Prs->NumOfPrims; i++)
  {
    if (Prs->Prims[i].MtlNo > 0 && Prs->Prims[i].MtlNo < DI6_RndMaterialsSize && DI6_RndMaterials[Prs->Prims[i].MtlNo].Trans == 1)
      DI6_RndPrimDraw(&Prs->Prims[i], World);
  }

  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (i = 0; i < Prs->NumOfPrims; i++)
  {
    if (Prs->Prims[i].MtlNo > 0 && Prs->Prims[i].MtlNo < DI6_RndMaterialsSize && DI6_RndMaterials[Prs->Prims[i].MtlNo].Trans < 1)
      DI6_RndPrimDraw(&Prs->Prims[i], World);
  }

  glCullFace(GL_BACK);
  for (i = 0; i < Prs->NumOfPrims; i++)
  {
    if (Prs->Prims[i].MtlNo > 0 && Prs->Prims[i].MtlNo < DI6_RndMaterialsSize && DI6_RndMaterials[Prs->Prims[i].MtlNo].Trans < 1)
      DI6_RndPrimDraw(&Prs->Prims[i], World);
  }

  glDisable(GL_CULL_FACE);
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
  INT flen, NoofP, NoofM, NoofT,
      v, p, m, t,
      first_mtl_no, first_tex_no;

  DBL minx = 1000, miny = 1000, minz = 1000;
  DBL maxx = -1000, maxy = -1000, maxz = -1000;

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

    for (v = 0; v < NoofV; v++)
    {
      V = (di6VERTEX *)ptr;

      minx = V->p.x < minx ? V->p.x : minx;
      miny = V->p.y < miny ? V->p.y : miny;
      minz = V->p.z < minz ? V->p.z : minz;
      maxx = V->p.x > maxx ? V->p.x : maxx;
      maxy = V->p.y > maxy ? V->p.y : maxy;
      maxz = V->p.z > maxz ? V->p.z : maxz;

      ptr += sizeof(di6VERTEX);
    }

    ptr -= sizeof(di6VERTEX) * NoofV;

    V = (di6VERTEX *)ptr;
    ptr += sizeof(di6VERTEX) * NoofV;

    I = (INT *)ptr;
    ptr += sizeof(INT) * NoofI;

    /* Add a new primitive */
    DI6_RndPrimCreate(&Prs->Prims[p], DI6_RND_PRIM_TRIMESH, V, NoofV, I, NoofI, p, VecSet(minx, miny, minz), VecSet(maxx, maxy, maxz));
    Prs->Prims[p].MtlNo = MtlNo;
  }

  /* Store first material number */
  first_mtl_no = DI6_RndMaterialsSize;


  /* Read all materials */
  for (m = 0; m < NoofM; m++)
  {
    /* Read one material */
    DI6_RndMtlAdd((di6MATERIAL *)ptr);
    ptr += sizeof(di6MATERIAL);
  }

  /* Update material numbers in primitives */
  for (p = 0; p < NoofP; p++)
    Prs->Prims[p].MtlNo += first_mtl_no;

  /* Store first texture number */
  first_tex_no = DI6_RndTexturesSize;

  /* Read all textures */
  for (t = 0; t < NoofT; t++)
  {
    /* Read one texture */
    di6TEXTURE *tex = (di6TEXTURE *)ptr;

    ptr += sizeof(di6TEXTURE);
    DI6_RndTexAddImg(tex->Name, tex->W, tex->H, (DWORD *)ptr);
    ptr += 4 * tex->W * tex->H;
  }

  /* Update texture numbers in materials */
  for (m = 0; m < NoofM; m++)
  {
    INT i;
    
    if (first_mtl_no + m < DI6_MAX_MATERIALS)
    {
      di6MATERIAL *mtl = &DI6_RndMaterials[first_mtl_no + m];
    
      for (i = 0; i < 8; i++)
        if (mtl->Tex[i] != -1)
        {
          mtl->Tex[i] += first_tex_no;
          //mtl->Tex[i] = -1;
        }
    }
  }

  free(mem);
  return TRUE;
} /* End of 'DI6_RndPrimsLoad' function */  

/* END OF 'RNDPRIMS.C' FILE */
