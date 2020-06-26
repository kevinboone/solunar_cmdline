<h1>Solunar - a simple command-line utility for calculating Sun and Moon 
rise and set, and related times</h1>

*RETIRED!*

_Please note that this utility is obsolete, and I won't be developing it any_
_further, or fixing any more bugs. I've started an entirely new implementation,_
_which is avilable here:_

https://github.com/kevinboone/solunar2

_Thanks to everybody who tested and contributed to this version over the years._

Version 0.1.3d
<p/>

<code>solunar</code> is a simple, command-line utility for rapidly displaying
sun- and moon-related data such as sunset times and equinoxes. It also
attempts to predict the combined influence of the sun and moon on wildlife
activity. This process -- sometimes called <i>solunar scoring</i> 
or <i>solunar forecasting</i> can be useful to people with an interest in
fishing, wildlife photography, and birdwatching. 
<code>solunar</code> builds under Cygwin on Windows, and 
on most Linux platforms. 
<code>solunar</code> produces output that can be readily parsed by scripts,
although it is reasonably human-readable. The utility 
contains a database of city locations, so locations can be specified
by name, rather than latitude and longitude, although specific latitude
and longitude can also be specified if required. 
<p/>
<code>Solunar</code> can also calculate the dates of lunar calendar
events like Easter.

<h3>Basic usage</h3>

Get sun and moon rise and set times, etc., in London, on the current day:

<pre style="background-color: #FFFFD0; padding: 5px">
$ <b>solunar -c london</b>
Selected city Europe/London
Using location 51:30N,000:07W
Date/time Mon May 21 13:52:52 2012 local

Today
                          Date: Monday 21 May 2012

Sun
                       Sunrise: 04:59
                        Sunset: 20:52

Moon
                    Moon phase: 0.02 new
                      Moonrise: 05:10
                       Moonset: 21:36

</pre> 


Partial city names can be given, so long as the name is unambiguous. To see
a full list of city names, run <code>solunar --cities</code>. Note that
city names containing spaces are represented in <code>solunar</code> with
underscores, as spaces confuse the shell; so <b>-c los_angeles</b> 
(but, int fact, 'angeles' will work, since it is unambiguous)
<p/>
Get sun and moon rise and set times, etc., in Paris, on January 1st, this year:

<pre style="background-color: #FFFFD0; padding: 5px">
<b>solunar -c paris -d "jan 1"</b>
</pre>


Get more detailed information: 

<pre style="background-color: #FFFFD0; padding: 5px">
<b>solunar -f -c paris -d "jan 1"</b>
</pre>


Get brief information for location 51.0 degrees north, 1.5 degrees west,
with times appropriate for the "syslocal" timezone (see 
'Note on timezones' below): 


<pre style="background-color: #FFFFD0; padding: 5px">
<b>solunar -l 51.0,-1.5 -d "jan 1"</b>
</pre>

Note that southerly latitudes are negative, as are westerly longitudes.
For more information on the accepted lat/long formats, 
run <code>solunar -l help</code>.
<p/>
When a city name is given using the <code>-c</code> switch, the times
displayed are local to the timezone in which that city is located.
When latitude and longitude are given alone, then times displayed
are in the syslocal timezone -- usually the timezone set in
the TZ environment variable, or UTC if no timezone is set.
The syslocal timezone will be used whether or not the specified location
happens to be in that timezone --
this may well be unhelpful if you need to know the
times local to that location, when you are in another, different location. 
<code>solunar</code> has no general way to work out
what timezone a specified lat/long location is in. To get local times
for a lat/long location, give the  
<code>-c</code> switch along with <code>-l</code>, naming a city  
in the required timezone.  For
example, the city database has no entry for Washington, DC. The following 
command specifies the Washington DC lat/long coordinates, but displays
the results for the syslocal timezone:

<pre style="background-color: #FFFFD0; padding: 5px">
<b>solunar -l 38.90,-77.03</b>
</pre>

The same command additionally using the name of a nearby city (at least, 
close enough to be in the same timezone) displays 
local times for Washington, DC:

<pre style="background-color: #FFFFD0; padding: 5px">
<b>solunar -l 38.90,-77.03 -c New_York</b>
</pre>

<pre style="background-color: #FFFFD0; padding: 5px">
<b>solunar -l 38.90,-77.03 --syslocal</b>
</pre>

You can also force the use of GMT/UTC times using the <code>--utc</code>
switch.

