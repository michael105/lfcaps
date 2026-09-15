#ifdef mlconfig

COMPILE prints printv open ioctl exit_errno memset pmemcpy itofmt errno \
	stpcpy strncmp

COMPILE printf eprintf fmtb fmtx fmtd strerror

SHRINKELF

return
#endif


#include "tools.h"
#include "options.h"


#define OPTIONS  \
	v,,"verbose", \
	h,,"help", \
	u,,"show usage", \
	x,,"dump hexadecimal values", \


// options: 
// -i inheritable
// -p permitt (default, if neither i nor p)
// -r remove
// -R remove all capabilities
// -a add (and/or modify) (default)
// -A Add (and/or modify) all capabilities
// -s set 
// -l list
// -L list names
// -e set effective (active)
// -E disable
// -v verbose
// -q short output
// -Q disable output
// -t test for capabilities of a given file
// -T test, and return 0, if cap is present. if not, EIDRM (removed), ENODATA (no caps), errno
// -c check if attrs are set (?)
// -k check for kernel capabilities, display version
// -V display version
// -h help
// -u usage
// -x dump hexadecimal xattr value (fmt: magic permitted inheritable permitted inheritable )

// -I: set/remove/check immutable

CHECK_OPTIONS;

USAGE( "[path]" );

HELP( "list, add or modify security capabilities of executables." );

#include "cap_names.h"

#define _LINUX_CAPABILITY_VERSION_1  0x19980330
#define _LINUX_CAPABILITY_U32S_1     1

#define _LINUX_CAPABILITY_VERSION_2  0x20071026  /* deprecated - use v3 */
#define _LINUX_CAPABILITY_U32S_2     2

#define _LINUX_CAPABILITY_VERSION_3  0x20080522
#define _LINUX_CAPABILITY_U32S_3     2


// write cap version 2.
// automatically converted to version 3 by the kernel,
// if needed. (rootid of a namespace)
#define CAPABILITY_VERSION _LINUX_CAPABILITY_VERSION_2
#define CAPABILITY_U32S 2

#define NUMBER_OF_CAP_SETS  3   /* effective, inheritable, permitted */
#define __CAP_BLKS   (_CAPABILITY_U32S)
#define CAP_SET_SIZE (__CAP_BLKS * sizeof(__u32))

#if __BYTE_ORDER == __BIG_ENDIAN
#define FIXUP_32BITS(x) bswap_32(x)
#else
#define FIXUP_32BITS(x) (x)
#endif

#define __CAP_MAXBITS (__CAP_BLKS * 32)

//#define _CAPABILITY_VERSION _CAPABILITY_VERSION_3
#define CAP_T_MAGIC 0xCA90D0

#if __BYTE_ORDER == __BIG_ENDIAN
  #define FIXUP(x) bswap_32(x)
 #else
  #define FIXUP(x) (x)
#endif

/*
typedef struct _cap_struct {
	struct __user_cap_header_struct head;
	struct __user_cap_data_struct capset[CAPABILITY_U32S];
	uid_t rootid;
} fcap_t; */

// use Version 3 data structure, compatible with v2
typedef struct vfs_ns_cap_data fcap_t;

/* 
The current attr format of security.capability, version 2
(every field is 4 Bytes):
magic permitted inheritable permitted inheritable (version3:rootid)



(uncertain, not important now..)
rootid is set automatically by the kernel, and the version changed to 3,
according to the manual, if capabilities are set within a namespace
and the kernel supports v3.
Cannot test, but version 3 is backwards compatible to version 2. 
(According to the kernel header)


Big Endian support is untested, and might be incomplete.

misc24
*/

// test if the kernel header definitions of the capablities
// are unchanged. (cap table names in the right order)
void check_capdefinitions(){
	for ( int a = 0; a < sizeof(capnames)/sizeof(capname_s); a++ ){
		if ( capnames[a].cap != (a) ){
			printf("capability definition doesn't match: %d\n",a);
			exit(1);
		}
	}

}


