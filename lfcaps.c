#include "lfcaps.h"




int lfcaps_read( lfcaps_t *caps, int fd, const char* path ){
	sys_fcap_t fc;
	int ret = syscaps_read( &fc, fd, path );

	if ( ret<0 ) return ret;
	
	caps->version=ret;
	caps->_permitted[0] = fc.data[0].permitted;
	caps->_permitted[1] = fc.data[1].permitted;
	caps->_inheritable[0] = fc.data[0].inheritable;
	caps->_inheritable[1] = fc.data[1].inheritable;
	caps->rootid = fc.rootid;

	return ret;
}


// read sys_fcap. return 0 for no capabilities,
// 1, 2 or 3 for success and the read file capability version
// else -ernno (<0), -EINVAL if the read attribute's size doesn't 
// match the attribute size of the capability version
int syscaps_read( sys_fcap_t *fc, int fd, const char* path ){
	int ret;

	if ( fd ){
		ret = fgetxattr(fd, XATTR_NAME_CAPS, fc, sizeof(sys_fcap_t));
	} else {
		ret = getxattr( path, XATTR_NAME_CAPS, fc, sizeof(sys_fcap_t));
	}

	if ( ERRNO(ret) == ENODATA ){
		bzero( fc, sizeof( sys_fcap_t ) ); // needs to be done explicite
		return(0);
	}
	else if ( ret <= 0 ){
		ewritesl("Cannot read capabilities");
		//printf( "%d%s%d\n",ret, " e: ", errno );
		return(-ERRNO(ret));
	}
	printvl(PVAR(ret));

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
int syscaps_write( fcap_t *fc, int fd, const char* path ){
	fc->magic_etc = FIXUP(VFS_CAP_REVISION_2 | VFS_CAP_FLAGS_EFFECTIVE );
	int ret;
	if ( fd ) 
		ret = fsetxattr( fd, XATTR_NAME_CAPS, fc, XATTR_CAPS_SZ_2, 0);
	else
		ret = setxattr( path, XATTR_NAME_CAPS, fc, XATTR_CAPS_SZ_2, 0);
	

	return(ret);
}


int _lfcaps_sprint( char *buf, lfcaps_capset_t capset, const char separator ){
# define lfcaps_sprint( _buf, _capset, ... ) _lfcaps_sprint( _buf, _capset, VA_DEFAULT( 1,',', __VA_ARGS__ ) )

	


	return(0);
}


MAIN{
	printsl( *argv );

	lfcaps_t fc;

	for ( argv++; *argv; argv++ ){
	
		int ret = lfcaps_read( &fc, 0, *argv );

		printvl( *argv, ": \terrno:", ret, 
				"\n  permitted:     ", FMT(.base=2), fc.permitted, 
				"\n  inheritable:   ", fc.inheritable );

		fc.permitted = 

	}






	exit(0);
}