<p/>
To get a list of equinoxes, etc., for the current year:

<pre style="background-color: #FFFFD0; padding: 5px">
<b>solunar --days</b>
</pre>

This also displays the dates of festivals like Easter which are derived from
the lunar calendar. However, this dating of Easter is the conventional 
Western one, and not all locations observe the same date, even when they 
observe Easter.

<p/>
For a full list of command-line switches:

<pre style="background-color: #FFFFD0; padding: 5px">
<b>solunar --longhelp</b>
</pre>

<h3> RC file</h3>

If the file $HOME/.solunar.rc exists, it will be read for configuration.
Values set in the file can be overridden by the command line. At present,
the only configuration that is read is
<p>
<pre style="background-color: #FFFFD0; padding: 5px">
<b>city=something</b>
</pre>

Setting a city this way allows <code>solunar</code> to be run without
arguments, to get today's timings.

<h3>Useful switches</h3>

<b>-t, --twelvehour</b>: use 12-hour (AM/PM) format for times, rather than
24-hour times.
<p/>
<b>-u, --utc</b>: use UTC (essentially GMT) times for input and output. 
<p/>
<b>-y, --syslocal</b>: use system local times for input and output. 
<p/>
<b>-c, --cities</b>: list cities in database. 
<p/>
<b>-s, --solunar</b>: show solunar scoring information (see below). 
<p/>
<b>--datetime help</b>: show a summary of date/time input formats. 

<h3>Solunar scoring</h3>

The influence of the sun and moon on animal activity -- particularly 
feeding activity -- is frequently noted, but not particularly well
understood. It is widely accepted that many animals are most active
at times near the full and new moons and, within a particular day,
at dawn and dusk. It also seems to be the case that animal activity
is greatest during moon transits (when the moon is directly overhead
or directly 'underfoot') and, to a lesser extent, at moonrise and
mmonset. The Sun's being at its highest point may also be significant, 
because this is when the light and heat from the Sun is most likely
to penetrate folliage and water, to warn the ground or river beds.
<p/>
It is often surmised that good times for
birdwatching, fishing, etc., will be when the significant events related
to the sun and moon coincide -- for example, when the moon is 
directly overhead at dusk. There is little experimental data to support
this surmise, but there is enough anecdotal evidence to make it interesting.
<p/>
To display solunar scoring information, use the <b>-s</b>
or <b>--solunar</b> switches. An example of the output is as follows:

<pre style="background-color: #FFFFD0; padding: 5px">
$ <b>solunar -s -c london -t</b>
Selected city Europe/London
Using location 51:30N,000:07W
Date/time Fri Sep  6 15:01:53 2013 local

Today
                          Date: Friday  6 September 2013

Sun
                       Sunrise:  6:20 am
                        Sunset:  7:35 pm

Moon
                    Moon phase: 0.04 new
                      Moonrise:  7:30 am
                       Moonset:  7:35 pm

Solunar
              Moon phase score: 89%
           Moon distance score: 56%
     Solunar coincidence score: 59%
            Solunar peak times:  6:45 am  1:15 pm  7:45 pm
         Overall solunar score: 68%
</pre>

The <i>moon phase score</i> is a figure indicating how favourable the
moon phase is to wildlife activity. This has a maximum of 100% at
full and new moons, and declines gradually to (a completely arbitrary)
25% at the moon quarters. The <i>moon distance score</i> estimates 
the effect of the closeness of the moon to the earth, with a figure
of 100% at perigee and (again, arbitrarily) 25% at apogee.
<i>Solunar coincidence score</i> is a measure of the overlap between
sun-related and moon-related events on the specified day. It is difficult
to predict the upper limit of this figure; by experiment the algorithm's
parameters have been adjusted to give a figure of 100% on days where there
is the greatest observed overlap between sun and moon influence. On some
days, this figure will be zero; this will happen when, for example, 
sun rise/set/transit times are completely different from moon rise/set/transit
times. The final figure, the overall score, is the unweighted average of
the three previous scores.
<p/>
The <i>solunar peak times</i> shows the times, if any, of the peak
overlap between sun and moon influence scores. This figure does not
indicate the duration or intensity of the overlapping inflence, only
its central time. The variation in duration and intensity can be
considerable. For more detail, use the <b>-f</b> or <b>--full</b> 
switches in conjuction with <b>--solunar</b>. This will display
a chart of the Sun, Moon, and combined influence at 30-minute intervals
for the day. From this the length and intensity of the combined 
influence should be clear.
<p/>
Note that the levels of sun and moon influence in the chart, and used
to derive the solunar peak times display, are relative to the maxima
for that day. That is, the solunar coincidence score could high, and
several peak times shown, but the day could still be inauspicious
because the moon is at apogee (for example). Solunar coincdences
might be of interest in predicting the level of wildlife activity 
on a specified day, but that doesn't mean that there will be an
overall high level of activity on that day. 
<p/>
There is little science to solunar scoring. The program's algorithm
has been tuned to give results that are broadly comparable with
several published almanacs that claim to offer such scoring.


