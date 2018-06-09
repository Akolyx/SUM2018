/* FILE NAME: SPHERE.C
 * PROGRAMMER: DI6
 * DATE: 07.06.2018
 * PURPOSE: Drawing of the sphere.
 */

#include "Sphere.h"
#include <time.h>
#define p 15

typedef DOUBLE DBL;

typedef struct
{
  DBL x, y, z;
} Vec;

Vec m[p + 1][2 * p];

/* Constructing a vector with given coordinates
 * ARGUMENTS:
 *   - coordinates:
 *       DBL x, DBL y, DBL z;
 * RETURNS:
 *   (Vec) - vector with given coordinates;
 */
Vec ConstructVec(DBL x, DBL y, DBL z)
{
  Vec v = {x, y, z};
  return v;
}

/* Rotating a vector along Z axis
 * ARGUMENTS:
 *   - vector:
 *       Vec v;
 *   - angle in degree:
 *       DBL angledeg;
 * RETURNS:
 *   (Vec) - new vector;
 */
Vec RotateZ(Vec v, DBL angledeg)
{
  DBL a;

  a = angledeg * PI / 180;

  return ConstructVec(v.x * cos(a) - v.y * sin(a), v.x * sin(a) + v.y * cos(a), v.z);
}

/* Rotating a vector along Y axis
 * ARGUMENTS:
 *   - vector:
 *       Vec v;
 *   - angle in degree:
 *       DBL angledeg;
 * RETURNS:
 *   (Vec) - new vector;
 */
Vec RotateY(Vec v, DBL angledeg)
{
  DBL a;

  a = angledeg * PI / 180;

  return ConstructVec(v.z * sin(a) + v.x * cos(a), v.y, v.z + cos(a) - v.x * sin(a));
}

/* Rotating a vector along X axis
 * ARGUMENTS:
 *   - vector:
 *       Vec v;
 *   - angle in degree:
 *       DBL angledeg;
 * RETURNS:
 *   (Vec) - new vector;
 */
Vec RotateX(Vec v, DBL angledeg)
{
  DBL a;

  a = angledeg * PI / 180;

  return ConstructVec(v.x, v.y * cos(a) - v.z * sin(a), v.y * sin(a) + v.z * cos(a));
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
  DBL f = 0, t = 0, d = PI / p;
  DBL sint, cost;

  for (i = 0; i <= p; i++)
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
  POINT pts[p + 1][2 * p + 1];
  DBL sq = 0.0;
  DBL t = (DBL)clock() / CLOCKS_PER_SEC;
  Vec res;

  for (i = 0; i <= p; i++)
    for (j = 0; j < 2 * p; j++)
    {
      res = RotateY(RotateZ(m[i][j], 20 * t), 10 * t);

      pts[i][j].x = (LONG)(x + r * res.x);
      pts[i][j].y = (LONG)(y + r * res.y);
    }

  for (i = 0; i < p; i++)
  {
    POINT pnts[4];

    for (j = 0; j < 2 * p; j++)
    {
      pnts[0] = pts[i][j];
      pnts[1] = pts[i][(j + 1) % (2 * p)];
      pnts[2] = pts[i + 1][(j + 1) % (2 * p)];
      pnts[3] = pts[i + 1][j];

      for (k = 0; k < 4; k++)
        sq += (pnts[k].x - pnts[(k + 1) % 4].x) * (pnts[k].y + pnts[(k + 1) % 4].y);

      if (sq < 0)
      {
        SetDCPenColor(hDC, RGB(200, 200, 200));
        Polygon(hDC, pnts, 4);
      }
    }
  }

  for (i = 0; i < p; i++)
  {
    POINT pnts[4];

    for (j = 0; j < 2 * p; j++)
    {
      pnts[0] = pts[i][j];
      pnts[1] = pts[i][(j + 1) % (2 * p)];
      pnts[2] = pts[i + 1][(j + 1) % (2 * p)];
      pnts[3] = pts[i + 1][j];

      for (k = 0; k < 4; k++)
        sq += (pnts[k].x - pnts[(k + 1) % 4].x) * (pnts[k].y + pnts[(k + 1) % 4].y);

      if (sq > 0)
      {
        SetDCPenColor(hDC, RGB(0, 0, 0));
        Polygon(hDC, pnts, 4);
      }
    }
  }
} /* End of 'DrawSphere' function */

/* End of 'SPHERE.C' file */