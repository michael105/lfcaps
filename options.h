#ifndef options4_h
// vt100 ctrl sequence, goto column 20 in the summaries
// (insert a tab, for parsing)
#define VTPOS "\t\e[20G"
								

//-------

#define _IF_STR_OPTION(a,c,...) __VA_OPT__( a, __VA_ARGS__, c, )
#define _STR_OPTION(a,b,c) _IF_STR_OPTION(a,c,b)
#define __STRING_OPTIONS(...) _FOREACH_TRIPLE(_STR_OPTION,,__VA_ARGS__)
#define _STRING_OPTIONS __STRING_OPTIONS(OPTIONS)
#define _T_STRING_OPTIONS(_tool) __STRING_OPTIONS(_tool##_OPTIONS)
//_STRING_OPTIONS

#define _T_INT_OPTS(_tool) __T_INT_OPTS(_tool)
#define ____T_INT_OPTS(a,...) __VA_OPT__(a,__VA_ARGS__)
#define ___T_INT_OPTS(...) ____T_INT_OPTS(__VA_ARGS__)
#define __T_INT_OPTS(_tool) ___T_INT_OPTS(_tool##_INT_OPTS)

#define __EMPTY x,x
#define __M2IF_OPT(a,c,x,...) __VA_OPT__( a,,c, )
#define _M2IF_OPT(a,c,b) __M2IF_OPT(a,c,b)
#define __IF_NOTOPT(a,c,exp) _M2IF_OPT( a,c,__EMPTY##exp)
#define _SHORT_OPT(a,b,c) __IF_NOTOPT( a,c,b )
#define ___SHORT_OPTIONS(...) FOREACH_TRIPLE(_SHORT_OPT,,__VA_ARGS__)
#define __SHORT_OPTIONS(...) ___SHORT_OPTIONS(__VA_ARGS__)
#define _OPTIONS __SHORT_OPTIONS(OPTIONS)
 //  _OPTIONS

#define __OPT(a,b,c) a,
#define __OPTS(...) _FOREACH_TRIPLE(__OPT,,__VA_ARGS__)
#define _OPTS __OPTS(_OPTIONS)

#define _T_OPTS(_tool) __OPTS(__SHORT_OPTIONS(_tool##_OPTIONS))
 //  _OPTS   sequence

#define __STRING_OPT(a,b,c) a,
#define __STRING_OPTS(...) _FOREACH_TRIPLE(__STRING_OPT,,__VA_ARGS__)
#define _STRING_OPTS __STRING_OPTS(_STRING_OPTIONS)
#define _T_STRING_OPTS(_tool) __STRING_OPTS(_T_STRING_OPTIONS(_tool))
//_STRING_OPTS, sequence
#define T_STRING_OPTS_STR(_tool) __OPTSTR(_T_STRING_OPTS(_tool))


#define __OPTSTR(...) CONCATS( __VA_ARGS__ )
#define _OPTSTR __OPTSTR( _STRING_OPTS _OPTS )
#define _T_OPTSTR(_tool) __OPTSTR( _T_STRING_OPTS(_tool) _T_OPTS(_tool) )
//_OPTSTR

#define _SHORT_OPTSTR __OPTSTR( _OPTS )
# define SHORT_OPTSTR __OPTSTR( _OPTS )
# define T_SHORT_OPTSTR(_tool) __OPTSTR( _T_OPTS(_tool) )
//_OPTSTR, string


#define _ASSIGN_STRN(a,b,c) ,b=a, _##b=_##a
#define __ASSIGN_STRING_NAMES(...) _FOREACH_TRIPLE(_ASSIGN_STRN,,__VA_ARGS__)
#define _ASSIGN_STRING_NAMES __ASSIGN_STRING_NAMES(_STRING_OPTIONS)
#define _T_ASSIGN_STRING_NAMES __ASSIGN_STRING_NAMES(_T_STRING_OPTIONS(_tool))
//_LONG_ASSIGN


