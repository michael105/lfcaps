// default settings and common definitions
//


// tiny and extended versions

//#define TINY_TOOL
#define EXTENDED_TOOL
#ifdef EXTENDED_TOOL
#define IFEXT(...) __VA_ARGS__
#define IFTINY(...)
#endif

#ifdef TINY_TOOL
#define IFEXT(...) 
#define IFTINY(...) __VA_ARGS__
#endif

#ifndef IFEXT
#define IFEXT(...) 
#define IFTINY(...) 
#endif


#ifdef MAIN
#undef MAIN
#endif

#ifdef TEST
#define MAIN_TEST int __attribute__((used))main(int argc,char **argv, char **__envp)
#define MAIN int tmain(int argc,char **argv, char **__envp)
#else
#define MAIN int __attribute__((used))main(int argc,char **argv, char **__envp)
// 
#define MAIN_TEST static inline int __main_test(int argc,char **argv, char **__envp)
#endif


#ifndef BUFSIZE
# define BUFSIZE 4000
#endif


#ifndef _ifdef_LICENSE
#define _ifdef_LICENSE
#define _ifdef_VERSION
#endif

#ifndef VERSION
#define VERSION "0.1-devel"
#endif

#ifndef COPYRIGHT
# define COPYRIGHT(_year) " " #_year " misc147 " IFDEF_ELSE(LICENSE,_Q(LICENSE),"CC-BY-SA / AGPL") " www.codeberg.org/misc1\n"
#endif



#ifdef _TOOL
#undef _TOOL
#endif

#define _TOOL _Q(TOOL)


#ifndef TOOL_BUILTIN
# define USAGE(...) \
void _usage(int exc){\
	ewritesl("Usage: " _TOOL " " OPTIONS_USAGE __VA_OPT__(" " __VA_ARGS__) );\
	if ( exc>=0 ) { \
		ewritesl(  _TOOL " -h for help" ); \
		exit(exc);\
	} \
}

# define usage(...) _usage( __VA_ARGS__ +0 )

void _usage(int);


#define TOOL_MAIN_IMPLEMENTATION() int CONC( TOOL, _main )( uint opts, char **argv )

#define TOOL_MAIN() CONC(TOOL,_main)( opts, argv )


#else // BUILTIN

#ifndef _USAGE_DEFINED
#define _USAGE_DEFINED
void __usage( const char* msg, uint size ){
	ewrites("Usage: ");
	ewrite( msg, size );
}
#endif

#endif


//# define HELP(...) _HELP(__VA_ARGS__,"")
# define HELP(_helptext,...) \
void __attribute__((noreturn))help(){\
	ewrites(_Q(TOOL) " " _Q(VERSION) "\n" _helptext "\n"); \
	usage(-1); \
	ewrites("\n" \
			OPTIONS_SUMMARY \
			"\n" \
			__VA_OPT__(__VA_ARGS__ "\n") \
			COPYRIGHT(2026));\
	exit(0);\
}


# define MAN(...) 



