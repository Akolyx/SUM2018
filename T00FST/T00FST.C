#include <windows.h>

void main()
{
  if (MessageBox(NULL, "Are you sure?", "London is situated in Africa", MB_YESNO | MB_ICONQUESTION) == IDNO)
    printf("True\n");
  else
    printf("False\n");
}