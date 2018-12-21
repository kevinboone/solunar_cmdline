/*=======================================================================
nameddays.c
Methods for getting the dates of days with names. Aggregates the
information from methods in holidays.c and astrodays.c 
(c)2005-2012 Kevin Boone
=======================================================================*/
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "datetime.h"
#include "astrodays.h"
#include "holidays.h"

/*=======================================================================
NamedDays_get_list_for_year 
=======================================================================*/
PointerList *NamedDays_get_list_for_year (int year, const char *tz, BOOL utc,
    BOOL southern)
  {
  PointerList *l = NULL;
  l = Holidays_get_list_for_year (l, year, tz, utc);
  l = AstroDays_get_list_for_year (l, year, tz, utc, southern);
  return l;
  }

