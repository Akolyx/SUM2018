/* FILE NAME: T06DETG.C
 * PROGRAMMER: DI6
 * DATE: 05.06.2018
 * PURPOSE: Calculate of determinant of matrix.
 */

#include <windows.h>
#include <stdio.h>

#define MAX 50

DOUBLE a[MAX][MAX];
DOUBLE Determinant;

INT n;
INT p[MAX];

FILE *F, *A;

DOUBLE CalcDet(INT n);

/* Load matrix from file
 * ARGUMENTS:
 *   - name of the file:
 *       CHAR *Filename;
 * RETURNS:
 *   (BOOL) - TRUE if matrix was loaded, FALSE otherwise.
 */
BOOL LoadMatrix(CHAR *Filename)
{
  int i, j;

  if ((F = fopen(Filename, "r")) == NULL)
    return FALSE;

  fscanf(F, "%d", &n);

  if (n < 0)
  {
    n = 0;
    return FALSE;
  }
  else
  {
    if (n > MAX)
      n = MAX;

    for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
        fscanf(F, "%lf", &a[i][j]);

    for (i = 0; i < n; i++)
    {
      for (j = 0; j < n; j++)
        fprintf(A, "%g ", a[i][j]);
      fprintf(A, "\n");
    }

    fclose(F);

    Determinant = CalcDet(n);

    return TRUE;
  }
} /* End of 'LoadMatrix' function */

/* Calaculating determinant of the matrix using Gauss's method
 * ARGUMENTS:
 *   - number of elements:
 *       INT n;
 * RETURNS:
 *   (DOUBLE) - answer.
 */
DOUBLE CalcDet(INT n)
{
  INT i, j, k;
  DOUBLE det = 1, d;

  for (i = 0; i < n; i++)
  {
    for (j = i + 1; (a[i][i] == 0) && (j < n); j++)
      for (k = i; k < n; k++)
        a[i][k] += a[j][k];

    if (a[i][i] == 0)
      return 0;

    for (j = i + 1; j < n; j++)
    {
      d = a[j][i] / a[i][i];
      for (k = i; k < n; k++)
        a[j][k] -= a[i][k] * d;
    }
  }

  for (i = 0; i < n; i++)
    det *= a[i][i];

  return det;
} /* End of 'CalcDet' function */

/* Main function */
VOID main( VOID )
{
  A = fopen("Determinant.txt", "w");

  fprintf(A, "Determinant of\n");

  LoadMatrix("m1.txt");

  fprintf(A, "= %g", Determinant);

  fclose(A);
} /* End of 'main' function */

/* End of 'T05DET' file */