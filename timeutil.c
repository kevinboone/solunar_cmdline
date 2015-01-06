/*=======================================================================
solunar
timeutil.c
Various functions for handling time and date
(c)2005-2012 Kevin Boone
=======================================================================*/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "roundutil.h"
#include "timeutil.h"

time_t timeutil_makeTimeGMT (const int year, const int month, const int day, const double hours)
{
	time_t local = timeutil_makeTimeLocal (year, month, day, hours);

	// This is really horrible -- why don't the MS C compilers 
        // support timegm?
	struct tm * tm1 = gmtime (&local);
	int min1 = tm1->tm_hour * 60 + tm1->tm_min; 
	struct tm * tm2 = localtime (&local);
	int min2 = tm2->tm_hour * 60 + tm2->tm_min;
	int min = min2 - min1;
	if (min <= -12 * 60) min = min + 24 * 60;
	return local + 60 * min;
	//return local;
}

time_t timeutil_makeTimeLocal (const int year, const int month, 
  const int day, const double hours)
{
	int seconds = (int)(hours * 3600.0);
	time_t midnight = timeutil_getMidnightLocal (year, month, day);
	return (time_t) (midnight + (time_t)seconds);
}

time_t timeutil_getMidnightLocal (const int year, const int month, const int day)
{
	struct tm t;
	memset (&t, 0, sizeof(t));
	t.tm_sec = 1;
	t.tm_mday = day;
	t.tm_mon = month - 1;
	t.tm_year = year - 1900;
	t.tm_isdst = -1;
	return mktime (&t);
};

time_t timeutil_get3AMLocal (const int year, const int month, const int day)
{
	struct tm t;
	memset (&t, 0, sizeof(t));
	t.tm_mday = day;
	t.tm_mon = month - 1;
	t.tm_year = year - 1900;
	t.tm_hour = 3;
	t.tm_isdst = -1;
	return mktime (&t);
};

/* Calculate local mean sideral time. Longitude is +ve to the east */
double timeutil_lmst (double mjd, double longitude)
  {
  double MJD0 = floor(mjd);
  double UT = (mjd - MJD0) * 24.0;
  double T = (MJD0 - 51544.5) / 36525.0;
  double GMST = 6.697374558
	+ 1.0027379093 * UT
	+ (8640184.812866 + (0.093104 - 6.2E-6 * T) * T) * T / 3600.0;
  double LMST = 24.0 * roundutil_pascalFrac((GMST + longitude / 15.0) / 24.0);
  return (LMST);
  }

/**
Calculate the day of the year, where Jan 1st is day 1.
Note that this method needs to know the year, because
leap years have an impact here
*/
int timeutil_getDayOfYear (int year, int month, int day)
  {
  int N1 = 275 * month / 9;
  int N2 = (month + 9) / 12;
  int N3 = (1 + ((year - 4 * (year / 4) + 2) / 3));
  int N = N1 - (N2 * N3) + day - 30;
  return N;
  }


/* Get the unix time as a modified julian date */
double timeutil_unix_to_MJD (time_t t)
{
  double mjd = timeutil_JD_to_MJD (timeutil_unix_to_JD (t));
  return mjd; 
}


// Convert a YMD-HMS date to a JD. Note that the hour figure is hours past
//  _midnight_ on the specified day, which is common usage. However, The JD is
//  expressed as fractional days _past noon_.  So do not correct the result of
//  this function to allow for the difference between noon and midnight, as it
//  is done internally
double timeutil_ymdhms_to_JD (int _year, int _month, int _day,
  int _hour, int _min, int _sec) 
{
  double year = _year;
  double month = _month;
  double day = _day - 1; 

  double a = floor ((14.0 - month) / 12.0);
  double y = year + 4800.0 - a;
  double m = month + 12.0 * a - 3.0;
  double res = day + floor ((153.0 * m + 2.0) / 5.0) + (365.0 * y) + floor
   (y / 4.0) - floor(y / 100.0) + floor(y / 400.0) - 32045.0 + 0.5;

  double hours = ((double)_hour - 0 * 12.0) + (double)_min / 60;
        
  return  res + hours / 24.0;
}


