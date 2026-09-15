// linux kernel header definitions.
#ifndef _LINUX_CAPABILITY_H

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
#define FIXUP(x) bswap_32(x)
#error untested
#else
#define FIXUP_32BITS(x) (x)
  #define FIXUP(x) (x)
#endif

#define __CAP_MAXBITS (__CAP_BLKS * 32)

//#define _CAPABILITY_VERSION _CAPABILITY_VERSION_3
#define CAP_T_MAGIC 0xCA90D0

#define VFS_CAP_REVISION_MASK	0xFF000000
#define VFS_CAP_REVISION_SHIFT	24
#define VFS_CAP_FLAGS_MASK	~VFS_CAP_REVISION_MASK
#define VFS_CAP_FLAGS_EFFECTIVE	0x000001

#define VFS_CAP_REVISION_1	0x01000000
#define VFS_CAP_U32_1           1
#define XATTR_CAPS_SZ_1         (sizeof(__le32)*(1 + 2*VFS_CAP_U32_1))

#define VFS_CAP_REVISION_2	0x02000000
#define VFS_CAP_U32_2           2
#define XATTR_CAPS_SZ_2         (sizeof(__le32)*(1 + 2*VFS_CAP_U32_2))

#define VFS_CAP_REVISION_3	0x03000000
#define VFS_CAP_U32_3           2
#define XATTR_CAPS_SZ_3         (sizeof(__le32)*(2 + 2*VFS_CAP_U32_3))

#define XATTR_CAPS_SZ           XATTR_CAPS_SZ_3
#define VFS_CAP_U32             VFS_CAP_U32_3
#define VFS_CAP_REVISION	VFS_CAP_REVISION_3


/*
 * same as vfs_cap_data but with a rootid at the end
 */
struct vfs_ns_cap_data {
	__le32 magic_etc;
	struct {
		__le32 permitted;    /* Little endian */
		__le32 inheritable;  /* Little endian */
	} data[VFS_CAP_U32];
	__le32 rootid;
};




#endif

