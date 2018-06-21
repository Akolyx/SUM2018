/* FILE NAME: VEC.H
 * PROGRAMMER: DI6
 * DATE: 09.06.2018
 * PURPOSE: Definitions of vectors and matrices.
 */

#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>

#pragma warning(disable: 4244 4305 4028)

/* Definitions of pi constant and function translating angle from degrees to radians */
#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180))

#define MatrMulMatr3(A, B, C) MatrMulMatr(MatrMulMatr(A, B), C)
#define MatrMulMatr4(A, B, C, D) MatrMulMatr(MatrMulMatr3(A, B, C), D)
#define MatrMulMatr5(A, B, C, D, E) MatrMulMatr(MatrMulMatr4(A, B, C, D), E)

/* Shorter names of two crucial types */
typedef DOUBLE DBL;
typedef FLOAT FLT;

/* Structure representing a vector */
typedef struct tagVEC
{
  FLT x, y, z; /* Vector components */
} VEC;

/* Structure representing a plane vector */
typedef struct tagVEC2
{
  FLT x, y; /* Vector components */
} VEC2;

/* Structure representing a 4-components vector */
typedef struct tagVEC4
{
  FLT x, y, z, w; /* Vector components */
} VEC4;

/* Structure representing a matrix 4x4 */
typedef struct tagMATR
{
  FLT M[4][4]; /* Two-dimensional array of matrix elements */
} MATR;

/***
* Functions to work with vectors
***/

/* Constructing a vector with given coordinates
 * ARGUMENTS:
 *   - coordinates:
 *       FLT x, y, z;
 * RETURNS:
 *   (Vec) - vector with given coordinates;
 */
__inline VEC VecSet( FLT x, FLT y, FLT z )
{
  VEC v = {x, y, z};
  return v;
} /* End of 'VecSet' function */

/* Constructing a two-dimensional vector with given coordinates
 * ARGUMENTS:
 *   - coordinates:
 *       FLT x, y;
 * RETURNS:
 *   (Vec) - vector with given coordinates;
 */
__inline VEC2 Vec2Set( FLT x, FLT y )
{
  VEC2 v = {x, y};
  return v;
} /* End of 'Vec2Set' function */

/* Constructing a four-dimensional vector with given coordinates
 * ARGUMENTS:
 *   - coordinates:
 *       FLT x, y, z, w;
 * RETURNS:
 *   (Vec) - vector with given coordinates;
 */
__inline VEC4 Vec4Set( FLT x, FLT y, FLT z, FLT w )
{
  VEC4 v = {x, y, z, w};
  return v;
} /* End of 'Vec4Set' function */

/* Constructing a vector with three coordinates equal to given number
 * ARGUMENTS:
 *   - the number:
 *       FLT a;
 * RETURNS:
 *   (Vec) - vector with x = y = z = a;
 */
__inline VEC VecSetEqual( FLT a )
{
  VEC v = {a, a, a};
  return v;
} /* End of 'VecSetEqual' function */

/* Addition of two vectors function
 * ARGUMENTS:
 *   - vectors:
 *       VEC v1, v2;
 * RETURNS:
 *   (VEC) - vector equal to v1 + v2;
 */