// read fcap. return 0 for no capabilities,
// 1, 2 or 3 for success and the read file capability version
// else -ernno (<0), -EINVAL if the read attribute's size doesn't 
// match the attribute size of the capability version
int cap_read_fd( fcap_t *fc, int fd ){
	int ret;

	errno = 0;
	ret = fgetxattr(fd, XATTR_NAME_CAPS, fc, sizeof(fcap_t));
	if ( errno == ENODATA )
		return(0);
	else if ( ret <= 0 ){
		ewritesl("Cannot read capabilities");
		printf( "%d%s%d\n",ret, " e: ", errno );
		return(-errno);
	}

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
int cap_write_fd( fcap_t *fc, int fd ){
	fc->magic_etc = FIXUP(VFS_CAP_REVISION_2 | VFS_CAP_FLAGS_EFFECTIVE );
	int ret = fsetxattr( fd, XATTR_NAME_CAPS, fc, XATTR_CAPS_SZ_2, 0);

	return(ret);
}

// convert a capset into text.
// return the count of chars writte nto buf.
// buf should be (at least) maxcaplen bytes.
int caps_to_text( char* buf, fcap_t *fc ){
	char* p = buf;
	char c = 0;

	for ( int a = 0; a<=1; a++ ){
		for ( int b = 0; ( b<32 ) && ( b+a*32 < CAPCOUNT ); b++ ){
			if ( (fc->data[a].permitted & ( 1<< (b) ) ) || (fc->data[a].inheritable & (1<<(b)) )  ){
				if ( c ){
					*p++=c;
				} else {
					 c = ',';
				}
				p = stpcpy( p, capnames[a*32+b].name );
				*p++='=';
				if ( fc->data[a].permitted & ( 1<< (b) ) ) 
					*p++='p';
				if ( fc->data[a].inheritable & ( 1<< (b) ) ) 
					*p++='i';
			}
		}
	}

	return(p-buf);
}


int getcap_main( uint opts, char *path ){
	int ver;
	int fd = open ( path, O_RDONLY ); // test, if path is an executable
	if ( fd<0 ){
		eprintf("Cannot access: %s\n%s\n", path,strerror(errno) );
		return(-errno);
	}

	fcap_t fc;
	ver = cap_read_fd( &fc ,fd );
	printvl("ver: ",ver);

	if ( ver == 0 ){
		if ( OPT(v) )
			printsl( path, ": No capabilities set" );
	} else if ( ver > 0 ){
		if ( OPT(v) )
			printvl( path, ": Caps Version ",ver );
		else 
			prints(path);

		char buf[MAX_CAPLEN];
		caps_to_text(buf,&fc);
		if ( fc.magic_etc & VFS_CAP_FLAGS_EFFECTIVE )
			printf(" (active) ");
		else 
			printf( " (inactive) ");
		printf("%s\n",buf);

		if ( ver == 3 ) // Version 3 (untested)
			printf("rootid: %d\n", fc.rootid );

		if ( OPT(x) ){
			uint *ui = (uint*)&(fc.data);
			printf("M:%8x P: %8x %8x I: %8x %8x\n", fc.magic_etc, ui[0],ui[2],ui[1],ui[3] );
		}
	}


	close(fd);
	return(0);
}

// a bitset of permitted and/or inheritable capabilities.
typedef union _capset {
	unsigned int set[2];
	unsigned long l;
} capset_t;

// convert a text into a capset
int text_to_capset( capset_t *r, const char* text ){
	r->l = 0;

	const char *p = text;

	while ( *p ){
		const char *e = p;
		while ( *e && *e!=',' ){
			printf("%c",*e);
			e++;
		}
		printl();
		
		for ( int a = 0; a<CAPCOUNT; a++ ){
			if ( ( strncmp( capnames[a].name, p, e-p-1 ) == 0 ) ||
					( strncmp( capnames[a].shortname, p, e-p-1 ) == 0 ) ){
				printsl("got: ",capnames[a].name);
				r->l |= (ulong)((ulong)1<<(ulong)a);
				p = e;
				break;
			}
		}
		if ( p != e ){
			ewrites("Unknown capability: ");
			write(2,p,e-p);
			return(0);
		}

		if ( *p ) p++;
	}


	return(1);
}

// big endian?
#define FCSET( _capset, _type, _op ) \
	fc->data[0]._type _op _capset->set[0], \
	fc->data[1]._type _op _capset->set[1]

void set_permitted( fcap_t *fc, capset_t *c ){
	FCSET( c, permitted, = );
}
void set_inheritable( fcap_t *fc, capset_t *c ){
	FCSET( c, inheritable, = );
}

void capset_to_fcap( fcap_t *fc, capset_t *permitted, capset_t *inheritable ){
	FCSET( permitted, permitted, = );
	FCSET( inheritable,inheritable, = );
}

void add_to_fcap( fcap_t *fc, capset_t *permitted, capset_t *inheritable ){
	FCSET( permitted, permitted, |= );
	FCSET( inheritable,inheritable, |= );
}

void del_from_fcap( fcap_t *fc, capset_t *permitted, capset_t *inheritable ){
	FCSET( permitted, permitted, &= ~ );
	FCSET( inheritable,inheritable, &= ~ );
}

#undef FCSET



MAIN{
	printf("start, maxcaplen: %d\n",maxcaplen);
	int ret;
	uint opts = 0;

	check_capdefinitions();

	PARSEARGV( 'h': help(), 'u': usage() );

	char *path = *argv;

	for ( ; *argv; *argv++ ){
		if ( (ret=getcap_main( opts, *argv )) < 0 )
			exit_errno(ret);
	}

	capset_t caps; 
	text_to_capset( &caps, "chown,fowner,bpf" );
	printf("%8x %8x\n",caps.set[0],caps.set[1]);


	fcap_t fc;
	//fc.data[0].permitted = caps.set[0];
	//fc.data[1].permitted = caps.set[1];
	set_permitted( &fc, &caps );

	char buf[256];
	caps_to_text( buf, &fc );
	printvl("buf: ",buf);

	fc.data[0].inheritable = caps.set[0];
	fc.data[1].inheritable = caps.set[1];
	caps_to_text( buf, &fc );
	printvl("buf: ",buf);

	int fd = open ( path, O_WRONLY );
	if ( fd<0 ){
		eprintf("Cannot access: %s\n%s\n", path,strerror(errno) );
		return(-errno);
	}
	cap_write_fd( &fc, fd );

		

	exit(0);
}


