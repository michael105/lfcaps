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




//+doc IFDEF(var,_then) expands to _then
// if var is defined (can also be empty)
// usage:
// #define _ifdef_VAR // needed
//
// IFDEF( VAR, code to insert )
//
// #define VARSTRING IFDEF(VAR)
// VARSTRING
//
// for macros with arguments:
// #define _ifdef_M(...)
// #IFDEF( M(a), this is not expanded )
// #define M(a) a*a
// #IFDEF( M(a), this is expanded )
//
// to use brackets, the brackets have to be put into another macro, 
// #define M(a) B((a+a)*2)
// what expands to 1*((a+a)*2)
// First char of the expansion has to be a letter or digit, which concatenates with _ifdef_ ..
// #define M(a) 0+((a+a)*2) whould work as well, but could 
// end in :  3*0+((a+a)*2) .... with a slightly different meaning
// also possible: #define M(a) 0;((a+a)*2)
// and:
// inline char* S(char* s){return(s);}
// #define M(a) S("a string")
// --- === might be the best solution, also for numbers.
//
//
//+def
#define IF_DEF(_def,...) __IF_DEF( _CAT(__VA_ARGS__),_def )

//+def
#define IFDEF(_def,...) __IF_DEF( _CAT(__VA_ARGS__), _def )
#define _IFDEF(_def,...) __IF_DEF( _CAT(__VA_ARGS__), _def )
#define _IF_DEF(_def,...) __IF_DEF( _CAT(__VA_ARGS__),_def )
#define __IF_DEF( _then, _def,... ) _IF_OPT(_ifdef_##_def,CAT(_then))

#define _ifdef_mini_errno
#define IFDEF_errno(...) _IFDEF( mini_errno, __VA_ARGS__ )
#define _IFDEF_errno(...) _IFDEF( mini_errno, __VA_ARGS__ )
#define _IFNOT_DEF_errno(...) _IFNOT_DEF( mini_errno, __VA_ARGS__ )
#define _IFDEF_ELSE_errno( _if, ... ) _IF_DEF(mini_errno,_if) _IFNOT_DEF( mini_errno, _CAT(__VA_ARGS__) )

#define __K ,
#define IFDEF_K(_def,...) __IF_DEF( __K _CAT(__VA_ARGS__), _def )

/* semikolons.
PK - PREPEND a semikolon,
AK - Append
OPK - optional, IFOPT
OAK - optional, IFOPT
*/
#define PK(...) , __VA_ARGS__
#define AK(...) __VA_ARGS__ ,
#define OAK(...) __VA_OPT__( __VA_ARGS__ , )
#define OPK(...) __VA_OPT__( , __VA_ARGS__ )



////+doc IFDEF(def) expands to the macro IFDEF_def
//// if def is defined (can also be empty)
//// usage:
//// #define _ifdef_VAR // needed
//// // what to insert, if VAR is defined
//// #define IFDEF_VAR ----- expand to this,  VAR is defined -----
//// #define VAR a,b,c  // option. 
////
//// IFDEF( VAR )
////
//// #define VARSTRING IFDEF(VAR)
////
//// VARSTRING
#define _MLI___IFDEF_LIST(_n,...) __VA_OPT__( IFDEF##_n )
#define _MLI__IFDEF_LIST(_n,...) __VA_OPT__(_MLI___IFDEF_LIST(_n,__VA_ARGS__))
#define _MLI_IFDEF_LIST(_n,_i,...) _MLI__IFDEF_LIST( _n, _ifdef_##_i )
#define IFDEF_LIST(_def) _MLI_IFDEF_LIST( _##_def, _def )



//+doc if _def is not defined, replace with IFNOTDEF_def
// _def may not have the content of its own name at the beginning,
// or it is recognized as undefined.
//
// example
// #define _VAR2_undef MARK_UNDEFINED  (needed line)
// #define IFNOTDEF_VAR2 -- content to fill in. (VAR2) --
// #define VAR2 1,2,3
// IFNOTDEF(VAR2)
//
//+def
#define IFNOTDEF_REPLACE(_def) _IFNOTDEF( _##_def, _def )
#define ___IFNOTDEF(_n,_i1,...) __VA_OPT__( IFNOTDEF##_n )
#define __IFNOTDEF(_n,_i,...) ___IFNOTDEF(_n,_i)
#define _IFNOTDEF(_n,_i,...) __IFNOTDEF(_n, _##_i##_undef)
#define MARK_UNDEFINED __marked__,__as_undefined__



//+def
#define IFNOT_DEF( _def, ... ) __IFNOT_DEF( _def, _CAT(__VA_ARGS__) )
#define _IFNOT_DEF( _def, ... ) __IFNOT_DEF( _def, _CAT(__VA_ARGS__) )
#define IFNDEF( _def, ... ) __IFNOT_DEF(_def, _CAT(__VA_ARGS__) )
#define __IFNOT_DEF(_def,...) _IFNOT_OPT( _ifdef_##_def,_CAT(__VA_ARGS__))


#define OPT_DEF(_def) __IF_DEF(_def, _def)
#define IFDEF_INS(_def) __IF_DEF(_def, _def)




#define IF_DEF_THEN_ELSE( _def, _then, _else ) _IF_DEF(_CAT(_def),_then) _IFNOT_DEF( _CAT(_def), _else )

// expand to _def, if defined, else to its variable arguments
#define INSERT_DEF_OR( _def, ...  ) _IF_DEF(_CAT(_def),_def) _IFNOT_DEF( _CAT(_def),__VA_ARGS__)
#define INSDEF_OR( _def, ...  ) _IF_DEF(_CAT(_def),_def) _IFNOT_DEF( _CAT(_def),__VA_ARGS__)


//#define _IF_DEF(_def,...) __IF_DEF( _CAT(__VA_ARGS__),_def )
//#define __IF_DEF( _then, _def,... ) _IF_OPT(_ifdef_##_def,_CAT(_then))


//+def
#define IFNDEF( _def, ... ) __IFNOT_DEF(_def, _CAT(__VA_ARGS__) )
#define __IFNOT_DEF(_def,...) _IFNOT_OPT( _ifdef_##_def,_CAT(__VA_ARGS__))

//+def

#define DEF_OR( _def, _or ) _IF_DEF_INSERT(_CAT(_def)) _IFNOT_DEF( _CAT(_def), _or )



//+def
#define IFELSE_DEF( _def, _then, _else ) _IF_DEF(_CAT(_def),_then) _IFNOT_DEF( _CAT(_def), _else )
#define IFDEF_ELSE( _def, _then, _else ) _IF_DEF(_CAT(_def),_then) _IFNOT_DEF( _CAT(_def), _else )


#define IF_DEF_INSERT(_def) __IF_DEF_INSERT( _def )
#define _IF_DEF_INSERT(_def) __IF_DEF_INSERT( _def )
#define __IF_DEF_INSERT( _def ) _IF_OPT(_ifdef_##_def,_def)


//#define IF_DEF_ELSE( _def, _else ) IF_DEF_INSERT(CAT(_def)) IFNOT_DEF( CAT(_def), _else )


