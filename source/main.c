#include <stdlib.h>
#include <errno.h>

#include "logger.h"

int main(int argc, char *argv[])
{
	if ( !logger_initialize() )
		exit(EXIT_FAILURE);
	
	log(INFO, "%s", "hello");
	errno = 5; log(PWARN, "something wrong!");
	errno = 6; log(PERRN, "failed to rollback");
	log(CRTC, "server died %d", argc);

	logger_destroy();

	return 0;
}
