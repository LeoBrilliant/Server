/*
 * open.h
 *
 *  Created on: 2016Äê5ÔÂ25ÈÕ
 *      Author: LeoBrilliant
 */

#ifndef OPEND_H_
#define OPEND_H_

#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

#define CS_OPEN "/home/LeoBrilliant/opend"
#define CL_OPEN "open"
#define MAXLINE 4096

extern int debug;
extern char errmsg[];
extern int oflag;
extern char * pathname;

typedef struct
{
	int fd;
	uid_t uid;
} Client;

extern Client * client;
extern int client_size;
int cli_args(int, char **);
int client_add(int, uid_t);
void client_del(int);
void loop(void);
void request(char *, int, int, uid_t);
int cli_conn(const char *);


void err_ret(const char *, ...);
void err_sys(const char *, ...);
void err_dump(const char *, ...);

#endif /* OPEND_H_ */
