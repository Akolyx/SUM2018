/* FILE NAME   : RNDSHD.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Shaders handle.
 * LAST UPDATE : 15.06.2018.
 */

#include <stdio.h>
#include <string.h>

#include "../../anim.h"

/* Load text from file function.
 * ARGUMENTS:
 *   - file name to be load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text (allocated through malloc).
 */
static CHAR * DI6_RndShdLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  CHAR *txt;
  INT flen;

  if ((F = fopen(FileName, "r")) == NULL)
    return NULL;
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  rewind(F);

  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  memset(txt, 0, flen + 1);
  fread(txt, 1, flen, F);
  fclose(F);
  return txt;
} /* End of 'DI6_RndShdLoadTextFromFile' function */

/* Store log to file function.
 * ARGUMENTS:
 *   - message file prefix, shader name and text:
 *       CHAR *PartName, *Text;
 * RETURNS: None.
 */
static VOID DI6_RndShdLog( CHAR *Prefix, CHAR *Type, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("BIN/SHADERS/DI6_SHD.LOG", "a")) == NULL)
    return;
  fprintf(F, "%s/%s.GLSL:\n%s\n", Prefix, Type, Text);
  fclose(F);
} /* End of 'DI6_RndShdLog' function */

/* Load shader program from .GLSL files function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) load shader program Id.
 */
INT DI6_RndShdLoad( CHAR *ShaderFileNamePrefix )
{
  struct
  {
    CHAR *Name; /* Shader file suffix name */
    INT Type;   /* Shader OpenGL type */
    INT Id;     /* Shader Id created by OpenGL */
  } shd[] =
  {
    {"VERT", GL_VERTEX_SHADER},
    {"FRAG", GL_FRAGMENT_SHADER}
  };
  INT i, prg = 0, res, Ns = sizeof(shd) / sizeof(shd[0]);
  CHAR *txt;
  BOOL is_ok = TRUE;
  static CHAR Buf[3000];

  for (i = 0; is_ok && i < Ns; i++)        
  {                                           
    /* Load shader text from file */
    sprintf(Buf, "%s/%s.GLSL", ShaderFileNamePrefix, shd[i].Name);
    if ((txt = DI6_RndShdLoadTextFromFile(Buf)) == NULL)
    {
      DI6_RndShdLog(ShaderFileNamePrefix, shd[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }
    /* Create shader */
    if ((shd[i].Id = glCreateShader(shd[i].Type)) == 0)
    {
      DI6_RndShdLog(ShaderFileNamePrefix, shd[i].Name, "Error create shader");
      free(txt);
      is_ok = FALSE;
      break;
    }
    /* Attach shader source code to shader */
    glShaderSource(shd[i].Id, 1, &txt, NULL);
    free(txt);

    /* Compile shader */
    glCompileShader(shd[i].Id);
    glGetShaderiv(shd[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
      DI6_RndShdLog(ShaderFileNamePrefix, shd[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }

  if (is_ok)
  {
    if ((prg = glCreateProgram()) == 0)
    {
      is_ok = FALSE;
      DI6_RndShdLog(ShaderFileNamePrefix, "PROG", "Error create program");
    }
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < Ns; i++)
        if (shd[i].Id != 0)
          glAttachShader(prg, shd[i].Id);
      /* Link shader program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        DI6_RndShdLog(ShaderFileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      } 
    }
  }

  if (!is_ok)
  {
    /* Remove all shaders */
    for (i = 0; i < Ns; i++)
      if (shd[i].Id != 0)
      {
        glDetachShader(prg, shd[i].Id);
        glDeleteShader(shd[i].Id);
      }
    /* Remove program */
    if (prg != 0)
      glDeleteProgram(prg);
    prg = 0;
  }

  return prg;
} /* End of 'DI6_RndShdLoad' function */


/* Unload shader program from memory files function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETURNS: None.
 */
VOID DI6_RndShdDelete( INT ProgId )
{
  INT shdrs[5], n, i;

  if (ProgId == 0)
    return;
  glGetAttachedShaders(ProgId, 5, &n, shdrs);
  for (i = 0; i < n; i++)
  {
    glDetachShader(ProgId, shdrs[i]);
    glDeleteShader(shdrs[i]);
  }
  glDeleteProgram(ProgId);
} /* End of 'DI6_RndShdDelete' function */

/***
 * Shader storage functions
 ***/

/* Array of shaders */
di6SHADER DI6_RndShaders[DI6_MAX_SHADERS];
/* Shaders array store size */
INT DI6_RndShadersSize;

/* Shader storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DI6_RndShdInit( VOID )
{
  DI6_RndShdAdd("BIN/SHADERS/DEFAULT");
} /* End of 'DI6_RndShdInit' function */

/* Shader storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DI6_RndShdClose( VOID )
{
  INT i;

  for (i = 0; i < DI6_RndShadersSize; i++)
    DI6_RndShdDelete(DI6_RndShaders[i].ProgId);
  DI6_RndShadersSize = 0;
} /* End of 'DI6_RndShdClose' function */

/* Add shader to stock from file function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) new shader stock number.
 */
INT DI6_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  if (DI6_RndShadersSize >= DI6_MAX_SHADERS)
    return 0;

  strncpy(DI6_RndShaders[DI6_RndShadersSize].Name, ShaderFileNamePrefix, DI6_STR_MAX - 1);
  DI6_RndShaders[DI6_RndShadersSize].ProgId = DI6_RndShdLoad(ShaderFileNamePrefix);

  return DI6_RndShadersSize++;
} /* End of 'DI6_RndShdAdd' function */

/* Update from file all load shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DI6_RndShdUpdate( VOID )
{
  INT i;

  for (i = 0; i < DI6_RndShadersSize; i++)
  {
    DI6_RndShdDelete(DI6_RndShaders[i].ProgId);
    DI6_RndShaders[i].ProgId = DI6_RndShdLoad(DI6_RndShaders[i].Name);
  }
} /* End of 'DI6_RndShdUpdate' function */

/* END OF 'RNDSHD.C' FILE */
