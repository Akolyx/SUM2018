/* FILE NAME: ANIM.H
 * PROGRAMMER: DI6
 * DATE: 11.06.2018
 * PURPOSE: Animation definitions.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "rnd\rnd.h"
#include <time.h>
#include <stdlib.h>

#define DI6_MAX_UNITS 3000

#define UNIT_BASE_FIELDS \
  VOID (*Init)( di6UNIT *Uni, di6ANIM *Ani );      \
  VOID (*Close)( di6UNIT *Uni, di6ANIM *Ani );     \
  VOID (*Response)( di6UNIT *Uni, di6ANIM *Ani );  \
  VOID (*Render)( di6UNIT *Uni, di6ANIM *Ani )

typedef struct tagdi6UNIT di6UNIT;

typedef struct tagdi6TIME
{
  DBL
    GlobalTime,
    GlobalDeltaTime,
    Time,
    DeltaTime,
    FPS;
  BOOL IsPause;
} di6TIME;

typedef struct tagdi6KEYBOARD
{
  BYTE Keys[256], 
       KeysOld[256], 
       KeysClick[256];
} di6KEYBOARD;

typedef struct tagdi6MOUSE
{
  INT x, y, z;
  INT dx, dy, dz;
} di6MOUSE;

typedef struct tagdi6JOYSTICK
{
  BYTE But[32],
       ButOld[32],
       ButClick[32];

  INT Prov;

  DBL x, y, z, r;

  DBL dx, dy, dz, dr;

} di6JOYSTICK;

typedef struct tagdi6ANIM
{
  HWND hWnd;
  HDC hDC;
  HBITMAP hBm;
  INT w, h;
  di6UNIT *Units[DI6_MAX_UNITS]; 
  INT NumOfUnits;
  di6TIME Time;
  di6KEYBOARD Keyboard;
  di6MOUSE Mouse;
  di6JOYSTICK Joystick;
} di6ANIM;

struct tagdi6UNIT
{
  UNIT_BASE_FIELDS;
};

extern di6ANIM DI6_Anim;

VOID DI6_AnimInit( HWND hWnd );
VOID DI6_AnimClose( VOID );
VOID DI6_AnimResize( INT w, INT h );
VOID DI6_AnimCopyFrame( HDC hDC );
VOID DI6_AnimRender( VOID );
VOID DI6_AnimUnitAdd( di6UNIT *Uni );

VOID DI6_UnitInit( di6UNIT *Uni, di6ANIM *Ani );
VOID DI6_UnitClose( di6UNIT *Uni, di6ANIM *Ani );
VOID DI6_UnitResponse( di6UNIT *Uni, di6ANIM *Ani );
VOID DI6_UnitRender( di6UNIT *Uni, di6ANIM *Ani );

di6UNIT * DI6_AnimUnitCreate( INT Size );

VOID DI6_AnimFlipFullScreen( VOID );

/* Response system */
VOID DI6_AnimInputResponse( VOID );

/* Timer */
VOID DI6_TimerInit( VOID );
VOID DI6_TimerResponse( VOID );
VOID DI6_AnimTimePause( VOID );

/* Exit function */
VOID DI6_AnimDoExit ( VOID );

#endif /* __ANIM_H_ */

/* End of 'ANIM.H' file */