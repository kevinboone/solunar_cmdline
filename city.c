/*=======================================================================
solunar
city.c
Functions for finding latitude and longitude of cities
(c)2005-2012 Kevin Boone
=======================================================================*/
#define _GNU_SOURCE
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "city.h"
#include "pointerlist.h"

/*=======================================================================
City_get_matching_name
Returns a PointerList of pointer to char*. Caller must
call PointerList_free (result, TRUE) to free. 
=======================================================================*/
PointerList *City_get_matching_name (const char *name)
  {
  PointerList *list = NULL;
  City *city = cities;
  while (city->name)
    {
    if (strcasestr (city->name, name))
      list = PointerList_append (list, strdup (city->name)); 
    city++;
    };

  return list;
  }

/*=======================================================================
City_new_from_name
Returns a city object whose name matches the supplied name, or NULL
if no match. Note that the name is expected to be unambiguous
=======================================================================*/
City *City_new_from_name (const char *name)
  {
  City *city = cities;
  while (city->name)
    {
    if (strcmp (city->name, name) == 0)
      {
      City *c = (City *) malloc (sizeof (City));
      memcpy (c, city, sizeof (City));
      c->country_code = strdup (city->country_code);
      c->name = strdup (city->name);
      return c;
      }
    city++;
    };
  return NULL;
  }

/*=======================================================================
City_free
=======================================================================*/
void City_free (City *self)
  {
  if (!self) return;
  if (self->country_code) free (self->country_code);
  if (self->name) free (self->name);
  free (self);
  }


/*=======================================================================
City_get_latlong
Returns this city location as a LatLong object. The caller must 
call LatLong_free() on the return, which will always be non-null
=======================================================================*/
LatLong *City_get_latlong (const City *self)
  {
  LatLong *l = LatLong_new_deg_min (self->lat_degrees, self->lat_minutes,
    self->lat_south, self->long_degrees, self->long_minutes, self->long_west);
  return l;
  }


