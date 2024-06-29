#include "logger.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <threads.h>
#include <errno.h>

#define LOGGER_BUFSIZ		BUFSIZ

#include "Cruzer-S/termcolor/termcolor.h"

struct logger {
	LoggerLevel level[LOGGER_MAX_LEVEL];
};

static const int default_level_index[] = {
	LOGGER_LEVEL_INFO, LOGGER_LEVEL_PINFO,
	LOGGER_LEVEL_WARN, LOGGER_LEVEL_PWARN,
	LOGGER_LEVEL_ERRN, LOGGER_LEVEL_PERRN,
	LOGGER_LEVEL_CRTC, LOGGER_LEVEL_PCRTC,
	LOGGER_LEVEL_VERB, LOGGER_LEVEL_PVERB
};

static struct logger_level *logger_level_init = &(struct logger_level) {
	.level = -1, .name = NULL, .format = NULL, .stream = NULL
};

static struct logger_level default_level[] = {
      { .level = LOGGER_LEVEL_INFO, .name = "info", .stream = (FILE *) -1,
	.format = "[%d %t][%n:%f:%l] (%p) "
	          STRINGIFY(fWhite) "%s" STRINGIFY(fDefault) "\n"	     },

      { .level = LOGGER_LEVEL_PINFO, .name = "info", .stream = (FILE *) -1,
	.format = "[%d %t][%n:%f:%l] (%p) "
		  STRINGIFY(fWhite) "%s: %e" STRINGIFY(fDefault) "\n"	     },

      { .level = LOGGER_LEVEL_WARN, .name = "warning", .stream = (FILE *) -1,
	.format = "[%d %t][%n:%f:%l] (%p) "
		  STRINGIFY(fOrange) "%s" STRINGIFY(fDefault) "\n"	     },

      { .level = LOGGER_LEVEL_PWARN, .name = "warning", .stream = (FILE *) -1,
	.format = "[%d %t][%n:%f:%l] (%p) "
		  STRINGIFY(fOrange) "%s: %e" STRINGIFY(fDefault) "\n"	     },

      { .level = LOGGER_LEVEL_ERRN, .name = "error", .stream = (FILE *) -2,
	.format = "[%d %t][%n:%f:%l] (%p) "
		  STRINGIFY(fMagenta) "%s" STRINGIFY(fDefault) "\n"	     },

      { .level = LOGGER_LEVEL_PWARN, .name = "error", .stream = (FILE *) -2,
	.format = "[%d %t][%n:%f:%l] (%p) "
		  STRINGIFY(fMagenta) "%s: %e" STRINGIFY(fDefault) "\n"	     },

      { .level = LOGGER_LEVEL_CRTC, .name = "critical", .stream = (FILE *) -2,
	.format = "[%d %t][%n:%f:%l] (%p) "
		  STRINGIFY(fRed) "%s" STRINGIFY(fDefault) "\n"		     },

      { .level = LOGGER_LEVEL_PCRTC, .name = "critical", .stream = (FILE *) -2,
	.format = "[%d %t][%n:%f:%l] (%p) "
		  STRINGIFY(fRed) "%s: %e" STRINGIFY(fDefault) "\n"          },

      { .level = LOGGER_LEVEL_VERB, .name = "verbose", .stream = (FILE *) -2,
	.format = "[%d %t][%n:%f:%l] (%p) "
		  STRINGIFY(fGrey) "%s" STRINGIFY(fDefault) "\n"	     },

      { .level = LOGGER_LEVEL_PVERB, .name = "verbose", .stream = (FILE *) -2,
	.format = "[%d %t][%n:%f:%l] (%p) "
		  STRINGIFY(fGrey) "%s: %e" STRINGIFY(fDefault) "\n"         }

};

Logger logger_create(void)
{
	Logger logger = malloc(sizeof(struct logger));
	if (logger == NULL)
		return NULL;

	for (int i = 0; i < LOGGER_MAX_LEVEL; i++)
		logger->level[i] = logger_level_init;

	return logger;
}

