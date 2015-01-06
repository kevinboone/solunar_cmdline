/*=======================================================================
solunar
solunar.c
(c)2005-2012 Kevin Boone
=======================================================================*/
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "defs.h"
#include "city.h"
#include "latlong.h"
#include "error.h"
#include "datetime.h"

#define PERIGEE 363285
#define APOGEE 405503

/*=======================================================================
Solunar_score_solar_sa
The parameters in this function are chosen to give a period of about
one hour with score 1.0 and a further hour at score 0.5, around sunset
and sunrise. The time period at high noon with score 1.0 varies from
around 3 hours at midsummer, to nothing at all in winter
=======================================================================*/
double Solunar_score_solar_sa (double sa, BOOL include_high_noon,
    BOOL include_sun_underfoot, double max_sa, double min_a)
  {
  double score = 0;
  if (include_high_noon && (sa > 0.0))
    {
    score = pow (sa / max_sa, 30.0); 
    if (score < 0.1) score = 0;
    }
  else if (include_sun_underfoot && (score == 0.0))
    {
    // Not implemented yet -- significance uncertain
    }
  if (score == 0.0)
    {
    score = pow ((1-fabs(sa)), 10);
    if (score < 0.1) score = 0;
    }
  return score;
  }


/*=======================================================================
Solunar_score_moon_sa
The parameters in this function are chosen to give a period of about
one hour with score 1.0 and a further hour at score 0.5, around moonset
and moonrise. 
=======================================================================*/
double Solunar_score_moon (double la, double max_la, double min_la)
  {
  //double normalized_la;
  //if (la >= 0) normalized_la = la / max_la;
  //else normalized_la = la / min_la;
 
  double score = 0.0;

  if (la > 0)
    {
    score = pow (la / max_la, 30.0); 
    if (score < 0.1) score = 0;
    }
  else
    {
    score = pow (fabs(la / min_la), 30.0); 
    if (score < 0.1) score = 0;
    }

  if (score == 0.0)
    {
    // Nothing from moon overhead/underfoot -- consider moonrise/set

    score = pow ((1-fabs(la)), 10);
    // Completely arbitrary -- score moonrise/set 50% of moon overhead/underfoot
    //  (since this is what other people seem to do)

    score /= 2.0;

    if (score < 0.1) score = 0;
    }


  return score;
  }


/*=======================================================================
Solunar_score_moon_phase
moon phase score varies between 0.25 at quarters and 1.0 at full and new.
So the score is three times as high at 'peak' periods (full and new moon)
=======================================================================*/
double Solunar_score_moon_phase (double phase)
  {
  return 0.25 + 3.0 * fabs (fabs(phase-0.5)-0.25);
  }


/*=======================================================================
Solunar_score_moon_distance
=======================================================================*/
double Solunar_score_moon_distance (double distance)
  {
  return (1.0 - (distance - PERIGEE) / (APOGEE - PERIGEE)) * 0.75 + 0.25;
  }




