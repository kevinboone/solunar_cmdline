/*=======================================================================
solunar
latlong.h
Definition of the LatLong object
(c)2005-2012 Kevin Boone
=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "latlong.h"


typedef struct _LatLongPriv
  {
  double lat, longt;
  } LatLongPriv;


/*=======================================================================
LatLong_new_parse
=======================================================================*/
LatLong *LatLong_new_parse (const char *s, Error **e)
  {
  double lat, longt;
  int lat_deg, lat_min, long_deg, long_min;
  char lat_sign, long_sign;
  if (sscanf (s, "%c%2d%2d%c%3d%2d", &lat_sign, &lat_deg, 
      &lat_min, &long_sign, &long_deg, &long_min) == 6)
    {
    LatLong *l = LatLong_new_deg_min (lat_deg, lat_min, 
      lat_sign == '-' ? TRUE : FALSE,
      long_deg, long_min, long_sign == '-' ? TRUE : FALSE);
    return l;
    }
  if (sscanf (s, "%lf,%lf", &lat, &longt) == 2)
    {
    LatLong *l = LatLong_new (lat,longt);
    return l;
    }
  *e = Error_new ("Can't parse latitude/longitude string"); 
  return NULL;
  }


/*=======================================================================
LatLong_new
East and North are +ve, West and South are -ve
=======================================================================*/
LatLong *LatLong_new (double lat, double longt)
  {
  LatLong *self = (LatLong *) malloc (sizeof (LatLong));
  self->priv = (LatLongPriv *) malloc (sizeof (LatLongPriv));
  self->priv->lat = lat;
  self->priv->longt = longt;
  return self;
  }


/*=======================================================================
LatLong_new_deg_min
=======================================================================*/
LatLong *LatLong_new_deg_min (int lat_deg, int lat_min, BOOL lat_south, 
    int long_deg, int long_min, BOOL long_south)
  {
  double longt = long_deg + long_min / 60.0;
  double lat = lat_deg + lat_min / 60.0;
  if (lat_south) lat = - lat;
  if (long_south) longt = - longt;
  return LatLong_new (lat, longt);
  }


/*=======================================================================
LatLong_free
=======================================================================*/
void LatLong_free (LatLong *self)
  {
  if (!self) return;
  if (self->priv) free (self->priv);
  free (self);
  }


/*=======================================================================
LatLong_to_string
Caller must free() the string
=======================================================================*/
char *LatLong_to_string (const LatLong *self)
  {
  char buff[100];
  double pos_lat = self->priv->lat;
  if (pos_lat < 0) pos_lat = -pos_lat;
  double pos_longt = self->priv->longt;
  if (pos_longt < 0) pos_longt = -pos_longt;
  int lat_deg = (int) pos_lat;
  int lat_min = (int) (60 * (pos_lat - (double)(lat_deg)));
  int longt_deg = (int) pos_longt;
  int longt_min = (int) (60 * (pos_longt - (double)(longt_deg)));
  snprintf (buff, sizeof (buff), "%02d:%02d%c,%03d:%02d%c",
    lat_deg, lat_min, self->priv->lat > 0 ? 'N' : 'S',
    longt_deg, longt_min, self->priv->longt > 0 ? 'E' : 'W');
  return strdup (buff);
  }

/*=======================================================================
LatLong_clone
=======================================================================*/
LatLong *LatLong_clone (const LatLong *other)
  {
  return LatLong_new (other->priv->lat, other->priv->longt);
  }


/*=======================================================================
LatLong_get_longitude
=======================================================================*/
double LatLong_get_longitude (const LatLong *self)
  {
  return self->priv->longt;
  }


/*=======================================================================
LatLong_get_latitude
=======================================================================*/
double LatLong_get_latitude (const LatLong *self)
  {
  return self->priv->lat;
  }


/*=======================================================================
LatLong_get_latitude
=======================================================================*/
BOOL LatLong_is_southern (const LatLong *self)
  {
  return self->priv->lat < 0;
  }

