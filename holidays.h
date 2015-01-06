/*=======================================================================
solunar
holidays.h
(c)2005-2012 Kevin Boone
=======================================================================*/
#pragma once

#include "defs.h"
#include "datetime.h"
#include "pointerlist.h"

DateTime *Holidays_get_easter_sunday (int year, const char *tz, BOOL utc);
PointerList *Holidays_get_list_for_year (PointerList *l, int year, 
     const char *tz, BOOL utc);

