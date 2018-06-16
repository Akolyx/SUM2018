/* FILE NAME: U_MEADOW.C
 * PROGRAMMER: DI6
 * DATE: 14.06.2018
 * PURPOSE: Unit cow.
 */

#include "../anim.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  di6PRIM Square;
} di6UNIT_MEADOW;

/* Meadow unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_MEADOW *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitInit( di6UNIT_MEADOW *Uni, di6ANIM *Ani )
{
  //Uni->Square.NumOfI;
} /* End of 'DI6_UnitInit' function */

/* Meadow unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_HELICOPTER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitResponse( di6UNIT_MEADOW *Uni, di6ANIM *Ani )
{

} /* End of 'DI6_UnitResponse' function */

/* Meadow unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       di6UNIT_HELICOPTER *Uni;
 *   - animation context:
 *       di6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DI6_UnitRender( di6UNIT_MEADOW *Uni, di6ANIM *Ani )
{
  
} /* End of 'DI6_UnitRender' function */

/* Meadow unit create function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (di6UNIT *) pointer to created unit
 */
di6UNIT * DI6_UnitCreateMeadow( VOID )
{
  di6UNIT_MEADOW *Uni;

  if ((Uni = (di6UNIT_MEADOW *)DI6_AnimUnitCreate(sizeof(di6UNIT_MEADOW))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DI6_UnitInit;
  Uni->Response = (VOID *)DI6_UnitResponse;
  Uni->Render = (VOID *)DI6_UnitRender;
  Uni->Close = (VOID *)DI6_UnitClose;

  return (di6UNIT *)Uni;
} /* End of 'DI6_UnitCreateBall' function */