// build a enum of options, prepended with _ (_a,_b,..) then 
// append the optiones, shifted by it's number. (a=1<<_a,..)
#define _OPT_ENUM(_opt) _opt=(1<<_##_opt)
#define _OPTIONS_ENUM(...) enum { ANY=-1, PREP(_,__VA_ARGS__), _FOREACH_K(_OPT_ENUM,__VA_ARGS__) _ASSIGN_STRING_NAMES }

#define _T_OPTIONS_ENUM(...) enum { ANY=-1, PREP(_,__VA_ARGS__), _FOREACH_K(_OPT_ENUM,__VA_ARGS__) }
# define OPTIONS_ENUM _OPTIONS_ENUM( _STRING_OPTS _OPTS )


# define T_OPTIONS_ENUM(_tool) _T_OPTIONS_ENUM( _T_STRING_OPTS(_tool) _T_OPTS(_tool) _T_ASSIGN_STRING_NAMES(_tool) )
//OPS ENUM
 //  OPTIONS_ENUM



#define _ifdef_SETTINGS
# define IFDEF_SETTINGS APPEND_K(SETTINGS)

#define _SETTINGS IFDEF_LIST(SETTINGS)


#define _ASSIGN_NAME(a,b,c) b=a,
#define __ASSIGN_STRINGS(...) _FOREACH_TRIPLE(_ASSIGN_NAME,,__VA_ARGS__)
#define _ASSIGN_STRINGS __ASSIGN_STRINGS(_STRING_OPTIONS)

#define _SETTINGS_ENUM(...) enum { __VA_ARGS__ _ASSIGN_STRINGS }
# define SETTINGS_ENUM _SETTINGS_ENUM( _STRING_OPTS _SETTINGS )
# define T_SETTINGS_ENUM(_tool) _SETTINGS_ENUM( _T_STRING_OPTS(_tool) _SETTINGS )
 //  SETTINGS_ENUM

#define _M3_STR_OPT(a,b,c) "[-" #a " " #b  "] " 
#define __STRING_OPTIONS_USAGE(...) _FOREACH_TRIPLE(_M3_STR_OPT,,__VA_ARGS__)
# define STRING_OPTIONS_USAGE __STRING_OPTIONS_USAGE(_STRING_OPTIONS)
# define T_STRING_OPTIONS_USAGE(_tool) __STRING_OPTIONS_USAGE(_T_STRING_OPTIONS(_tool))
 //  STRING_OPTIONS_USAGE


#define __SHORT_OPTIONS_USAGE(...) __VA_OPT__( "[-"_SHORT_OPTSTR"] " )
#define _SHORT_OPTIONS_USAGE(...) __SHORT_OPTIONS_USAGE(__VA_ARGS__)
# define SHORT_OPTIONS_USAGE _SHORT_OPTIONS_USAGE(_OPTS)
# define ___SHORT_OPTIONS_USAGE(...) _SHORT_OPTIONS_USAGE(__VA_ARGS__)
# define T_SHORT_OPTIONS_USAGE(_tool) ___SHORT_OPTIONS_USAGE(_T_OPTS(_tool))
 //  SHORT_OPTIONS_USAGE

# define OPTIONS_USAGE SHORT_OPTIONS_USAGE STRING_OPTIONS_USAGE
 //  OPTIONS_USAGE

# define T_OPTIONS_USAGE(_tool) T_SHORT_OPTIONS_USAGE(_tool) T_STRING_OPTIONS_USAGE(_tool)

#ifdef VTPOS
#define _STR_OPT_SUMMARY(a,b,c) " -"#a " " #b VTPOS c "\n"
#else
#define _STR_OPT_SUMMARY(a,b,c) " -"#a " " #b "\t" c "\n"
#endif
#define __STRING_OPTIONS_SUMMARY(...) __VA_OPT__(_FOREACH_TRIPLE(_STR_OPT_SUMMARY,,__VA_ARGS__))
# define STRING_OPTIONS_SUMMARY __STRING_OPTIONS_SUMMARY(_STRING_OPTIONS)
 //  STRING_OPTIONS_SUMMARY


