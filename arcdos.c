/*
 * $Header: arcdos.c,v 1.8 88/08/01 15:07:15 hyc Exp $
 */

/*
 * ARC - Archive utility - ARCDOS
 *
 * Version 1.44, created on 07/25/86 at 14:17:38
 *
 * (C) COPYRIGHT 1985 by System Enhancement Associates; ALL RIGHTS RESERVED
 *
 * By:  Thom Henderson
 *
 * Description: This file contains certain DOS level routines that assist in
 * doing fancy things with an archive, primarily reading and setting the date
 * and time last modified.
 *
 * These are, by nature, system dependant functions.  But they are also, by
 * nature, very expendable.
 *
 * Language: Computer Innovations Optimizing C86
 */
#include <stdio.h>
#include "arc.h"

#if	MSDOS
#include <dos.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utime.h>
#include <time.h>
#endif

#if	UNIX
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

struct timeval {	/* man page said <sys/types.h>, but it */
	long tv_sec;	/* really seems to be in <sys/time.h>, */
	long tv_usec;	/* but why bother... */
};
#endif

#if	GEMDOS
#include <osbind.h>
#endif

char	*strcpy(), *strcat(), *malloc();

void
getstamp(f, date, time)		/* get a file's date/time stamp */
	FILE           *f;	/* file to get stamp from */
        unsigned    *date, *time;  /* storage for the stamp */
{
#if	GEMDOS
	int	fd, ret[2];

	fd = fileno(f);
	Fdatime(ret, fd, 0);
	*date = ret[1];
	*time = ret[0];
#endif
#if     UNIX || MSDOS
	struct stat	buf;
	struct tm	*localtime(), *t;

	fstat(fileno(f), &buf);
	t=localtime(&(buf.st_mtime));
	*date = (unsigned short) (((t->tm_year - 80) << 9) +
				((t->tm_mon + 1) << 5) + t->tm_mday);
	*time = (unsigned short) ((t->tm_hour << 11) +
				(t->tm_min << 5) + t->tm_sec / 2);
#endif
}

void
setstamp(f, date, time)		/* set a file's date/time stamp */
	char           *f;	/* filename to stamp */
	unsigned short    date, time;	/* desired date, time */
{
#if     MSDOS
	struct tm	tm;
        struct utimbuf  utb;

        tm.tm_sec  = (time & 31) * 2;
        tm.tm_min  = (time >> 5) & 63;
	tm.tm_hour = (time >> 11);
	tm.tm_mday = date & 31;
        tm.tm_mon  = ((date >> 5) & 15) - 1;
        tm.tm_year = (date >> 9) + 80;

        utb.actime  = mktime(&tm);
        utb.modtime = mktime(&tm);
        utime(f, &utb);
#endif
#if	GEMDOS
	int	fd, set[2];

	fd = Fopen(f, 0);
	set[0] = time;
	set[1] = date;
	Fdatime(set, fd, 1);
	Fclose(fd);
#endif
#if	UNIX
	struct tm	tm;
	struct timeval  tvp[2];
        int     utimes();

	tm.tm_sec = (time & 31) * 2;
	tm.tm_min = (time >> 5) & 63;
	tm.tm_hour = (time >> 11);
	tm.tm_mday = date & 31;
	tm.tm_mon = ((date >> 5) & 15) - 1;
	tm.tm_year = (date >> 9) + 80;
	tvp[0].tv_sec = tmclock(&tm);
	tvp[1].tv_sec = tvp[0].tv_sec;
	tvp[0].tv_usec = tvp[1].tv_usec = 0;
	utimes(f, tvp);
#endif
}

#if	UNIX
int
izadir(filename)		/* Is filename a directory? */
	char           *filename;
{
	struct stat     buf;

	if (stat(filename, &buf) != 0)
		return (0);	/* Ignore if stat fails since */
	else
		return (buf.st_mode & S_IFDIR);	/* bad files trapped later */
}
#endif
