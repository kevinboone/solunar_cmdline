/*=======================================================================
solunar
city.h
(c)2005-2012 Kevin Boone
=======================================================================*/
#pragma once

#include "defs.h"
#include "latlong.h"

typedef struct City {
  char *name;
  char *country_code;
  int lat_degrees; 
  int lat_minutes;
  BOOL lat_south;
  int long_degrees;
  int long_minutes;
  BOOL long_west;
} City;

extern City cities[];

struct _PointerList *City_get_matching_name (const char *name);
City *City_new_from_name (const char *name);
void City_free (City *self);
LatLong *City_get_latlong (const City *self);



