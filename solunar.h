/*=======================================================================
solunar
solunar.h
(c)2005-2013 Kevin Boone
=======================================================================*/

double Solunar_score_solar_sa (double sa, BOOL include_high_noon,
    BOOL include_sun_underfoot, double max_sa, double min_sa);

double Solunar_score_moon (double la, double max_la, double min_la);


double Solunar_score_moon_phase (double phase);

double Solunar_score_moon_distance (double distance);

