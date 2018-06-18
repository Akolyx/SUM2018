/* FILE NAME: MOV.C
 * PROGRAMMER: DI6
 * DATE: 18.06.2018
 * PURPOSE: Movement function.
 */
#include <stdio.h>

#include "anim.h"

DBL DI6_Speed( di6MOV *m )
{
  CHAR Buf[100];

  m->Acceleration = min(m->Acceleration + m->Jerk * DI6_Anim.Timer.GlobalDeltaTime, m->AcMax);
  if (m->Speed + m->Acceleration * DI6_Anim.Timer.GlobalDeltaTime, m->SpeedMax > 0)
    m->Speed = m->Speed + m->Acceleration * DI6_Anim.Timer.GlobalDeltaTime, m->SpeedMax;
  else
  {
    m->Speed = m->Acceleration = m->AcEx = 0;
  }

  /*sprintf(Buf, " Speed = %.3f", m->Speed);
  SetWindowText(DI6_Anim.hWnd, Buf);

  memset(Buf, 0, sizeof(Buf));*/

  return m->Speed;
}

/* End of 'MOV.C' file */