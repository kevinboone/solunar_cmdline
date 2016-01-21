/*=======================================================================
solunar
datetime.c
Definition of DateTime object
Stores date and time in a locate-neutral way. The date/time can 
optionally be associated with a name, e.g., 'vernal equinox'
(c)2005-2012 Kevin Boone
=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include "timeutil.h"
#include "datetime.h"

extern char *strptime (const char *s, const char *fmt, struct tm *tm);

typedef struct _DateTimePriv
  {
  time_t utime;
  char *name; // e.g., Christmas day
  } DateTimePriv;


/*=======================================================================
my_setenv
A variant of setenv() that allows a NULL value to call unsetenv; 
setenv() itself does not accept NULL as a value
=======================================================================*/
void my_setenv (const char *name, const char *value, BOOL dummy)
  {
  //printf ("name=%s value=%s\n", name, value); 
  if (value)
    setenv (name, value, 1);  
  else
    unsetenv (name);
  }


/*=======================================================================
getenv_dup
Gets a copy of an environment string. Caller must free it. Note that the
return value might be null (which cannot be freed)
=======================================================================*/
static char *getenv_dup (const char *env)
  {
  char *s = getenv (env);
  if (s) return strdup (s);
  return NULL;
  }

/*=======================================================================
DateTime_new_utime
=======================================================================*/
DateTime *DateTime_new_utime (time_t utime)
  {
  DateTime *self = (DateTime *) malloc (sizeof (DateTime));
  self->priv = (DateTimePriv *) malloc (sizeof (DateTimePriv));
  self->priv->utime = utime;
  self->priv->name = NULL;
  return self;
  }


/*=======================================================================
DateTime_new_julian
=======================================================================*/
DateTime *DateTime_new_julian (double jd)
  {
  DateTime *self = (DateTime *) malloc (sizeof (DateTime));
  self->priv = (DateTimePriv *) malloc (sizeof (DateTimePriv));
  self->priv->utime = (jd - 2440587.5) * 86400;
  self->priv->name = NULL;
  return self;
  }


/*=======================================================================
DateTime_new_centre
Creates a new datetime half-way between the two specified values. 
The result should always be between d1 and d2, but we fix a bug
in this method where sometimes the calculted sunrise comes out exactly
24 hours early, which would make the value of high noon 12 hours
early. It would be better to fix the bug rather than work around it
here, but I can't find it :/
=======================================================================*/
DateTime *DateTime_new_centre (const DateTime *d1, const DateTime *d2)
  {
  DateTime *self = (DateTime *) malloc (sizeof (DateTime));
  self->priv = (DateTimePriv *) malloc (sizeof (DateTimePriv));
  if (d2->priv->utime < d1->priv->utime)
    {
    // Watch out here -- time_t is no big enough to hold the sum of two
    //  times
    self->priv->utime = (time_t) 
      (((uint64_t)d2->priv->utime + (uint64_t)(d1->priv->utime + 24 * 3600)) / 2); 
    }
  else
    {
    self->priv->utime = (time_t) 
      (((uint64_t)d2->priv->utime + (uint64_t)d1->priv->utime) / 2); 
    }
  self->priv->name = NULL;
  return self;
  }


/*=======================================================================
DateTime_set_name
=======================================================================*/
void DateTime_set_name (const DateTime *self, const char *name)
  {
  if (self->priv->name) free (self->priv->name);
  if (name)
    self->priv->name = strdup (name);
  else
    self->priv->name = NULL;
  }


/*=======================================================================
DateTime_get_name
=======================================================================*/
const char* DateTime_get_name (const DateTime *self)
  {
  return self->priv->name;
  }


/*=======================================================================
DateTime_clone
=======================================================================*/
DateTime *DateTime_clone (const DateTime *other)
  {
  DateTime *r = DateTime_new_utime (other->priv->utime);
  DateTime_set_name (r, other->priv->name);
  return r;
  }


