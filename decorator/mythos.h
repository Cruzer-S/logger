#ifndef LOGGER_DECORATOR_H__
#define LOGGER_DECORATOR_H__

#include <stdio.h>	// fprintf(), stdout, stderr
#include <stdlib.h>	// exit(), EXIT_FAILURE
#include <string.h>	// strerror()
#include <errno.h>	// errno

#define TEXT_COLOR_SHORTHAND
#include "text_color.h"	// change text color

/****************************************************************
 * Declare log type                              		*
 ***************************************************************/
#define pr_INF(...) DEFINE_PR(INF, __VA_ARGS__)
#define pr_NTE(...) DEFINE_PR(NTE, __VA_ARGS__)
#define pr_CAU(...) DEFINE_PR(CAU, __VA_ARGS__)
#define pr_ERR(...) DEFINE_PR(ERR, __VA_ARGS__)
#define pr_CRT(...) DEFINE_PR(CRT, __VA_ARGS__)
#define pr_URG(...) DEFINE_PR(URG, __VA_ARGS__)

#define pr_NLN(...) DEFINE_PR(NLN, __VA_ARGS__)

/****************************************************************
 * Logger Settings                              		*
 ***************************************************************/
#define LOGGER_START_STATEMENT 	// None
#define LOGGER_END_STATEMENT	// None

#define LOGGER_FUNCTION		fprintf

#define LOGGER_START_ARGS	// None
#define LOGGER_END_ARGS		, __FILE__, __func__, __LINE__

#define LOGGER_PREFIX		"[%s:%s:%d] "
#define LOGGER_POSTFIX		"\n"

/****************************************************************
 * pr_XXX Settings                              		*
 ***************************************************************/
// --------------------------------------------------------------
// pr_INF() settings
// --------------------------------------------------------------
#define LOGGER_INF_START_STATEMENT	// None
#define LOGGER_INF_END_STATEMENT	// None

#define LOGGER_INF_BSA			stdout
#define LOGGER_INF_ASA			// None

#define LOGGER_INF_BLP			"[" TCFbW "INF" TCRS "]"
#define LOGGER_INF_ALP			// None

#define LOGGER_INF_BEA			// None
#define LOGGER_INF_AEA			// None

#define LOGGER_INF_PREFIX		// None
#define LOGGER_INF_POSTFIX		// None

#define LOGGER_INF_ARGS			// None

// --------------------------------------------------------------
// pr_NTE() settings
// --------------------------------------------------------------
#define LOGGER_NTE_START_STATEMENT	// None
#define LOGGER_NTE_END_STATEMENT	// None

#define LOGGER_NTE_BSA			stdout
#define LOGGER_NTE_ASA			// None

#define LOGGER_NTE_BLP			"[" TCFbY "NTE" TCRS "]"
#define LOGGER_NTE_ALP			// None

#define LOGGER_NTE_BEA			// None
#define LOGGER_NTE_AEA			// None

#define LOGGER_NTE_PREFIX		// None
#define LOGGER_NTE_POSTFIX		// None

#define LOGGER_NTE_ARGS			// None

// --------------------------------------------------------------
// pr_CAU() settings
// --------------------------------------------------------------
#define LOGGER_CAU_START_STATEMENT	// None
#define LOGGER_CAU_END_STATEMENT	// None

#define LOGGER_CAU_BSA			stdout
#define LOGGER_CAU_ASA			// None

#define LOGGER_CAU_BLP			"[" TCFY "CAU" TCRS "]"
#define LOGGER_CAU_ALP			// None

#define LOGGER_CAU_BEA			// None
#define LOGGER_CAU_AEA			// None

#define LOGGER_CAU_PREFIX		// None
#define LOGGER_CAU_POSTFIX		"%s"

#define LOGGER_CAU_ARGS			strerror(errno)

// --------------------------------------------------------------
// pr_ERR() settings
// --------------------------------------------------------------
#define LOGGER_ERR_START_STATEMENT	// None
#define LOGGER_ERR_END_STATEMENT	// exit(EXIT_FAILURE);

#define LOGGER_ERR_BSA			stderr
#define LOGGER_ERR_ASA			// None

#define LOGGER_ERR_BLP			"[" TCFbR "ERR" TCRS "]"
#define LOGGER_ERR_ALP			// None

#define LOGGER_ERR_BEA			// None
#define LOGGER_ERR_AEA			// None

#define LOGGER_ERR_PREFIX		// None
#define LOGGER_ERR_POSTFIX		"%s"

#define LOGGER_ERR_ARGS			strerror(errno)

// --------------------------------------------------------------
// pr_CRT() settings
// --------------------------------------------------------------
#define LOGGER_CRT_START_STATEMENT	// None
#define LOGGER_CRT_END_STATEMENT	// exit(EXIT_FAILURE);

#define LOGGER_CRT_BSA			stderr
#define LOGGER_CRT_ASA			// None

#define LOGGER_CRT_BLP			"[" TCFbY "CRT" TCRS "]"
#define LOGGER_CRT_ALP			// None

#define LOGGER_CRT_BEA			// None
#define LOGGER_CRT_AEA			// None

#define LOGGER_CRT_PREFIX		// None
#define LOGGER_CRT_POSTFIX		// None

#define LOGGER_CRT_ARGS			// None

// --------------------------------------------------------------
// pr_URG() settings
// --------------------------------------------------------------
#define LOGGER_URG_START_STATEMENT	// None
#define LOGGER_URG_END_STATEMENT	// exit(EXIT_FAILURE);

#define LOGGER_URG_BSA			stderr
#define LOGGER_URG_ASA			// None

#define LOGGER_URG_BLP			"[" TCFR "URG" TCRS "]"
#define LOGGER_URG_ALP			// None

#define LOGGER_URG_BEA			// None
#define LOGGER_URG_AEA			// None

#define LOGGER_URG_PREFIX		// None
#define LOGGER_URG_POSTFIX		// None

#define LOGGER_URG_ARGS			// None

// --------------------------------------------------------------
// pr_NLN() settings
// --------------------------------------------------------------
#define LOGGER_NLN_START_STATEMENT	fputc('\n', stdout); \
					break;
#define LOGGER_NLN_END_STATEMENT	// exit(EXIT_FAILURE);

#define LOGGER_NLN_BSA			stdout
#define LOGGER_NLN_ASA			// None

#define LOGGER_NLN_BLP			// "[" TCFR "URG" TCRS "]"
#define LOGGER_NLN_ALP			// None

#define LOGGER_NLN_BEA			// None
#define LOGGER_NLN_AEA			// None

#define LOGGER_NLN_PREFIX		// None
#define LOGGER_NLN_POSTFIX		// None

#define LOGGER_NLN_ARGS			// None

/****************************************************************
 * end of pr_XXX Settings                              		*
 ***************************************************************/
#endif
