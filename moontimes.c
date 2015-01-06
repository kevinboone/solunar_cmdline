/*=======================================================================
solunar
moontimes.c
Functions for finding moonrise, moonset, etc 
(c)2005-2012 Kevin Boone
=======================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "moontimes.h" 
#include "timeutil.h" 
#include "trigutil.h" 
#include "roundutil.h" 
#include "mathutil.h" 


static double DegRad = M_PI / 180.0;

const double MoonTimes_epoch  = 2444238.5;

/*
* Constants defining the Sun's apparent orbit
*
* Ecliptic longitude of the Sun at epoch 1980.0
*/
const double MoonTimes_elonge = 278.833540;
/*
* Ecliptic longitude of the Sun at perigee
*/
const double MoonTimes_elongp = 282.596403;
/*
* Eccentricity of Earth's orbit
*/
const double MoonTimes_eccent = 0.016718;
/*
* Semi-major axis of Earth's orbit, km
*/
const double MoonTimes_sunsmax = 1.495985e8;
/*
* Sun's angular size, degrees, at semi-major axis distance
*/
const double MoonTimes_sunangsiz = 0.533128;
/*
* Moon mean longitude at epoch
*/
double MoonTimes_mmlong = 64.975464;
/*
* Mean longitude of the perigee at the epoch
*/
double MoonTimes_mmlongp = 349.383063;
/*
* Mean longitude of the node at the epoch
*/
double MoonTimes_mlnode = 151.950429;
/*
* Inclination of the Moon's orbit
*/
double MoonTimes_minc = 5.145396;
/*
* Eccentricity of the Moon's orbit
*/
double MoonTimes_mecc = 0.054900;
/*
* Moon's angular size at distance a from Earth
*/
double MoonTimes_mangsiz = 0.5181;
/*
* Semi-major axis of Moon's orbit in km
*/
double MoonTimes_msmax = 384401.0;
/*
* Parallax at distance a from Earth
*/
double MoonTimes_mparallax = 0.9507;
/*
* Synodic month (new Moon to new Moon)
*/
double MoonTimes_synmonth = 29.53058868;
/*
* Base date for E. W. Brown's numbered series of lunations
*/
double MoonTimes_moonRad = 1737.4;
double MoonTimes_lunatbase = 2423436.0;
/*
* Properties of the Earth
* Radius of Earth in kilometres
*/
double MoonTimes_earthrad = 6378.16;

/*
* Limiting parameter for the Kepler equation.
*/
double MoonTimes_kEpsilon = 1.0e-6;


static double kepler (double m, double ecc)
{
  m *= DegRad;
  double delta, e = m;
  for (delta = 1.0; fabs(delta) > MoonTimes_kEpsilon;)
   {
     delta = e - ecc * sin(e) - m;
     e -= delta / (1 - ecc * cos(e));
   }
  return e;
}

void MoonTimes_get_moon_state_jd (double jd, double *phase, double *age,
   double *distance)
{
  double day = jd - MoonTimes_epoch;
 
  double N = fixAngle((360.0 / 365.2422) * day);                  
  double M = fixAngle (N + MoonTimes_elonge - MoonTimes_elongp);
  double Ec = kepler (M, MoonTimes_eccent);

  Ec = sqrt((1.0 + MoonTimes_eccent) / 
     (1.0 - MoonTimes_eccent)) * tan(Ec / 2.0);
  Ec = 2.0 * atan(Ec) / DegRad; 

  double Lambdasun = fixAngle(Ec + MoonTimes_elongp);

  // Calculate Moon position 
  double ml = fixAngle(13.1763966 * day + MoonTimes_mmlong);        
  double MM = fixAngle(ml - 0.1114041 * day - MoonTimes_mmlongp);   
  double Ev = 1.2739 * sinDeg(2.0 * (ml - Lambdasun) - MM);     
  double Ae = 0.1858 * sinDeg(M);
  double A3 = 0.37 * sinDeg(M);
  double MmP = MM + Ev - Ae - A3;
  double mEc = 6.2886 * sinDeg (MmP);
  double A4 = 0.214 * sinDeg(2 * MmP);
  double lP = ml + Ev + mEc - Ae + A4;
  double V = 0.6583 * sinDeg(2 * (lP - Lambdasun));
  double lPP = lP + V;

  // Calculate Moon phase 

  double MoonAgeDegrees = lPP - Lambdasun;
  double MoonDist = (MoonTimes_msmax * 
    (1.0 - MoonTimes_mecc * MoonTimes_mecc)) /
       (1.0 + MoonTimes_mecc * cosDeg(MmP + mEc));

  double MoonFraction = fixAngle (MoonAgeDegrees) / 360.0;
  *phase = MoonFraction;
  *age = MoonFraction * MoonTimes_synmonth;
  *distance = MoonDist;
}


