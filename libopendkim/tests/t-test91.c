/*
**  Copyright (c) 2005-2008 Sendmail, Inc. and its suppliers.
**    All rights reserved.
**
**  Copyright (c) 2009, 2011, 2012, The Trusted Domain Project.
**    All rights reserved.
*/

#include "build-config.h"

/* system includes */
#include <sys/types.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#ifdef USE_GNUTLS
# include <gnutls/gnutls.h>
#endif /* USE_GNUTLS */

/* libopendkim includes */
#include "../dkim.h"
#include "t-testdata.h"

#define	MAXHEADER	4096

#define SIG2 "v=1; a=rsa-sha1; c=relaxed/simple; d=example.com; s=test;\r\n\tt=1172620939; bh=ll/0h2aWgG+D3ewmE4Y3pY7Ukz8=;\r\n\th=Received:Received:Received:From:To:Date:Subject:Message-ID;\r\n\tb=Q4G/ki/5soDXGxs43JfV+qEKDr5X3GgTDNeZqWL3zLLC5DXWWzmnKRcU8NH4Wsfkh\r\n\t o5tMo4NRmqnB2eZtozsyXdHo2ekUPLxuAQJomM4JHaPTfsraHwkibQIkPpW5hf/Rc2\r\n\t 0QgP48iQBjxqcOSn/Vwk5QDup4Qj1vgOxBqTqwdg="

/*
**  MAIN -- program mainline
**
**  Parameters:
**  	The usual.
**
**  Return value:
**  	Exit status.
*/

int
main(int argc, char **argv)
{
	u_int flags;
	DKIM_STAT status;
	uint64_t fixed_time;
	DKIM *dkim;
	DKIM_LIB *lib;
	dkim_sigkey_t key;
	unsigned char hdr[MAXHEADER + 1];

	printf("*** relaxed/simple rsa-sha1 signing with CRLF fixing\n");

#ifdef USE_GNUTLS
	(void) gnutls_global_init();
#endif /* USE_GNUTLS */

	/* instantiate the library */
	lib = dkim_init(NULL, NULL);
	assert(lib != NULL);

#ifdef TEST_KEEP_FILES
	/* set flags */
	flags = (DKIM_LIBFLAGS_TMPFILES|DKIM_LIBFLAGS_KEEPFILES);
	(void) dkim_options(lib, DKIM_OP_SETOPT, DKIM_OPTS_FLAGS, &flags,
	                    sizeof flags);
#endif /* TEST_KEEP_FILES */

	(void) dkim_options(lib, DKIM_OP_GETOPT, DKIM_OPTS_FLAGS, &flags,
	                    sizeof flags);
	flags |= DKIM_LIBFLAGS_FIXCRLF;
	(void) dkim_options(lib, DKIM_OP_SETOPT, DKIM_OPTS_FLAGS, &flags,
	                    sizeof flags);

	key = KEY;

	dkim = dkim_sign(lib, JOBID, NULL, key, SELECTOR, DOMAIN,
	                 DKIM_CANON_RELAXED, DKIM_CANON_SIMPLE,
	                 DKIM_SIGN_RSASHA1, -1L, &status);
	assert(dkim != NULL);

	/* fix signing time */
	fixed_time = 1172620939;
	(void) dkim_options(lib, DKIM_OP_SETOPT, DKIM_OPTS_FIXEDTIME,
	                    &fixed_time, sizeof fixed_time);

	status = dkim_header(dkim, HEADER02, strlen(HEADER02));
	assert(status == DKIM_STAT_OK);

	status = dkim_header(dkim, HEADER03, strlen(HEADER03));
	assert(status == DKIM_STAT_OK);

	status = dkim_header(dkim, HEADER04, strlen(HEADER04));
	assert(status == DKIM_STAT_OK);

	status = dkim_header(dkim, HEADER05, strlen(HEADER05));
	assert(status == DKIM_STAT_OK);

	status = dkim_header(dkim, HEADER06, strlen(HEADER06));
	assert(status == DKIM_STAT_OK);

	status = dkim_header(dkim, HEADER07, strlen(HEADER07));
	assert(status == DKIM_STAT_OK);

	status = dkim_header(dkim, HEADER08, strlen(HEADER08));
	assert(status == DKIM_STAT_OK);

	status = dkim_header(dkim, HEADER09, strlen(HEADER09));
	assert(status == DKIM_STAT_OK);

	status = dkim_eoh(dkim);
	assert(status == DKIM_STAT_OK);

	status = dkim_body(dkim, NBODY00, strlen(NBODY00));
	assert(status == DKIM_STAT_OK);

	status = dkim_body(dkim, NBODY01, strlen(NBODY01));
	assert(status == DKIM_STAT_OK);

	status = dkim_body(dkim, NBODY01A, strlen(NBODY01A));
	assert(status == DKIM_STAT_OK);
	status = dkim_body(dkim, NBODY01B, strlen(NBODY01B));
	assert(status == DKIM_STAT_OK);
	status = dkim_body(dkim, NBODY01C, strlen(NBODY01C));
	assert(status == DKIM_STAT_OK);
	status = dkim_body(dkim, NBODY01D, strlen(NBODY01D));
	assert(status == DKIM_STAT_OK);
	status = dkim_body(dkim, NBODY01E, strlen(NBODY01E));
	assert(status == DKIM_STAT_OK);

	status = dkim_body(dkim, NBODY02, strlen(NBODY02));
	assert(status == DKIM_STAT_OK);

	status = dkim_body(dkim, NBODY03, strlen(NBODY03));
	assert(status == DKIM_STAT_OK);

	status = dkim_body(dkim, NBODY04, strlen(NBODY04));
	assert(status == DKIM_STAT_OK);

	status = dkim_body(dkim, NBODY03, strlen(NBODY03));
	assert(status == DKIM_STAT_OK);

	status = dkim_body(dkim, NBODY03, strlen(NBODY03));
	assert(status == DKIM_STAT_OK);

	status = dkim_body(dkim, NBODY05, strlen(NBODY05));
	assert(status == DKIM_STAT_OK);

	status = dkim_body(dkim, NBODY03, strlen(NBODY03));
	assert(status == DKIM_STAT_OK);

	status = dkim_body(dkim, NBODY03, strlen(NBODY03));
	assert(status == DKIM_STAT_OK);

	status = dkim_eom(dkim, NULL);
	assert(status == DKIM_STAT_OK);

	memset(hdr, '\0', sizeof hdr);
	status = dkim_getsighdr(dkim, hdr, sizeof hdr,
	                        strlen(DKIM_SIGNHEADER) + 2);
	assert(status == DKIM_STAT_OK);
	assert(strcmp(SIG2, hdr) == 0);

	status = dkim_free(dkim);
	assert(status == DKIM_STAT_OK);

	dkim_close(lib);

	return 0;
}