#ifdef VTPOS
#define _SHORT_OPT_SUMMARY(a,b,c) " -"#a   VTPOS c "\n"
#else
#define _SHORT_OPT_SUMMARY(a,b,c) " -"#a "\t\t" c "\n"
#endif
#define __SHORT_OPTIONS_SUMMARY(...) _FOREACH_TRIPLE(_SHORT_OPT_SUMMARY,,__VA_ARGS__)
# define SHORT_OPTIONS_SUMMARY __SHORT_OPTIONS_SUMMARY(_OPTIONS)
# define T_SHORT_OPTIONS_SUMMARY(_tool) __SHORT_OPTIONS_SUMMARY(_T_OPTIONS(_tool))
//SHORT_OPTIONS_SUMMARY


#ifdef VTPOS
#define _OPT_SUMMARY(a,b,c) " -"#a " " #b  VTPOS c "\n"
#else
// trouble: inserts always two \t
#define _OPT_SUMMARY(a,b,c) " -"#a IFOPT(b,b) IFOPT(_STRING_OPTIONS,IFNOT_OPT(b,"")) "\t" c "\n"
#endif
#define __OPTIONS_SUMMARY(...) _FOREACH_TRIPLE(_OPT_SUMMARY,,__VA_ARGS__)
# define OPTIONS_SUMMARY __OPTIONS_SUMMARY(OPTIONS)
# define T_OPTIONS_SUMMARY(_tool) __OPTIONS_SUMMARY(_T_OPTIONS(_tool))
//OPTIONS_SUMMARY



// needed for IF_DEF
#define _ifdef_LONGINT_OPTS
#define _ifdef_INT_OPTS
#define _ifdef_OPTS

#define _OE64(__l) _##__l
#define _OE64B(__l) __l = (1UL<<_##__l)
# define OPTIONS64_ENUM enum { _a=10, \
	FOREACH_K(_OE64,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z), \
	FOREACH_K(_OE64B,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z) }


#define _CHECK_OPTION(a,b,c) { a, #b, c },
#define _CHECK_OPTIONS(...) inline void __attribute__((always_inline))check_options(){\
	enum letters { a=10,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z };\
	struct { enum letters opt; char* name; char* help; } options[] = { _FOREACH_TRIPLE(_CHECK_OPTION,,__VA_ARGS__) }; }

# define CHECK_OPTIONS \
	enum { OPTIONS_not_checked_CHECK_OPTIONS=0 }; \
	_CHECK_OPTIONS(OPTIONS)

#define _CHECKOPTIONS(_tool,...) \
	   enum { CONC(_tool,_OPTIONS_not_checked)=0 }; \
	inline void __attribute__((always_inline))CONC(check_options_,_tool)(){\
	enum letters { a=10,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z };\
	struct { enum letters opt; char* name; char* help; } options[] = { _FOREACH_TRIPLE(_CHECK_OPTION,,__VA_ARGS__) }; }


# define CHECKOPTIONS(_tool) _CHECKOPTIONS(_tool,_tool##_OPTIONS)

// GET/SET variables of the setting.
// defaults to char*, 2nd/3rd optional argument is the type:
// str,ulong,long,uint,int,char,uchar
//
// get setting variable, default char* (str)
# define GET(_name,...) ({ _GET(_name,__VA_OPT__(__VA_ARGS__,) str); })
# define T_GET(_tool,_name,...) ({ _T_GET(_tool,_name,__VA_OPT__(__VA_ARGS__,) str); })
#define _GET(_name,_type,...) ({ SETTINGS_ENUM; setting[_name].value_ ##_type; })
#define _T_GET(_tool,_name,_type,...) ({ T_SETTINGS_ENUM(_tool); setting[_name].value_ ##_type; })


