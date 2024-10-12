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

#define DEFINE_LEVEL(L, N, S, F) {			\
	.level = (LOGGER_LEVEL_##L), .name = (N),	\
	.stream = (FILE *) S, .format = (F)		\
}

#include "Cruzer-S/termcolor/termcolor.h"

struct logger {
	LoggerLevel level[LOGGER_MAX_LEVEL];
} *logger;

static const int default_level_index[] = {
	LOGGER_LEVEL_INFO, LOGGER_LEVEL_PINFO,
	LOGGER_LEVEL_WARN, LOGGER_LEVEL_PWARN,
	LOGGER_LEVEL_ERRN, LOGGER_LEVEL_PERRN,
	LOGGER_LEVEL_CRTC, LOGGER_LEVEL_PCRTC,
	LOGGER_LEVEL_VERB, LOGGER_LEVEL_PVERB
};

static LoggerLevel logger_level_init = &(struct logger_level) {
	.level = -1, .name = NULL, .format = NULL, .stream = NULL
};

static struct logger_level default_level[] = {
	DEFINE_LEVEL(INFO, "info", -1, "[%d %t][%n:%f:%l] (%p) "
		     STRINGIFY(fWhite) "%s" STRINGIFY(fDefault) "\n"),

      	DEFINE_LEVEL(PINFO, "info", -1, "[%d %t][%n:%f:%l] (%p) "
		     STRINGIFY(fWhite) "%s: %e" STRINGIFY(fDefault) "\n"),

      	DEFINE_LEVEL(WARN, "warning", -1, "[%d %t][%n:%f:%l] (%p) "
		     STRINGIFY(fOrange) "%s" STRINGIFY(fDefault) "\n"),

      	DEFINE_LEVEL(PWARN, "warning", -1, "[%d %t][%n:%f:%l] (%p) "
		     STRINGIFY(fOrange) "%s: %e" STRINGIFY(fDefault) "\n"),

	DEFINE_LEVEL(ERRN, "error", -2, "[%d %t][%n:%f:%l] (%p) "
	      	     STRINGIFY(fMagenta) "%s" STRINGIFY(fDefault) "\n"),
	
	DEFINE_LEVEL(PWARN, "error", -2, "[%d %t][%n:%f:%l] (%p) "
		     STRINGIFY(fMagenta) "%s: %e" STRINGIFY(fDefault) "\n"),

      	DEFINE_LEVEL(CRTC, "critical", -2, "[%d %t][%n:%f:%l] (%p) "
		     STRINGIFY(fRed) "%s" STRINGIFY(fDefault) "\n"),

      	DEFINE_LEVEL(PCRTC, "critical", -2, "[%d %t][%n:%f:%l] (%p) "
		     STRINGIFY(fRed) "%s: %e" STRINGIFY(fDefault) "\n"),

      	DEFINE_LEVEL(VERB,  "verbose", -2, "[%d %t][%n:%f:%l] (%p) "
		     STRINGIFY(fLightGray) "%s" STRINGIFY(fDefault) "\n"),

      	DEFINE_LEVEL(PVERB, "verbose", -2, "[%d %t][%n:%f:%l] (%p) "
		     STRINGIFY(fLightGray) "%s: %e" STRINGIFY(fDefault) "\n")
};

static char *get_time_string(char *format)
{
	thread_local static char buffer[128];
	time_t t = time(NULL);

	strftime(buffer, sizeof buffer, format, localtime(&t));

	return buffer;
}

int logger_log(int level,
	       const char *file, const char *function, const char *line,
	       const char *formatted, ...)
{
	va_list list;
	int retval;

	char buffer[LOGGER_BUFSIZ];

	char *buf_ptr, *copy_ptr;
	char *fmt_start, *fmt_end;

	size_t copy_len, remain_len;

	if ( !logger )
		return -1;

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


bool logger_initialize(void)
{
	const int nlevel = sizeof(default_level) / sizeof(struct logger_level);
	
	logger = malloc(sizeof(struct logger));
	if (logger == NULL)
		return false;

	for (int i = 0; i < LOGGER_MAX_LEVEL; i++)
		logger->level[i] = logger_level_init;

	for (int i = 0; i < nlevel; i++) {
		if (default_level[i].stream == (FILE *) -1)
			default_level[i].stream = stdout;

		if (default_level[i].stream == (FILE *) -2)
				default_level[i].stream = stderr;

		logger->level[i] = &default_level[i];
	}

	return true;
}

void logger_destroy(void)
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
