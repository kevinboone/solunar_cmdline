/*=======================================================================
nameddays.h
(c)2005-2012 Kevin Boone
=======================================================================*/
#pragma once

#include "defs.h"
#include "pointerlist.h"

PointerList *NamedDays_get_list_for_year (int year, const char *tz, BOOL utc);

