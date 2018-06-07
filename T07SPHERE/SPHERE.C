/* FILE NAME: SPHERE.C
 * PROGRAMMER: DI6
 * DATE: 07.06.2018
 * PURPOSE: Drawing of the sphere.
 */

#include "Sphere.h"
#include <time.h>
#define p 15

typedef struct
{
  DOUBLE x, y, z;
} Vec;

typedef struct
{
  DOUBLE x, y;
} POINTD;

Vec m[p][2 * p];

/* Constructing a vector with given coordinates
 * ARGUMENTS:
 *   - coordinates:
 *       DOUBLE x, DOUBLE y, DOUBLE z;
 * RETURNS:
 *   (Vec) - vector with given coordinates;
 */
Vec ConstructVec(DOUBLE x, DOUBLE y, DOUBLE z)
{
  Vec v = {x, y, z};
  return v;
}

/* Rotating a vector along Z axis
 * ARGUMENTS:
 *   - coordinates:
 *       DOUBLE x, DOUBLE y;
 *   - angle in degree:
 *       DOUBLE angledeg;
 * RETURNS:
 *   (POINT) - new x and y coordinates;
 */
POINTD RotateZ(DOUBLE x, DOUBLE y, DOUBLE angledeg)
{
  DOUBLE x1, y1, a;
  POINTD pt;

  a = angledeg * PI / 180;

  x1 = x * cos(a) - y * sin(a);
  y1 = x * sin(a) + y * cos(a);

  pt.x = x1;
  pt.y = y1;

  return pt;
}

/* Rotating a vector along Y axis
 * ARGUMENTS:
 *   - coordinates:
 *       DOUBLE z, DOUBLE x;
 *   - angle in degree:
 *       DOUBLE angledeg;
 * RETURNS:
 *   (POINT) - new x and y coordinates;
 */
POINTD RotateY(DOUBLE z, DOUBLE x, DOUBLE angledeg)
{
  DOUBLE z1, x1, a;
  POINTD pt;

  a = angledeg * PI / 180;

  z1 = z * cos(a) - x * sin(a);
  x1 = z * sin(a) + x * cos(a);

  pt.x = z1;
  pt.y = x1;

  return pt;
}

/* Modeling a unit sphere
 * ARGUMENTS:
 *   None;
 * RETURNS:
 *   None;
 */
VOID ModelSphere()
{
  INT i, j;
  DOUBLE f = 0, t = 0, d = PI / p;
  DOUBLE sint, cost;

  for (i = 0; i < p; i++)
  { 
    sint = sin(t);
    cost = cos(t);

    for (j = 0; j < 2 * p; j++)
    {
      f += d;

      m[i][j] = ConstructVec(sint * sin(f), cost, sint * cos(f));
    }

    t += d;
  }
}

/* Drawing of sphere
 * ARGUMENTS:
 *   - handle of paint contest:
 *       HDC hDC;
 *   - coordinates of center of the sphere:
 *       INT x, INT y;
 *   - radius of the sphere:
 *       INT r;
 * RETURNS:
 *   None.
 */
void DrawSphere( HDC hDC, INT x, INT y, INT r)
{
  INT i, j, k;
  POINT pts[p][2 * p];
  DOUBLE sq = 0.0;
  LONG t = clock() / CLOCKS_PER_SEC;
  POINTD ptd;

  for (i = 0; i < p; i++)
    for (j = 0; j < 2 * p; j++)
    {
      ptd = RotateZ(m[i][j].x, m[i][j].y, (DOUBLE)(20));

      pts[i][j].x = (LONG)(x + r * ptd.x);
      pts[i][j].y = (LONG)(y + r * ptd.y);
    }

  for (i = 0; i < p - 1; i++)
    for (j = 0; j < 2 * p - 1; j++)
    {
      POINT pnts[4];

      pnts[0] = pts[i][j];
      pnts[1] = pts[i][j + 1];
      pnts[2] = pts[i + 1][j + 1];
      pnts[3] = pts[i + 1][j];

      for (k = 0; k < 4; k++)
        sq += (pnts[k].x - pnts[(k + 1) % 4].x) * (pnts[k].y + pnts[(k + 1) % 4].y) / 2;

      if (sq > 0)
      {
        SetDCPenColor(hDC, RGB(0, 0, 0));
        Polygon(hDC, pnts, 4);
      }
      else
      {
        SetDCPenColor(hDC, RGB(200 + i, (200 + j) % 256, (200 + i + j) % 256));
        Polygon(hDC, pnts, 4);
      }
    }
  
} /* End of 'DrawSphere' function */

/* End of 'SPHERE.C' file */