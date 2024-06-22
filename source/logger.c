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

struct log_level {
	FILE *fp;
	char *name;
	char *format;
};

struct logger {
	struct log_level level[LOGGER_MAX_LEVEL];
};

Logger logger_create(void)
{
	Logger logger = malloc(sizeof(struct logger));
	if (logger == NULL)
		return NULL;

	for (int i = 0; i < LOGGER_MAX_LEVEL; i++) {
		logger->level[i].name = NULL;
		logger->level[i].format = NULL;
	}

	return logger;
}

bool logger_define_level(
		Logger logger,
		int level, char *name, 
		FILE *fp
) {
	if (logger->level[level].name != NULL)
		return false;
	
	logger->level[level].name = strdup(name);
	if (logger->level[level].name == NULL)
		return false;

	logger->level[level].fp = fp;

	return true;
}

bool logger_set_format(Logger logger, int level, char *format)
{
	logger->level[level].format = strdup(format);
	if (logger->level[level].format == NULL)
		return false;

	return true;
}

static char *get_time_string(char *format)
{
	thread_local static char buffer[128];
	time_t t = time(NULL);

	strftime(buffer, sizeof buffer, format, localtime(&t));

	return buffer;
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
	
	fmt_start = logger->level[level].format;
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
			case 'p': copy_ptr = logger->level[level].name; break;
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

	retval = vfprintf(logger->level[level].fp, buffer, list);

	va_end(list);

	return retval;
}

void logger_destroy(Logger logger)
{
	for (int i = 0; i < LOGGER_MAX_LEVEL; i++) {
		free(logger->level[i].name);
		free(logger->level[i].format);
	}

	free(logger);
}
