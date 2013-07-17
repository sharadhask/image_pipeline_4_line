/* autofocus 
 */

#include "ipipe.h"

uint16_t autofocus(uint16_t focus, uint32_t edginess, 
                   uint32_t *prev_edginess, int16_t *focus_direction, uint16_t focus_stepsize )
{
 
  int16_t temp;
  temp = focus + (*focus_direction) * (focus_stepsize);
  if (temp < 0)
	temp = 0;
  if (temp > FOCUS_AT_INFINITY)
	temp = FOCUS_AT_INFINITY;

  if (edginess < (*prev_edginess))
	*focus_direction = -1*(*focus_direction);

   *prev_edginess = edginess;
   
   focus = (uint16_t) temp;
   return focus;

}
