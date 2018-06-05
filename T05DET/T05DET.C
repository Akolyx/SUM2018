/* FILE NAME: T05DET.C
 * PROGRAMMER: DI6
 * DATE: 05.06.2018
 * PURPOSE: Calculate of determinant of matrix.
 */

#include <windows.h>
#include <stdio.h>

#define MAX 10

DOUBLE a[MAX][MAX];
DOUBLE Determinant;

INT n;
INT p[MAX];

INT PermLex(INT n);

FILE *F, *A;

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

    for (i = 0; i < n; i++)
      p[i] = i;

    while (PermLex(n));

    return TRUE;
  }
} /* End of 'LoadMatrix' function */

/* Adding a single element of sum of determinant
 * ARGUMENTS:
 *   - number of elements:
 *       INT n;
 *   - if the permutation is even (1) or odd (-1):
 *       INT par;
 * RETURNS:
 *   None.
 */
VOID Det( INT n , INT par)
{
  INT i;
  DOUBLE add = 1;
  
  for (i = 0; i < n; i++)
    add *= a[i][p[i]];

  Determinant += add * par;
} /* End of 'Det' function */

/* Exchanging two variables
 * ARGUMENTS:
 *   - two pointers on int variables:
 *       INT *a, INT *b;
 * RETURNS:
 *   None.
 */
VOID Swap( INT *a, INT *b )
{
  INT tmp = *a;
  *a = *b;
  *b = tmp;
} /* End of 'Swap' function */

/* Sort out the permutations of second indices and induce the Det for each one
 * ARGUMENTS:
 *   - number of elements:
 *       INT n;
 * RETURNS:
 *   (INT) - 0, if the permutation is the last in the lexicographic order, 1 otherwise.
 */
INT PermLex(INT n)
{
  INT i = -1, k;
  INT min = 11, posmin = -1;
  INT r, q;

  static INT par = 1;

  Det(n, par);

  for (k = n - 1; k > 0; k--)
    if (p[k - 1] < p[k])
    {   
      i = k - 1;
      break;
    }

  if (i == -1)
    return 0;

  for (k = i + 1; k < n; k++)
  {
    if ((p[k] > p[i]) && (p[k] < min))
    {
      min = p[k];
      posmin = k;
    }
  }

  Swap(&p[i], &p[posmin]);
  par *= -1;

  for (r = i + 1; r < n - 1; r++)
    for (q = r + 1; q < n; q++)
      if (p[r] > p[q])
      {
        Swap(&p[r], &p[q]);
        par *= -1;
      }

  return 1;
} /* End of 'PermLex' function */

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