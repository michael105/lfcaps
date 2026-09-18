#include "lfcaps.h"


// little endian only
#define FIXUP(x) (x)


// read sys_fcap. return 0 for no capabilities,
// 1, 2 or 3 for success and the read file capability version
// else -ernno (<0), -EINVAL if the read attribute's size doesn't 
// match the attribute size of the capability version
int lfcaps_sysread( sys_fcap_t *fc, int fd, const char* path ){
	int ret;

	if ( fd ){
		ret = fgetxattr(fd, XATTR_NAME_CAPS, fc, sizeof(sys_fcap_t));
	} else {
		ret = getxattr( path, XATTR_NAME_CAPS, fc, sizeof(sys_fcap_t));
	}

	if ( ERRNO(ret) == ENODATA || ret==0 ){
		bzero( fc, sizeof( sys_fcap_t ) ); // needs to be done explicite
		return(0);
	}
	else if ( ret <= 0 ){
		//ewritesl("Cannot read capabilities");
		//printf( "%d%s%d\n",ret, " e: ", errno );
		return(-ERRNO(ret));
	}
	//printvl(PVAR(ret));

	int version;
	int size = -1;
	switch (FIXUP(fc->magic_etc) & VFS_CAP_REVISION_MASK) {
		case VFS_CAP_REVISION_1:
			size = XATTR_CAPS_SZ_1;
			version = 1;
			break;
		case VFS_CAP_REVISION_2:
			size = XATTR_CAPS_SZ_2;
			version = 2;
			break;
		case VFS_CAP_REVISION_3:
			size = XATTR_CAPS_SZ_3;
			version = 3;
	}
	if ( size != ret )
		return ( -EINVAL );
	
	return( version );
}


// write capabilities to fd
int lfcaps_syswrite( sys_fcap_t *fc, int fd, const char* path ){
	// version 2 is converted to v3 by the kernel, if rootid is set.
	fc->magic_etc = FIXUP(VFS_CAP_REVISION_2 | VFS_CAP_FLAGS_EFFECTIVE );
	int ret;
	if ( fd ) 
		ret = fsetxattr( fd, XATTR_NAME_CAPS, fc, XATTR_CAPS_SZ_2, 0);
	else
		ret = setxattr( path, XATTR_NAME_CAPS, fc, XATTR_CAPS_SZ_2, 0);

	return(ret);
}

int lfcaps_write( lfcaps_t *fc, int fd, const char* path ){
	sys_fcap_t sfc;
	sfc.rootid = fc->rootid;
	sfc.data[0].permitted = fc->_permitted[0];
	sfc.data[1].permitted = fc->_permitted[1];
	sfc.data[0].inheritable = fc->_inheritable[0];
	sfc.data[1].inheritable = fc->_inheritable[1];
	return ( lfcaps_syswrite( &sfc, fd, path ) );
}



int lfcaps_read( lfcaps_t *caps, int fd, const char* path ){
	sys_fcap_t fc;
	int ret = lfcaps_sysread( &fc, fd, path );

	if ( ret<0 ) return ret;
	
	//caps->version=ret;
	caps->_permitted[0] = fc.data[0].permitted;
	caps->_permitted[1] = fc.data[1].permitted;
	caps->_inheritable[0] = fc.data[0].inheritable;
	caps->_inheritable[1] = fc.data[1].inheritable;
	caps->rootid = fc.rootid;
	caps->version = ret;

	return ret;
}


int _lfcaps_sprint( char *buf, lfcaps_capset_t capset, const char separator ){
	# define __lfcaps_sprint( _buf, _capset, _separator, ... ) _lfcaps_sprint( _buf, _capset, _separator )
	# define lfcaps_sprint( _buf, _capset, ... ) __lfcaps_sprint( _buf, _capset, __VA_OPT__(__VA_ARGS__,) ',' )

	char *p = buf;
	int cappos = 0; 

	#define CN(_a,_b,_c) _b "\0"
	const char* capstr =
		#include "cap_table.h"
		;
	#undef CN
	#define CN(_a,_b,_c) sizeof(_b),
	const char capindex[] = {
		#include "cap_table.h"
		0	};
	#undef CN

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
	# define lfcaps_strtocap( _str, ... ) _lfcaps_strtocap( _str, __VA_ARGS__+0 )
	#define CN(_a,_b,_c) _b "\0"
	const char* capstr =
		#include "cap_table.h"
		;
	#undef CN

	int r = 0;
	for ( const char *p = capstr; *p; ){
		for ( const char *ps = str; *ps==*p; ps++,p++ ){
			if ( *p == 0 || *ps == separator ) // match
				return( 1UL<<r );
		}
		while ( *p ) p++;
		p++;
		r++;
	}
		
	return 0; // not found
}


