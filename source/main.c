#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "logger.h"

int main(int argc, char *argv[])
{
	Logger logger = logger_create();

	if (logger == NULL)
		exit(EXIT_FAILURE);

	logger_use_default_form(logger);
	
	log(logger, INFO, "%s", "hello");
	errno = 5; log(logger, PWARN, "something wrong!");
	errno = 6; log(logger, PERRN, "failed to rollback");
	log(logger, CRTC, "server died %d", argc);

	logger_destroy(logger);

	return 0;
}
