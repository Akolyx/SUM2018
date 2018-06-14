/* FILE NAME: TIME.C
 * PROGRAMMER: DI6
 * DATE: 13.06.2018
 * PURPOSE: Functions to track time changes.
 */

#include "anim.h"

UINT64
    StartTime,
    OldTime,
    OldTimeFPS,
    PauseTime,
    TimePerSec,
    FrameCounter;

VOID DI6_TimerInit( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  PauseTime = 0;

  DI6_Anim.Timer.IsPause = FALSE;
  DI6_Anim.Timer.FPS = 30.0;
}

VOID DI6_TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);
  
  DI6_Anim.Timer.GlobalTime = (FLT)(t.QuadPart - StartTime) / TimePerSec;
  DI6_Anim.Timer.GlobalDeltaTime = (FLT)(t.QuadPart - OldTime) / TimePerSec;

  if (DI6_Anim.Timer.IsPause)
  {
    DI6_Anim.Timer.DeltaTime = 0;
    PauseTime += (t.QuadPart - OldTime);
  }
  else
  {
    DI6_Anim.Timer.DeltaTime = DI6_Anim.Timer.GlobalDeltaTime;
    DI6_Anim.Timer.Time = (FLT)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }

  FrameCounter++;

  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    DI6_Anim.Timer.FPS = FrameCounter * TimePerSec / (FLT)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
}

VOID DI6_AnimTimePause( VOID )
{
  DI6_Anim.Timer.IsPause = !DI6_Anim.Timer.IsPause;
}

/*End of 'TIME.C' file */