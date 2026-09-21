#ifndef lfcaps_h
#define lfcaps_h

#define LFCAPS_VERSION 0
#define LFCAPS_REVISION 3

#ifndef MLIB
#include <linux/capability.h>
#endif

#if VFS_CAP_REVISION != VFS_CAP_REVISION_2 \
	 &&  VFS_CAP_REVISION != VFS_CAP_REVISION_3
// untested. bitshifts/union could be wrong.
#error vfs cap revision 2 or 3 needed
#endif
#ifndef __LITTLE_ENDIAN 
#error little endian needed
#endif

// bitfield of capabilities
typedef uint64_t lfcaps_capset_t;

// 64bit user definitions
typedef struct _lfcaps_t {
	union {
		lfcaps_capset_t permitted;
		uint32_t _permitted[2];
	};
	union {
		lfcaps_capset_t inheritable;
		uint32_t _inheritable[2];
	};
	uint32_t rootid; // namespace id
	char version; // is only read ,
					  // written is version 2,
					  // upgraded to version 3, if supported
					  // by the kernel and rootid is set.
} lfcaps_t;


// version 3 struct (including namespace rootid)
// is backwards compatible to version 2
typedef struct vfs_ns_cap_data sys_fcap_t;


// read and write caps, 
// reorder the read capabilities into the lfcaps_t struct
// read: return 0 for no capabilities set,
//   1-3 : success, version of the capabilties
//   -errno: error
int lfcaps_read( lfcaps_t *caps, const char* path );
int lfcaps_readfd( lfcaps_t *caps, int fd );

// returns a negative value for errors,
// else 0
int lfcaps_write( lfcaps_t *fc, const char* path );
int lfcaps_writefd( lfcaps_t *fc, int fd );


// read and write without reordering,
// if path is 0, fd is used.
int lfcaps_sysread( sys_fcap_t *fc, int fd, const char* path );
int lfcaps_syswrite( sys_fcap_t *fc, int fd, const char* path );


// write capnames as ascii into buf. 
// buf should be at least LFCAPS_MAXSTRLEN (500)
//MAKRO: int lfcaps_sprint( buf, capset, separator=',' )
# define lfcaps_sprint( _buf, _capset, ... ) \
	__lfcaps_sprint( _buf, _capset, __VA_OPT__(__VA_ARGS__,) ',' )

# define __lfcaps_sprint( _buf, _capset, _separator, ... ) \
	_lfcaps_sprint( _buf, _capset, _separator )

int _lfcaps_sprint( char *buf, lfcaps_capset_t capset, const char separator /* =',' */ );


// return the value of the capability given in str.
// the capability can end with 0 or 'separator'. 
// returns 0, if not found
// e.g. lfcaps_capset_t caps = lfcaps_strtocap( "sys_chroot" );
// add sys_admin (not ptrace)
// caps |= lfcaps_strtocap( "sys_admin,sys_ptrace", ',' );
//MAKRO: lfcaps_capset_t lfcaps_strtocap( const char* str, char* separator = 0 );
# define lfcaps_strtocap( _str, ... ) _lfcaps_strtocap( _str, __VA_ARGS__+0 )
lfcaps_capset_t _lfcaps_strtocap( const char* str, char separator /* = 0 */ );

// look for the substring str within the captable names.
// set ambivalence to 1, to allow ambivalent substrings,
// the first occurance is returned as match.
// returns 0, if not found or ambigous (substring ist multiple within
// the list of capnames, and ambivalence is 0 (default))
//
// lfcaps_capset_t caps = lfcaps_strtocap_substr( "chroot" );
// add sys_admin (not ptrace)
// caps |= lfcaps_strtocap( "admin,ptr", ',' ); fails (admin is ambigous)
// caps |= lfcaps_strtocap( "admin,ptr", ',', 1 ); ok, add admin to the capset
//
// lfcaps_strtocap_substr( _str, separator = 0, ambivalence = 0 ) 
# define lfcaps_strtocap_substr( _str, ... ) __lfcaps_strtocap_substr( _str, __VA_ARGS__+0, 0 )
# define __lfcaps_strtocap_substr( _str, _sep, _amb, ... ) _lfcaps_strtocap_substr( _str, _sep, _amb )
lfcaps_capset_t _lfcaps_strtocap_substr( const char* str, char separator, int ambivalence );
	

# define LFCAPS_COUNT CAP_COUNT

// max len of all capnames including a separator.
# define LFCAPS_MAXSTRLEN 500

// the enum of the capabilities, a 64 bit bitfield. (40 used)
// LFCAP_CHOWN, LFCAP_DAC_OVERRIDE,..
// list of capabilities is in 'man 7 capabilities'.
enum {
	#define _LFCAP_(_CAP,...) _LFCAP_##_CAP,
	#include "cap_table.h"
	#undef _LFCAP_
	#define _LFCAP_(_CAP,...) LFCAP_##_CAP=(1UL<<_LFCAP_##_CAP),
	#include "cap_table.h"
	#undef _LFCAP_
};


#ifndef MLIB

// errno defs
#define ERRNO(_ret) (_ret<0?errno:0)

#endif


#endif