// set a value of the setting. Default: char* (str)
# define SET(_name,_value,...) { _SET(_name,_value,__VA_OPT__(__VA_ARGS__,) str ); }
# define T_SET(_tool,_name,_value,...) { _T_SET(_tool,_name,_value,__VA_OPT__(__VA_ARGS__,) str ); }
#define _SET(_name,_value,_type,...) { SETTINGS_ENUM; setting[_name].value_ ##_type = _value; }
#define _T_SET(_tool,_name,_value,_type,...) { T_SETTINGS_ENUM(_tool); setting[_name].value_ ##_type = _value; }


typedef union {
		char *value_str;
		ulong value_ulong;
		long value_long;
		uint value_uint;
		int value_int;
		char value_char;
		uchar value_uchar;
} setting_t;



//#define __DECLARE_SETTING(...) __VA_OPT__( setting_t setting[ COUNT(__VA_ARGS__) + OPTIONS_not_checked_CHECK_OPTIONS ] )
#define __DECLARE_SETTING(...) setting_t setting[ COUNT(__VA_ARGS__) ]
//#define __DECLARE_SETTING(...) __VA_OPT__( setting_t setting[ COUNT(__VA_ARGS__) ] )
#define _DECLARE_SETTING( _s ) __DECLARE_SETTING(_s)

// string opts, and setting variables
// _SETTINGS can be optionally defined, to add other settings to the setting array
# define DECLARE_SETTING _DECLARE_SETTING( _STRING_OPTS _SETTINGS  )


#ifdef OPTIONS64
# define OPT(_opt)    ({ OPTIONS64_ENUM; opts & ( _opt ); })
# define SETOPT(_opt) ({ OPTIONS64_ENUM; opts |= ( _opt ); })
# define DELOPT(_opt) ({ OPTIONS64_ENUM; tnode->opts = tnode->opts & ~( _opt ); })
#else
# define OPT(_opt)    ({ OPTIONS_ENUM; opts & ( _opt ); })
# define SETOPT(_opt) ({ OPTIONS_ENUM; opts |= ( _opt ); })
# define DELOPT(_opt) ({ OPTIONS_ENUM; opts = opts & ~( _opt ); })
#endif

# define OPT_BITS(_opt) ({ OPTIONS_ENUM; _opt; })
# define OPT_BITS64(_opt) ({ OPTIONS64_ENUM; _opt; })
# define T_OPT_BITS(_tool,_opt) ({ T_OPTIONS_ENUM; _opt; })
# define OPT_NUM(_opt)  ({ OPTIONS_ENUM; _opt; })


# define T_OPT(_tool,_opt)    ({ T_OPTIONS_ENUM(_tool); opts & ( _opt ); })
# define T_SETOPT(_tool,_opt) ({ T_OPTIONS_ENUM(_tool); opts |= ( _opt ); })
# define T_DELOPT(_tool,_opt) ({ T_OPTIONS_ENUM(_tool); opts = opts & ~( _opt ); })

#define OPTBIT64(_x) (1UL<<(_x))
# define OPTFIELD64(...) FOREACH(OPTBIT64,|,__VA_ARGS__ ) IFNOT_OPT(__VA_ARGS__,+0)
#define OPTFIELD64B(...) ({ OPT64_enum; uint64_t _ret =  FOREACH(OPTBIT64,|,__VA_ARGS__) IFNOT_OPT(__VA_ARGS__,0); _ret; })



#define __OPTION_CASE(_arg) case _arg ; break;

//# define PARSEARGV(...) OPTIONS_not_checked_CHECK_OPTIONS; while ( *++argv && **argv=='-' ){\

