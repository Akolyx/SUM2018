#include <windows.h>
#include <stdio.h>

#define MAX 10

INT p[MAX];

FILE *F;

CHAR* parity[] = {"Even", "Odd"};

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
}

VOID Swap( INT *a, INT *b )
{
  INT tmp = *a;
  *a = *b;
  *b = tmp;
}

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
}

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
}