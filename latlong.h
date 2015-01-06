/*=======================================================================
solunar
latlong.h
(c)2005-2012 Kevin Boone
=======================================================================*/
#pragma once

#include "defs.h"
#include "error.h"

typedef struct _LatLong
  {
  struct _LatLongPriv *priv;
  } LatLong;


void LatLong_free (LatLong *self);
LatLong *LatLong_new_parse (const char *str, Error **error);
LatLong *LatLong_new (double lat, double longt);
LatLong *LatLong_new_deg_min (int lat_deg, int lat_min, BOOL lat_south, 
    int long_deg, int long_min, BOOL long_south);
LatLong *LatLong_clone (const LatLong *other);
char *LatLong_to_string (const LatLong *self);
double LatLong_get_latitude (const LatLong *self);
double LatLong_get_longitude (const LatLong *self);
BOOL LatLong_is_southern (const LatLong *self);

