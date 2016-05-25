/*
 * client_man.cpp
 *
 *  Created on: 2016Äê5ÔÂ25ÈÕ
 *      Author: LeoBrilliant
 */

#include "opend.h"

#define NALLOC 10

static void client_alloc(void)
{
	int i;
	if(client == NULL)
	{
		client = (Client*)malloc(NALLOC * sizeof(Client));
	}
	else
	{
		client = (Client*)realloc(client, (client_size + NALLOC) * sizeof(Client));
	}

	if(client == NULL)
	{
		err_sys("can't alloc for client array");
	}

	for(i = client_size; i < client_size + NALLOC; ++i)
	{
		client[i].fd = -1;
	}

	client_size += NALLOC;
}

int client_add(int fd, uid_t uid)
{
	int i;
	if(client == NULL)
	{
		client_alloc();
	}
again:
	for(i = 0; i < client_size; ++i)
	{
		if(client[i].fd == -1)
		{
			client[i].fd = fd;
			client[i].uid = uid;
			return i;
		}
	}
	client_alloc();
	goto again;
	return -1;
}

void client_del(int fd)
{
	int i;
	for(i = 0; i < client_size; ++i)
	{
		if(client[i].fd == fd)
		{
			client[i].fd = -1;
			return;
		}
	}

	log_quit("can't find client entry for fd %d", fd);
}

