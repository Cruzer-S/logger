#ifndef LOGGER_H__
#define LOGGER_H__

#include <stdbool.h>	// false

#include "macro_fanatic.h"

/********************************************************************************
 * Define logger frame                                             		*
 *******************************************************************************/
#define LOGGER_IS_EMPTY0(X, Y) X
#define LOGGER_IS_EMPTY1(X, Y) Y

#define LOGGER_PRINT(BSA, ASA, BLP, ALP, BEA, AEA, MSG, ...) do{		\
	LOGGER_START_STATEMENT							\
	LOGGER_FUNCTION(							\
		BSA		LOGGER_START_ARGS		ASA,		\
										\
		BLP	LOGGER_PREFIX   MSG   LOGGER_POSTFIX 	ALP		\
										\
		BEA		LOGGER_END_ARGS			AEA		\
										\
		, ## __VA_ARGS__						\
	);									\
	LOGGER_END_STATEMENT							\
} while (false)

#define DEFINE_PR(X, ...)							\
	CONCAT(LOGGER_IS_EMPTY, IS_EMPTY(LOGGER_##X##_ARGS))			\
	(									\
		DEFINE_PR2(X, ## __VA_ARGS__, LOGGER_##X##_ARGS),		\
		DEFINE_PR2(X, ## __VA_ARGS__)					\
	)

#define DEFINE_PR2(X, MSG, ...) do {						\
	LOGGER_##X##_START_STATEMENT						\
	LOGGER_PRINT(								\
		LOGGER_##X##_BSA,		LOGGER_##X##_ASA,		\
		LOGGER_##X##_BLP,		LOGGER_##X##_ALP,		\
		LOGGER_##X##_BEA,		LOGGER_##X##_AEA,		\
										\
		LOGGER_##X##_PREFIX	MSG	LOGGER_##X##_POSTFIX		\
										\
		, ## __VA_ARGS__						\
	);									\
	LOGGER_##X##_END_STATEMENT						\
} while (false)

#define pr(TYPE, ...) CONCAT(pr_, IS_EMPTY(__VA_ARGS__))(TYPE, ## __VA_ARGS__)
#define pr_0(TYPE, MSG, ...) pr_##TYPE(MSG, ## __VA_ARGS__)
#define pr_1(TYPE) pr_0(TYPE, "")

/********************************************************************************
 * Load decorator						                *
 *******************************************************************************/
#ifndef LOGGER_DECORATOR
	#define LOGGER_DECORATOR mythos
#endif

#define SEPERATOR(X) X
#define STRINGIFY(X) #X
#define TO_STR(X) STRINGIFY(X)
#define INCLUDE_PATH(DIRS, FILENAME) TO_STR(					\
	SEPERATOR(DIRS)/SEPERATOR(FILENAME).h					\
)

#include INCLUDE_PATH(decorator, LOGGER_DECORATOR)

#undef INCLUDE_PATH
#undef TO_STR
#undef STRINGIFY
#undef SEPERATOR

#endif // #ifndef LOGGER_H__