# define PARSEARGV(...) \
{\
	char *arg0 = *argv; \
	while ( *++argv && **argv=='-' ){\
		argc--;\
		for ( const char *_o = *argv+1; *_o; _o++ ){\
			const char* _emsg = "Unknown option:   -",\
					*_optstr = _OPTSTR, *_p=_optstr;\
			while ( *_p && *_p != *_o) _p++;\
			opts |= 1 << (_p - _optstr);\
			IF_OPT( _STRING_OPTS ,\
				if ( _p-_optstr < COUNT(_STRING_OPTS) ){\
					*argv++;\
					if ( !*argv ){\
						_p = "";\
						_emsg = "Missing argument: -";\
					} else {\
						IF_DEF( LONGINT_OPTS, if ( OPT_BITS( LONGINT_OPTS ) & ( 1<<(_p-_optstr) ) ) \
							SET(_p-_optstr,strtol(*argv,0,0),long) else )\
						IF_DEF( INT_OPTS, if ( OPT_BITS( INT_OPTS ) & ( 1<<(_p-_optstr) ) ) \
							SET(_p-_optstr,atoi(*argv),int) else )\
						SET(_p-_optstr,*argv);\
					}\
				} )\
			switch (*_o){\
				FOREACH(__OPTION_CASE,,__VA_ARGS__)\
				default: \
					if ( *_p ) break;\
					ewrite(_emsg,20);\
					char _b[2] = {*_o,'\n'};\
					ewrite(_b,2);\
					_usage(EINVAL); \
			}\
		}\
	}\
  *(--argv) = arg0; \
}
	
// throws errors.?
//				IF_OPT_ELSE(__VA_ARGS__,FOREACH(__OPTION_CASE,,__VA_ARGS__),\
						__OPTION_CASE('h':help()) __OPTION_CASE('u':usage()) )\


//# define FOREACH_ARGV(_var) for (char *_var=*argv;(_var=*argv);*argv++) 

# define IF_ARGV() if ( *argv )

#define FOREACH_ARGV for (char *arg=*argv;(arg=*argv);*argv++) 

// ============================
// opt64 options

#define OPT64_enum enum _opt64 { a=10,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z };

	# define _BCHOPT(_x) (_x>='a'&&_x<='z' ? _x-'a'+10 : _x>='A' && _x<='Z'? _x-'A'+36 : _x>='0' && _x<='9' ? _x-'0' : -1)

	# define OPT64BIT(_x) (1UL<<_BCHOPT(_x))


#define options4_h
#endif

#ifdef tool_options
#define _T_OPTIONS(_t) __T_OPTIONS(_t)
#define __T_OPTIONS(_tool) _tool##_OPTIONS
#define OPTIONS __SHORT_OPTIONS(_T_OPTIONS( TOOL ))
#endif


// ============================

#ifdef STANDALONE_options


#define OPTIONS \
	h,, "Show this help",\
	d,, "all files", \
	e,name, "belong to", \
	g,glob,  "globbing option", \
	v,, "some option", \
	i,infile,"Input file", \
	s,outfile,"Output file", \
	u,,"Show usage" \



// compiletime only check of the consistency of the options definition
CHECK_OPTIONS


//#define LONGINT_OPTS e // bitfield, to convert. combine: g|e|v ...
//#define INT_OPTS g

#define SETTINGS val1, val2

DECLARE_SETTING;




void usage(int exc){
	ewritesl("Usage: options " OPTIONS_USAGE  );
	if ( exc>=0 ) exit(exc);
}

void help(){
	ewritesl("\noptions\n");
	usage(-1); //OPTIONS_USAGE
	ewritesl("\nhowto use options\n" "\n" OPTIONS_SUMMARY );
	exit(0);
}

MAIN{

	uint opts = 0;
	int verbose = 0;

	PARSEARGV( 'h': help(), 'v': verbose++, 'u':usage(0) );


	if ( OPT(v) ){
		ewritesl("Opt v");
		printf("verbosity: %d\n",verbose);
		SET(val1,"hello");
	}


	if ( OPT(g+v) ){
		ewritesl("Opt g|v");
		printsl(GET(val2));
	}

	//printf("opt g, num: %ld\n",GET(g)*2 );
	//printf("opt g, num: %ld\n",GET(glob)*2 );

	//int i = GET(g,int);


	FOREACH_ARGV
		printsl("=> .. ", arg );

#define ITERATE(_var) for (;_var;_var++)
	ITERATE(*argv) {
		printsl("arg: ", *argv);
	}

//	char c = -3;
//	ITERATE(c){
//		printf("c: %d\n",c);
//	}

	exit(0);
}

#endif