bool logger_define_level(
		Logger logger, LoggerLevel level
) {
	LoggerLevel new_level;

	if (level->level < 0 || level->level >= LOGGER_MAX_LEVEL)
		goto RETURN_FALSE;

	if (logger->level[level->level] != logger_level_init)
		goto RETURN_FALSE;

	new_level = malloc(sizeof(struct logger_level));
	if (new_level == NULL)
		goto RETURN_FALSE;

	new_level->name = strdup(level->name);
	if (new_level->name == NULL)
		goto FREE_NEW_LEVEL;

	new_level->format = strdup(level->format);
	if (new_level->format == NULL)
		goto FREE_NAME;

	new_level->stream = level->stream;
	new_level->level = level->level;

	logger->level[new_level->level] = new_level;

	return true;

FREE_NAME:	free(new_level->name);
FREE_NEW_LEVEL:	free(new_level);
RETURN_FALSE:	return false;
}

static char *get_time_string(char *format)
{
	thread_local static char buffer[128];
	time_t t = time(NULL);

	strftime(buffer, sizeof buffer, format, localtime(&t));

	return buffer;
}

void logger_use_default_form(Logger logger)
{
	const int nlevel = sizeof(default_level) / sizeof(struct logger_level);

	for (int i = 0; i < nlevel; i++) {
		if (default_level[i].stream == (FILE *) -1)
			default_level[i].stream = stdout;

		if (default_level[i].stream == (FILE *) -2)
				default_level[i].stream = stderr;

		logger->level[i] = &default_level[i];
	}
}

int logger_log(
	Logger logger, int level,
	const char *file, const char *function, const char *line,
	const char *formatted, ...
) {
	va_list list;
	int retval;

	char buffer[LOGGER_BUFSIZ];

	char *buf_ptr, *copy_ptr;
	char *fmt_start, *fmt_end;

	size_t copy_len, remain_len;

	int errn = errno; // errno can be changed by standard libraries
	
	fmt_start = logger->level[level]->format;
	buf_ptr = buffer;
	remain_len = sizeof buffer - 1;
 
	while (remain_len > 0 && fmt_start && *fmt_start) {
		if (*fmt_start != '%') {
			fmt_end = strchr(fmt_start, '%');
			if (fmt_end == NULL)
				copy_len = strlen(fmt_start);
			else
				copy_len = fmt_end - fmt_start;

			copy_ptr = fmt_start;

			fmt_start = fmt_end;
		} else {
			switch ( *(fmt_start + 1) ) {
			case 'l': copy_ptr = (char *) line; break;
			case 'f': copy_ptr = (char *) function; break;
			case 'n': copy_ptr = (char *) file; break;
			case 's': copy_ptr = (char *) formatted; break;
			case 'd': copy_ptr = get_time_string("%x"); break;
			case 't': copy_ptr = get_time_string("%X"); break;
			case 'p': copy_ptr = logger->level[level]->name; break;
			case 'e': copy_ptr = strerror(errn); break;
			}

			copy_len = strlen(copy_ptr);
			fmt_start += 2;
		}

		if (copy_len >= remain_len)
			copy_len = remain_len;

		strncpy(buf_ptr, copy_ptr, copy_len);
		buf_ptr += copy_len;

		remain_len -= copy_len;
	}

	buffer[(sizeof(buffer) - remain_len) - 1] = '\0';

	va_start(list, formatted);

	retval = vfprintf(logger->level[level]->stream, buffer, list);

	va_end(list);

	return retval;
}

void logger_destroy(Logger logger)
{
	for (int i = 0; i < LOGGER_MAX_LEVEL; i++) {
		if (logger->level[i] == logger_level_init)
			continue;

		if (logger->level[i] == &default_level[i])
			continue;

		free(logger->level[i]->name);
		free(logger->level[i]->format);
		free(logger->level[i]);
	}

	free(logger);
}
