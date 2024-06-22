#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "logger.h"

#include "Cruzer-S/termcolor/termcolor.h"

#define LOGGER_LEVEL_INFO 1
#define LOGGER_LEVEL_WARN 2
#define LOGGER_LEVEL_ERRN 3
#define LOGGER_LEVEL_CRTC 4

void define_level(Logger logger, int level, char *name, FILE *fp, char *color)
{
	char format[1024] = "[%d %t][%n:%l:%f] (%p) ";

	strcat(format, color);
	strcat(format, "%s");
	strcat(format, STRINGIFY(fDefault));
	strcat(format, "\n");

	logger_define_level(logger, level, name, fp);
	logger_set_format(logger, level, format);
}

Logger create_my_logger(void)
{
	typedef struct level {
		int level;
		char *name;
		char *color;
		FILE *fp;
	} Level;

	Logger logger;
	Level levels[] = {
	    { LOGGER_LEVEL_INFO, "info", STRINGIFY(fWhite), stdout },
	    { LOGGER_LEVEL_WARN, "warn", STRINGIFY(fOrange), stdout },
	    { LOGGER_LEVEL_ERRN, "errn", STRINGIFY(fMagenta), stderr },
	    { LOGGER_LEVEL_CRTC, "crtc", STRINGIFY(fRed), stderr }
	};
	
	logger = logger_create();
	if (logger == NULL)
		exit(EXIT_FAILURE);

     	for (int i = 0; i < sizeof(levels) / sizeof(Level); i++)
		define_level(
			logger, levels[i].level, levels[i].name,
	       		levels[i].fp, levels[i].color
		);

	return logger;
}

int main(int argc, char *argv[])
{
	Logger logger = create_my_logger();

	if (logger == NULL)
		exit(EXIT_FAILURE);
	
	log(logger, INFO, "%s", "hello");
	log(logger, WARN, "something wrong!");
	log(logger, ERRN, "failed to rollback");
	log(logger, CRTC, "server died %d", argc);

	logger_destroy(logger);

	return 0;
}
