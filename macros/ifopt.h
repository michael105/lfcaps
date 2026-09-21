// macros (in development, experimental state..)
// license: cc-by-sa 4.0
// author: misc147 www.github.com/michael105
//
// I kindly ask to name me and the given internet address
// if you're using this code, also in parts or ideas.
// In my understanding, the cc-by-sa 4.0 license allows using this code
// even in commercial projects, as long as you
// give attribution and republish the copied code parts or files.
//





// expands to it's variable arguments, if _opt is not empty
#define IFOPT( _opt, ... ) __IF_OPT( _CAT(__VA_ARGS__), _opt )
#define _IFOPT( _opt, ... ) __IF_OPT( _CAT(__VA_ARGS__), _opt )
#define IF_OPT( _opt, ... ) __IF_OPT( _CAT(__VA_ARGS__), _opt )
#define IF_OPT_THEN( _opt, ... ) __IF_OPT( _CAT(__VA_ARGS__), _opt )

#define INSERT_OPT_OR( _opt, ... ) _opt _IFNOT_OPT( _opt, _CAT(__VA_ARGS__) )
#define _INSERT_OPT_OR( _opt, ... ) _opt _IFNOT_OPT( _opt, _CAT(__VA_ARGS__) )


#define _IF_OPT_THEN( _opt, ... ) __IF_OPT( _CAT(__VA_ARGS__), _opt )

#define OPT_TEST(_opt) _IF_OPT_THEN(_opt,__opt_exists)

#define IFNOT_OPT(_opt,...) __IFNOT_OPT(OPT_TEST(_opt),__VA_ARGS__)
#define IFNOTOPT(_opt,...) __IFNOT_OPT(OPT_TEST(_opt),__VA_ARGS__)


#define IF_OPT_THEN_ELSE(_opt,_then,...) _IF_OPT(_opt,_CAT(_then)) _IFNOT_OPT(_opt,_CAT(__VA_ARGS__))
#define IFOPT_THEN_ELSE(_opt,_then,...) _IF_OPT(_opt,_CAT(_then)) _IFNOT_OPT(_opt,_CAT(__VA_ARGS__))
#define IFOPT_ELSE(_opt,_then,...) _IF_OPT(_opt,_CAT(_then)) _IFNOT_OPT(_opt,_CAT(__VA_ARGS__))
#define _IFOPT_ELSE(_opt,_then,...) _IF_OPT(_opt,_CAT(_then)) _IFNOT_OPT(_opt,_CAT(__VA_ARGS__))

#define IFNOT_OPT_THEN_ELSE(_opt,_then,...) __IFNOT_OPT(_opt,_CAT(_then)) __IF_OPT( _opt, _CAT(__VA_ARGS__))


// -----------

#define _IF_OPT( _opt, ... ) __IF_OPT( _CAT(__VA_ARGS__), _opt )
#define __IF_OPT( _then, ... ) __VA_OPT__( _then )

#define IF_OPT_INSERT( _opt ) __IF_OPT_INSERT( _opt, _opt )
#define _IF_OPT_INSERT( _opt ) __IF_OPT_INSERT( _opt, _opt )
#define __IF_OPT_INSERT( _then, ... ) __VA_OPT__( _then )


#define __vacant_ __vacant_1,__vacant_2

#define _IFNOT_OPT(_opt,...) __IFNOT_OPT(_opt,__VA_ARGS__)


#define ______IFNOT_OPT(a,x,...) __VA_OPT__( a )
#define _____IFNOT_OPT(a,x) ______IFNOT_OPT(_CAT(a),x)
#define ____IFNOT_OPT(_if_not,_opt) _____IFNOT_OPT(_CAT(_if_not),__vacant_##_opt)
#define ___IFNOT_OPT(_opt,...) ____IFNOT_OPT(_CAT(__VA_ARGS__),_opt)
#define __IFNOT_OPT(_opt,...) ___IFNOT_OPT(_IF_OPT(_opt,__opt_exists),_CAT(__VA_ARGS__))



#define _IFELSE_OPT(_opt,_if,_else) _IF_OPT(_opt,_CAT(_if)) _IFNOT_OPT(_opt,_CAT(_else))