void MoonTimes_get_lunar_ephemeris (double mjd, double *_ra, double *_dec)
{
  const double CosEPS = 0.91748;
  const double SinEPS = 0.39778;
  const double ARC = 206264.8062;

  const double P2 = M_PI * 2.0;
  const double JD = mjd + 2400000.5; 
  double t = (JD - 2451545.0)/36525.0;

  double L0 = roundutil_pascalFrac(0.606433 + 1336.855225 * t); 
  double L = P2 * roundutil_pascalFrac(0.374897 + 1325.552410 * t);
  double LS = P2 * roundutil_pascalFrac(0.993133 + 99.997361 * t);
  double D = P2 * roundutil_pascalFrac(0.827361 + 1236.853086 * t);
  double F = P2 * roundutil_pascalFrac(0.259086 + 1342.227825 * t);

  double DL =  22640 * sin(L)  -4586 * sin(L - 2*D) +2370 * sin(2*D);
  DL +=  +769 * sin(2*L)  -668 * sin(LS) -412 * sin(2*F);
  DL +=  -212 * sin(2*L - 2*D) -206 * sin(L + LS - 2*D);
  DL +=  +192 * sin(L + 2*D) -165 * sin(LS - 2*D);
  DL +=  -125 * sin(D) -110 * sin(L + LS) +148 * sin(L - LS);
  DL +=   -55 * sin(2*F - 2*D);

  double S = F + (DL + 412 * sin(2*F) + 541* sin(LS)) / ARC;
  double H = F - 2*D;
  double N =   -526 * sin(H)+44 * sin(L + H) -31 * sin(-L + H);
  N +=   -23 * sin(LS + H) +11 * sin(-LS + H) -25 * sin(-2*L + F);
  N +=   +21 * sin(-L + F);

  double L_moon = P2 * roundutil_pascalFrac(L0 + DL / 1296000);
  double B_moon = (18520.0 * sin(S) + N) /ARC;
  double CB = cos(B_moon);
  double X = CB * cos(L_moon);
  double V = CB * sin(L_moon);
  double W = sin(B_moon);
  double Y = CosEPS * V - SinEPS * W;
  double Z = SinEPS * V + CosEPS * W;
  double RHO = sqrt(1.0 - Z*Z);
  double dec = (360.0 / P2) * atan(Z / RHO);
  double ra = (48.0 / P2) * atan(Y / (X + RHO));

  if (ra < 0) ra += 24 ;

  *_ra = ra;
  *_dec = dec;
}


/*=======================================================================
DateTime_getSinAltitude
=======================================================================*/
double MoonTimes_getSinAltitude (double longitude, double latitude, double mjd)
{
  double cosLatitude = cosDeg (latitude);
  double sinLatitude = sinDeg (latitude);
  double ra, dec;
  MoonTimes_get_lunar_ephemeris(mjd, &ra, &dec);
  double TAU = 15.0 * (timeutil_lmst (mjd, longitude) - ra);
  double result	= sinLatitude * sinDeg(dec)
    + cosLatitude * cosDeg(dec) * cosDeg(TAU);
return result;
}


/*=======================================================================
DateTime_get_phase_name
=======================================================================*/
const char *MoonTimes_get_phase_name (double phase)
{
  if (phase < 0 + 0.0625) return "new";
  if (phase < 0.125 + 0.0625) return "waxing crescent";
  if (phase < 0.25 + 0.0625) return "1st quarter";
  if (phase < 0.375 + 0.0625) return "waxing gibbous";
  if (phase < 0.5 + 0.0625) return "full";
  if (phase < 0.625 + 0.0625) return "waning gibbous";
  if (phase < 0.75 + 0.0625) return "3rd quarter";
  if (phase < 0.875 + 0.0625) return "waning crescent";
  return "new";
}

