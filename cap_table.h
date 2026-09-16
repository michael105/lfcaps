#include <linux/capability.h>
// table of capabilities and their names.
// sorted, 0-40, by number
CN(CHOWN, "chown", "chown" )
CN(DAC_OVERRIDE, "dac_override", "ovrd" )
CN(DAC_READ_SEARCH, "dac_read_search" , "dacrs" )
CN(FOWNER, "fowner", "fown" )
CN(FSETID, "fsetid", "fsid" )
CN(KILL, "kill", "kill" )
CN(SETGID, "setgid", "sgid" )
CN(SETUID, "setuid", "suid" )
CN(SETPCAP, "setpcap", "spcap" )
CN(LINUX_IMMUTABLE, "linux_immutable", "immutbl" )
CN(NET_BIND_SERVICE, "net_bind_service", "bind" )
CN(NET_BROADCAST, "net_broadcast", "brdcast" )
CN(NET_ADMIN, "net_admin", "netadmin" )
CN(NET_RAW, "net_raw", "netraw" )
CN(IPC_LOCK, "ipc_lock", "ilock" )
CN(IPC_OWNER, "ipc_owner", "iown" )
CN(SYS_MODULE, "sys_module", "module" )
CN(SYS_RAWIO, "sys_rawio", "rawio" )
CN(SYS_CHROOT, "sys_chroot", "chroot" )
CN(SYS_PTRACE, "sys_ptrace", "ptrace" )
CN(SYS_PACCT, "sys_pacct", "pacct" )
CN(SYS_ADMIN, "sys_admin", "admin" )
CN(SYS_BOOT, "sys_boot", "boot" )
CN(SYS_NICE, "sys_nice", "nice" )
CN(SYS_RESOURCE, "sys_resource", "resource" )
CN(SYS_TIME, "sys_time", "time" )
CN(SYS_TTY_CONFIG, "sys_tty_config", "ttyconf" )
CN(MKNOD, "mknod", "mknod" )
CN(LEASE, "lease", "lease" )
CN(AUDIT_WRITE, "audit_write", "auditwr" )
CN(AUDIT_CONTROL, "audit_control", "auditctl" )
CN(SETFCAP, "setfcap", "fcap" )
// 31
CN(MAC_OVERRIDE, "mac_override", "macovrd" )
CN(MAC_ADMIN, "mac_admin", "macadmin" )
CN(SYSLOG, "syslog", "log" )
CN(WAKE_ALARM, "wake_alarm", "wake" )
CN(BLOCK_SUSPEND, "block_suspend", "blksusp" )
CN(AUDIT_READ, "audit_read", "auditr" )
CN(PERFMON, "perfmon", "pmon" )
CN(BPF, "bpf", "bpf" )
CN(CHECKPOINT_RESTORE, "checkpoint_restore", "chkpoint" )
 

#if CAP_CHOWN != 0 || CAP_DAC_OVERRIDE != 1 || CAP_DAC_READ_SEARCH != 2 || CAP_FOWNER != 3 || CAP_FSETID != 4 || CAP_KILL != 5 || CAP_SETGID != 6 || CAP_SETUID != 7 || CAP_SETPCAP != 8 || CAP_LINUX_IMMUTABLE != 9 || CAP_NET_BIND_SERVICE != 10 || CAP_NET_BROADCAST != 11 || CAP_NET_ADMIN != 12 || CAP_NET_RAW != 13 || CAP_IPC_LOCK != 14 || CAP_IPC_OWNER != 15 || CAP_SYS_MODULE != 16 || CAP_SYS_RAWIO != 17 || CAP_SYS_CHROOT != 18 || CAP_SYS_PTRACE != 19 || CAP_SYS_PACCT != 20 || CAP_SYS_ADMIN != 21 || CAP_SYS_BOOT != 22 || CAP_SYS_NICE != 23 || CAP_SYS_RESOURCE != 24 || CAP_SYS_TIME != 25 || CAP_SYS_TTY_CONFIG != 26 || CAP_MKNOD != 27 || CAP_LEASE != 28 || CAP_AUDIT_WRITE != 29 || CAP_AUDIT_CONTROL != 30 || CAP_SETFCAP != 31
#define _DEF_ERR 
#endif

#ifdef _DEF_ERR
#error mismatch with kernel definitions
#endif




//C(CAP_EPOLLWAKEUP, "epollwakeup" ) // same as BLOCK_SUSPEND
