
cap_t* cap_init( struct _cap_alloc_s* raw_data)
{
	cap_t* result;

	//raw_data = calloc(1, sizeof(struct _cap_alloc_s));

	raw_data->magic = CAP_T_MAGIC;
	raw_data->size = sizeof(struct _cap_alloc_s);

	result = &raw_data->u.set;
	result->head.version = _CAPABILITY_VERSION;
	capget(&result->head, NULL);      /* load the kernel-capability version */

	switch (result->head.version) {
#ifdef _LINUX_CAPABILITY_VERSION_1
		case _LINUX_CAPABILITY_VERSION_1:
			writesl("v1");
			break;
#endif
#ifdef _LINUX_CAPABILITY_VERSION_2
		case _LINUX_CAPABILITY_VERSION_2:
			writesl("v2");
			break;
#endif
#ifdef _LINUX_CAPABILITY_VERSION_3
		case _LINUX_CAPABILITY_VERSION_3:
			writesl("v3");
			break;
#endif
		default:                          /* No idea what to do */
			ewritesl("Unknown capability version");
			result = NULL;
			break;
	}

	return result;
}

#define CAP_TEXT_BUFFER_ZONE 100


struct cap_launch_s {
	//__u8 mutex;
	/*
	 * Once forked but before active privilege is changed, this
	 * function (if non-NULL) is called.
	 */
	int (*custom_setup_fn)(void *detail);

	/*
	 * user and groups to be used by the forked child.
	 */
	int change_uids;
	uid_t uid;

	int change_gids;
	gid_t gid;
	int ngroups;
	const gid_t *groups;

	/*
	 * mode holds the preferred capability mode. Any non-uncertain
	 * setting here will require an empty ambient set.
	 */
	int change_mode;
	//cap_mode_t mode;

	/*
	 * i,a,[n]b caps. These bitmaps hold all of the capability sets that
	 * cap_launch will affect. nb holds values to be lowered in the bounding
	 * set.
	 */
	struct cap_iab_s *iab;

	/* chroot holds a preferred chroot for the launched child. */
	char *chroot;

	/*
	 * execve style arguments
	 */
	const char *arg0;
	const char *const *argv;
	const char *const *envp;
};
struct cap_iab_s {
	//__u8 mutex;
	__u32 i[_CAPABILITY_U32S];
	__u32 a[_CAPABILITY_U32S];
	__u32 nb[_CAPABILITY_U32S];
};
typedef struct _cap_alloc_s {
	__u32 magic;
	__u32 size;
	union {
		struct _cap_struct set;
		struct cap_iab_s iab;
		struct cap_launch_s launcher;
	} u;
} cap_alloc_s;