/*=======================================================================
DateTime_parse
Parse a string against a format, write results to tm
=======================================================================*/
BOOL DateTime_parse (const char *str, const char *fmt, struct tm *tm)
  {
  time_t now = time (NULL);
  BOOL ret;

  memcpy (tm, localtime (&now), sizeof (struct tm));
  tm->tm_hour = 2;
  tm->tm_min = 0;
  tm->tm_sec = 0;
  tm->tm_isdst = -1;
  char *last = strptime (str, fmt, tm);
  if (tm->tm_year < 50) tm->tm_year += 2000;
  if (last != str + strlen (str))
    ret = FALSE;
  else
    ret = TRUE;

  return ret;
  }

/*=======================================================================
DateTime_new_parse
=======================================================================*/
DateTime *DateTime_new_parse (const char *str, Error **error, const char *tz,
    BOOL utc)
  {
  struct tm tm;
  time_t utime = 0;
  char *oldtz = NULL;

  if (DateTime_parse (str, "%e/%m/%Y %H:%M", &tm)) goto success;
  if (DateTime_parse (str, "%e/%m/%Y", &tm)) goto success;
  if (DateTime_parse (str, "%e/%m %M:M", &tm)) goto success;
  if (DateTime_parse (str, "%e/%m", &tm)) goto success;
  if (DateTime_parse (str, "%b %e %Y %H:%M", &tm)) goto success;
  if (DateTime_parse (str, "%b %e %H:%M", &tm)) goto success;
  if (DateTime_parse (str, "%b %e %Y", &tm)) goto success;
  if (DateTime_parse (str, "%b %e", &tm)) goto success;
  if (DateTime_parse (str, "%H:%M", &tm)) goto success;
  if (DateTime_parse (str, "%j#%Y", &tm)) goto success;

  // Failed
  *error = Error_new ("Can't parse date");
  return NULL;

  success:
  if (utc) tz = "UTC0";
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }
  utime = mktime (&tm);
  if (tz)
    {
    ////if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }

  DateTime *dt = DateTime_new_utime (utime);
  return dt;
  }

/*=======================================================================
DateTime_new_dmy_name
=======================================================================*/
DateTime *DateTime_new_dmy (int day, int month, int year, 
    const char *tz, BOOL utc)
  {
  return DateTime_new_dmy_name (day, month, year, NULL, tz, utc);
  }

/*=======================================================================
DateTime_new_dmy_name
Creates a new datetime at midnight on the specified date. Note that
we need to pass timezone info, because midnight in one zone is not
the same universal time as midnight in another
=======================================================================*/
DateTime *DateTime_new_dmy_name (int day, int month, int year, 
    const char *name, const char *tz, BOOL utc)
  {
  struct tm tm;
  time_t utime = 0;
  char *oldtz = NULL;
  if (utc) tz = "UTC0";
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }
  tm.tm_mday = day;
  tm.tm_mon = month - 1;
  tm.tm_year = year - 1900;
  tm.tm_hour = 0; 
  tm.tm_min = 0; 
  tm.tm_sec = 0; 
  tm.tm_isdst = -1; 
  utime = mktime (&tm);
  if (tz)
    {
    //if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }

  DateTime *r = DateTime_new_utime (utime);
  DateTime_set_name (r, name);
  return r;
  }

/*=======================================================================
DateTime_new_today
=======================================================================*/
DateTime *DateTime_new_today (void)
  {
  return DateTime_new_utime (time (NULL));
  }


/*=======================================================================
DateTime_date_to_string_syslocal
Caller must free string
=======================================================================*/
char *DateTime_date_to_string_syslocal (const DateTime *self)
  {
  struct tm *tm = localtime(&(self->priv->utime));
  char s[100];
  strftime (s, sizeof (s), "%A %e %B %Y", tm);
  return strdup (s);
  }


/*=======================================================================
DateTime_date_to_string_syslocal
Caller must free string
=======================================================================*/
char *DateTime_date_to_string_local (const DateTime *self, const char *tz)
  {
  char *oldtz = NULL;
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }
  struct tm *tm = localtime(&(self->priv->utime));
  char s[100];
  strftime (s, sizeof (s), "%A %e %B %Y", tm);
  if (s[strlen(s) - 1] == 10) s[strlen(s) - 1] = 0;

  if (tz)
    {
    //if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }
  return strdup (s);
  }




