#ifndef UUID_H
#define UUID_H

/*
 * Copyright (c) 1990- 1993, 1996 Open Software Foundation, Inc.
 * Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, Ca. &
 * Digital Equipment Corporation, Maynard, Mass.
 * Copyright (c) 1998 Microsoft.
 * To anyone who acknowledges that this file is provided "AS IS"
 * without any express or implied warranty: permission to use, copy,
 * modify, and distribute this file for any purpose is hereby
 * granted without fee, provided that the above copyright notices and
 * this notice appears in all source code copies, and that none of
 * the names of Open Software Foundation, Inc., Hewlett-Packard
 * Company, or Digital Equipment Corporation be used in advertising
 * or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Neither Open Software
 * Foundation, Inc., Hewlett-Packard Company, Microsoft, nor Digital Equipment
 * Corporation makes any representations about the suitability of
 * this software for any purpose.
 */
#include "os_ini.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! . */
typedef struct _uuid_ {
	/*! . */
	OS_UINT32_t time_low;
	/*! . */
	OS_UINT16_t time_mid;
	/*! . */
	OS_UINT16_t time_hi_and_version;
	/*! . */
	OS_UINT8_t clock_seq_hi_and_reserved;
	/*! . */
	OS_UINT8_t clock_seq_low;
	/*! . */
	OS_UINT8_t node[6];
} os_uuid_t;


void os_uuid_init();
void os_uuid_destroy();
/*!
 * \brief Generate a UUID.
 */
int os_uuid_create(
	/*! . */
	os_uuid_t * id);

/*!
 * \brief Out will be xxxx-xx-xx-xx-xxxxxx format.
 */
void os_uuid_unpack(
	/*! . */
	os_uuid_t * u,
	/*! . */
	char *out);

/*!
 * \brief Create a UUID using a "name" from a "name space"
 */
void os_uuid_create_from_name(
	/*! Resulting UUID. */
	os_uuid_t * uid,
	/*! UUID to serve as context, so identical names from different name
	* spaces generate different UUIDs. */
	os_uuid_t nsid,
	/*! The name from which to generate a UUID. */
	void *name,
	/*! The length of the name. */
	int namelen);

/*!
 * \brief Compare two UUID's "lexically".
 *
 * \return
 *	-1   u1 is lexically before u2
 *	 0   u1 is equal to u2
 *	 1   u1 is lexically after u2
 *
 * \note Lexical ordering is not temporal ordering!
 */
int os_uuid_compare(
	/*! . */
	os_uuid_t * u1,
	/*! . */
	os_uuid_t * u2);

#ifdef __cplusplus
}
#endif

#endif /* UUID_H */