__inline VEC VecAddVec( VEC v1, VEC v2 )
{
  return VecSet(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
} /* End of 'VecAddVec' function */

/* Subtraction of two vectors function
 * ARGUMENTS:
 *   - vectors:
 *       VEC v1, v2;
 * RETURNS:
 *   (VEC) - vector equal to v1 - v2;
 */
__inline VEC VecSubVec( VEC v1, VEC v2 )
{
  return VecSet(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
} /* End of 'VecSubVec' function */

/* Multiplication vector to number function
 * ARGUMENTS:
 *   - vector:
 *       VEC v;
 *   - number:
 *       FLT n;
 * RETURNS:
 *   (VEC) - vector equal to v * n;
 */
__inline VEC VecMulNum( VEC v, FLT n )
{
  return VecSet(v.x * n, v.y * n, v.z * n);
} /* End of 'VecMulNum' function */

/* Division vector to number function
 * ARGUMENTS:
 *   - vector:
 *       VEC v;
 *   - number:
 *       FLT n;
 * RETURNS:
 *   (VEC) - vector equal to v / n;
 */
__inline VEC VecDivNum( VEC v, FLT n )
{
  return VecSet(v.x / n, v.y / n, v.z / n);
} /* End of 'VecDivNum' function */

/* Negative vector function
 * ARGUMENTS:
 *   - vector:
 *       VEC v;
 * RETURNS:
 *   (VEC) - vector equal to -v;
 */
__inline VEC VecNeg( VEC v )
{
  return VecSet(-v.x, -v.y, -v.z);
} /* End of 'VecNeg' function */

/* Dot production of two vectors function
 * ARGUMENTS:
 *   - vectors:
 *       VEC v1, v2;
 * RETURNS:
 *   (FLT) - vector equal to dot production of v1 and v2;
 */
__inline FLT VecDotVec( VEC v1, VEC v2 )
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
} /* End of 'VecDotVec' function */

/* Cross production of two vectors function
 * ARGUMENTS:
 *   - vectors:
 *       VEC v1, v2;
 * RETURNS:
 *   (VEC) - vector equal to cross production of v1 and v2;
 */
__inline VEC VecCrossVec( VEC v1, VEC v2 )
{
  return VecSet(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
} /* End of 'VecCrossVec' function */

/* Returning a square of vector length function
 * ARGUMENTS:
 *   - vector:
 *       VEC v;
 * RETURNS:
 *   (FLT) - number equal to the square of vector length;
 */
__inline FLT VecLen2( VEC v )
{
  return v.x * v.x + v.y * v.y + v.z * v.z;
} /* End of 'VecLen2' function */

/* Returning a vector length function
 * ARGUMENTS:
 *   - vector:
 *       VEC v;
 * RETURNS:
 *   (FLT) - number equal to vector length;
 */
__inline FLT VecLen( VEC v )
{
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
} /* End of 'VecLen' function */

/* Vector normalization function
 * ARGUMENTS:
 *   - vector:
 *       VEC v;
 * RETURNS:
 *   (VEC) - vector equal to v / (length of v);
 */
__inline VEC VecNormalize( VEC v )
{
  FLT len = VecLen(v);

  if (len == 0 || len == 1)
    return v;

  return VecDivNum(v, len);
} /* End of 'VecNormalize' function */

/***
* End of vector functions block
***/

/***
* Functions to work with matrices
***/

/* Unit matrix */
static MATR UnitMatrix = 
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

/* Returning unit matrix function
 * ARGUMENTS:
 *   None
 * RETURNS:
 *   (MATR) - unit matrix;
 */
__inline MATR MatrIdentity( VOID )
{
  return UnitMatrix;
} /* End of 'MatrIdentity' function */

/* Returning matrix of translation function
 * ARGUMENTS:
 *   - vector on which the translation is going to happen:
 *       VEC t;
 * RETURNS:
 *   (MATR) - matrix to translate on the given vector;
 */
__inline MATR MatrTranslate( VEC t )
{
  MATR r = MatrIdentity();

  r.M[3][0] = t.x;
  r.M[3][1] = t.y;
  r.M[3][2] = t.z;

  return r;
} /* End of 'MatrTranslate' function */

/* Returning matrix of scaling function
 * ARGUMENTS:
 *   - vector on which the scaling is going to happen:
 *       VEC s;
 * RETURNS:
 *   (MATR) - matrix to scale on the given vector;
 */
__inline MATR MatrScale( VEC s )
{
  MATR r = MatrIdentity();

  r.M[0][0] = s.x;
  r.M[1][1] = s.y;
  r.M[2][2] = s.z;

  return r;
} /* End of 'MatrScale' function */

/* Returning matrix of rotation along X axis function
 * ARGUMENTS:
 *   - angle on which the rotation is going to happen:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) - matrix to rotate on the given angle along X axis;
 */
__inline MATR MatrRotateX( FLT AngleInDegree )
{
  FLT a = D2R(AngleInDegree), co = cos(a), si = sin(a);
  MATR r = MatrIdentity();

  r.M[1][1] = co;
  r.M[1][2] = si;
  r.M[2][1] = -si;
  r.M[2][2] = co;

  return r;
} /* End of 'MatrRotateX' function */

/* Returning matrix of rotation along Y axis function
 * ARGUMENTS:
 *   - angle on which the rotation is going to happen:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) - matrix to rotate on the given angle along Y axis;
 */
__inline MATR MatrRotateY( FLT AngleInDegree )
{
  FLT a = D2R(AngleInDegree), co = cos(a), si = sin(a);
  MATR r = MatrIdentity();

  r.M[0][0] = co;
  r.M[2][0] = si;
  r.M[0][2] = -si;
  r.M[2][2] = co;

  return r;
} /* End of 'MatrRotateY' function */

/* Returning matrix of rotation along Z axis function
 * ARGUMENTS:
 *   - angle on which the rotation is going to happen:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) - matrix to rotate on the given angle along Z axis;
 */
__inline MATR MatrRotateZ( FLT AngleInDegree )
{
  FLT a = D2R(AngleInDegree), co = cos(a), si = sin(a);
  MATR r = MatrIdentity();

  r.M[0][0] = co;
  r.M[0][1] = si;
  r.M[1][0] = -si;
  r.M[1][1] = co;

  return r;
} /* End of  'MatrRotateX' function */

/* Returning matrix of rotation along given vector function
 * ARGUMENTS:
 *   - angle on which the rotation is going to happen:
 *       FLT AngleInDegree;
 *   - vector
 *       VEC r;
 * RETURNS:
 *   (MATR) - matrix to rotate on the given angle along given vector;
 */
__inline MATR MatrRotate( FLT AngleInDegree, VEC r )
{
  FLT a = D2R(AngleInDegree), si = sin(a), co = cos(a);
  FLT oc = 1 - co;
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
} /* End of 'MatrRotate' functions */

/* Returning production of two matrices function
 * ARGUMENTS:
 *   - matrices:
 *       MATR m1, m2;
 * RETURNS:
 *   (MATR) - matrix equal to m1 * m2;
 */
__inline MATR MatrMulMatr( MATR m1, MATR m2 )
{
  INT i, j, k;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.M[i][j] = 0, k = 0; k < 4; k++)
        r.M[i][j] += m1.M[i][k] * m2.M[k][j];

  return r;
} /* End of 'MatrMulMatr' function */

/* Returning transposed matrix function
 * ARGUMENTS:
 *   - matrix:
 *       MATR m;
 * RETURNS:
 *   (MATR) - matrix equal to transposed m;
 */
__inline MATR MatrTranspose( MATR m )
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.M[i][j] = m.M[j][i];

  return r;
} /* End of 'MatrTranspose' function */

