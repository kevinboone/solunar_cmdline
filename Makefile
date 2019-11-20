.SUFFIXES: .o .c


all: solunar

VERSION=0.1.3d

MYCFLAGS=-Wall -DVERSION=\"$(VERSION)\" $(CFLAGS)
MYLDFLAGS=$(LDFLAGS)

CC=gcc

OBJS=main.o city.o pointerlist.o error.o latlong.o datetime.o suntimes.o roundutil.o trigutil.o timeutil.o moontimes.o mathutil.o holidays.o astrodays.o nameddays.o solunar.o

solunar: $(OBJS)
	$(CC) $(MYLDFLAGS)  -s -o solunar $(OBJS) -lm

.c.o:
	$(CC) $(MYCFLAGS) -o $*.o -c $*.c

clean:
	rm -f *.o solunar

# Uncomment these lines if you want to parse zone.tab into a more
# up-to-date cityinfo.h. And, if your system have a zone.tab. And if you
# have perl. The included data should be OK for most purposes.

#cityinfo.h: /usr/share/zoneinfo/zone.tab parse_zoneinfo.pl
#	./parse_zoneinfo.pl

install:
	mkdir -p $(DESTDIR)/usr/bin
	cp -p solunar $(DESTDIR)/usr/bin/

include dependencies.mak

