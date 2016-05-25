/*
 * cli_conn.cpp
 *
 *  Created on: 2016Äê5ÔÂ25ÈÕ
 *      Author: LeoBrilliant
 */

#include "opend.h"
#include <fcntl.h>
//#include <stropts.h>

int cli_conn(const char * name)
{
	int fd;
	if((fd = open(name, O_RDWR)) < 0)
		return -1;
/*
	if(isastream(fd) == 0)
	{
		close(fd);
		return -2;
	}
*/

	return fd;
}