/*=======================================================================
DateTime_date_to_string_utc
Caller must free string
=======================================================================*/
char *DateTime_date_to_string_UTC (const DateTime *self)
  {
  struct tm *tm = gmtime(&(self->priv->utime));
  char s[100];
  strftime (s, sizeof (s), "%A %e %B %Y", tm);
  return strdup (s);
  }


/*=======================================================================
DateTime_to_string_local
Caller must free string
If tz is null, gives the local time at the home locale 
=======================================================================*/
char *DateTime_to_string_local (const DateTime *self, const char *tz)
  {
  char *oldtz = NULL;
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }
  char *s = ctime (&(self->priv->utime));
  if (s[strlen(s) - 1] == 10) s[strlen(s) - 1] = 0;
  if (tz)
    {
    //if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }
  return strdup (s);
  }


/*=======================================================================
DateTime_to_string_syslocal
Caller must free string
=======================================================================*/
char *DateTime_to_string_syslocal (const DateTime *self)
  {
  char *s = ctime (&(self->priv->utime));
  if (s[strlen(s) - 1] == 10) s[strlen(s) - 1] = 0;
  return strdup (s);
  }


/*=======================================================================
DateTime_to_string_utc
Caller must free string
=======================================================================*/
char *DateTime_to_string_UTC (const DateTime *self)
  {
  char *oldtz = NULL;
  char *tz = "GMT0";
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }
  char *s = ctime (&(self->priv->utime));
  if (s[strlen(s) - 1] == 10) s[strlen(s) - 1] = 0;
  if (tz)
    {
    //if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }
  return strdup (s);
  }


/*=======================================================================
DateTime_time_to_string_utc
Caller must free string
=======================================================================*/
char *DateTime_time_to_string_UTC (const DateTime *self, BOOL twelve_hour)
  {
  char *oldtz = NULL;
  char *tz = "UTC0";
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }
  struct tm *tm = localtime(&(self->priv->utime));
  //struct tm *tm = gmtime(&(self->priv->utime));
  char s[20];
  if (twelve_hour)
    {
    snprintf (s, sizeof (s), "%02d:%02d %s", 
      tm->tm_hour <= 12  ? tm->tm_hour : tm->tm_hour - 12 , tm->tm_min,
      tm->tm_hour >= 12 ? "pm": "am");
    }
  else
    snprintf (s, sizeof (s), "%02d:%02d", tm->tm_hour, tm->tm_min);
  if (tz)
    {
    //if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }
  return strdup (s);
  }


/*=======================================================================
DateTime_time_to_string_local
Caller must free string
=======================================================================*/
char *DateTime_time_to_string_local (const DateTime *self, const char *tz, 
    BOOL twelve_hour)
  {
  char *oldtz = NULL;
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }
  struct tm *tm = localtime(&(self->priv->utime));
  char s[20];
  if (twelve_hour)
    {
    snprintf (s, sizeof (s), "%2d:%02d %s", 
      tm->tm_hour <= 12  ? tm->tm_hour : tm->tm_hour - 12 , tm->tm_min,
      tm->tm_hour >= 12 ? "pm": "am");
    }
  else
    //snprintf (s, sizeof (s), "%02d:%02d", tm->tm_hour, tm->tm_min);
    //snprintf (s, sizeof (s), "%02d:%02d %d/%d", tm->tm_hour, tm->tm_min, tm->tm_mday, tm->tm_mon + 1);
    snprintf (s, sizeof (s), "%02d:%02d", tm->tm_hour, tm->tm_min);
  if (tz)
    {
    //if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }
  return strdup (s);
  }


/*=======================================================================
DateTime_time_to_string_syslocal
Caller must free string
=======================================================================*/
char *DateTime_time_to_string_syslocal (const DateTime *self, BOOL twelve_hour)
  {
  struct tm *tm = localtime(&(self->priv->utime));
  char s[20];
  if (twelve_hour)
    {
    snprintf (s, sizeof (s), "%2d:%02d %s", 
      tm->tm_hour <= 12  ? tm->tm_hour : tm->tm_hour - 12 , tm->tm_min,
      tm->tm_hour >= 12 ? "pm": "am");
    }
  else
    snprintf (s, sizeof (s), "%02d:%02d", tm->tm_hour, tm->tm_min);
  return strdup (s);
  }


