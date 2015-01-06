/*=======================================================================
solunar
roundutil.c
Various rounding functions
(c)2005-2012 Kevin Boone
=======================================================================*/
#include <math.h>
#include "roundutil.h"

double roundutil_roundTowardsZero (const double value)
{
  double result = floor(fabs(value));
  if (value < 0.0)
    result = (-result);
  return (result);	
}

double roundutil_pascalFrac (const double value)
{
  double result = value - roundutil_roundTowardsZero(value);
  if (result < 0.0)
    result += 1.0;
  return (result);
}