<h3>Notes on the calculations</h3>

With the exception of solunar scoring, the calculations are all based 
on published algorithms, and I have checked
them against reliable data sources so far as possible. There are
potential sources of error that the program can't control, and some
issues of interpretation.
<p/>
The most troublesome potential source of error is the system's timezone
database. By default, results are worked out in UTC and then converted to
the local time of the specified city. This relies on the system's own
time and date being correct, and having a proper understanding of daylight 
savings changes. In general, these issues seems to be handled reasonably well
in modern Linux distributions, but there's not much <code>solunar</code> can
do if the platform data is incorrect.
<p/>
Issues of interpretation include those relating to uncertainty about exactly 
what position of the sun in the sky constitutes sunset (and similar
considerations for the moon.)
The sun is not a uniform disc, so there has to be a convention for the
angle of zenith that we take as sunset. Most publications that give sunset
times seem to take the Zenith angle as 90 degrees and 50 minutes, so 
<code>solunar</code> does the same. However, the <code>--full</code> switch
will make it display sunset according to other popular zeniths.
In particular, civil twilight has a zenith 6 degrees below conventional
sunset, and denotes the time before which outdoor activities are reasonably
practicable. Nautical and astonomical twilight have zeniths 12 and 18 degrees
below conventional sunset. In practice, many parts of the world will experience
no astronomical sunset for at least part of the year. Some, of course,
experience no sunset at all for part of the year. 
<p/>
Although there will always be at most one sunset on a given day, and
one sunrise, there can be zero, one, or two moonrises and sets. So to
capture all these events we have to consider the position of the moon
at a series of time intervals, and then determine the horizon-crossing
points, interpolating if necessary (at least, I have not been able to
find a better way to do this). This means that there is even more scope
for disagreement in lunar event times than solar events. Published
sources seem to vary by +/- ten minutes or so.
<p/>
Solunar scoring -- estimating the influence of sun and moon on wildlife
activity -- is highly speculative, with little scientific justification.
The algorithms used by <code>solunar</code> for this purpose are not
based on any particular science -- there isn't any -- but have been 
tuned to produce output that is broadly comparable with other published
solunar tables.  

<h3>A note on timezones</h3>

There are three timezones that are of potential interest to the
<code>solunar</code> program:
<ul>
<li>The timezone in which the host computer believes it 
is located (which 
<code>solunar</code> refers to as the "syslocal" timezone);</li>
<li>The UTC (GMT) timezone, in which all calculations are carried out; and </li>
<li>The timezone of some selected location (<code>solunar</code> refers
to this as the "local" timezone, and is distinct from the syslocal
timezone).</li>
</ul>

In general, if a particular city is selected (<code>-c</code> switch),
then the timezone of that city is used both to parse input dates,
and to adjust output times to that location. If the <code>--utc</code>
switch is given, then input and output are UTC times. If <code>--syslocal</code>
is given, then input and output will be system local, including allowance for
daylight savings, even if a city is specified. So, for example,
using <code>-c</code> and <code>--syslocal</code> together allow 
<code>solunar</code> to answer questions like "What time is it
here, when it is sunset in Paris?<.
<p/>
If no city is specified, but only a location (latitude/longitude),
then input and output will be system local, unless <code>--utc</code>
is specified. 
<p/>
Internally, "syslocal" is interpreted as meaning "apply no particular
timezone converion beyond what the algorithms insist on." The results
are platform-dependent to some extent but, in practice, both Linux
and Cygwin both take the working timezone from the <code>TZ</code>
environment variable, if it is set, or the file <code>/etc/localtime</code>
if it exists, or UTC if neither is present.

<h3>Limitations</h3>