char *cap_to_text(cap_t *caps, ssize_t *length_p)
{
	char buf[CAP_TEXT_SIZE+CAP_TEXT_BUFFER_ZONE];
	char *p, *base;
	int histo[8];
	int m, t;
	unsigned n;

	/* Check arguments */
	if (!good_cap_t(caps)) {
		errno = EINVAL;
		return NULL;
	}

	_cap_debugcap("e = ", *caps, CAP_EFFECTIVE);
	_cap_debugcap("i = ", *caps, CAP_INHERITABLE);
	_cap_debugcap("p = ", *caps, CAP_PERMITTED);

	memset(histo, 0, sizeof(histo));

	/* default prevailing state to the named bits */
	cap_value_t cmb = cap_max_bits();
	for (n = 0; n < cmb; n++)
		histo[getstateflags(caps, n)]++;

	/* find which combination of capability sets shares the most bits
		we bias to preferring non-set (m=0) with the >= 0 test. Failing
		to do this causes strange things to happen with older systems
		that don't know about bits 32+. */
	for (m=t=7; t--; )
		if (histo[t] >= histo[m])
		m = t;

	/* blank is not a valid capability set */
	base = buf;
	p = sprintf(buf, "=%s%s%s",
			(m & LIBCAP_EFF) ? "e" : "",
			(m & LIBCAP_INH) ? "i" : "",
			(m & LIBCAP_PER) ? "p" : "" ) + buf;

	for (t = 8; t--; ) {
		if (t == m || !histo[t]) {
			continue;
		}
		*p++ = ' ';
		for (n = 0; n < cmb; n++) {
			if (getstateflags(caps, n) == t) {
				char *this_cap_name = cap_to_name(n);
				if (this_cap_name == NULL) {
					return NULL;
				}
				if ((strlen(this_cap_name) + (p - buf)) > CAP_TEXT_SIZE) {
					cap_free(this_cap_name);
					errno = ERANGE;
					return NULL;
				}
				p += sprintf(p, "%s,", this_cap_name);
				cap_free(this_cap_name);
			}
		}
		p--;
		n = t & ~m;
		if (n) {
			char op = '+';
			if (base[0] == '=' && base[1] == ' ') {
				/*
				 * Special case all lowered default "= foo,...+eip
				 * ..." as "foo,...=eip ...". (Equivalent but shorter.)
				 */
				base += 2;
				op = '=';
			}
			p += sprintf(p, "%c%s%s%s", op,
					(n & LIBCAP_EFF) ? "e" : "",
					(n & LIBCAP_INH) ? "i" : "",
					(n & LIBCAP_PER) ? "p" : "");
		}
		n = ~t & m;
		if (n) {
			p += sprintf(p, "-%s%s%s",
					(n & LIBCAP_EFF) ? "e" : "",
					(n & LIBCAP_INH) ? "i" : "",
					(n & LIBCAP_PER) ? "p" : "");
		}
		if (p - buf > CAP_TEXT_SIZE) {
			errno = ERANGE;
			return NULL;
		}
	}

	/* capture remaining unnamed bits - which must all be +. */
	memset(histo, 0, sizeof(histo));
	for (n = cmb; n < __CAP_MAXBITS; n++)
		histo[getstateflags(caps, n)]++;

	for (t = 8; t-- > 1; ) {
		if (!histo[t]) {
			continue;
		}
		*p++ = ' ';
		for (n = cmb; n < __CAP_MAXBITS; n++) {
			if (getstateflags(caps, n) == t) {
				char *this_cap_name = cap_to_name(n);
				if (this_cap_name == NULL) {
					return NULL;
				}
				if ((strlen(this_cap_name) + (p - buf)) > CAP_TEXT_SIZE) {
					cap_free(this_cap_name);
					errno = ERANGE;
					return NULL;
				}
				p += sprintf(p, "%s,", this_cap_name);
				cap_free(this_cap_name);
			}
		}
		p--;
		p += sprintf(p, "+%s%s%s",
				(t & LIBCAP_EFF) ? "e" : "",
				(t & LIBCAP_INH) ? "i" : "",
				(t & LIBCAP_PER) ? "p" : "");
		if (p - buf > CAP_TEXT_SIZE) {
			errno = ERANGE;
			return NULL;
		}
	}

	_cap_debug("%s", base);
	if (length_p) {
		*length_p = p - base;
	}

	return (_libcap_strdup(base));
}


fcaps_load( fcap_t *)
{
	__u32 magic_etc;
	unsigned tocopy, i;

	magic_etc = FIXUP_32BITS(rawvfscap->magic_etc);
	switch (magic_etc & VFS_CAP_REVISION_MASK) {
		case VFS_CAP_REVISION_1:
			tocopy = VFS_CAP_U32_1;
			bytes -= XATTR_CAPS_SZ_1;
			writesl("v1");
			break;

		case VFS_CAP_REVISION_2:
			tocopy = VFS_CAP_U32_2;
			bytes -= XATTR_CAPS_SZ_2;
			writesl("v2");
			break;

		case VFS_CAP_REVISION_3:
			tocopy = VFS_CAP_U32_3;
			bytes -= XATTR_CAPS_SZ_3;
			result->rootid = FIXUP_32BITS(rawvfscap->rootid);
			writesl("v3");
			break;

		default:
			result = NULL;
			return result;
	}

	/*
	 * Verify that we loaded exactly the right number of bytes
	 */
	if (bytes != 0) {
		result = NULL;
		return result;
	}

	for (i=0; i < tocopy; i++) {
		result->u[i].flat[CAP_INHERITABLE]
			= FIXUP_32BITS(rawvfscap->data[i].inheritable);
		result->u[i].flat[CAP_PERMITTED]
			= FIXUP_32BITS(rawvfscap->data[i].permitted);
		if (magic_etc & VFS_CAP_FLAGS_EFFECTIVE) {
			result->u[i].flat[CAP_EFFECTIVE]
				= result->u[i].flat[CAP_INHERITABLE]
				| result->u[i].flat[CAP_PERMITTED];
		}
	}
	while (i < __CAP_BLKS) {
		result->u[i].flat[CAP_INHERITABLE]
			= result->u[i].flat[CAP_PERMITTED]
			= result->u[i].flat[CAP_EFFECTIVE] = 0;
		i++;
	}

	return result;
}


int cap_get_fd(cap_alloc_s* buf, int fildes){
	cap_t* result;

	/* allocate a new capability set */
	result = cap_init( buf );
	if (result) {
		struct vfs_ns_cap_data rawvfscap;
		int sizeofcaps;

		writes("get cap\n");

		/* fill the capability sets via system call */
		sizeofcaps = fgetxattr(fildes, XATTR_NAME_CAPS,
				&rawvfscap, sizeof(rawvfscap));
		if (sizeofcaps < sizeof(rawvfscap.magic_etc)) {
			result = NULL;
		} else {
			result = _fcaps_load(&rawvfscap, result, sizeofcaps);
		}
	}

	return result;
}