/*=======================================================================
DateTime_free
=======================================================================*/
void DateTime_free (DateTime *self)
  {
  if (!self) return;
  if (self->priv)
    {
    if (self->priv->name) free (self->priv->name);
    free (self->priv);
    }
  free (self);
  }


/*=======================================================================
DateTime_get_day_of_year
=======================================================================*/
int DateTime_get_day_of_year (const DateTime *self, const char *tz)
  {
  struct tm tm;
  char *oldtz = NULL;
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }
  memcpy (&tm, gmtime (&self->priv->utime), sizeof (struct tm));
  if (tz)
    {
    //if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }
  return tm.tm_yday + 1;
  }


/*=======================================================================
DateTime_set_time_hours_fracion
=======================================================================*/
void DateTime_set_time_hours_fraction (DateTime *self, double hours)
  {
  struct tm tm;
  double h, m, s;
  memcpy (&tm, gmtime (&self->priv->utime), sizeof (struct tm));
  h = floor (hours);
  m = floor ((hours - h) * 60);
  s = (hours - h - m / 60) * 3600;
  tm.tm_hour = h;
  tm.tm_min = m;
  tm.tm_sec = s;

  char *oldtz = NULL;
  char *tz = "UTC0";
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }

  self->priv->utime = mktime (&tm);

  if (tz)
    {
    //if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    }

  }


/*=======================================================================
DateTime_get_juian_date
=======================================================================*/
double DateTime_get_julian_date (const DateTime *self)
  {
  double jd = timeutil_unix_to_JD (self->priv->utime);
  return jd; 
  }


/*=======================================================================
DateTime_get_modified_juian_date
=======================================================================*/
double DateTime_get_modified_julian_date (const DateTime *self)
  {
  double jd = timeutil_unix_to_MJD (self->priv->utime);
  return jd; 
  }


/*=======================================================================
DateTime_get_day_start
Caller must free result
Get midnight on the day in which this datetime falls
Umm... I'm not sure if we should take DST into account here
=======================================================================*/
DateTime *DateTime_get_day_start (const DateTime *self, const char *tz)
  {
  char *oldtz = NULL;
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }

  struct tm tm;
  memcpy (&tm, localtime (&self->priv->utime), sizeof (struct tm));
  tm.tm_hour = 0;
  tm.tm_min = 0;
  tm.tm_sec = 0;

  time_t utime = mktime (&tm);

  if (tz)
    {
    //if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }

  DateTime *dt = DateTime_new_utime (utime);

  return dt;
  }


/*=======================================================================
DateTime_get_day_end
Caller must free result
Get 23:59:59 on the day in which this datetime falls
=======================================================================*/
DateTime *DateTime_get_day_end (const DateTime *self, const char *tz)
  {
  char *oldtz = NULL;
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }

  struct tm tm;
  memcpy (&tm, localtime (&self->priv->utime), sizeof (struct tm));
  tm.tm_hour = 23;
  tm.tm_min = 59;
  tm.tm_sec = 59;

  time_t utime = mktime (&tm);

  if (tz)
    {
    if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }

  DateTime *dt = DateTime_new_utime (utime);
  return dt;
  }


/*=======================================================================
DateTime_seconds_difference
=======================================================================*/
long DateTime_seconds_difference (const DateTime *start, const DateTime *end)
  {
  return end->priv->utime - start->priv->utime;
  }


/*=======================================================================
DateTime_add_seconds
=======================================================================*/
void DateTime_add_seconds (DateTime *self, long seconds)
  {
  self->priv->utime += seconds;
  }


/*=======================================================================
DateTime_add_days
Nightmare! We can't just add 24*3600*days seconds, because we might be
crossing at DST boundary. We want the same time on the days a certan
distance from the current day
=======================================================================*/
void DateTime_add_days (DateTime *self, int days, const char *tz, BOOL utc)
  {
  struct tm tm;
  char *oldtz = NULL;
  if (utc) tz = "UTC0";
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }

  memcpy (&tm, localtime (&(self->priv->utime)), sizeof (struct tm));

  // mktime seems to cope with mday values > 31 and < 0, by adjusting
  //  the other fields to match. This even deals with DST. I am unsure
  //  whether this behaviour can be relied up on all systems
  tm.tm_mday += days;

  tm.tm_isdst = -1;
  self->priv->utime = mktime (&tm);

  if (tz)
    {
    //if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }

  }


