#pragma once

#include "defs.h"
#include "pointerlist.h"

PointerList *AstroDays_get_list_for_year (PointerList *l, int year, 
     const char *tz, BOOL utc, BOOL southern);

DateTime *AstroDays_get_vernal_equinox (int year);
DateTime *AstroDays_get_autumnal_equinox (int year);
DateTime *AstroDays_get_summer_solstice (int year, BOOL southern);
DateTime *AstroDays_get_winter_solstice (int year, BOOL southern);

