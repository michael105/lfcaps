#ifndef lfcaps_h
#define lfcaps_h

#ifndef MLIB
#include "kerneldefs.h"
#endif

// little endian only
#define FIXUP(x) (x)

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
	uint32_t rootid; // namespace id, version 3 only
	char version;
} lfcaps_t;


typedef struct vfs_ns_cap_data sys_fcap_t;

//# define HASCAPS( _capset, _capabilities )

//# define SETCAPS( _capset, _capabilities ) _capset = 

//# define ADDCAPS( _capset, _capabilities )

//# define DELCAPS( _capset, _capabilities )

// read caps, reorder the read capabilities into th lfcaps_t struct
// fd or path can be 0
int lfcaps_read( lfcaps_t *caps, int fd, const char* path );

// read capabilities of fd or path
//int syscaps_read( sys_fcap_t *fc, int fd, const char* path );


# define LFCAPS_MAX 40

# define LFCAPS_MAXSTRLEN 432




enum {
	#define CN(_CAP,...) _LFCAP_##_CAP,
	#include "cap_table.h"
	#undef CN
	#define CN(_CAP,...) LFCAP_##_CAP=(1UL<<_LFCAP_##_CAP),
	#include "cap_table.h"
	#undef CN
};




#ifndef MLIB

// errno defs
#define ERRNO(_ret) (_ret<0?errno:0)

#endif


#endif

