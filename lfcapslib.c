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
#endif

#include "lfcapslib.h"

// little endian arch only,
// (this is little endian to arch endian)
// big endian should work, but I cannot test
// FIXUP would need to byteswap
#define FIXUP(x) (x)


// read sys_fcap. return 0 for no capabilities,
// 1, 2 or 3 for success and the read file capability version
// else -ernno (<0), -EINVAL if the read attribute's size doesn't 
// match the attribute size of the capability version
int lfcaps_sysread( sys_fcap_t *fc, int fd, const char* path ){
	int ret;

	bzero( fc, sizeof( sys_fcap_t ) ); // set rootid etc to 0 as well
												
	if ( path )
		ret = getxattr( path, XATTR_NAME_CAPS, fc, sizeof(sys_fcap_t) );
	else 
		ret = fgetxattr( fd, XATTR_NAME_CAPS, fc, sizeof(sys_fcap_t) );

	if ( ERRNO(ret) == ENODATA || ret==0 )
		return(0);

	else if ( ret < 0 ){
		//ewritesl("Cannot read capabilities");
		return(-ERRNO(ret));
	}

	uint rev = ( FIXUP(fc->magic_etc) >> VFS_CAP_REVISION_SHIFT ) 
			& ( VFS_CAP_REVISION_MASK >> VFS_CAP_REVISION_SHIFT );

	int sz = -EINVAL;
	if ( rev == (VFS_CAP_REVISION_1>>VFS_CAP_REVISION_SHIFT) )
		sz = XATTR_CAPS_SZ_1;
	else if	( rev == (VFS_CAP_REVISION_2>>VFS_CAP_REVISION_SHIFT) )
		sz = XATTR_CAPS_SZ_2;
	else if	( rev == (VFS_CAP_REVISION_3>>VFS_CAP_REVISION_SHIFT) )
		sz =  XATTR_CAPS_SZ_3;

	if ( sz != ret ) // also different revision
		return( sz );

	return( rev );
}


// write capabilities to fd
int lfcaps_syswrite( sys_fcap_t *fc, int fd, const char* path ){
	int sz;
	if ( !fc->rootid ){
		fc->magic_etc = FIXUP(VFS_CAP_REVISION_2 | VFS_CAP_FLAGS_EFFECTIVE );
		sz = XATTR_CAPS_SZ_2;
	} else { // or fail with cap rev 2
		fc->magic_etc = FIXUP(VFS_CAP_REVISION_3 | VFS_CAP_FLAGS_EFFECTIVE );
		sz = XATTR_CAPS_SZ_3;
	}

	if ( path ) 
		return( setxattr( path, XATTR_NAME_CAPS, fc, sz, 0) );

	return( fsetxattr( fd, XATTR_NAME_CAPS, fc, sz, 0) );
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
		#include "lfcapslib.h"
		;
	#undef _LFCAP_
	#define _LFCAP_(_a,_b,_c) sizeof(_b),
	const char capindex[] = {
		#include "lfcapslib.h"
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
		#include "lfcapslib.h"
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
// returns 0 if not found, LFCAP_ERROR for ambigous substrings
lfcaps_capset_t _lfcaps_strtocap_substr( const char* str, char separator, int ambivalence, int verbose ){
	#define _LFCAP_(_a,_b,_c) _b "\0"
	const char* capstr =
		#include "lfcapslib.h"
		;
	const uint capstrsz = sizeof(
		#include "lfcapslib.h" 
			);
	#undef _LFCAP_

	lfcaps_capset_t ret = 0;
	int r = 0;
	const char *match, *pos = capstr;

	for ( const char *p = capstr; p< capstr+capstrsz; p++ ){
			for ( const char *ps = str, *pp = p; *ps++ == *pp++ ; ){
				if ( *ps == 0 || *ps == separator ){ // match
					if ( ambivalence ) 
						return( 1UL<<r );
					if ( ret ){ // ambivalent substring
						if ( verbose ){
							eprintsl( "Ambivalent substring: ", match, " == ", pos );
						}
						return LFCAP_ERROR;
					}
					ret = ( 1UL<<r );
					match = pos;
				}
			} 
		if ( *p == 0 ){
			pos = p+1;
			r++;
		}
	}
		
	return ret; // 0, if not found
}



lfcaps_capset_t _lfcaps_strtocapset_substr( const char* str, char separator, int ambivalence, int verbose ){

	lfcaps_capset_t caps = 0;

	for ( const char *cps = str; *cps; ){
		lfcaps_capset_t c = lfcaps_strtocap_substr( cps, ',',0,1 );
		if ( c==0 && verbose ) ewrites( "capability not found: " );
		caps |= c;
		const char* pos = cps;
		do {
			cps ++;
		} while ( *cps && *cps != separator );
		if ( c <= 0 ){ 
			if ( verbose ) {
				ewrite( pos, cps-pos );
				ewritesl();
			}
			return LFCAP_ERROR;
		}
		if ( !*cps ) break;
		cps++;
	}

	return caps;
}

