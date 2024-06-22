#ifndef LOGGER_H__
#define LOGGER_H__

#include <stdio.h>
#include <stdbool.h>

#define LOGGER_MAX_LEVEL	10

#define STRINGIFY_INTERNAL(X) # X
#define STRINGIFY(X) STRINGIFY_INTERNAL(X)

#define log(LOG, LEVEL, ...) 						\
		logger_log(LOG, LOGGER_LEVEL_##LEVEL,		\
			__FILE__, __func__, STRINGIFY(__LINE__),	\
			__VA_ARGS__					\
		)

typedef struct logger *Logger;
typedef struct log_level *LogLevel; 

Logger logger_create(void);

bool logger_define_level(Logger, int level, char *name, FILE * );
bool logger_set_format(Logger , int level, char *format);

int logger_log(Logger , int ,
	const char *file, const char *function, const char *line,
	const char *formatted, ...
);

void logger_destroy(Logger );

#endif
