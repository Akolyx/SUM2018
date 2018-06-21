/* FILE NAME: MOV.C
 * PROGRAMMER: DI6
 * DATE: 18.06.2018
 * PURPOSE: Movement function.
 */
#include <stdio.h>

#include "anim.h"

DBL DI6_Shift( di6MOV *m , DBL DeltaTime, INT IsAff, BOOL IsCameraFree )
{
  if (DeltaTime == 0)
    return 0;
  else if (IsCameraFree)
  {
    IsAff = 0;
  }

  if (IsAff == 0)
  {
    m->Acceleration *= 0.9;
    m->Speed *= 0.9999;
  }
  else
  {
    m->Acceleration = m->Acceleration + m->AcEx * IsAff * DeltaTime;

    m->Speed = max(min(m->Speed + m->Acceleration * DeltaTime, m->SpeedMax), m->SpeedMin);
  }

  return m->Speed * DeltaTime;
}

/* End of 'MOV.C' file */