/*=======================================================================
solunar
mathutil.c
Functions for finding maxima, minima, axis crossings, etc.
(c)2005-2012 Kevin Boone
=======================================================================*/
#include <math.h>
#include "mathutil.h"

void mathutil_get_maxima (double *x, double *y, int npoints, double *mins,
  int maxmins, int *nmins)
{
  *nmins = 0;

  int i;
  for (i = 1; i < npoints - 1  && *nmins < maxmins; i++)
  {
    double y0 = y[i - 1];
    double y1 = y[i];
    double y2 = y[i + 1];
    if (y0 < y1 && y2 < y1)
    {
      double A = (0.5 * (y0 + y2)) - y1;
      double B = (0.5 * (y2 - y0));
      double xExtreme = -B / (2.0 * A);
        double xguess = x[i-1] + ((1 + xExtreme) * (x[i] - x[i-1])); 
        mins[*nmins] = xguess;
        (*nmins)++;
    }
  } 
}

 
void mathutil_get_minima (double *x, double *y, int npoints, double *mins,
  int maxmins, int *nmins)
{
  *nmins = 0;

  int i;
  for (i = 1; i < npoints - 1  && *nmins < maxmins; i++)
  {
    double y0 = y[i - 1];
    double y1 = y[i];
    double y2 = y[i + 1];
    if (y0 > y1 && y2 > y1)
    {
      double A = (0.5 * (y0 + y2)) - y1;
      double B = (0.5 * (y2 - y0));
      double xExtreme = -B / (2.0 * A);
      double xguess = x[i-1] + ((1 + xExtreme) * (x[i] - x[i-1])); 
        mins[*nmins] = xguess;
        (*nmins)++;
    }
  } 
}

// Find zero-crossings using muller's method 
void mathutil_get_positive_axis_crossings (double *x, double *y, 
  int npoints, double *mins, int maxmins, int *nmins)
{
  *nmins = 0;

  int i;
  for (i = 1; i < npoints - 1  && *nmins < maxmins; i++)
  {
    double y0 = y[i - 1];
    double y1 = y[i];
    double y2 = y[i + 1];
    if (y0 < 0 && y1 <= 0 && y2 > 0)
    {
      double root1 = 0.0;
      double root2 = 0.0;
      double A = (0.5 * (y0 + y2)) - y1;
      double B = (0.5 * (y2 - y0));
      double C = y1;
      double xExtreme = -B / (2.0 * A);
      double discriminant = (B * B) - 4.0 * A * C;
      if (discriminant >= 0.0)
      { 
        double DX = 0.5 * sqrt(discriminant) / fabs(A);
        root1 = xExtreme - DX;
        root2 = xExtreme + DX;
        double bisect = 0;
        if (fabs (root2) < 1.0)
          bisect = root2;
        else if (fabs (root1) < 1.0)
          bisect = root1;
        double xguess = x[i] + (bisect * (x[i] - x[i-1])); 
        mins[*nmins] = xguess;
        (*nmins)++;
      }
    }
  } 
}

// Find zero-crossings using muller's method 
void mathutil_get_negative_axis_crossings (double *x, double *y, 
  int npoints, double *mins, int maxmins, int *nmins)
{
  *nmins = 0;

  int i;
  for (i = 1; i < npoints - 1  && *nmins < maxmins; i++)
  {
    double y0 = y[i - 1];
    double y1 = y[i];
    double y2 = y[i + 1];
    if (y0 > 0 && y1 >= 0 && y2 < 0)
    {
      double root1 = 0.0;
      double root2 = 0.0;
      double A = (0.5 * (y0 + y2)) - y1;
      double B = (0.5 * (y2 - y0));
      double C = y1;
      double xExtreme = -B / (2.0 * A);
      double discriminant = (B * B) - 4.0 * A * C;
      if (discriminant >= 0.0)
      { 
        double DX = 0.5 * sqrt(discriminant) / fabs(A);
        root1 = xExtreme - DX;
        root2 = xExtreme + DX;
        double bisect = 0;
        if (fabs (root2) < 1.0)
          bisect = root2;
        else if (fabs (root1) < 1.0)
          bisect = root1;
        double xguess = x[i] + (bisect * (x[i] - x[i-1])); 
        mins[*nmins] = xguess;
        (*nmins)++;
      }
    }
  } 
}

 

