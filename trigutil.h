/*=======================================================================
solunar
trigutil.h
(c)2005-2012 Kevin Boone
=======================================================================*/
#pragma once

/**
sin of an angle in degrees
*/
extern double sinDeg (double deg);

/**
acos of an angle, result in degrees
*/
extern double acosDeg (double x);

/**
asin of an angle, result in degrees
*/
extern double asinDeg (double x);

/**
tan of an angle in degrees
*/
extern double tanDeg (double deg);

/**
cos of an angle in degrees
*/
extern double cosDeg (double deg);

/**
atan2 of an angle, result in degrees
*/
extern double atan2Deg (double x, double y);

/* Reduce an angle to the range 0-360 degrees */
double fixAngle (double angle);

