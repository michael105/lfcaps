/* BSD 2clause

copyright 2026 misc147 codeberg.org/misc1

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ‘AS IS˜ AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef lfcaps_h
#define lfcaps_h

#define LFCAPS_VERSION 0
#define LFCAPS_REVISION 5

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
// lfcaps_strtocap_substr( _str, separator = 0, ambivalence = 0, verbose = 0 ) 
# define lfcaps_strtocap_substr( _str, ... ) __lfcaps_strtocap_substr( _str, __VA_ARGS__+0, 0, 0 )

#define __lfcaps_strtocap_substr( _str, _sep, _amb, _verbose, ... ) _lfcaps_strtocap_substr( _str, _sep, _amb, _verbose )
lfcaps_capset_t _lfcaps_strtocap_substr( const char* str, char separator, int ambivalence, int verbose );
	

# define LFCAPS_COUNT CAP_COUNT

// max len of all capnames including a separator.
# define LFCAPS_MAXSTRLEN 500

// the enum of the capabilities, a 64 bit bitfield. (40 used)
// LFCAP_CHOWN, LFCAP_DAC_OVERRIDE,..
// list of capabilities is in 'man 7 capabilities'.
enum {
	#define _LFCAP_(_CAP,...) _LFCAP_##_CAP,
	#include "lfcapslib.h"
	#undef _LFCAP_
	#define _LFCAP_(_CAP,...) LFCAP_##_CAP=(1UL<<_LFCAP_##_CAP),
	#include "lfcapslib.h"
	#undef _LFCAP_
};


#ifndef MLIB

// errno defs
#define ERRNO(_ret) (_ret<0?errno:0)

#endif


#else // recursed inclusion
#ifdef _LFCAP_

//  
// table of capabilities and their names.
// sorted, 0-40, by number


_LFCAP_(CHOWN, "chown", "chown" )
_LFCAP_(DAC_OVERRIDE, "dac_override", "ovrd" )
_LFCAP_(DAC_READ_SEARCH, "dac_read_search" , "dacrs" )
_LFCAP_(FOWNER, "fowner", "fown" )
_LFCAP_(FSETID, "fsetid", "fsid" )
_LFCAP_(KILL, "kill", "kill" )
_LFCAP_(SETGID, "setgid", "sgid" )
_LFCAP_(SETUID, "setuid", "suid" )
_LFCAP_(SETPCAP, "setpcap", "spcap" )
_LFCAP_(LINUX_IMMUTABLE, "linux_immutable", "immutbl" )
_LFCAP_(NET_BIND_SERVICE, "net_bind_service", "bind" )
_LFCAP_(NET_BROADCAST, "net_broadcast", "brdcast" )
_LFCAP_(NET_ADMIN, "net_admin", "netadmin" )
_LFCAP_(NET_RAW, "net_raw", "netraw" )
_LFCAP_(IPC_LOCK, "ipc_lock", "ilock" )
_LFCAP_(IPC_OWNER, "ipc_owner", "iown" )
_LFCAP_(SYS_MODULE, "sys_module", "module" )
_LFCAP_(SYS_RAWIO, "sys_rawio", "rawio" )
_LFCAP_(SYS_CHROOT, "sys_chroot", "chroot" )
_LFCAP_(SYS_PTRACE, "sys_ptrace", "ptrace" )
_LFCAP_(SYS_PACCT, "sys_pacct", "pacct" )
_LFCAP_(SYS_ADMIN, "sys_admin", "admin" )
_LFCAP_(SYS_BOOT, "sys_boot", "boot" )
_LFCAP_(SYS_NICE, "sys_nice", "nice" )
_LFCAP_(SYS_RESOURCE, "sys_resource", "resource" )
_LFCAP_(SYS_TIME, "sys_time", "time" )
_LFCAP_(SYS_TTY_CONFIG, "sys_tty_config", "ttyconf" )
_LFCAP_(MKNOD, "mknod", "mknod" )
_LFCAP_(LEASE, "lease", "lease" )
_LFCAP_(AUDIT_WRITE, "audit_write", "auditwr" )
_LFCAP_(AUDIT_CONTROL, "audit_control", "auditctl" )
_LFCAP_(SETFCAP, "setfcap", "fcap" )
// test kernel defs and existence
#ifndef CAP_MAC_OVERRIDE
#define CAP_COUNT 31
#else
#if CAP_MAC_OVERRIDE != 32
#error mismatch
#endif
_LFCAP_(MAC_OVERRIDE, "mac_override", "macovrd" )
#ifndef CAP_MAC_ADMIN
#define CAP_COUNT 32
#else
#if CAP_MAC_ADMIN != 33
#error mismatch
#endif
_LFCAP_(MAC_ADMIN, "mac_admin", "macadmin" )
#ifndef CAP_SYSLOG
#define CAP_COUNT 33
#else
#if CAP_SYSLOG != 34
#error mismatch
#endif
_LFCAP_(SYSLOG, "syslog", "log" )
#ifndef CAP_WAKE_ALARM
#define CAP_COUNT 34
#else
#if CAP_WAKE_ALARM != 35
#error mismatch
#endif
_LFCAP_(WAKE_ALARM, "wake_alarm", "wake" )
#ifndef CAP_BLOCK_SUSPEND
#define CAP_COUNT 35
#else
#if CAP_BLOCK_SUSPEND != 36
#error mismatch
#endif
_LFCAP_(BLOCK_SUSPEND, "block_suspend", "blksusp" )
#ifndef CAP_AUDIT_READ
#define CAP_COUNT 36
#else
#if CAP_AUDIT_READ != 37
#error mismatch
#endif
_LFCAP_(AUDIT_READ, "audit_read", "auditr" )
#ifndef CAP_PERFMON
#define CAP_COUNT 37
#else
#if CAP_PERFMON != 38
#error mismatch
#endif
_LFCAP_(PERFMON, "perfmon", "pmon" )
#ifndef CAP_BPF
#define CAP_COUNT 38
#else
#if CAP_BPF != 39
#error mismatch
#endif
_LFCAP_(BPF, "bpf", "bpf" )
#ifndef CAP_CHECKPOINT_RESTORE
#define CAP_COUNT 39
#else
#define CAP_COUNT 40
#if CAP_CHECKPOINT_RESTORE != 40
#error mismatch
#endif
_LFCAP_(CHECKPOINT_RESTORE, "checkpoint_restore", "chkpoint" )
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
 

#if CAP_CHOWN != 0 || CAP_DAC_OVERRIDE != 1 || CAP_DAC_READ_SEARCH != 2 \
	|| CAP_FOWNER != 3 || CAP_FSETID != 4 || CAP_KILL != 5 || CAP_SETGID != 6 \
   || CAP_SETUID != 7 || CAP_SETPCAP != 8 || CAP_LINUX_IMMUTABLE != 9 \
   || CAP_NET_BIND_SERVICE != 10 || CAP_NET_BROADCAST != 11 \
   || CAP_NET_ADMIN != 12 || CAP_NET_RAW != 13 || CAP_IPC_LOCK != 14 \
   || CAP_IPC_OWNER != 15 || CAP_SYS_MODULE != 16 || CAP_SYS_RAWIO != 17 \
   || CAP_SYS_CHROOT != 18 || CAP_SYS_PTRACE != 19 || CAP_SYS_PACCT != 20 \
   || CAP_SYS_ADMIN != 21 || CAP_SYS_BOOT != 22 || CAP_SYS_NICE != 23 \
   || CAP_SYS_RESOURCE != 24 || CAP_SYS_TIME != 25 || CAP_SYS_TTY_CONFIG != 26 \
   || CAP_MKNOD != 27 || CAP_LEASE != 28 || CAP_AUDIT_WRITE != 29 \
   || CAP_AUDIT_CONTROL != 30 || CAP_SETFCAP != 31

#error mismatch with kernel definitions

#endif

#endif
#endif

