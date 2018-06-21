/* FILE NAME: RNDBASE.C
 * PROGRAMMER: DI6
 * DATE: 09.06.2018
 * PURPOSE: Base functions.
 */

#include "../anim.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32s")

VOID DI6_RndInit( HWND hWnd )
{
  INT i;
  HFONT hFnt, hOldFnt;

  PIXELFORMATDESCRIPTOR pfd = {0};

  DI6_Anim.hWnd = hWnd;
  DI6_Anim.hDC = GetDC(DI6_Anim.hWnd);

  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(DI6_Anim.hDC, &pfd);

  SetPixelFormat(DI6_Anim.hDC, i, &pfd);
  DI6_Anim.hRC = wglCreateContext(DI6_Anim.hDC);
  wglMakeCurrent(DI6_Anim.hDC, DI6_Anim.hRC);

  if (glewInit() != GLEW_OK)
    exit(0);
  if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
    exit(0);

  DI6_RndShdInit();
  DI6_RndMtlInit();
  DI6_RndTexInit();

  glClearColor(255, 255, 255, 1);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  DI6_RndProjSet();

  /*** Generate font ***/
  hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE, RUSSIAN_CHARSET,
    OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH | FF_SCRIPT, "Bookman Old Style");
  hOldFnt = SelectObject(DI6_Anim.hDC, hFnt);
  wglUseFontBitmaps(DI6_Anim.hDC, 0, 256, 102);
  hOldFnt = SelectObject(DI6_Anim.hDC, hOldFnt);
  DeleteObject(hFnt);

} /* End of 'DI6_RndInit' function */

VOID DI6_RndClose( VOID )
{
  DI6_RndShdClose();
  DI6_RndTexClose();

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(DI6_Anim.hRC);
  ReleaseDC(DI6_Anim.hWnd, DI6_Anim.hDC);
} /* End of 'DI6_RndClose' function */

VOID DI6_RndResize( INT w, INT h )
{
  glViewport(0, 0, w, h);

  DI6_Anim.w = w;
  DI6_Anim.h = h;

  DI6_RndProjSet();
} /* End of 'DI6_RndResize' function */


VOID DI6_RndStart( VOID )
{
  static DBL ReloadTime = 0;

  if (DI6_Anim.Timer.GlobalTime - ReloadTime > 3)
  {
    DI6_RndShdUpdate();
    ReloadTime = DI6_Anim.Timer.GlobalTime;
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
} /* End of 'DI6_RndStart' function */

VOID DI6_RndEnd( VOID )
{
  glFinish();
} /* End of 'DI6_RndEnd' function */

VOID DI6_RndCopyFrame( VOID )
{
  SwapBuffers(DI6_Anim.hDC);
} /* End of 'DI6_RndCopyFrame' function */

/* Generating a pseudorandom number between 0 and 1 function
 * ARGUMENTS: None.
 * RETURNS:
 *   (FLT) - Pseudorandom number;
 */
FLT DI6_Rand0_1( VOID )
{
  return (rand() % RAND_MAX) / (FLT)RAND_MAX;
} /* End of 'DI6_Rand0_1' function */

/* Generating a pseudorandom number between 0 and 1 function
 * ARGUMENTS: None.
 * RETURNS:
 *   (FLT) - Pseudorandom number;
 */
FLT DI6_Rand_1_1( VOID )
{
  return 2 * DI6_Rand0_1() - 1;
} /* End of 'DI6_Rand_1_1' function */

/* End of '' file */