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

//C(CAP_EPOLLWAKEUP, "epollwakeup" ) // same as BLOCK_SUSPEND



//#include "cap_table.h"