/* Returning determinant of matrix 3x3 function
 * ARGUMENTS:
 *   - elements of the matrix:
 *       FLT a11, a12, a13, a21, a22, a23, a31, a32, a33;
 * RETURNS:
 *   (FLT) - determinant of the matrix constructed of given elements;
 */
__inline FLT MatrDeterm3x3( FLT a11, FLT a12, FLT a13, FLT a21, FLT a22, FLT a23, FLT a31, FLT a32, FLT a33 )
{
  return a11 * a22 * a33 - a11 * a23 * a32 - a12 * a21 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31;
} /* End of 'MatrDeterm3x3' function */

/* Returning determinant of matrix 4x4 function
 * ARGUMENTS:
 *   - matrix:
 *       MATR m;
 * RETURNS:
 *   (FLT) - determinant of the matrix m;
 */
__inline FLT MatrDeterm( MATR m )
{
  return
    m.M[0][0] * MatrDeterm3x3(m.M[1][1], m.M[1][2], m.M[1][3], m.M[2][1], m.M[2][2], m.M[2][3], m.M[3][1], m.M[3][2], m.M[3][3]) -
    m.M[0][1] * MatrDeterm3x3(m.M[1][0], m.M[1][2], m.M[1][3], m.M[2][0], m.M[2][2], m.M[2][3], m.M[3][0], m.M[3][2], m.M[3][3]) +
    m.M[0][2] * MatrDeterm3x3(m.M[1][0], m.M[1][1], m.M[1][3], m.M[2][0], m.M[2][1], m.M[2][3], m.M[3][0], m.M[3][1], m.M[3][3]) -
    m.M[0][3] * MatrDeterm3x3(m.M[1][0], m.M[1][1], m.M[1][2], m.M[2][0], m.M[2][1], m.M[2][2], m.M[3][0], m.M[3][1], m.M[3][2]);
} /* End of 'MatrDeterm' function */