/*=======================================================================
DateTime_is_same_day
Returns true if the two datetimes fall on the same calendar day
Note -- don't need tz info despite use of gmtime, because if two 
events are on the same day in one zone, they are on the same day in
another, even if they both are on a different day
=======================================================================*/
BOOL DateTime_is_same_day (const DateTime *self, const DateTime *other)
  {
  struct tm self_tm, other_tm;
  struct tm *tm = gmtime(&(self->priv->utime));
  memcpy (&self_tm, tm, sizeof (struct tm));
  tm = gmtime(&(other->priv->utime));
  memcpy (&other_tm, tm, sizeof (struct tm));
  if (self_tm.tm_mday == other_tm.tm_mday 
     && self_tm.tm_mon == other_tm.tm_mon 
     && self_tm.tm_year == other_tm.tm_year) return TRUE;
  return FALSE;
  }


/*=======================================================================
DateTime_is_same_day_of_year
As is_same_day, but ignores the year. This is important for comparing
anniversaries: Christmas day is the 25th December in any year
=======================================================================*/
BOOL DateTime_is_same_day_of_year (const DateTime *self, const DateTime *other)
  {
  struct tm self_tm, other_tm;
  struct tm *tm = gmtime(&(self->priv->utime));
  memcpy (&self_tm, tm, sizeof (struct tm));
  tm = gmtime(&(other->priv->utime));
  memcpy (&other_tm, tm, sizeof (struct tm));
  if (self_tm.tm_mday == other_tm.tm_mday 
     && self_tm.tm_mon == other_tm.tm_mon) return TRUE;
  return FALSE;
  }

/*=======================================================================
DateTime_get_ymdhms
=======================================================================*/
void DateTime_get_ymdhms (const DateTime *self, int *year, int *month, int *day,
      int *hours, int *minutes, int *seconds, const char *tz, BOOL utc)
  { 
  struct tm tm;
  char *oldtz = NULL;
  if (utc) tz = "UTC0";
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }

  memcpy (&tm, localtime (&self->priv->utime), sizeof (struct tm));

  *year = tm.tm_year + 1900;
  *month = tm.tm_mon + 1;
  *day = tm.tm_mday;
  *hours = tm.tm_hour;
  *minutes = tm.tm_min;
  *seconds = tm.tm_sec;

  if (tz)
    {
    //if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }
  }

/*=======================================================================
DateTime_get_jan_first
Get start of year -- midnight on january first. Note the we need tz 
information, as midnight occurs at different universal times in 
different zones.
=======================================================================*/
DateTime *DateTime_get_jan_first (const DateTime *self, 
     const char *tz, BOOL utc)
  {
  struct tm tm;
  char *oldtz = NULL;
  if (utc) tz = "UTC0";
  if (tz)
    {
    oldtz = getenv_dup ("TZ");
    my_setenv ("TZ", tz, 1);
    tzset ();
    }

  memcpy (&tm, localtime (&(self->priv->utime)), sizeof (struct tm));

  tm.tm_mday = 0;
  tm.tm_mon = 0;
  /* tm.tm_year unchanged */
  tm.tm_hour = 0;
  tm.tm_min = 0;
  tm.tm_sec = 0;
  tm.tm_isdst = -1;
  DateTime *r = DateTime_new_utime (mktime (&tm));

  if (tz)
    {
    //if (oldtz)
      {
      my_setenv ("TZ", oldtz, 1);
      if (oldtz) free (oldtz);
      tzset ();
      }
    //unmy_setenv ("TZ");
    }

  return r;
  }


/*=======================================================================
clone_offset_days
Create a new datetime object the specified days offset from the one
supplied
=======================================================================*/
DateTime *DateTime_clone_offset_days (const DateTime *dt, int days, 
     const char *name, const char *tz, BOOL utc) 
  {
  DateTime *r = DateTime_clone (dt);
  DateTime_set_name (r, name);
  DateTime_add_days (r, days, tz, utc);
  return r;
  }



