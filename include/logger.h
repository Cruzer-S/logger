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
#define LOGGER_LEVEL_VERB	8
#define LOGGER_LEVEL_PVERB	9

#define log(LEVEL, ...) (						\
		logger_log(LOGGER_LEVEL_##LEVEL,			\
			__FILE__, __func__, STRINGIFY(__LINE__),	\
			__VA_ARGS__					\
		)							\
	)

typedef struct logger_level {
	int level;
	char *name;
	char *format;
	FILE *stream;
} *LoggerLevel;

bool logger_initialize(void);

bool logger_define_level(LoggerLevel );

int logger_log(int level,
	       const char *file, const char *function, const char *line,
	       const char *formatted, ...);

void logger_destroy(void);

#endif