/* Returning inversed matrix function
 * ARGUMENTS:
 *   - matrix:
 *       MATR m;
 * RETURNS:
 *   (MATR) - matrix inversed to m;
 */
__inline MATR MatrInverse( MATR m )
{
  INT i, j;
  MATR r;
  FLT det = MatrDeterm(m);

  if (det == 0)
    return UnitMatrix;

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

  /* division on determinant */
  for (i = 0; i < 4; i++)
   for (j = 0; j < 4; j++)
     r.M[i][j] /= det;

  return r;
} /* End of 'MatrInverse' function */


/* Returning a transformed coordinates of related vector function
 * ARGUMENTS:
 *   - vector:
 *       VEC v;
 *   - transformation matrix:
 *       MATR m;
 * RETURNS:
 *   (VEC) - transformed vector;
 */
__inline VEC PointTransform( VEC v, MATR m )
{
  FLT x = 0, y = 0, z = 0;

  x = v.x * m.M[0][0] + v.y * m.M[1][0] + v.z * m.M[2][0] + m.M[3][0];
  y = v.x * m.M[0][1] + v.y * m.M[1][1] + v.z * m.M[2][1] + m.M[3][1];
  z = v.x * m.M[0][2] + v.y * m.M[1][2] + v.z * m.M[2][2] + m.M[3][2];

  return VecSet(x, y, z);
} /* End of 'PointTransform' function */

/* Returning a transformed coordinates of free vector function
 * ARGUMENTS:
 *   - vector:
 *       VEC v;
 *   - transformation matrix:
 *       MATR m;
 * RETURNS:
 *   (VEC) - transformed vector;
 */
__inline VEC VectorTransform( VEC v, MATR m )
{
  FLT x = 0, y = 0, z = 0;

  x = v.x * m.M[0][0] + v.y * m.M[1][0] + v.z * m.M[2][0];
  y = v.x * m.M[0][1] + v.y * m.M[1][1] + v.z * m.M[2][1];
  z = v.x * m.M[0][2] + v.y * m.M[1][2] + v.z * m.M[2][2];

  return VecSet(x, y, z);
} /* End of 'VectorTransform' function */

/* Returning a transformed coordinates of a vector function
 * ARGUMENTS:
 *   - vector:
 *       VEC v;
 *   - transformation matrix:
 *       MATR m;
 * RETURNS:
 *   (VEC) - transformed vector;
 */
__inline VEC VecMulMatr4x4( VEC v, MATR m )
{
  FLT x, y, z, w;

  x = v.x * m.M[0][0] + v.y * m.M[1][0] + v.z * m.M[2][0] + m.M[3][0];
  y = v.x * m.M[0][1] + v.y * m.M[1][1] + v.z * m.M[2][1] + m.M[3][1];
  z = v.x * m.M[0][2] + v.y * m.M[1][2] + v.z * m.M[2][2] + m.M[3][2];
  w = v.x * m.M[0][3] + v.y * m.M[1][3] + v.z * m.M[2][3] + m.M[3][3];

  return VecSet(x / w, y / w, z / w);
} /* End of 'VecMulMatr4x4' function */

/* Returning a matrix of view function
 * ARGUMENTS:
 *   - vectors of our location, point at which we see and approximate up direction:
 *       VEC loc, at, up1;
 * RETURNS:
 *   (MATR) - matrix to transform to camera view;
 */
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
} /* End of 'MatrView' function */

/* Returning a matrix transforming scene to normalized cube (-1 ... 1) function
 * ARGUMENTS:
 *   - coordinates of view frustum:
 *       FLT l, r, b, t, n, f;
 * RETURNS:
 *   (MATR) - matrix to transform to normalized cube;
 */
__inline MATR MatrFrustum( FLT l, FLT r, FLT b, FLT t, FLT n, FLT f )
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
} /* End of 'MatrFrustum' function */

/***
* End of matrix functions block
***/

#endif /* __VEC_H_ */

/* End of 'VEC.H' file */