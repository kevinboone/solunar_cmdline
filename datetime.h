/*=======================================================================
solunar
datetime.h
(c)2005-2012 Kevin Boone
=======================================================================*/
#pragma once

#include "defs.h"
#include "error.h"

typedef struct _DateTime
  {
  struct _DateTimePriv *priv;
  } DateTime;


void DateTime_free (DateTime *self);

DateTime *DateTime_new_parse (const char *str, Error **error, const char *tz,
  BOOL utc);

DateTime *DateTime_new_today (void);

DateTime *DateTime_new_centre (const DateTime *d1, const DateTime *d2);

DateTime *DateTime_new_julian (double jd);

char *DateTime_to_string_local (const DateTime *self, const char *tz);

char *DateTime_to_string_syslocal (const DateTime *self);

char *DateTime_to_string_UTC (const DateTime *self);

int DateTime_get_day_of_year (const DateTime *self, const char *tz);

void DateTime_set_time_hours_fraction (DateTime *self, double hours);

DateTime *DateTime_clone (const DateTime *other);

char *DateTime_time_to_string_UTC (const DateTime *self, BOOL twelve_hour);

char *DateTime_time_to_string_local (const DateTime *self, const char *tz, 
    BOOL twelve_hour);

char *DateTime_time_to_string_syslocal (const DateTime *self, 
    BOOL twelve_hour);

double DateTime_get_julian_date (const DateTime *self);
double DateTime_get_modified_julian_date (const DateTime *self);

DateTime *DateTime_get_day_start (const DateTime *self, const char *tz);
DateTime *DateTime_get_day_end (const DateTime *self, const char *tz);

long DateTime_seconds_difference (const DateTime *start, const DateTime *end);
void DateTime_add_seconds (DateTime *self, long seconds);

char *DateTime_date_to_string_syslocal (const DateTime *self);
char *DateTime_date_to_string_local (const DateTime *self, const char *tz);
char *DateTime_date_to_string_UTC (const DateTime *self);

void DateTime_set_name (const DateTime *self, const char *name);
const char* DateTime_get_name (const DateTime *self);

DateTime *DateTime_new_dmy_name (int day, int month, int year, 
    const char *name, const char *tz, BOOL utc);

DateTime *DateTime_new_dmy (int day, int month, int year, 
    const char *tz, BOOL utc);

BOOL DateTime_is_same_day (const DateTime *self, const DateTime *other);
BOOL DateTime_is_same_day_of_year (const DateTime *self, const DateTime *other);

void DateTime_get_ymdhms (const DateTime *self, int *year, int *month, int *day,
  int *hour, int *minute, int *seconds, const char *tz, BOOL utc);

void DateTime_add_days (DateTime *self, int days, const char *tz, BOOL UTC);

DateTime *DateTime_get_jan_first (const DateTime *self, 
  const char *tz, BOOL utc);

DateTime *DateTime_clone_offset_days (const DateTime *dt, int days, 
     const char *name, const char *tz, BOOL utc);

