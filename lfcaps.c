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
#endif


#include "lfcaps.h"

// little endian arch only,
// (little endian to arch endian)
#define FIXUP(x) (x)


// read sys_fcap. return 0 for no capabilities,
// 1, 2 or 3 for success and the read file capability version
// else -ernno (<0), -EINVAL if the read attribute's size doesn't 
// match the attribute size of the capability version
int lfcaps_sysread( sys_fcap_t *fc, int fd, const char* path ){
	int ret;

	if ( path )
		ret = getxattr( path, XATTR_NAME_CAPS, fc, sizeof(sys_fcap_t) );
	else 
		ret = fgetxattr( fd, XATTR_NAME_CAPS, fc, sizeof(sys_fcap_t) );
	
	bzero( fc, sizeof( sys_fcap_t ) ); // set rootid etc to 0 as well

	if ( ERRNO(ret) == ENODATA || ret==0 )
		return(0);

	else if ( ret < 0 ){
		//ewritesl("Cannot read capabilities");
		return(-ERRNO(ret));
	}

	uint rev = ( FIXUP(fc->magic_etc) >> VFS_CAP_REVISION_SHIFT ) 
			& ( VFS_CAP_REVISION_MASK >> VFS_CAP_REVISION_SHIFT );

	if (( rev == (VFS_CAP_REVISION_1>>VFS_CAP_REVISION_SHIFT)
				&& XATTR_CAPS_SZ_1 != ret ) ||
	 	( rev == (VFS_CAP_REVISION_2>>VFS_CAP_REVISION_SHIFT)
				&& XATTR_CAPS_SZ_2 != ret ) ||
		( rev == (VFS_CAP_REVISION_3>>VFS_CAP_REVISION_SHIFT)
				&& XATTR_CAPS_SZ_3 != ret ) )
		return( -EINVAL );

	return( rev );
}


// write capabilities to fd
int lfcaps_syswrite( sys_fcap_t *fc, int fd, const char* path ){
	// version 2 is converted to v3 by the kernel, if rootid is set,
	// according to the kernel docs
	fc->magic_etc = FIXUP(VFS_CAP_REVISION_2 | VFS_CAP_FLAGS_EFFECTIVE );
	int ret;
	if ( path ) 
		ret = setxattr( path, XATTR_NAME_CAPS, fc, XATTR_CAPS_SZ_2, 0);
	else
		ret = fsetxattr( fd, XATTR_NAME_CAPS, fc, XATTR_CAPS_SZ_2, 0);

	return(ret);
}

int _lfcaps_write( lfcaps_t *fc, int fd, const char* path ){
	sys_fcap_t sfc;
	sfc.rootid = fc->rootid;
	sfc.data[0].permitted = fc->_permitted[0];
	sfc.data[1].permitted = fc->_permitted[1];
	sfc.data[0].inheritable = fc->_inheritable[0];
	sfc.data[1].inheritable = fc->_inheritable[1];
	return ( lfcaps_syswrite( &sfc, fd, path ) );
}

int lfcaps_write( lfcaps_t *fc, const char* path ){
	return( _lfcaps_write( fc, 0, path ) );
}

int lfcaps_writefd( lfcaps_t *fc, int fd ){
	return( _lfcaps_write( fc, fd, 0 ) );
}




int _lfcaps_read( lfcaps_t *caps, int fd, const char* path ){
	sys_fcap_t fc;
	int ret = lfcaps_sysread( &fc, fd, path );

	if ( ret<0 ) return ret;
	
	caps->_permitted[0] = fc.data[0].permitted;
	caps->_permitted[1] = fc.data[1].permitted;
	caps->_inheritable[0] = fc.data[0].inheritable;
	caps->_inheritable[1] = fc.data[1].inheritable;
	caps->rootid = fc.rootid;
	caps->version = ret;

	return ret;
}

int lfcaps_read( lfcaps_t *caps, const char* path ){
	return( _lfcaps_read( caps, 0, path ) );
}

int lfcaps_readfd( lfcaps_t *caps, int fd ){
	return( _lfcaps_read( caps, fd, 0 ) );
}

int _lfcaps_sprint( char *buf, lfcaps_capset_t capset, const char separator ){
	char *p = buf;
	int cappos = 0; 

	#define _LFCAP_(_a,_b,_c) _b "\0"
	const char* capstr =
		#include "cap_table.h"
		;
	#undef _LFCAP_
	#define _LFCAP_(_a,_b,_c) sizeof(_b),
	const char capindex[] = {
		#include "cap_table.h"
		0	};
	#undef _LFCAP_

	for ( const char *pi = capindex; capset && *pi; pi++, capset >>=1 ){
		if ( capset&0x1 ){
			if ( p!=buf ) *p++ = separator;
			p = stpcpy( p, capstr + cappos );
		}
		cappos += *pi;
	}
	*p = 0;

	return(p-buf);
}

lfcaps_capset_t _lfcaps_strtocap( const char* str, char separator ){
	#define _LFCAP_(_a,_b,_c) _b "\0"
	const char* capstr =
		#include "cap_table.h"
		;
	#undef _LFCAP_

	int r = 0;
	for ( const char *p = capstr; *p; ){
		for ( const char *ps = str; *ps==*p; ps++,p++ ){
			if ( *p == 0 && ( ( ! *ps ) || *ps==separator ) ) // exact match
				return( 1UL<<r );
		}
		while ( *p ) p++;
		p++;
		r++;
	}
		
	return 0; // not found
}


// substring version, look for the char str within the table of capnames,
// the end of  can be 'separator' or 0, 
// separator defaults to 0.
// the first cpabality wit the capname containing str anywhere within the string 
// is returned as match, 
// returns 0 if not found.
lfcaps_capset_t _lfcaps_strtocap_substr( const char* str, char separator, int ambivalence ){
	#define _LFCAP_(_a,_b,_c) _b "\0"
	const char* capstr =
		#include "cap_table.h"
		;
	const uint capstrsz = sizeof(
		#include "cap_table.h" 
			);
	#undef _LFCAP_

	lfcaps_capset_t ret = 0;

	int r = 0;
	const char *op = capstr;
	for ( const char *p = capstr; p< capstr+capstrsz; p++ ){
			for ( const char *ps = str, *pp = p; *ps++ == *pp++ ; ){
				if ( *ps == 0 || *ps == separator ){ // match
					//printvl( "match: ", PVAR(r,op) );
					if ( ambivalence ) 
						return( 1UL<<r );
					if ( ret ) // ambivalent substring
				      return 0;
					ret = ( 1UL<<r );
				}
			} 
		if ( *p == 0 ){
			r++;
			//p++;
			op = p;
		}
	}
		
	return ret; // 0, if not found
}



#ifdef LFCAPS_STANDALONE
/* standalone implementation */

#ifndef MLIB

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
		for ( char *cps = GET(capnames); *cps; ){
			//printsl( "cps: ", cps );
			lfcaps_capset_t c = lfcaps_strtocap_substr( cps, ',' );
			if ( !c ){
				ret = EINVAL;
				ewrites( "ambivalent capability / not found: " );
			}
			caps |= c;
			do {
				if ( !c ) ewrite( cps, 1 );
				cps ++;
			} while ( *cps && *cps != ',' );
			if ( !c ) ewritesl("");
			if ( !*cps ) break;
			cps++;
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
			#include "cap_table.h"
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
#endif



