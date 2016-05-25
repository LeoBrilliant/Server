/*
 * loop.cpp
 *
 *  Created on: 2016Äê5ÔÂ25ÈÕ
 *      Author: LeoBrilliant
 */


#include "opend.h"
#include <poll.h>

void loop(void)
{
	int i, maxi, listenfd, clifd, nread;
	char buf[MAXLINE];
	uid_t uid;
	struct pollfd * pollfds;

	if((pollfds = (pollfd *) malloc(open_max() * sizeof(struct pollfd))) == NULL)
	{
		err_sys("malloc error");
	}

	if((listenfd = serv_listen(CS_OPEN)) < 0)
	{
		log_sys("serv_listen error");
	}

	client_add(listenfd, 0);
	pollfds[0].fd = listenfd;
	pollfds[0].events = POLLIN;
	maxi = 0;

	while(true)
	{
		if(poll(pollfds, maxi + 1, -1) < 0)
		{
			log_sys("poll error");
		}

		if(pollfds[0].revents & POLLIN)
		{
			if((clifd = serv_accept(listenfd, &uid)) < 0)
			{
				log_sys("serv_accept error: %d", clifd);
			}

			i = client_add(clifd, uid);
			pollfds[i].fd = clifd;
			pollfds[i].events = POLLIN;

			if(i > maxi)
			{
				maxi = i;
			}

			log_msg("new connection: uid %d, fd %d", uid, clifd);
		}

		for(i = 1; i <= maxi; ++i)
		{
			if((clifd = client[i].fd) < 0)
			{
				continue;
			}

			if(pollfds[i].revents & POLLHUP)
			{
				goto hungup;
			}
			else if(pollfds[i].revents & POLLIN)
			{
				if((nread = read(clifd, buf, MAXLINE)) < 0)
				{
					log_sys("read error on fd %d", clifd);
				}
				else if(nread == 0)
				{
hungup:
					log_msg("closed: uid %d, fd %d", client[i].uid, clifd);
					client_del(clifd);
					pollfds[i].fd = -1;
					close(clifd);
				}
				else
				{
					request(buf, nread, clifd, client[i].uid);
				}
			}
		}
	}
}

