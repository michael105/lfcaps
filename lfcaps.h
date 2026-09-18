#ifndef lfcaps_h
#define lfcaps_h

#ifndef MLIB
//#include "kerneldefs.h"
#include <linux/capability.h>
#endif


typedef uint64_t lfcaps_capset_t;

// 64bit user definitions
typedef struct _lfcaps_t {
	union {
		uint64_t permitted;
		uint32_t _permitted[2];
	};
	union {
		uint64_t inheritable;
		uint32_t _inheritable[2];
	};
	uint32_t rootid; // namespace id (version 3)
	char version;
} lfcaps_t;


typedef struct vfs_ns_cap_data sys_fcap_t;


// read caps, reorder the read capabilities into th lfcaps_t struct
// fd or path can be 0
int lfcaps_read( lfcaps_t *caps, int fd, const char* path );

int lfcaps_write( lfcaps_t *fc, int fd, const char* path );

int lfcaps_sysread( sys_fcap_t *fc, int fd, const char* path );

int lfcaps_syswrite( sys_fcap_t *fc, int fd, const char* path );

//MAKRO: int lfcaps_sprint( buf, capset, separator=',' )
int _lfcaps_sprint( char *buf, lfcaps_capset_t capset, const char separator /* =',' */ );

//MAKRO: lfcaps_capset_t lfcaps_strtocap( const char* str, char* separator = 0 );
lfcaps_capset_t _lfcaps_strtocap( const char* str, char separator /* = 0 */ );


# define LFCAPS_COUNT CAP_COUNT

# define LFCAPS_MAXSTRLEN 500


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

