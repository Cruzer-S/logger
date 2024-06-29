#ifndef LOGGER_H__
#define LOGGER_H__

#include <stdio.h>
#include <stdbool.h>

#define STRINGIFY_INTERNAL(X) # X
#define STRINGIFY(X) STRINGIFY_INTERNAL(X)

#define LOGGER_MAX_LEVEL	10

#define LOGGER_LEVEL_INFO	0
#define LOGGER_LEVEL_PINFO	1
#define LOGGER_LEVEL_WARN	2
#define LOGGER_LEVEL_PWARN	3
#define LOGGER_LEVEL_ERRN	4
#define LOGGER_LEVEL_PERRN	5
#define LOGGER_LEVEL_CRTC	6
#define LOGGER_LEVEL_PCRTC	7

#define log(LOG, LEVEL, ...) 						\
		logger_log(LOG, LOGGER_LEVEL_##LEVEL,		\
			__FILE__, __func__, STRINGIFY(__LINE__),	\
			__VA_ARGS__					\
		)

typedef struct logger *Logger;

typedef struct logger_level {
	int level;
	char *name;
	char *format;
	FILE *stream;
} *LoggerLevel;

Logger logger_create(void);

void logger_use_default_form(Logger logger);

bool logger_define_level(Logger, LoggerLevel );

int logger_log(Logger , int ,
	const char *file, const char *function, const char *line,
	const char *formatted, ...
);

void logger_destroy(Logger );

#endif
