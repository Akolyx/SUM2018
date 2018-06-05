/* FILE NAME: T04PERM.C
 * PROGRAMMER: DI6
 * DATE: 05.06.2018
 * PURPOSE: Enumerating of all permutations from 1 to n.
 */

#include <windows.h>
#include <stdio.h>

#define MAX 10

INT p[MAX];

FILE *F;

CHAR* parity[] = {"Even", "Odd"};

/* Output the permutation in file
 * ARGUMENTS:
 *   - number of elements:
 *       INT n;
 *   - if the permutation is even (0) or odd (1):
 *       INT par;
 * RETURNS:
 *   None.
 */
VOID Store( INT n , INT par)
{
  INT i;

  if (F == NULL)
    return;
  else
  {
    for (i = 0; i < n; i++)
      fprintf(F, "%d ", p[i]);
    fprintf(F, "- %s\n", parity[par]);
  }
} /* End of 'Store' function */

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

  static INT par = 0;

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
  par = 1 - par;

  for (r = i + 1; r < n - 1; r++)
    for (q = r + 1; q < n; q++)
      if (p[r] > p[q])
      {
        Swap(&p[r], &p[q]);
        par = 1 - par;
      }
  Store(n, par);

  return 1;
} /* End of 'PermLex' function */

/* Main function */
VOID main( VOID )
{
  INT i, n;

  F = fopen("PERM.TXT", "w");

  scanf("%d", &n);

  if (n > MAX)
  {
    fprintf(F, "Inadequate n\n");
  }
  else
  {
    for (i = 0; i < n; i++)
    {
      p[i] = i + 1;
    }

    Store(n, 0);

    while (PermLex(n));
  }

  fclose(F);
} /* End of 'main' function */

/* End of 'T04PERM' file */