<ul>
<li>Events like equinoxes are displayed to the nearest minute but, in fact, 
it's hard to get an accuracy of better than 30 minutes or so. These are
annual events, after all. </li>
<li>The --days switch produces a list for the current year. To get a list for a
different year, you will need to specify a full date in that year, even though
only the year part will be used. The times shown for events that have
a time (equinoxes, for example) will be system-local unless you
specify a city (<code>-c</code>) or specify UTC times (<code>--utc</code>).</li>
<li><code>solunar</code> will tell you what time it currently is in a different location if you specify a city, but it won't tell you (for example) 
what UK time corresponds to 9:30 AM in Los Angeles -- it is not intended 
to be a general timezone converter.</li>
<li>If a date is specified but no time, then the time is taken to be 2AM.
So, for example, the distance from the earth to the moon will be given at
2AM on the specificed day. In practice, this seems highly unlikely to be
significant. The use of 2AM, rather than midnight should reduce the likelihood
of a nasty surprise 
when selecting a day at the boundary between daylight
savings changes: midnight local time might end up being in an unexpected
day.</li>
<li><code>solunar</code> has no Unix/Linux <code>man</code> page as yet --
vounteers to write one are most welcome.</li>
</ul>


<h3>Compiling</h3>

Source code is available from <a href="https://github.com/kevinboone/solunar_cmdline" target="_blank">github</a>. You can check out the latest version,
or download and unpack a ZIP file.
Then the usual:

<pre>
% make
% sudo make install
</pre>

Note that <code>solunar</code> uses GNU-cc specific methods of handling
time and date. Consequently it
won't build under MinGW (and won't work even if it can be made to build).
Interestingly it <i>will</i> build for Android using the Native Development
Kit, and does seem to work correctly. It will also build <i>on</i> an
Android device that has a GCC compiler. Of course, this is only useful if you
routinely use the command prompt on Android. For OS/X, you might need
to use the
alternative <code>Makefile.OSX</code>. <code>solunar</code> may build
on other Posix-like plaforms, perhaps with changes to the Makefile. 



<h3>Legal</h3>

<code>solunar</code> is copyright (c)2005-2019 Kevin Boone, and is released
under the GNU Public Licence, version 3.0. That means, essentially, that
you are welcome to use this program however you see fit, at your own risk, so
long as you do not redistribute it without acknowledging the original
author.
<p/>
The algorithms used in the program are published in various textbooks and, 
so far as I know, there are no particular legal restrictions on their
use and distribution. 

<h3>Revision history</h3>

<b>Version 0.1.0, May 2012</b><br/>
First release, based on the original Java implementation of
2005-2011.
<p/>

<b>Version 0.1.1, June 2013</b><br/>
Fixed incorrect text label on moonset.</br>
Fixed a problem with TZ environment handling, that led to a hugely incorrect
timezone compensation on some platforms.</br/>
Added 12-hour time display.</br>
Added solunar scoring feature.</br>
Fixed some minor memory leaks.</br>
Corrected some errors in the documentation.
<p/>

<b>Version 0.1.2, June 2014</b><br/>

Fixed a problem with timezone handling that caused the wrong day to be
used between midnight and 1AM during daylight savings time.
<p/> 
 

<b>Version 0.1.2a, March 2015</b><br/>

Fixed bug in name parsing in parse_zoneinfo.pl (thanks to Cezary Kruk)
<p/> 

<b>Version 0.1.3, January 2016</b><br/>

Added <code>--syslocal</code> switch; various bug fixes.
<p/> 

<b>Version 0.1.3a, January 2016</b><br/>

Fixed a bug in working out when a day started and ended, that affected
mostly pacific timezones. Tidied up the display of the "Today" 
date. Made system local the default timezone for situations where
a location (lat/long) is given, but no city -- this seems to be
what users prefer.
<p/> 

<b>Version 0.1.3b, May 2017</b><br/>

Tidied up Makefile, so that it respected CFLAGS and LDFLAGS environment
variables. This is to make it easier to cross-compile.
<p/> 


<b>Version 0.1.3c, December 2018</b><br/>

Fixed summer/winter naming to depend on hemisphere of selected location.
<p/> 


<b>Version 0.1.3d, November 2019</b><br/>

Preliminary RC file support. Tidied up some printf() usages that
caused complaints from modern GCC versions.
<p/> 


<h3>Downloads</h3>

<code>solunar</code> is already in the repositories for various 
Linux distributions. Where that is not the case, it is easy to build from
source.
Please note that the latest source will nearly alway be 
more recent than any pre-built binaries, and include the latest bug
fixes, some of which are important.