/* Get the unix time as a julian date */
double timeutil_unix_to_JD (time_t t)
{
  struct tm *tm = gmtime (&t);
  return timeutil_ymdhms_to_JD (tm->tm_year + 1900, tm->tm_mon + 1, 
    tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
}


/* Convert julian to modified julian date */
double timeutil_JD_to_MJD (double t)
{
  return t - 2400000.5;
}

/* Convert modified julian to julian date */
double timeutil_MJD_to_JD (double t)
{
  return t + 2400000.5;
}


int timeutil_get_days_in_month (int year, int month)
{
   static const int monthDays[MONTHS_IN_YEAR] =
      {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

   if (month == FEB) return (timeutil_is_leap_year (year) ? 29 : 28);
   return monthDays [month - 1];

}


BOOL timeutil_is_leap_year (int year)
{
  if ((year%100!=0 || year%400==0) && (year%4==0)) return TRUE;
  return FALSE;
}

// Result is 1-7, not 0-6
int timeutil_get_day_of_week_of_first_jan (int year, int monday_first)
{
  int days = 365*year + (year-1)/4 - (year-1)/100 + (year-1)/400;
  if (monday_first)
    return (days - 1) % 7 + 1;
  else
    return days % 7 + 1;
}


// Jan 1 == 1
int timeutil_get_day_of_year (int year, int month, int day)
{
   int i, days = 0;
   for (i = 1; i < month; i++)
     days += timeutil_get_days_in_month (year, i);
   days += day;
   return days;
}


int timeutil_get_day_of_week (int year, int month, int day, BOOL monday_first)
{
  int dayOne = timeutil_get_day_of_week_of_first_jan (year, monday_first);
  int dayOneFromZero = dayOne - 1;
  int dayOfYear = timeutil_get_day_of_year (year, month, day);
  int dayOfYearFromZero = dayOfYear - 1;
  return (dayOfYearFromZero + dayOneFromZero) % 7;
}


const char *timeutil_get_month_name_english (const int month)
{
  static const char *monthNames[MONTHS_IN_YEAR] =
        {"January", "February", "March", "April", "May", "June", "July",
"August", "September", "October", "November", "December"};
  return monthNames[month - 1];
}


void timeutil_JD_to_DMY (double jd, int *_year, int *_month, int *_day)
{
    double _val = jd;
    //double ut ;
    int jdn ;
    int year, month, day ;
    BOOL julian ;
    long x, z, m, d, y ;
    long daysPer400Years = 146097L ;
    long fudgedDaysPer4000Years = 1460970L + 31 ;
    long LASTJULJDN = 2299160L ;

    double val = _val + 0.5 ; //  Convert astronomical JDN to chronological

    jdn = (int)floor(val) ;
    //ut = val - jdn ;
    julian = (jdn <= LASTJULJDN) ;
    x = jdn + 68569L ;

    if (julian)
      {
      x += 38 ;
      daysPer400Years = 146100L ;
      fudgedDaysPer4000Years = 1461000L + 1 ;
      }
    z = 4 * x / daysPer400Years ;
    x = x - (daysPer400Years * z + 3) / 4 ;
    y = 4000 * (x + 1) / fudgedDaysPer4000Years ;
    x = x - 1461 * y / 4 + 31 ;
    m = 80 * x / 2447 ;
    d = x - 2447 * m / 80 ;
    x = m / 11 ;
    m = m + 2 - 12 * x ;
    y = 100 * (z - 49) + y + x ;
    year = (int)y ;
    month = (int)m ;
    day = (int)d ;
    if (year <= 0) // adjust BC years
      year-- ;

    *_year = year;
    *_month = month;
    *_day = day;
}

const int timeutil_SECONDS_PER_DAY = 24 * 60 * 60;


