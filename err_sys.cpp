/*
 * err_sys.cpp
 *
 *  Created on: 2016Äê5ÔÂ25ÈÕ
 *      Author: LeoBrilliant
 */

#include "opend.h"
#include <errno.h>
#include <stdarg.h>
#include <syslog.h>

extern int log_to_stderr;

static void err_doit(int, int, const char *, va_list);
static void log_doit(int errnoflag, int priority, const char * fmt, va_list ap);

void err_ret(const char * fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
}

void err_sys(const char * fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	exit(1);
}

void err_dump(const char * fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	abort();
}

void err_quit(const char * fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
	exit(1);
}

static void err_doit(int errnoflag, int error, const char * fmt, va_list ap)
{
	char buf[MAXLINE];
	vsnprintf(buf, MAXLINE, fmt, ap);
	if(errnoflag)
	{
		snprintf(buf+strlen(buf), MAXLINE - strlen(buf), ": %s", strerror(error));
	}
	strcat(buf, "\n");
	fflush(stdout);
	fputs(buf, stderr);
	fflush(NULL);
}

void log_open(const char * ident, int option, int facility)
{
	if(log_to_stderr == 0)
		openlog(ident, option, facility);
}

void log_sys(const char * fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}

void log_msg(const char * fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
}

void log_quit(const char * fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}

static void log_doit(int errnoflag, int priority, const char * fmt, va_list ap)
{
	int errno_save;
	char buf[MAXLINE];
	errno_save = errno;
	vsnprintf(buf, MAXLINE, fmt, ap);
	if(errnoflag)
	{
		snprintf(buf+strlen(buf), MAXLINE - strlen(buf), ": %s", strerror(errno_save));
	}
	strcat(buf, "\n");
	if(log_to_stderr)
	{
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}
	else
	{
		syslog(priority, buf);
	}
}
