/* FILE NAME: T08ANIM.C
 * PROGRAMMER: DI6
 * DATE: 07.06.2018
 * PURPOSE: Checking VEC.C.
 */

#include <stdio.h>
#include "VEC.H"

static MATR m = 
{
  {
    {1, 2, 4, 0},
    {0, 8, 2, 0},
    {4, 0, 0, 0},
    {0, 0, 8, 1}
  }
};

VOID main( VOID )
{
  INT i, j;
  FILE *F;
  MATR m1;

  m1 = MatrRotateY(90);

  F = fopen("Check.txt", "w");

  for (i = 0; i < 4; i++)
  {  
    for (j = 0; j < 4; j++)
      fprintf(F, "%lf ", m1.M[i][j]);

    fprintf(F, "\n");
  }

  fclose(F);
}