/* FILE NAME: VEC.H
 * PROGRAMMER: DI6
 * DATE: 07.06.2018
 * PURPOSE: Definitions of vectors and matrixes.
 */

#include <windows.h>
#include <math.h>

#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180))

typedef DOUBLE DBL;

typedef struct tagVEC
{
  DBL x, y, z;
} VEC;

typedef struct tagMATR
{
  DBL M[4][4];
} MATR;

static MATR UnitMatrix = 
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

__inline VOID ModelSphere();
__inline VOID DrawSphere( HDC hDC, INT w, INT h,INT x, INT y, INT r);

__inline VEC VecSet( DBL x, DBL y, DBL z )
{
  VEC v = {x, y, z};
  return v;
}

__inline VEC VecAddVec( VEC v1, VEC v2 )
{
  return VecSet(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

__inline VEC VecSubVec( VEC v1, VEC v2 )
{
  return VecSet(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

__inline VEC VecMulNum( VEC v1, DBL n )
{
  return VecSet(v1.x * n, v1.y * n, v1.z * n);
}

__inline VEC VecDivNum( VEC v1, DBL n )
{
  return VecSet(v1.x / n, v1.y / n, v1.z / n);
}

__inline VEC VecNeg( VEC v )
{
  return VecSet(-v.x, -v.y, -v.z);
}

__inline DBL VecDotVec( VEC v1, VEC v2 )
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

__inline VEC VecCrossVec( VEC v1, VEC v2 )
{
  return VecSet(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

__inline DBL VecLen2( VEC v )
{
  return v.x * v.x + v.y * v.y + v.z * v.z;
}

__inline DBL VecLen( VEC v )
{
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

__inline VEC VecNormalize( VEC v )
{
  DBL len = VecLen(v);

  if (len == 0 || len == 1)
    return v;

  return VecDivNum(v, len);
}

__inline MATR MatrTranslate( VEC t )
{
  MATR r = UnitMatrix;

  r.M[3][0] = t.x;
  r.M[3][1] = t.y;
  r.M[3][2] = t.z;

  return r;
}

__inline MATR MatrScale( VEC s )
{
  MATR r = UnitMatrix;

  r.M[0][0] = s.x;
  r.M[1][1] = s.y;
  r.M[2][2] = s.z;

  return r;
}

__inline MATR MatrRotateX( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree), co = cos(a), si = sin(a);
  MATR r = UnitMatrix;

  r.M[1][1] = co;
  r.M[1][2] = si;
  r.M[2][1] = -si;
  r.M[2][2] = co;

  return r;
}

__inline MATR MatrRotateY( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree), co = cos(a), si = sin(a);
  MATR r = UnitMatrix;

  r.M[0][0] = co;
  r.M[2][0] = si;
  r.M[0][2] = -si;
  r.M[2][2] = co;

  return r;
}

__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree), co = cos(a), si = sin(a);
  MATR r = UnitMatrix;

  r.M[0][0] = co;
  r.M[0][1] = si;
  r.M[1][0] = -si;
  r.M[1][1] = co;

  return r;
}

__inline MATR MatrRotate( DBL AngleInDegree, VEC r )
{
  DBL a = D2R(AngleInDegree), si = sin(a), co = cos(a);
  DBL oc = 1 - co;
  VEC v = VecNormalize(r);

  MATR m =
  {
    {
      {co + v.x * v.x * oc, v.x * v.y * oc + v.z * si, v.x * v.z * oc - v.y * si, 0},
      {v.y * v.x * oc - v.z * si, co + v.y * v.y * oc, v.y * v.z * oc + v.x * si, 0},
      {v.z * v.x * oc + v.y * si, v.z * v.y * oc - v.x * si, co + v.z * v.z * oc, 0},
      {0, 0, 0, 1}
    }
  };

  return m;
}

__inline MATR MatrMulMatr( MATR m1, MATR m2 )
{
  INT i, j, k;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.M[i][j] = 0, k = 0; k < 4; k++)
        r.M[i][j] += m1.M[i][k] * m2.M[k][j];

  return r;
}
__inline MATR MatrTranspose( MATR m )
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.M[i][j] = m.M[j][i];

  return r;
}


__inline DBL MatrDeterm3x3( DBL a11, DBL a12, DBL a13, DBL a21, DBL a22, DBL a23, DBL a31, DBL a32, DBL a33 )
{
  return a11 * a22 * a33 - a11 * a23 * a32 - a12 * a21 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31;
}

__inline DBL MatrDeterm( MATR m )
{
  return
    m.M[0][0] * MatrDeterm3x3(m.M[1][1], m.M[1][2], m.M[1][3], m.M[2][1], m.M[2][2], m.M[2][3], m.M[3][1], m.M[3][2], m.M[3][3]) -
    m.M[0][1] * MatrDeterm3x3(m.M[1][0], m.M[1][2], m.M[1][3], m.M[2][0], m.M[2][2], m.M[2][3], m.M[3][0], m.M[3][2], m.M[3][3]) +
    m.M[0][2] * MatrDeterm3x3(m.M[1][0], m.M[1][1], m.M[1][3], m.M[2][0], m.M[2][1], m.M[2][3], m.M[3][0], m.M[3][1], m.M[3][3]) -
    m.M[0][3] * MatrDeterm3x3(m.M[1][0], m.M[1][1], m.M[1][2], m.M[2][0], m.M[2][1], m.M[2][2], m.M[3][0], m.M[3][1], m.M[3][2]);
}

__inline MATR MatrInverse( MATR m )
{
  INT i, j;
  MATR r;
  DBL det = MatrDeterm(m);

  if (det == 0)
    return UnitMatrix;

  /* строим присоединенную матрицу */
  /* build adjoint matrix */
  r.M[0][0] = MatrDeterm3x3(m.M[1][1], m.M[1][2], m.M[1][3], m.M[2][1], m.M[2][2], m.M[2][3], m.M[3][1], m.M[3][2], m.M[3][3]);
  r.M[1][0] = -MatrDeterm3x3(m.M[1][0], m.M[1][2], m.M[1][3], m.M[2][0], m.M[2][2], m.M[2][3], m.M[3][0], m.M[3][2], m.M[3][3]);
  r.M[2][0] = MatrDeterm3x3(m.M[1][0], m.M[1][1], m.M[1][3], m.M[2][0], m.M[2][1], m.M[2][3], m.M[3][0], m.M[3][1], m.M[3][3]);
  r.M[3][0] = -MatrDeterm3x3(m.M[1][0], m.M[1][1], m.M[1][2], m.M[2][0], m.M[2][1], m.M[2][2], m.M[3][0], m.M[3][1], m.M[3][2]);
  r.M[0][1] = -MatrDeterm3x3(m.M[0][1], m.M[0][2], m.M[0][3], m.M[2][1], m.M[2][2], m.M[2][3], m.M[3][1], m.M[3][2], m.M[3][3]);
  r.M[1][1] = MatrDeterm3x3(m.M[0][0], m.M[0][2], m.M[0][3], m.M[2][0], m.M[2][2], m.M[2][3], m.M[3][0], m.M[3][2], m.M[3][3]);
  r.M[2][1] = -MatrDeterm3x3(m.M[0][0], m.M[0][1], m.M[0][3], m.M[2][0], m.M[2][1], m.M[2][3], m.M[3][0], m.M[3][1], m.M[3][3]);
  r.M[3][1] = MatrDeterm3x3(m.M[0][0], m.M[0][1], m.M[0][2], m.M[2][0], m.M[2][1], m.M[2][2], m.M[3][0], m.M[3][1], m.M[3][2]);
  r.M[0][2] = MatrDeterm3x3(m.M[0][1], m.M[0][2], m.M[0][3], m.M[1][1], m.M[1][2], m.M[1][3], m.M[3][1], m.M[3][2], m.M[3][3]);
  r.M[1][2] = -MatrDeterm3x3(m.M[0][0], m.M[0][2], m.M[0][3], m.M[1][0], m.M[1][2], m.M[1][3], m.M[3][0], m.M[3][2], m.M[3][3]);
  r.M[2][2] = MatrDeterm3x3(m.M[0][0], m.M[0][1], m.M[0][3], m.M[1][0], m.M[1][1], m.M[1][3], m.M[3][0], m.M[3][1], m.M[3][3]);
  r.M[3][2] = -MatrDeterm3x3(m.M[0][0], m.M[0][1], m.M[0][2], m.M[1][0], m.M[1][1], m.M[1][2], m.M[3][0], m.M[3][1], m.M[3][2]);
  r.M[0][3] = -MatrDeterm3x3(m.M[0][1], m.M[0][2], m.M[0][3], m.M[1][1], m.M[1][2], m.M[1][3], m.M[2][1], m.M[2][2], m.M[2][3]); 
  r.M[1][3] = MatrDeterm3x3(m.M[0][0], m.M[0][2], m.M[0][3], m.M[1][0], m.M[1][2], m.M[1][3], m.M[2][0], m.M[2][2], m.M[2][3]);
  r.M[2][3] = -MatrDeterm3x3(m.M[0][0], m.M[0][1], m.M[0][3], m.M[1][0], m.M[1][1], m.M[1][3], m.M[2][0], m.M[2][1], m.M[2][3]);
  r.M[3][3] = MatrDeterm3x3(m.M[0][0], m.M[0][1], m.M[0][2], m.M[1][0], m.M[1][1], m.M[1][2], m.M[2][0], m.M[2][1], m.M[2][2]);

  /* делим на определитель */
  for (i = 0; i < 4; i++)
   for (j = 0; j < 4; j++)
     r.M[i][j] /= det;

  return r;
}

__inline VEC PointTransform( VEC v, MATR m )
{
  DBL x = 0, y = 0, z = 0;

  x = v.x * m.M[0][0] + v.y * m.M[1][0] + v.z * m.M[2][0] + m.M[3][0];
  y = v.x * m.M[0][1] + v.y * m.M[1][1] + v.z * m.M[2][1] + m.M[3][1];
  z = v.x * m.M[0][2] + v.y * m.M[1][2] + v.z * m.M[2][2] + m.M[3][2];

  return VecSet(x, y, z);
}

__inline VEC VectorTransform( VEC v, MATR m )
{
  DBL x = 0, y = 0, z = 0;

  x = v.x * m.M[0][0] + v.y * m.M[1][0] + v.z * m.M[2][0];
  y = v.x * m.M[0][1] + v.y * m.M[1][1] + v.z * m.M[2][1];
  z = v.x * m.M[0][2] + v.y * m.M[1][2] + v.z * m.M[2][2];

  return VecSet(x, y, z);
}

__inline VEC VecMulMatr4x4( VEC v, MATR m )
{
  DBL x, y, z, w;

  x = v.x * m.M[0][0] + v.y * m.M[1][0] + v.z * m.M[2][0] + m.M[3][0];
  y = v.x * m.M[0][1] + v.y * m.M[1][1] + v.z * m.M[2][1] + m.M[3][1];
  z = v.x * m.M[0][2] + v.y * m.M[1][2] + v.z * m.M[2][2] + m.M[3][2];
  w = v.x * m.M[0][3] + v.y * m.M[1][3] + v.z * m.M[2][3] + m.M[3][3];

  return VecSet(x / w, y / w, z / w);
}

__inline MATR MatrView( VEC loc, VEC at, VEC up1 )
{
  VEC dir = VecNormalize(VecSubVec(at, loc)),
      right = VecNormalize(VecCrossVec(dir, up1)),
      up = VecNormalize(VecCrossVec(right, dir));

  MATR m = 
  {
    {
      {right.x, up.x, -dir.x, 0},
      {right.y, up.y, -dir.y, 0},
      {right.z, up.z, -dir.z, 0},
      {-VecDotVec(loc, right), -VecDotVec(loc, up), VecDotVec(loc, dir), 1}
    }
  };

  return m;
}

__inline MATR MatrFrustum( DBL l, DBL r, DBL b, DBL t, DBL n, DBL f )
{
  MATR m = 
  {
    {
      {(2 * n) / (r - l), 0, 0, 0},
      {0, (2 * n) / (t - b), 0, 0},
      {(r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1},
      {0, 0, -(2 * n * f) / (f - n), 0}
    }
  };

  return m;
}