// fuzzy version, look for the char str within the table of capnames,
// the end of  can be 'separator' or 0, 
// separator defaults to 0.
// the first cpabality wit the capname containing str anywhere within the string 
// is returned as match, 
// returns 0 if not found.
lfcaps_capset_t _lfcaps_strtocap_fz( const char* str, char separator ){
# define lfcaps_strtocap_fz( _str, ... ) _lfcaps_strtocap_fz( _str, __VA_ARGS__+0 )
	#define CN(_a,_b,_c) _b "\0"
	const char* capstr =
		#include "cap_table.h"
		;
	const uint capstrsz = sizeof(
		#include "cap_table.h" 
			);
	#undef CN

	int r = 0;
	const char *op = capstr;
	for ( const char *p = capstr; p< capstr+capstrsz; p++ ){
			for ( const char *ps = str, *pp = p; *ps++ == *pp++ ; ){
				if ( *ps == 0 || *ps == separator ){ // match
					//printvl( "match: ", PVAR(r,op) );
					return( 1UL<<r );
				}
			} 
		if ( *p == 0 ){
			r++;
			//p++;
			op = p;
		}
	}
		
	return 0; // not found
}




/* standalone implementation */

#include "options.h"
#include "tools.h"



#define OPTIONS \
	h,,"help", \
	u,,"show usage", \
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
//v,,"verbose", 
	

USAGE( "[file] [file2] .." );

HELP( "" );

DECLARE_SETTING;

uint lfcaps_main( uint opts, int argc, char *argv[] ){
	char buf[LFCAPS_MAXSTRLEN];
	int ret = 0;
	
	lfcaps_capset_t caps = 0;
	if ( !OPT(p|i) ) SETOPT(p);

	if ( OPT(n) ){
		for ( char *cps = GET(capnames); *cps; ){
			//printsl( "cps: ", cps );
			caps |= lfcaps_strtocap_fz( cps, ',' );
			do {
				cps ++;
			} while ( *cps && *cps != ',' );
			if ( !*cps ) break;
			cps++;
		}
		//lfcaps_sprint( buf, caps );
		//printvl( "capabilities: ", buf );
	} else if ( OPT(t) )
		caps = -1;

	for ( argv++; *argv; argv++ ){
		lfcaps_t ctcaps = { 0 };
		if ( OPT(l|a|d|L|t) ){
			int r = lfcaps_read( &ctcaps, 0, *argv );
			if ( r<0 ){
				printvl( *argv, ": ", strerror( -r ) );
				ret = -r;
				continue;
			}
			if ( OPT(L) ){
				if ( r ){
					printv( *argv, ": " );
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
				if ( OPT(p) ){
					ret |= !(caps & ctcaps.permitted);
				}
				if ( OPT(i) ){
					ret |= !(caps & ctcaps.inheritable);
				}
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
			r = lfcaps_write( &ctcaps,0,*argv );
			if ( r<0 ){
				printvl( *argv, ": ", strerror( -r ) );
				ret = -r;
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

	PARSEARGV( 'h': help(), 'u': usage() );

	if ( OPT(N) ){
		#define CN(_a,_b,_c) _b,
		const char* captbl[] = {
			#include "cap_table.h"
			};
		#undef CN
		SHELLSORT( captbl, LFCAPS_COUNT, ( strcmp(*a,*b) < 0 ) );
		for ( int a = 0; a < LFCAPS_COUNT; a++ )
			printsl( captbl[a] );
		exit(0);
	}

	if ( argc<2 )
		usage();

	int ret = lfcaps_main( opts, argc, argv );

	exit(ret);
}