/*=======================================================================
DateTime_get_moon_state
Note that moon phases are inverted for the southern hemisphere, although
the cycle is deemed to start on the same date, and description is
the same 
=======================================================================*/
void MoonTimes_get_moon_state (const DateTime *date, double *phase, 
   double *age, double *distance)
{
  return MoonTimes_get_moon_state_jd (DateTime_get_julian_date (date),
    phase, age, distance);
}


/*=======================================================================
DateTime_get_moon_rises
Determines zero or more moonrises withing the specified start and
end times. Results are written into events[] and *nevents specifies
the number found. The caller must free the contents of events[], if
any 
=======================================================================*/
void MoonTimes_get_moon_rises (const LatLong *latlong, 
       DateTime *start, DateTime *end, int interval, 
       DateTime *events[], int max_events, int *nevents)
{
  long seconds_difference = DateTime_seconds_difference (start, end); 
  int npoints = seconds_difference / interval;

  double *x = (double *) malloc (npoints * sizeof (double));
  double *y = (double *) malloc (npoints * sizeof (double));

  DateTime *tx = DateTime_clone (start);

  int i;
  for (i = 0; i < npoints; i++)
  {
    double alt = MoonTimes_getSinAltitude 
      (LatLong_get_longitude (latlong), LatLong_get_latitude (latlong), 
       DateTime_get_modified_julian_date (tx));
    x[i] = i * interval;
    y[i] = alt;
    DateTime_add_seconds (tx, interval); 
  }

  // Note that x values are in seconds relative to 00:00 on the day
  //  in question

  double *d_events = (double *) malloc (max_events * sizeof (double));
  mathutil_get_positive_axis_crossings (x, y, npoints, d_events, 
    max_events, nevents);

  for (i = 0; i < *nevents; i++) 
    { 
    long offset_from_start = (long) d_events [i]; 
    events[i] = DateTime_clone (start);
    DateTime_add_seconds (events[i], offset_from_start); 
    }
  free (d_events);

  DateTime_free (tx);
  free (x);
  free (y);
}


/*=======================================================================
DateTime_get_moon_sets
Determines zero or more moonrises withing the specified start and
end times. Results are written into events[] and *nevents specifies
the number found. The caller must free the contents of events[], if
any 
=======================================================================*/
void MoonTimes_get_moon_sets (const LatLong *latlong, 
       DateTime *start, DateTime *end, int interval, 
       DateTime *events[], int max_events, int *nevents)
{
  long seconds_difference = DateTime_seconds_difference (start, end); 
  int npoints = seconds_difference / interval;

  double *x = (double *) malloc (npoints * sizeof (double));
  double *y = (double *) malloc (npoints * sizeof (double));

  DateTime *tx = DateTime_clone (start);

  int i;
  for (i = 0; i < npoints; i++)
  {
    double alt = MoonTimes_getSinAltitude 
      (LatLong_get_longitude (latlong), LatLong_get_latitude (latlong), 
       DateTime_get_modified_julian_date (tx));
    x[i] = i * interval;
    y[i] = alt;
    DateTime_add_seconds (tx, interval); 
  }

  // Note that x values are in seconds relative to 00:00 on the day
  //  in question

  double *d_events = (double *) malloc (max_events * sizeof (double));
  mathutil_get_negative_axis_crossings (x, y, npoints, d_events, 
    max_events, nevents);

  for (i = 0; i < *nevents; i++) 
    { 
    long offset_from_start = (long) d_events [i]; 
    events[i] = DateTime_clone (start);
    DateTime_add_seconds (events[i], offset_from_start); 
    }
  free (d_events);

  DateTime_free (tx);
  free (x);
  free (y);
}



/*=======================================================================
SunTimes_get_SA
=======================================================================*/
double MoonTimes_get_SA (const LatLong *latlong, 
    const DateTime *datetime)
  {
  double mjd = DateTime_get_modified_julian_date (datetime);
  return MoonTimes_getSinAltitude (LatLong_get_longitude (latlong), 
    LatLong_get_latitude (latlong), mjd);
  }



