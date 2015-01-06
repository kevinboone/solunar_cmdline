/*=======================================================================
solunar
trigutil.c
Convenience functions for doing trig in degrees
(c)2005-2012 Kevin Boone
=======================================================================*/
#include <math.h>
#include "trigutil.h"

/**
sin of an angle in degrees
*/
double sinDeg (double deg)
  {
  return sin (deg * 2.0 * M_PI / 360.0);
  }

/**
acos of an angle, result in degrees
*/
double acosDeg (double x)
  {
  return acos (x) * 360.0 / (2 * M_PI);
  }

/**
asin of an angle, result in degrees
*/
double asinDeg (double x)
  {
  return asin (x) * 360.0 / (2 * M_PI);
  }

/**
tan of an angle in degrees
*/
double tanDeg (double deg)
  {
  return tan (deg * 2.0 * M_PI / 360.0);
  }

/**
cos of an angle in degrees
*/
double cosDeg (double deg)
  {
  return cos (deg * 2.0 * M_PI / 360.0);
  }

/**
atan2 of an angle, result in degrees
*/
double atan2Deg (double x, double y)
  {
  return atan2 (x, y) * 360.0 / (2 * M_PI);
  }


/* Reduce an angle to the range 0-360 degrees */
double fixAngle (double angle)
{
  double result = angle - 360.0 * (floor(angle / 360.0));
  return result;
}




