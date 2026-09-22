/* standalone implementation */

/* BSD 2clause

copyright 2026 misc147 codeberg.org/misc1

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ‘AS IS˜ AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef MLIB

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <linux/xattr.h>

typedef unsigned char uchar;


#define TOOL lfcaps
#define VERSION LFCAPS_VERSION.LFCAPS_REVISION-beta
#include "macros/macrolib.h"
#include "macros/SHELLSORT.h"
#define ewrite(_buf,_len) write(STDERR_FILENO,_buf,_len)
#define ewrites(_buf) ewrite(_buf,sizeof(_buf)-1)
#define ewritesl(_buf) ewrite(_buf "\n",sizeof(_buf))
#define _prints(_p) write(STDOUT_FILENO,_p,strlen(_p))
#define _eprints(_p) write(STDOUT_FILENO,_p,strlen(_p))
#define prints(...) FOREACH_K(_prints,__VA_ARGS__)
#define printsl(...) FOREACH_K(_prints,__VA_OPT__(__VA_ARGS__,) "\n")
#define eprintsl(...) FOREACH_K(_eprints,__VA_OPT__(__VA_ARGS__,) "\n")
#define eprints(...) FOREACH_K(_eprints,__VA_ARGS__)
#endif

#include "lfcapslib.h"

#include "macros/options.h"
#include "macros/tools.h"


#define OPTIONS \
	h,,"help", \
	u,,"show usage", \
	v,,"verbose", \
	l,,"list caps (default)", \
	L,,"list caps, if present", \
	a,,"add caps", \
	s,,"set caps", \
	d,,"delete caps", \
	c,,"clear all caps", \
	i,,"modify inheritable capset", \
	p,,"modify permitted capset (default)", \
	t,,"test for capabilities", \
	n,capnames,"capset, caps separated by ','", \
	N,,"list cap names"

//x,capset,"capset as hex/octal"

USAGE( "[file] [file2] .." );

HELP( "read and modify file capablity sets.\n", 
R"(
capset is given with -n "capnames".
Several caps separated with ','
caps can be named by substrings only.
e.g. "chroot" instead of sys_chroot.
The substring needs to be non ambigous.

Both inheritable and permitted capset can be modified/tested at once,
by the flags '-p -i'
)" );


int lfcaps_main( setting_t *setting, uint opts, int argc, char *argv[] ){
	char buf[LFCAPS_MAXSTRLEN];
	int ret = 0;
	
	lfcaps_capset_t caps = 0;
	if ( !OPT(p|i) ) SETOPT(p);
	if ( !OPT(a|l|L|s|d|c|t ) ) SETOPT(l);

	if ( OPT(n) ){
		caps = _lfcaps_strtocapset_substr( GET(capnames), ',',0,1 );
			if ( caps & LFCAP_ERROR ){
				exit(EINVAL);
			}
	} else if ( OPT(t) ) // test for any cap
		caps = -1;

	for ( argv++; *argv; argv++ ){
		lfcaps_t ctcaps = { 0 };
		if ( OPT(l|a|d|L|t) ){
			int r = lfcaps_read( &ctcaps, *argv );
			if ( r<0 ){
				ret = ERRNO(r);
				eprintsl( *argv, ": ", strerror( ret ) );
				continue;
			}
			if ( OPT(L) ){
				if ( r ){
					prints( *argv, ": " );
					//printv( *argv, "fcaps v.", (int)ctcaps.version,": " );
					if ( ctcaps.permitted ){
						lfcaps_sprint( buf, ctcaps.permitted );
						prints( " permitted=", buf );
					} 
					if ( ctcaps.inheritable ){
						lfcaps_sprint( buf, ctcaps.inheritable );
						prints( " inheritable=", buf );
					}
					printsl();
				} else if ( OPT(v) )
					prints( *argv, " No capabilites\n" );
				continue;
			}
			if ( OPT(t) ){
				int rt = 0;
				if ( OPT(p) ){
					rt = !(caps & ctcaps.permitted);
				}
				if ( OPT(i) ){
					rt |= !(caps & ctcaps.inheritable);
				}
				if ( OPT(v) ){
					if ( rt==0 ) printsl( *argv, ": cap is set" );
					else printsl( *argv, ": cap not set" );
				}

				ret |= rt;
				continue;
			}


			if ( OPT(d) ){
				if ( OPT(i) ) ctcaps.inheritable &= ~caps;
				if ( OPT(p) ) ctcaps.permitted &= ~caps;
			}
			if ( OPT(a) ){ 
				if ( OPT(i) ) ctcaps.inheritable |= caps;
				if ( OPT(p) ) ctcaps.permitted |= caps;
			}
		}

		if ( OPT(s) ){
			if ( OPT(i) ) ctcaps.inheritable = caps;
			if ( OPT(p) ) ctcaps.permitted = caps;
		}

		// also OPT(c)
		if ( OPT( a|s|d|c ) ){
			int r = lfcaps_write( &ctcaps,*argv );
			if ( r<0 ){
				ret = ERRNO(r);
				eprintsl( *argv, ": ", strerror( ret ) );
				continue;
			}
		}

		if ( OPT(l) ){
			printsl( *argv );
			if ( ctcaps.permitted | ctcaps.inheritable ){
				if ( ctcaps.permitted ){
					lfcaps_sprint( buf, ctcaps.permitted );
					printsl( "permitted: ", buf );
				} 
				if ( ctcaps.inheritable ){
					lfcaps_sprint( buf, ctcaps.inheritable );
					printsl( "inheritable: ", buf );
				}
			} else {
				printsl( "No capabilities" );
			}
		}
	}

	return(ret);
}



MAIN{
	uint opts = 0;
	DECLARE_SETTING;

	PARSEARGV( 'h': help(), 'u': usage() );

	if ( OPT(N) ){
		#define _LFCAP_(_a,_b,_c) _b,
		const char* captbl[] = {
			#include "lfcapslib.h"
			};
		#undef _LFCAP_
		SHELLSORT( captbl, LFCAPS_COUNT, ( strcmp(*a,*b) < 0 ) );
		for ( int a = 0; a < LFCAPS_COUNT; a++ )
			printsl( captbl[a] );
		exit(0);
	}

	if ( argc<2 )
		usage();

	int ret = lfcaps_main( setting, opts, argc, argv );

	exit(ret);
}



