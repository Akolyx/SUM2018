#include <windows.h>

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  INT l, r, answer;

  char Buf[200];
  INT m;

  sprintf(Buf, "Hello! You are to choose any number between 1 and 100 and memorize it. Computer will try to find out what number have you chosen");

  MessageBox(NULL, Buf, "Inception", MB_OK | MB_ICONINFORMATION);

  l = 1;
  r = 100;

  while (l < r)
  {
    m = (l + r) / 2;
    sprintf(Buf, "Is your number more (Yes), less (No) or equal (Cancel) to %i?", m);
    answer = MessageBox(NULL, Buf, "Question", MB_YESNOCANCEL | MB_ICONQUESTION);

    if (answer == IDYES)
      l = m + 1;
    else if (answer == IDNO)
      r = m - 1;
    else
    {
      sprintf(Buf, "Your number is %i. Goodbye", m);
      MessageBox(NULL, Buf, "The End", MB_OK);
      return 0;
    }
  }

  sprintf(Buf, "Is your number equal to %i?", l);
  answer = MessageBox(NULL, Buf, "Final question", MB_YESNO | MB_ICONQUESTION);

  if (answer == IDYES)
  { 
    sprintf(Buf, "Your number is %i. Goodbye", l);
    MessageBox(NULL, Buf, "The End", MB_OK);
  }
  else
  {
    sprintf(Buf, "You are lying, and not on the sofa. Goodbye");
    MessageBox(NULL, Buf, "Lier!", MB_OK | MB_ICONERROR);
  }
  return 0;
}