/*=======================================================================
solunar
timeutil.c
Various functions for handling time and date
(c)2005-2012 Kevin Boone
=======================================================================*/
#pragma once

#include <time.h>
#include "defs.h" 

#define SECONDS_PER_DAY 86400

#define MAX_DAYS_IN_MONTH 31
#define MONTHS_IN_YEAR 12 
#define JAN 1
#define FEB 2
#define MAR 3
#define APR 4
#define MAY 5
#define JUN 6
#define JUL 7
#define AUG 8
#define SEP 9
#define OCT 10
#define NOV 11
#define DEC 12
#define SUN 0
#define MON 1


/* NOTE all these functions take 'local' time to be either the platform
default or the setting of the TZ environment variable */


extern time_t timeutil_getMidnightLocal (int year, int month, const int day);

extern time_t timeutil_get3AMLocal (int year, int month, int day);

extern time_t timeutil_makeTimeLocal (int year, int month, int day, 
  double hours);

extern time_t timeutil_makeTimeGMT (int year, int month, int day, double hours);

// Local mean sidereal time. Longitude is +ve to the east
extern double timeutil_lmst (double mjd, double longitude);

extern int timeutil_getDayOfYear (int year, int month, int day);

/* Get the unix time as a modified julian date */
double timeutil_unix_to_MJD (time_t t);

/* Get the unix time as a julian date */
double timeutil_unix_to_JD (time_t t);

/* Convert julian to modified julian date */
double timeutil_JD_to_MJD (double t);

/* Convert modified julian to julian date */
double timeutil_MJD_to_JD (double t);

// Convert a YMD-HMS date to a JD. Note that the hour figure is hours
//  past _midnight_ on the specified day, which is common usage. However,
//  The JD is expressed as hours _past noon_. Do not correct the result of
//  this function to allow for the difference between noon and midnight, as
//  it is done internally
double timeutil_ymdhms_to_JD (int _year, int _month, int _day,
  int _hour, int _min, int _sec);

int timeutil_get_days_in_month (int year, int month);

BOOL timeutil_is_leap_year (int year);

int timeutil_get_day_of_week (int year, int month, int day, BOOL monday_first);

int timeutil_get_day_of_week_of_first_jan (int year, int monday_first);

int timeutil_get_day_of_year (int year, int month, int day);

const char *timeutil_get_month_name_english (const int month);

void timeutil_JD_to_DMY (double jd, int *year, int *month, int *day);

double timeutil_unix_to_MJD (time_t t);

