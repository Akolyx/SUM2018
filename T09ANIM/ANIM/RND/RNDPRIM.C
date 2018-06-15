/* FILE NAME: RNDPRIM.C
 * PROGRAMMER: DI6
 * DATE: 09.06.2018
 * PURPOSE: Stuff to work with primitives.
 */

#include "../anim.h"
#include <stdio.h>
/* Create primitive by vertex and index sizes function.
 * ARGUMENTS:
 *   - primitive to create:
 *       di6PRIM *Pr;
 *   - primitive type:
 *       di6PRIM_TYPE Type;
 *   - vertex array:
 *       di6VERTEX *V;
 *   - vertex array size:
 *       INT NoofV;
 *   - index array:
 *       INT *I;
 *   - index array size:
 *       INT NoofI;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DI6_RndPrimCreate( di6PRIM *Pr, di6PRIM_TYPE Type, di6VERTEX *V, INT NoofV, INT *I, INT NoofI, INT NumInArray )
{
  memset(Pr, 0, sizeof(di6PRIM));

  glGenBuffers(1, &Pr->VBuf);
  glGenVertexArrays(1, &Pr->VA);

  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(di6VERTEX) * NoofV, V, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(di6VERTEX), (VOID *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(di6VERTEX), (VOID *)sizeof(VEC));
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(di6VERTEX), (VOID *)(sizeof(VEC) + sizeof(VEC2)));
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(di6VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);

  if (NoofI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, I, GL_STATIC_DRAW);
    Pr->NumOfI = NoofI;
  }
  else
    Pr->NumOfI = NoofV;

  Pr->NumInArray = NumInArray;

  Pr->Type = Type;

  Pr->Trans = MatrIdentity();

  return TRUE;
} /* End of 'DI6_RndPrimCreate' function */

/* Free primitive function.
 * ARGUMENTS:
 *   - primitive to free up:
 *       di6PRIM *Pr;
 * RETURNS: None.
 */
VOID DI6_RndPrimFree( di6PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);

  if (Pr->IBuf != 0)
    glDeleteBuffers(1, &Pr->IBuf);

  memset(Pr, 0, sizeof(di6PRIM));
} /* End of 'DI6_RndPrimFree' function */

/* Draw primitive function.
 * ARGUMENTS:
 *   - primitive to draw:
 *       di6PRIM *Pr;
 *   - world transformation matrix:
 *       MATR World
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
VOID DI6_RndPrimDraw( di6PRIM *Pr, MATR World )
{
  INT gl_prim_type, prg;

  INT loc;

  //glLoadMatrixf(WVP.M[0]);

  prg = DI6_RndMtlApply(Pr->MtlNo);

  glUseProgram(prg);

  if ((loc = glGetUniformLocation(prg, "MatrVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DI6_RndMatrVP.M[0]);
  if ((loc = glGetUniformLocation(prg, "MatrWorld")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, World.M[0]);
  if ((loc = glGetUniformLocation(prg, "Num")) != -1)
    glUniform1i(loc, Pr->NumInArray);
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, DI6_Anim.Timer.GlobalTime);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  gl_prim_type = Pr->Type == DI6_RND_PRIM_TRIMESH ? GL_TRIANGLES : GL_TRIANGLE_STRIP;

  glBindVertexArray(Pr->VA);
  if (Pr->IBuf == 0)
    glDrawArrays(gl_prim_type, 0, Pr->NumOfI);
  else
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(gl_prim_type, Pr->NumOfI, GL_UNSIGNED_INT, NULL);
  }
  glBindVertexArray(0);
  

} /* End of 'DI6_RndPrimDraw' function */

/* Load primitive from "*.OBJ" file function.
 * ARGUMENTS:
 *   - primitive to be load to:
 *       di6PRIM *Pr;
 *   - filename to load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DI6_RndPrimLoad( di6PRIM *Pr, CHAR *Filename)
{
  INT nv, nf, size, i;
  di6VERTEX *V;
  INT *I;
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
      nf += 3;

  size = sizeof(di6VERTEX) * nv + sizeof(INT) * nf;

  V = malloc(size);
  if (V == NULL)
  {
    fclose(F);
    return FALSE;
  }

  memset(V, 0, size);
  I = (INT *)(V + nv);

  i = 0;

  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      FLT x, y, z;

      sscanf(Buf + 2, "%f%f%f", &x, &y, &z);
      V[nv++].p = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
      sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
      sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
      sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3);
      I[nf++] = n1 - 1;
      I[nf++] = n2 - 1;
      I[nf++] = n3 - 1;
    }

    i++;
  }
  fclose(F);

  DI6_RndPrimCreate(Pr, DI6_RND_PRIM_TRIMESH, V, nv, I, nf, 0);
  free(V);
  return TRUE;
} /* End of 'DI6_RndPrimLoad' function */

/* End of 'RNDPRIM.C' file */