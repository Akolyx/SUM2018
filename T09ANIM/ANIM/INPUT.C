/* FILE NAME: INPUT.C
 * PROGRAMMER: DI6
 * DATE: 13.06.2018
 * PURPOSE: Functions to work with input devices.
 */

#include "anim.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm")

extern DI6_MouseWheel;

VOID DI6_AnimInputResponse( VOID )
{
  INT i;

  POINT pt;

  /*Keyboard response */

  GetKeyboardState(DI6_Anim.Keyboard.Keys);
  for (i = 0; i < 256; i++)
  {
    DI6_Anim.Keyboard.Keys[i] >>= 7;
    DI6_Anim.Keyboard.KeysClick[i] = DI6_Anim.Keyboard.Keys[i] && !DI6_Anim.Keyboard.KeysOld[i];
  }

  memcpy(DI6_Anim.Keyboard.KeysOld, DI6_Anim.Keyboard.Keys, 256);

  /* Mouse response */

  GetCursorPos(&pt);
  ScreenToClient(DI6_Anim.hWnd, &pt);
  
  DI6_Anim.Mouse.dx = pt.x - DI6_Anim.Mouse.x;
  DI6_Anim.Mouse.dy = pt.y - DI6_Anim.Mouse.y;
  
  DI6_Anim.Mouse.x = pt.x;
  DI6_Anim.Mouse.y = pt.y;

  DI6_Anim.Mouse.dz = DI6_MouseWheel;
  DI6_Anim.Mouse.z += DI6_MouseWheel;
  DI6_MouseWheel = 0;

  /* Joystick response */

  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          DI6_Anim.Joystick.But[i] = (ji.dwButtons >> i) & 1;
          DI6_Anim.Joystick.ButClick[i] = DI6_Anim.Joystick.But[i] && !DI6_Anim.Joystick.ButOld[i];
          DI6_Anim.Joystick.ButOld[i] = DI6_Anim.Joystick.But[i];
        }

        /* Axes */

        DI6_Anim.Joystick.x = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin) - 1;
        DI6_Anim.Joystick.y = 2.0 * (ji.dwXpos - jc.wYmin) / (jc.wYmax - jc.wYmin) - 1;
        DI6_Anim.Joystick.z = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin) - 1;
        DI6_Anim.Joystick.r = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin) - 1;

        /* Point of view */
        DI6_Anim.Joystick.Prov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
}