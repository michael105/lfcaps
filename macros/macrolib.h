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


#ifndef MACROLIB
#define MACROLIB


// for more than 32 repetitions
//#define MLIB_MACROLOOPS_64
// more than 16 repetitions
#define MLIB_MACROLOOPS_32


#define VA_OPT_ARG(_n,...) _FETCH(_n,__VA_ARGS__)
#define _VA_OPT_ARG(_n,...) _FETCH(_n,__VA_ARGS__)

#define VA_DEFAULT(_n,_default,...) _INSERT_OPT_OR(_VA_OPT_ARG(_n,__VA_ARGS__),_default)
#define _VA_DEFAULT(_n,_default,...) _INSERT_OPT_OR(_VA_OPT_ARG(_n,__VA_ARGS__),_default)

//#include "vararg.h"
//#include "sequences.h"
#include "ifdef.h"
#include "ifopt.h"

/*  
script in /scripts; replacing @1 and @2
-> expand on the fly, set max count as option

all macros do have a _MACRO equivalent,
to be able to filter repetitios errormessages
at different levels
"internal" loop macros are prefixed with __
-> evtl prefix internal with ___  

FUNCTIONS in capitals, variables smalls

TODO: several loops can be replaced with FOREACH / FOREACH_ARG
*/
//+inline

#define MIN(a,b) (a<b?a:b)

//+doc quote a string or macro
//+def
#define QUOTE(x) __QUOTE(x)
#define Q(x) __QUOTE(x)
#define _Q(x) __QUOTE(x)
#define _QUOTE(x) __QUOTE(x)
#define __QUOTE(x) #x


//+doc replaced with the current line number, stringified
//+def
#define __STRLINE__ _QUOTE(__LINE__)

//+doc call _F with supplied elements, if more than 1, ..
//+def
#define MORETHAN1(_F,_e1,...) __VA_OPT__(_F(_e1,__VA_ARGS__))
#define MORETHAN2(_F,_e1,_e2,...) __VA_OPT__(_F(_e1,_e2,__VA_ARGS__))

//+doc expands the argument list _args, and calls _macro with the list (or function)
//+def
#define EXPAND(_macro,_args) _macro(_args)
#define _EXPAND(_macro,_args) _macro(_args)


//+doc get the size in bytes of all variables supplied
//+def
#define BYTES_OF(...) (_FOREACH(__BYTES,+,__VA_ARGS__))
#define _BYTES_OF(...) (_FOREACH(__BYTES,+,__VA_ARGS__))
#define __BYTES(_var) sizeof(_var)

//+doc store a list of variables, restore them later with restore
// this copies the full contents of char* (deep copy), not the pointer
//+def
#define STORE(...) STORE_N(__store,__VA_ARGS__)

//+doc
//+def
#define RESTORE(...) RESTORE_FROM(__store,__VA_ARGS__)

//+doc store within _mem. (needs to be large enough.)
//+def
#define STORE_AT(_mem,...)  { char *_p_stack = _mem; _FOREACH(__STORE,;,__VA_ARGS__); }
#define _STORE_AT(_mem,...)  { char *_p_stack = _mem; _FOREACH(__STORE,;,__VA_ARGS__); }

#define __STORE(_var) _p_stack = pmemcpy( _p_stack, &_var, sizeof(_var) )

//+doc use the named _stackname for storage, creates _stackname at the stack
//+def
#define STORE_N(_stackname,...) char _stackname[ _BYTES_OF(__VA_ARGS__) ]; _STORE_AT(_stackname,__VA_ARGS__)
#define _STORE_N(_stackname,...) char _stackname[ _BYTES_OF(__VA_ARGS__) ]; _STORE_AT(_stackname,__VA_ARGS__)


//+doc restore from _stackname ( defined by STORE_N, or any variable )
//+def
#define RESTORE_FROM(_stackname,...) { char *_p_stack = _stackname; _FOREACH(_RESTORE,;,__VA_ARGS__); }
#define _RESTORE_FROM(_stackname,...) { char *_p_stack = _stackname; _FOREACH(_RESTORE,;,__VA_ARGS__); }
#define _RESTORE(_var) pmemcpy(&_var,_p_stack, sizeof(_var)); _p_stack += sizeof(_var)

//+doc restore from _stackname ( defined by STORE_N, or any variable )
//+def
#define RESTORE_N(_stackname,...) _RESTORE_FROM(_stackname,__VA_ARGS__)

// STORE_MALLOC: malloc, store, return pointer
// STORE_AT: store at dest






//+doc Append a comma to every element, also the last
#define APPEND_K(...) _FOREACH(__APPEND_K,,__VA_ARGS__)
#define __APPEND_K(...) __VA_OPT__( __VA_ARGS__ , )


//+doc Append _append to every element, also the last
#define APPEND(_append,...) JOIN(_append,__VA_ARGS__)_append
#define __APPEND(...) __VA_OPT__( __VA_ARGS__ , )




//+doc Prepend a comma to every element, also the first
#define PREPEND_K(...) _FOREACH(__PREPEND_K,,__VA_ARGS__)
#define __PREPEND_K(...) __VA_OPT__( , __VA_ARGS__ )




// (intern) Error, too many elements
#define __LOOP_ERROR ERROR: too many elements __FILE__, line __LINE__


#define FOREACH_TUPLEPAIR(_m,...) __FOREACH_TUPLEPAIR_1(_m,__VA_ARGS__)
//#define __FOREACH_TUPLEPAIR_@1(_a,_b,...) _a __VA_OPT__(,__FOREACH_TUPLEPAIR_@2(__VA_ARGS__))
#define __FOREACH_TUPLEPAIR_1(_m,_a,_b,...) _m(_a,_b) __VA_OPT__(,__FOREACH_TUPLEPAIR_2(_m,_b,__VA_ARGS__))
#define __FOREACH_TUPLEPAIR_2(_m,_a,_b,...) _m(_a,_b) __VA_OPT__(,__FOREACH_TUPLEPAIR_3(_m,_b,__VA_ARGS__))
#define __FOREACH_TUPLEPAIR_3(_m,_a,_b,...) _m(_a,_b) __VA_OPT__(,__FOREACH_TUPLEPAIR_4(_m,_b,__VA_ARGS__))
#define __FOREACH_TUPLEPAIR_4(_m,_a,_b,...) _m(_a,_b) __VA_OPT__(,__FOREACH_TUPLEPAIR_5(_m,_b,__VA_ARGS__))

//#define M(a,b) a,b =
///FOREACH_TUPLEPAIR(M,x,y,z,v,L);
//x,y = ,y,z = ,z,v = ,v,L = ;



//+doc expands to the first element of 2 in a (even) list 
//+def
#define EACH_FIRST_OF2(...) __EACH_FIRST_OF2_1(__VA_ARGS__)
//#define __EACH_FIRST_OF2_@1(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_@2(__VA_ARGS__))
#define __EACH_FIRST_OF2_1(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_2(__VA_ARGS__))
#define __EACH_FIRST_OF2_2(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_3(__VA_ARGS__))
#define __EACH_FIRST_OF2_3(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_4(__VA_ARGS__))
#define __EACH_FIRST_OF2_4(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_5(__VA_ARGS__))
#define __EACH_FIRST_OF2_5(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_6(__VA_ARGS__))
#define __EACH_FIRST_OF2_6(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_7(__VA_ARGS__))
#define __EACH_FIRST_OF2_7(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_8(__VA_ARGS__))
#define __EACH_FIRST_OF2_8(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_9(__VA_ARGS__))
#define __EACH_FIRST_OF2_9(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_10(__VA_ARGS__))
#define __EACH_FIRST_OF2_10(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_11(__VA_ARGS__))
#define __EACH_FIRST_OF2_11(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_12(__VA_ARGS__))
#define __EACH_FIRST_OF2_12(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_13(__VA_ARGS__))
#define __EACH_FIRST_OF2_13(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_14(__VA_ARGS__))
#define __EACH_FIRST_OF2_14(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_15(__VA_ARGS__))
#define __EACH_FIRST_OF2_15(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_16(__VA_ARGS__))
#define __EACH_FIRST_OF2_16(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_17(__VA_ARGS__))
#define __EACH_FIRST_OF2_17(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_18(__VA_ARGS__))
#define __EACH_FIRST_OF2_18(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_19(__VA_ARGS__))
#define __EACH_FIRST_OF2_19(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_20(__VA_ARGS__))
#define __EACH_FIRST_OF2_20(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_21(__VA_ARGS__))
#define __EACH_FIRST_OF2_21(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_22(__VA_ARGS__))
#define __EACH_FIRST_OF2_22(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_23(__VA_ARGS__))
#define __EACH_FIRST_OF2_23(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_24(__VA_ARGS__))
#define __EACH_FIRST_OF2_24(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_25(__VA_ARGS__))
#define __EACH_FIRST_OF2_25(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_26(__VA_ARGS__))
#define __EACH_FIRST_OF2_26(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_27(__VA_ARGS__))
#define __EACH_FIRST_OF2_27(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_28(__VA_ARGS__))
#define __EACH_FIRST_OF2_28(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_29(__VA_ARGS__))
#define __EACH_FIRST_OF2_29(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_30(__VA_ARGS__))
#define __EACH_FIRST_OF2_30(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_31(__VA_ARGS__))
#define __EACH_FIRST_OF2_31(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_32(__VA_ARGS__))
#define __EACH_FIRST_OF2_32(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_33(__VA_ARGS__))
#define __EACH_FIRST_OF2_33(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_34(__VA_ARGS__))
#define __EACH_FIRST_OF2_34(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_35(__VA_ARGS__))
#define __EACH_FIRST_OF2_35(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_36(__VA_ARGS__))
#define __EACH_FIRST_OF2_36(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_37(__VA_ARGS__))
#define __EACH_FIRST_OF2_37(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_38(__VA_ARGS__))
#define __EACH_FIRST_OF2_38(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_39(__VA_ARGS__))
#define __EACH_FIRST_OF2_39(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_40(__VA_ARGS__))
#define __EACH_FIRST_OF2_40(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_41(__VA_ARGS__))
#define __EACH_FIRST_OF2_41(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_42(__VA_ARGS__))
#define __EACH_FIRST_OF2_42(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_43(__VA_ARGS__))
#define __EACH_FIRST_OF2_43(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_44(__VA_ARGS__))
#define __EACH_FIRST_OF2_44(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_45(__VA_ARGS__))
#define __EACH_FIRST_OF2_45(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_46(__VA_ARGS__))
#define __EACH_FIRST_OF2_46(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_47(__VA_ARGS__))
#define __EACH_FIRST_OF2_47(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_48(__VA_ARGS__))
#define __EACH_FIRST_OF2_48(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_49(__VA_ARGS__))
#define __EACH_FIRST_OF2_49(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_50(__VA_ARGS__))
#define __EACH_FIRST_OF2_50(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_51(__VA_ARGS__))
#define __EACH_FIRST_OF2_51(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_52(__VA_ARGS__))
#define __EACH_FIRST_OF2_52(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_53(__VA_ARGS__))
#define __EACH_FIRST_OF2_53(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_54(__VA_ARGS__))
#define __EACH_FIRST_OF2_54(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_55(__VA_ARGS__))
#define __EACH_FIRST_OF2_55(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_56(__VA_ARGS__))
#define __EACH_FIRST_OF2_56(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_57(__VA_ARGS__))
#define __EACH_FIRST_OF2_57(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_58(__VA_ARGS__))
#define __EACH_FIRST_OF2_58(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_59(__VA_ARGS__))
#define __EACH_FIRST_OF2_59(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_60(__VA_ARGS__))
#define __EACH_FIRST_OF2_60(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_61(__VA_ARGS__))
#define __EACH_FIRST_OF2_61(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_62(__VA_ARGS__))
#define __EACH_FIRST_OF2_62(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_63(__VA_ARGS__))
#define __EACH_FIRST_OF2_63(_a,_b,...) _a __VA_OPT__(,__EACH_FIRST_OF2_64(__VA_ARGS__))
#define __EACH_FIRST_OF2_64(_a,_b,...) _a __VA_OPT__(,__LOOP_ERROR)

//+doc expands to the second element of 2 in a (even) list 
//+def
#define EACH_SECOND_OF2(...) __EACH_SECOND_OF2_1(__VA_ARGS__)
//#define __EACH_SECOND_OF2_@1(_a,_b,...) _a __VA_OPT__(,__EACH_SECOND_OF2_@2(__VA_ARGS__))
#define __EACH_SECOND_OF2_1(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_2(__VA_ARGS__))
#define __EACH_SECOND_OF2_2(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_3(__VA_ARGS__))
#define __EACH_SECOND_OF2_3(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_4(__VA_ARGS__))
#define __EACH_SECOND_OF2_4(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_5(__VA_ARGS__))
#define __EACH_SECOND_OF2_5(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_6(__VA_ARGS__))
#define __EACH_SECOND_OF2_6(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_7(__VA_ARGS__))
#define __EACH_SECOND_OF2_7(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_8(__VA_ARGS__))
#define __EACH_SECOND_OF2_8(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_9(__VA_ARGS__))
#define __EACH_SECOND_OF2_9(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_10(__VA_ARGS__))
#define __EACH_SECOND_OF2_10(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_11(__VA_ARGS__))
#define __EACH_SECOND_OF2_11(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_12(__VA_ARGS__))
#define __EACH_SECOND_OF2_12(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_13(__VA_ARGS__))
#define __EACH_SECOND_OF2_13(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_14(__VA_ARGS__))
#define __EACH_SECOND_OF2_14(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_15(__VA_ARGS__))
#define __EACH_SECOND_OF2_15(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_16(__VA_ARGS__))
#define __EACH_SECOND_OF2_16(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_17(__VA_ARGS__))
#define __EACH_SECOND_OF2_17(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_18(__VA_ARGS__))
#define __EACH_SECOND_OF2_18(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_19(__VA_ARGS__))
#define __EACH_SECOND_OF2_19(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_20(__VA_ARGS__))
#define __EACH_SECOND_OF2_20(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_21(__VA_ARGS__))
#define __EACH_SECOND_OF2_21(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_22(__VA_ARGS__))
#define __EACH_SECOND_OF2_22(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_23(__VA_ARGS__))
#define __EACH_SECOND_OF2_23(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_24(__VA_ARGS__))
#define __EACH_SECOND_OF2_24(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_25(__VA_ARGS__))
#define __EACH_SECOND_OF2_25(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_26(__VA_ARGS__))
#define __EACH_SECOND_OF2_26(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_27(__VA_ARGS__))
#define __EACH_SECOND_OF2_27(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_28(__VA_ARGS__))
#define __EACH_SECOND_OF2_28(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_29(__VA_ARGS__))
#define __EACH_SECOND_OF2_29(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_30(__VA_ARGS__))
#define __EACH_SECOND_OF2_30(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_31(__VA_ARGS__))
#define __EACH_SECOND_OF2_31(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_32(__VA_ARGS__))
#define __EACH_SECOND_OF2_32(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_33(__VA_ARGS__))
#define __EACH_SECOND_OF2_33(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_34(__VA_ARGS__))
#define __EACH_SECOND_OF2_34(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_35(__VA_ARGS__))
#define __EACH_SECOND_OF2_35(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_36(__VA_ARGS__))
#define __EACH_SECOND_OF2_36(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_37(__VA_ARGS__))
#define __EACH_SECOND_OF2_37(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_38(__VA_ARGS__))
#define __EACH_SECOND_OF2_38(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_39(__VA_ARGS__))
#define __EACH_SECOND_OF2_39(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_40(__VA_ARGS__))
#define __EACH_SECOND_OF2_40(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_41(__VA_ARGS__))
#define __EACH_SECOND_OF2_41(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_42(__VA_ARGS__))
#define __EACH_SECOND_OF2_42(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_43(__VA_ARGS__))
#define __EACH_SECOND_OF2_43(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_44(__VA_ARGS__))
#define __EACH_SECOND_OF2_44(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_45(__VA_ARGS__))
#define __EACH_SECOND_OF2_45(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_46(__VA_ARGS__))
#define __EACH_SECOND_OF2_46(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_47(__VA_ARGS__))
#define __EACH_SECOND_OF2_47(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_48(__VA_ARGS__))
#define __EACH_SECOND_OF2_48(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_49(__VA_ARGS__))
#define __EACH_SECOND_OF2_49(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_50(__VA_ARGS__))
#define __EACH_SECOND_OF2_50(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_51(__VA_ARGS__))
#define __EACH_SECOND_OF2_51(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_52(__VA_ARGS__))
#define __EACH_SECOND_OF2_52(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_53(__VA_ARGS__))
#define __EACH_SECOND_OF2_53(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_54(__VA_ARGS__))
#define __EACH_SECOND_OF2_54(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_55(__VA_ARGS__))
#define __EACH_SECOND_OF2_55(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_56(__VA_ARGS__))
#define __EACH_SECOND_OF2_56(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_57(__VA_ARGS__))
#define __EACH_SECOND_OF2_57(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_58(__VA_ARGS__))
#define __EACH_SECOND_OF2_58(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_59(__VA_ARGS__))
#define __EACH_SECOND_OF2_59(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_60(__VA_ARGS__))
#define __EACH_SECOND_OF2_60(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_61(__VA_ARGS__))
#define __EACH_SECOND_OF2_61(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_62(__VA_ARGS__))
#define __EACH_SECOND_OF2_62(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_63(__VA_ARGS__))
#define __EACH_SECOND_OF2_63(_a,_b,...) _b __VA_OPT__(,__EACH_SECOND_OF2_64(__VA_ARGS__))
#define __EACH_SECOND_OF2_64(_a,_b,...) _b __VA_OPT__(,__LOOP_ERROR)


//join elements with commata (if more than 1 elements are supplied)
#define JOIN_K(...) __VA_OPT__( __JOIN_K1(__VA_ARGS__) )

#define __JOIN_K1(_a,...) _a __VA_OPT__(,__JOIN_K2(__VA_ARGS__) )
#define __JOIN_K2(_a,...) _a __VA_OPT__(,__JOIN_K3(__VA_ARGS__) )
#define __JOIN_K3(_a,...) _a __VA_OPT__(,__JOIN_K4(__VA_ARGS__) )
#define __JOIN_K4(_a,...) _a __VA_OPT__(,__JOIN_K5(__VA_ARGS__) )
#define __JOIN_K5(_a,...) _a __VA_OPT__(,__JOIN_K6(__VA_ARGS__) )
#define __JOIN_K6(_a,...) _a __VA_OPT__(,__JOIN_K7(__VA_ARGS__) )
#define __JOIN_K7(_a,...) _a __VA_OPT__(,__JOIN_K8(__VA_ARGS__) )
#define __JOIN_K8(_a,...) _a __VA_OPT__(,__JOIN_K9(__VA_ARGS__) )
#define __JOIN_K9(_a,...) _a __VA_OPT__(,__JOIN_K10(__VA_ARGS__) )
#define __JOIN_K10(_a,...) _a __VA_OPT__(,__JOIN_K11(__VA_ARGS__) )
#define __JOIN_K11(_a,...) _a __VA_OPT__(,__JOIN_K12(__VA_ARGS__) )
#define __JOIN_K12(_a,...) _a __VA_OPT__(,__JOIN_K13(__VA_ARGS__) )
#define __JOIN_K13(_a,...) _a __VA_OPT__(,__JOIN_K14(__VA_ARGS__) )
#define __JOIN_K14(_a,...) _a __VA_OPT__(,__JOIN_K15(__VA_ARGS__) )
#define __JOIN_K15(_a,...) _a __VA_OPT__(,__JOIN_K16(__VA_ARGS__) )
#define __JOIN_K16(_a,...) _a __VA_OPT__(,__JOIN_K17(__VA_ARGS__) )
#define __JOIN_K17(_a,...) _a __VA_OPT__(,__JOIN_K18(__VA_ARGS__) )
#define __JOIN_K18(_a,...) _a __VA_OPT__(,__JOIN_K19(__VA_ARGS__) )
#define __JOIN_K19(_a,...) _a __VA_OPT__(,__JOIN_K20(__VA_ARGS__) )
#define __JOIN_K20(_a,...) _a __VA_OPT__(,__JOIN_K21(__VA_ARGS__) )
#define __JOIN_K21(_a,...) _a __VA_OPT__(,__JOIN_K22(__VA_ARGS__) )
#define __JOIN_K22(_a,...) _a __VA_OPT__(,__JOIN_K23(__VA_ARGS__) )
#define __JOIN_K23(_a,...) _a __VA_OPT__(,__JOIN_K24(__VA_ARGS__) )
#define __JOIN_K24(_a,...) _a __VA_OPT__(,__JOIN_K25(__VA_ARGS__) )
#define __JOIN_K25(_a,...) _a __VA_OPT__(,__JOIN_K26(__VA_ARGS__) )
#define __JOIN_K26(_a,...) _a __VA_OPT__(,__JOIN_K27(__VA_ARGS__) )
#define __JOIN_K27(_a,...) _a __VA_OPT__(,__JOIN_K28(__VA_ARGS__) )
#define __JOIN_K28(_a,...) _a __VA_OPT__(,__JOIN_K29(__VA_ARGS__) )
#define __JOIN_K29(_a,...) _a __VA_OPT__(,__JOIN_K30(__VA_ARGS__) )
#define __JOIN_K30(_a,...) _a __VA_OPT__(,__JOIN_K31(__VA_ARGS__) )
#define __JOIN_K31(_a,...) _a __VA_OPT__(,__JOIN_K32(__VA_ARGS__) )
#define __JOIN_K32(_a,...) _a __VA_OPT__(,__JOIN_K33(__VA_ARGS__) )
#define __JOIN_K33(_a,...) _a __VA_OPT__(,__JOIN_K34(__VA_ARGS__) )
#define __JOIN_K34(_a,...) _a __VA_OPT__(,__JOIN_K35(__VA_ARGS__) )
#define __JOIN_K35(_a,...) _a __VA_OPT__(,__JOIN_K36(__VA_ARGS__) )
#define __JOIN_K36(_a,...) _a __VA_OPT__(,__JOIN_K37(__VA_ARGS__) )
#define __JOIN_K37(_a,...) _a __VA_OPT__(,__JOIN_K38(__VA_ARGS__) )
#define __JOIN_K38(_a,...) _a __VA_OPT__(,__JOIN_K39(__VA_ARGS__) )
#define __JOIN_K39(_a,...) _a __VA_OPT__(,__JOIN_K40(__VA_ARGS__) )
#define __JOIN_K40(_a,...) _a __VA_OPT__(,__JOIN_K41(__VA_ARGS__) )
#define __JOIN_K41(_a,...) _a __VA_OPT__(,__JOIN_K42(__VA_ARGS__) )
#define __JOIN_K42(_a,...) _a __VA_OPT__(,__JOIN_K43(__VA_ARGS__) )
#define __JOIN_K43(_a,...) _a __VA_OPT__(,__JOIN_K44(__VA_ARGS__) )
#define __JOIN_K44(_a,...) _a __VA_OPT__(,__JOIN_K45(__VA_ARGS__) )
#define __JOIN_K45(_a,...) _a __VA_OPT__(,__JOIN_K46(__VA_ARGS__) )
#define __JOIN_K46(_a,...) _a __VA_OPT__(,__JOIN_K47(__VA_ARGS__) )
#define __JOIN_K47(_a,...) _a __VA_OPT__(,__JOIN_K48(__VA_ARGS__) )
#define __JOIN_K48(_a,...) _a __VA_OPT__(,__JOIN_K49(__VA_ARGS__) )
#define __JOIN_K49(_a,...) _a __VA_OPT__(,__JOIN_K50(__VA_ARGS__) )
#define __JOIN_K50(_a,...) _a __VA_OPT__(,__JOIN_K51(__VA_ARGS__) )
#define __JOIN_K51(_a,...) _a __VA_OPT__(,__JOIN_K52(__VA_ARGS__) )
#define __JOIN_K52(_a,...) _a __VA_OPT__(,__JOIN_K53(__VA_ARGS__) )
#define __JOIN_K53(_a,...) _a __VA_OPT__(,__JOIN_K54(__VA_ARGS__) )
#define __JOIN_K54(_a,...) _a __VA_OPT__(,__JOIN_K55(__VA_ARGS__) )
#define __JOIN_K55(_a,...) _a __VA_OPT__(,__JOIN_K56(__VA_ARGS__) )
#define __JOIN_K56(_a,...) _a __VA_OPT__(,__JOIN_K57(__VA_ARGS__) )
#define __JOIN_K57(_a,...) _a __VA_OPT__(,__JOIN_K58(__VA_ARGS__) )
#define __JOIN_K58(_a,...) _a __VA_OPT__(,__JOIN_K59(__VA_ARGS__) )
#define __JOIN_K59(_a,...) _a __VA_OPT__(,__JOIN_K60(__VA_ARGS__) )
#define __JOIN_K60(_a,...) _a __VA_OPT__(,__JOIN_K61(__VA_ARGS__) )
#define __JOIN_K61(_a,...) _a __VA_OPT__(,__JOIN_K62(__VA_ARGS__) )
#define __JOIN_K62(_a,...) _a __VA_OPT__(,__JOIN_K63(__VA_ARGS__) )
#define __JOIN_K63(_a,...) _a __VA_OPT__(,__JOIN_K64(__VA_ARGS__) )
#define __JOIN_K64(_a,...) _a __VA_OPT__(,__JOIN_K65(__VA_ARGS__) )
#define __JOIN_K65(...) __LOOP_ERROR (JOIN_K)


// Join strings: JOINS( sep, a1,a2,a3,..) 
// gets to "a1sepa2sepa3" ...
// sep can be empty
// CONCAT( a1, a2, a3, .. )
// concatenate to "a1a2a3"


//+doc JOINS
// Macro JOINS( separator, ... )
// stringify ("") and join all arguments with sep (not stringified)
// The first argument sep isn't stringified, the others are.
// example
// JOINS( "_" , k, l, r,t )
// "k" "_" "l" "_" "r" "_" "t"
//+def JOINS
#define JOINS( sep, ... ) __VA_OPT__( __JOINS0( sep,__VA_ARGS__ ) )
#define __JOINS0(J,a,...)  #a __VA_OPT__( __JOINS1( J,__VA_ARGS__) )

//DEF __JOINS@1(J,a,...)  J #a __VA_OPT__( __JOINS@2( J,__VA_ARGS__) )
#define __JOINS1(J,a,...)  J #a __VA_OPT__( __JOINS2( J,__VA_ARGS__) )
#define __JOINS2(J,a,...)  J #a __VA_OPT__( __JOINS3( J,__VA_ARGS__) )
#define __JOINS3(J,a,...)  J #a __VA_OPT__( __JOINS4( J,__VA_ARGS__) )
#define __JOINS4(J,a,...)  J #a __VA_OPT__( __JOINS5( J,__VA_ARGS__) )
#define __JOINS5(J,a,...)  J #a __VA_OPT__( __JOINS6( J,__VA_ARGS__) )
#define __JOINS6(J,a,...)  J #a __VA_OPT__( __JOINS7( J,__VA_ARGS__) )
#define __JOINS7(J,a,...)  J #a __VA_OPT__( __JOINS8( J,__VA_ARGS__) )
#define __JOINS8(J,a,...)  J #a __VA_OPT__( __JOINS9( J,__VA_ARGS__) )
#define __JOINS9(J,a,...)  J #a __VA_OPT__( __JOINS10( J,__VA_ARGS__) )
#define __JOINS10(J,a,...)  J #a __VA_OPT__( __JOINS11( J,__VA_ARGS__) )
#define __JOINS11(J,a,...)  J #a __VA_OPT__( __JOINS12( J,__VA_ARGS__) )
#define __JOINS12(J,a,...)  J #a __VA_OPT__( __JOINS13( J,__VA_ARGS__) )
#define __JOINS13(J,a,...)  J #a __VA_OPT__( __JOINS14( J,__VA_ARGS__) )
#define __JOINS14(J,a,...)  J #a __VA_OPT__( __JOINS15( J,__VA_ARGS__) )
#define __JOINS15(J,a,...)  J #a __VA_OPT__( __JOINS16( J,__VA_ARGS__) )
#define __JOINS16(J,a,...)  J #a __VA_OPT__( __JOINS17( J,__VA_ARGS__) )
#define __JOINS17(J,a,...)  J #a __VA_OPT__( __JOINS18( J,__VA_ARGS__) )
#define __JOINS18(J,a,...)  J #a __VA_OPT__( __JOINS19( J,__VA_ARGS__) )
#define __JOINS19(J,a,...)  J #a __VA_OPT__( __JOINS20( J,__VA_ARGS__) )
#define __JOINS20(J,a,...)  J #a __VA_OPT__( __JOINS21( J,__VA_ARGS__) )
#define __JOINS21(J,a,...)  J #a __VA_OPT__( __JOINS22( J,__VA_ARGS__) )
#define __JOINS22(J,a,...)  J #a __VA_OPT__( __JOINS23( J,__VA_ARGS__) )
#define __JOINS23(J,a,...)  J #a __VA_OPT__( __JOINS24( J,__VA_ARGS__) )
#define __JOINS24(J,a,...)  J #a __VA_OPT__( __JOINS25( J,__VA_ARGS__) )
#define __JOINS25(J,a,...)  J #a __VA_OPT__( __JOINS26( J,__VA_ARGS__) )
#define __JOINS26(J,a,...)  J #a __VA_OPT__( __JOINS27( J,__VA_ARGS__) )
#define __JOINS27(J,a,...)  J #a __VA_OPT__( __JOINS28( J,__VA_ARGS__) )
#define __JOINS28(J,a,...)  J #a __VA_OPT__( __JOINS29( J,__VA_ARGS__) )
#define __JOINS29(J,a,...)  J #a __VA_OPT__( __JOINS30( J,__VA_ARGS__) )
#define __JOINS30(J,a,...)  J #a __VA_OPT__( __JOINS31( J,__VA_ARGS__) )
#define __JOINS31(J,a,...)  J #a __VA_OPT__( __JOINS32( J,__VA_ARGS__) )
#define __JOINS32(J,a,...)  J #a __VA_OPT__( __JOINS33( J,__VA_ARGS__) )
#ifndef MLIB_MACROLOOPS_64
#define __JOINS33(J,a,...) __LOOP_ERROR
#else
#define __JOINS33(J,a,...)  J #a __VA_OPT__( __JOINS34( J,__VA_ARGS__) )
#define __JOINS34(J,a,...)  J #a __VA_OPT__( __JOINS35( J,__VA_ARGS__) )
#define __JOINS35(J,a,...)  J #a __VA_OPT__( __JOINS36( J,__VA_ARGS__) )
#define __JOINS36(J,a,...)  J #a __VA_OPT__( __JOINS37( J,__VA_ARGS__) )
#define __JOINS37(J,a,...)  J #a __VA_OPT__( __JOINS38( J,__VA_ARGS__) )
#define __JOINS38(J,a,...)  J #a __VA_OPT__( __JOINS39( J,__VA_ARGS__) )
#define __JOINS39(J,a,...)  J #a __VA_OPT__( __JOINS40( J,__VA_ARGS__) )
#define __JOINS40(J,a,...)  J #a __VA_OPT__( __JOINS41( J,__VA_ARGS__) )
#define __JOINS41(J,a,...)  J #a __VA_OPT__( __JOINS42( J,__VA_ARGS__) )
#define __JOINS42(J,a,...)  J #a __VA_OPT__( __JOINS43( J,__VA_ARGS__) )
#define __JOINS43(J,a,...)  J #a __VA_OPT__( __JOINS44( J,__VA_ARGS__) )
#define __JOINS44(J,a,...)  J #a __VA_OPT__( __JOINS45( J,__VA_ARGS__) )
#define __JOINS45(J,a,...)  J #a __VA_OPT__( __JOINS46( J,__VA_ARGS__) )
#define __JOINS46(J,a,...)  J #a __VA_OPT__( __JOINS47( J,__VA_ARGS__) )
#define __JOINS47(J,a,...)  J #a __VA_OPT__( __JOINS48( J,__VA_ARGS__) )
#define __JOINS48(J,a,...)  J #a __VA_OPT__( __JOINS49( J,__VA_ARGS__) )
#define __JOINS49(J,a,...)  J #a __VA_OPT__( __JOINS50( J,__VA_ARGS__) )
#define __JOINS50(J,a,...)  J #a __VA_OPT__( __JOINS51( J,__VA_ARGS__) )
#define __JOINS51(J,a,...)  J #a __VA_OPT__( __JOINS52( J,__VA_ARGS__) )
#define __JOINS52(J,a,...)  J #a __VA_OPT__( __JOINS53( J,__VA_ARGS__) )
#define __JOINS53(J,a,...)  J #a __VA_OPT__( __JOINS54( J,__VA_ARGS__) )
#define __JOINS54(J,a,...)  J #a __VA_OPT__( __JOINS55( J,__VA_ARGS__) )
#define __JOINS55(J,a,...)  J #a __VA_OPT__( __JOINS56( J,__VA_ARGS__) )
#define __JOINS56(J,a,...)  J #a __VA_OPT__( __JOINS57( J,__VA_ARGS__) )
#define __JOINS57(J,a,...)  J #a __VA_OPT__( __JOINS58( J,__VA_ARGS__) )
#define __JOINS58(J,a,...)  J #a __VA_OPT__( __JOINS59( J,__VA_ARGS__) )
#define __JOINS59(J,a,...)  J #a __VA_OPT__( __JOINS60( J,__VA_ARGS__) )
#define __JOINS60(J,a,...)  J #a __VA_OPT__( __JOINS61( J,__VA_ARGS__) )
#define __JOINS61(J,a,...)  J #a __VA_OPT__( __JOINS62( J,__VA_ARGS__) )
#define __JOINS62(J,a,...)  J #a __VA_OPT__( __JOINS63( J,__VA_ARGS__) )
#define __JOINS63(J,a,...)  J #a __VA_OPT__( __JOINS64( J,__VA_ARGS__) )
#define __JOINS64(J,a,...)  J #a __VA_OPT__( __JOINS65( J,__VA_ARGS__) )
#define __JOINS65(J,a,...) __LOOP_ERROR
#endif


//+doc PREPENDS
// Macro PREPENDS( prepend, ... )
// stringify ("") and prepend all arguments with prepend (not stringified)
//+def PREPENDS
#define PREPENDS( prepend, a, ...) prepend#a __VA_OPT__( __JOINS1( prepend,__VA_ARGS__ ) )


#define __JOINSREV0(J,a,...)  J #a __VA_OPT__( ERROR: too many agruments at __LINE__ )
#define __JOINSREV1(J,a,...)  J #a __VA_OPT__( __JOINSREV0( J,__VA_ARGS__) )
#define __JOINSREV2(J,a,...)  J #a __VA_OPT__( __JOINSREV1( J,__VA_ARGS__) )
#define __JOINSREV3(J,a,...)  J #a __VA_OPT__( __JOINSREV2( J,__VA_ARGS__) )
#define __JOINSREV4(J,a,...)  J #a __VA_OPT__( __JOINSREV3( J,__VA_ARGS__) )
#define __JOINSREV5(J,a,...)  J #a __VA_OPT__( __JOINSREV4( J,__VA_ARGS__) )
#define __JOINSREV6(J,a,...)  J #a __VA_OPT__( __JOINSREV5( J,__VA_ARGS__) )
#define __JOINSREV7(J,a,...)  J #a __VA_OPT__( __JOINSREV6( J,__VA_ARGS__) )
#define __JOINSREV8(J,a,...)  J #a __VA_OPT__( __JOINSREV7( J,__VA_ARGS__) )
#define __JOINSREV9(J,a,...)  J #a __VA_OPT__( __JOINSREV8( J,__VA_ARGS__) )
#define __JOINSREV10(J,a,...) J #a __VA_OPT__( __JOINSREV9( J,__VA_ARGS__) )
#define __JOINSREV11(J,a,...) J #a __VA_OPT__( __JOINSREV10( J,__VA_ARGS__) )
#define __JOINSREV12(J,a,...) J #a __VA_OPT__( __JOINSREV11( J,__VA_ARGS__) )
#define __JOINSREV13(J,a,...) J #a __VA_OPT__( __JOINSREV12( J,__VA_ARGS__) )
#define __JOINSREV14(J,a,...) J #a __VA_OPT__( __JOINSREV13( J,__VA_ARGS__) )
#define __JOINSREV15(J,a,...) J #a __VA_OPT__( __JOINSREV14( J,__VA_ARGS__) )
#define __JOINSREV16(J,a,...) J #a __VA_OPT__( __JOINSREV15( J,__VA_ARGS__) )
#define __JOINSREV17(J,a,...) J #a __VA_OPT__( __JOINSREV16( J,__VA_ARGS__) )
#define __JOINSREV18(J,a,...) J #a __VA_OPT__( __JOINSREV17( J,__VA_ARGS__) )
#define __JOINSREV19(J,a,...) J #a __VA_OPT__( __JOINSREV18( J,__VA_ARGS__) )
#define __JOINSREV20(J,a,...) J #a __VA_OPT__( __JOINSREV19( J,__VA_ARGS__) )
#define __JOINSREV21(J,a,...) J #a __VA_OPT__( __JOINSREV20( J,__VA_ARGS__) )
#define __JOINSREV22(J,a,...) J #a __VA_OPT__( __JOINSREV21( J,__VA_ARGS__) )
#define __JOINSREV23(J,a,...) J #a __VA_OPT__( __JOINSREV22( J,__VA_ARGS__) )
#define __JOINSREV24(J,a,...) J #a __VA_OPT__( __JOINSREV23( J,__VA_ARGS__) )
#define __JOINSREV25(J,a,...) J #a __VA_OPT__( __JOINSREV24( J,__VA_ARGS__) )
#define __JOINSREV26(J,a,...) J #a __VA_OPT__( __JOINSREV25( J,__VA_ARGS__) )
#define __JOINSREV27(J,a,...) J #a __VA_OPT__( __JOINSREV26( J,__VA_ARGS__) )
#define __JOINSREV28(J,a,...) J #a __VA_OPT__( __JOINSREV27( J,__VA_ARGS__) )
#define __JOINSREV29(J,a,...) J #a __VA_OPT__( __JOINSREV28( J,__VA_ARGS__) )
#define __JOINSREV30(J,a,...) J #a __VA_OPT__( __JOINSREV29( J,__VA_ARGS__) )
#define __JOINSREV31(J,a,...) J #a __VA_OPT__( __JOINSREV30( J,__VA_ARGS__) )
#define __JOINSREV32(J,a,...) J #a __VA_OPT__( __JOINSREV31( J,__VA_ARGS__) )
#define __JOINSREV33(J,a,...) J #a __VA_OPT__( __JOINSREV32( J,__VA_ARGS__) )
#define __JOINSREV34(J,a,...) J #a __VA_OPT__( __JOINSREV33( J,__VA_ARGS__) )
#define __JOINSREV35(J,a,...) J #a __VA_OPT__( __JOINSREV34( J,__VA_ARGS__) )
#define __JOINSREV36(J,a,...) J #a __VA_OPT__( __JOINSREV35( J,__VA_ARGS__) )
#define __JOINSREV37(J,a,...) J #a __VA_OPT__( __JOINSREV36( J,__VA_ARGS__) )
#define __JOINSREV38(J,a,...) J #a __VA_OPT__( __JOINSREV37( J,__VA_ARGS__) )
#define __JOINSREV39(J,a,...) J #a __VA_OPT__( __JOINSREV38( J,__VA_ARGS__) )
#define __JOINSREV40(J,a,...) J #a __VA_OPT__( __JOINSREV39( J,__VA_ARGS__) )
#define __JOINSREV41(J,a,...) J #a __VA_OPT__( __JOINSREV40( J,__VA_ARGS__) )
#define __JOINSREV42(J,a,...) J #a __VA_OPT__( __JOINSREV41( J,__VA_ARGS__) )
#define __JOINSREV43(J,a,...) J #a __VA_OPT__( __JOINSREV42( J,__VA_ARGS__) )
#define __JOINSREV44(J,a,...) J #a __VA_OPT__( __JOINSREV43( J,__VA_ARGS__) )
#define __JOINSREV45(J,a,...) J #a __VA_OPT__( __JOINSREV44( J,__VA_ARGS__) )
#define __JOINSREV46(J,a,...) J #a __VA_OPT__( __JOINSREV45( J,__VA_ARGS__) )
#define __JOINSREV47(J,a,...) J #a __VA_OPT__( __JOINSREV46( J,__VA_ARGS__) )
#define __JOINSREV48(J,a,...) J #a __VA_OPT__( __JOINSREV47( J,__VA_ARGS__) )
#define __JOINSREV49(J,a,...) J #a __VA_OPT__( __JOINSREV48( J,__VA_ARGS__) )
#define __JOINSREV50(J,a,...) J #a __VA_OPT__( __JOINSREV49( J,__VA_ARGS__) )
#define __JOINSREV51(J,a,...) J #a __VA_OPT__( __JOINSREV50( J,__VA_ARGS__) )
#define __JOINSREV52(J,a,...) J #a __VA_OPT__( __JOINSREV51( J,__VA_ARGS__) )
#define __JOINSREV53(J,a,...) J #a __VA_OPT__( __JOINSREV52( J,__VA_ARGS__) )
#define __JOINSREV54(J,a,...) J #a __VA_OPT__( __JOINSREV53( J,__VA_ARGS__) )
#define __JOINSREV55(J,a,...) J #a __VA_OPT__( __JOINSREV54( J,__VA_ARGS__) )
#define __JOINSREV56(J,a,...) J #a __VA_OPT__( __JOINSREV55( J,__VA_ARGS__) )
#define __JOINSREV57(J,a,...) J #a __VA_OPT__( __JOINSREV56( J,__VA_ARGS__) )
#define __JOINSREV58(J,a,...) J #a __VA_OPT__( __JOINSREV57( J,__VA_ARGS__) )
#define __JOINSREV59(J,a,...) J #a __VA_OPT__( __JOINSREV58( J,__VA_ARGS__) )
#define __JOINSREV60(J,a,...) J #a __VA_OPT__( __JOINSREV59( J,__VA_ARGS__) )
#define __JOINSREV61(J,a,...) J #a __VA_OPT__( __JOINSREV60( J,__VA_ARGS__) )
#define __JOINSREV62(J,a,...) J #a __VA_OPT__( __JOINSREV61( J,__VA_ARGS__) )
#define __JOINSREV63(J,a,...) J #a __VA_OPT__( __JOINSREV62( J,__VA_ARGS__) )
#define __JOINSREV64(J,a,...) J #a __VA_OPT__( __JOINSREV63( J,__VA_ARGS__) )



//+doc JOINSREV
// Macro JOINSREV( separator, ... )
// stringify (""), reverse and join all arguments with separator (not stringified)
//+def JOINSREV
#define JOINSREV( sep, a, ...) #a __VA_OPT__( __JOINSREV64( sep,__VA_ARGS__ ) )


//+doc PREPENDSREV
// Macro PREPENDSREV( prepend, ... )
// stringify (""), reverse and prepend all arguments with PREP (not stringified)
//+def PREPENDSREV
#define PREPENDSREV( PREP, a, ...) sep #a __VA_OPT__( __JOINSREV64( PREP,__VA_ARGS__ ) )



//+doc JOIN
// Join all arguments with SEP
//+def JOIN
#define JOIN( _sep, ...) FOREACH(_ML_JOIN,_sep,__VA_ARGS__)
#define _ML_JOIN(_a) _a
//#define JOIN( SEP, a, ...) a __VA_OPT__( __JOIN64( SEP,__VA_ARGS__ ) )


#define __JOINREV0(sep,a,...)   __VA_OPT__( ERROR: too many agruments at __LINE__ )
#define __JOINREV1(sep,a,...)   __VA_OPT__( __JOINREV0( sep,__VA_ARGS__) )   a sep
#define __JOINREV2(sep,a,...)   __VA_OPT__( __JOINREV1( sep,__VA_ARGS__) )   a sep
#define __JOINREV3(sep,a,...)   __VA_OPT__( __JOINREV2( sep,__VA_ARGS__) )   a sep
#define __JOINREV4(sep,a,...)   __VA_OPT__( __JOINREV3( sep,__VA_ARGS__) )   a sep
#define __JOINREV5(sep,a,...)   __VA_OPT__( __JOINREV4( sep,__VA_ARGS__) )   a sep
#define __JOINREV6(sep,a,...)   __VA_OPT__( __JOINREV5( sep,__VA_ARGS__) )   a sep
#define __JOINREV7(sep,a,...)   __VA_OPT__( __JOINREV6( sep,__VA_ARGS__) )   a sep
#define __JOINREV8(sep,a,...)   __VA_OPT__( __JOINREV7( sep,__VA_ARGS__) )   a sep
#define __JOINREV9(sep,a,...)   __VA_OPT__( __JOINREV8( sep,__VA_ARGS__) )   a sep
#define __JOINREV10(sep,a,...)  __VA_OPT__( __JOINREV9( sep,__VA_ARGS__) )   a sep
#define __JOINREV11(sep,a,...)  __VA_OPT__( __JOINREV10( sep,__VA_ARGS__) )  a sep
#define __JOINREV12(sep,a,...)  __VA_OPT__( __JOINREV11( sep,__VA_ARGS__) )  a sep
#define __JOINREV13(sep,a,...)  __VA_OPT__( __JOINREV12( sep,__VA_ARGS__) )  a sep
#define __JOINREV14(sep,a,...)  __VA_OPT__( __JOINREV13( sep,__VA_ARGS__) )  a sep
#define __JOINREV15(sep,a,...)  __VA_OPT__( __JOINREV14( sep,__VA_ARGS__) )  a sep
#define __JOINREV16(sep,a,...)  __VA_OPT__( __JOINREV15( sep,__VA_ARGS__) )  a sep
#define __JOINREV17(sep,a,...)  __VA_OPT__( __JOINREV16( sep,__VA_ARGS__) )  a sep
#define __JOINREV18(sep,a,...)  __VA_OPT__( __JOINREV17( sep,__VA_ARGS__) )  a sep
#define __JOINREV19(sep,a,...)  __VA_OPT__( __JOINREV18( sep,__VA_ARGS__) )  a sep
#define __JOINREV20(sep,a,...)  __VA_OPT__( __JOINREV19( sep,__VA_ARGS__) )  a sep
#define __JOINREV21(sep,a,...)  __VA_OPT__( __JOINREV20( sep,__VA_ARGS__) )  a sep
#define __JOINREV22(sep,a,...)  __VA_OPT__( __JOINREV21( sep,__VA_ARGS__) )  a sep
#define __JOINREV23(sep,a,...)  __VA_OPT__( __JOINREV22( sep,__VA_ARGS__) )  a sep
#define __JOINREV24(sep,a,...)  __VA_OPT__( __JOINREV23( sep,__VA_ARGS__) )  a sep
#define __JOINREV25(sep,a,...)  __VA_OPT__( __JOINREV24( sep,__VA_ARGS__) )  a sep
#define __JOINREV26(sep,a,...)  __VA_OPT__( __JOINREV25( sep,__VA_ARGS__) )  a sep
#define __JOINREV27(sep,a,...)  __VA_OPT__( __JOINREV26( sep,__VA_ARGS__) )  a sep
#define __JOINREV28(sep,a,...)  __VA_OPT__( __JOINREV27( sep,__VA_ARGS__) )  a sep
#define __JOINREV29(sep,a,...)  __VA_OPT__( __JOINREV28( sep,__VA_ARGS__) )  a sep
#define __JOINREV30(sep,a,...)  __VA_OPT__( __JOINREV29( sep,__VA_ARGS__) )  a sep
#define __JOINREV31(sep,a,...)  __VA_OPT__( __JOINREV30( sep,__VA_ARGS__) )  a sep
#define __JOINREV32(sep,a,...)  __VA_OPT__( __JOINREV31( sep,__VA_ARGS__) )  a 

//+doc JOINREV
// Reverse, and join all arguments with SEP
//+def JOINREV
#define JOINREV( SEP,...) __VA_OPT__( __JOINREV32( SEP,__VA_ARGS__ ) ) 

/*
!perl -e ' foreach my $a ( 1..64 ){ print "#define FOREACHK$a(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK".($a+1)."( _F, __VA_ARGS__ ) )\n";}' | xcs
 */


#define __CONCATS0(a,...)   #a __VA_OPT__( ERROR: too many arguments at __LINE__ )
#define __CONCATS1(a,...)   #a __VA_OPT__( __CONCATS0( __VA_ARGS__) )
#define __CONCATS2(a,...)   #a __VA_OPT__( __CONCATS1( __VA_ARGS__) )
#define __CONCATS3(a,...)   #a __VA_OPT__( __CONCATS2( __VA_ARGS__) )
#define __CONCATS4(a,...)   #a __VA_OPT__( __CONCATS3( __VA_ARGS__) )
#define __CONCATS5(a,...)   #a __VA_OPT__( __CONCATS4( __VA_ARGS__) )
#define __CONCATS6(a,...)   #a __VA_OPT__( __CONCATS5( __VA_ARGS__) )
#define __CONCATS7(a,...)   #a __VA_OPT__( __CONCATS6( __VA_ARGS__) )
#define __CONCATS8(a,...)   #a __VA_OPT__( __CONCATS7( __VA_ARGS__) )
#define __CONCATS9(a,...)   #a __VA_OPT__( __CONCATS8( __VA_ARGS__) )
#define __CONCATS10(a,...)  #a __VA_OPT__( __CONCATS9( __VA_ARGS__) )
#define __CONCATS11(a,...)  #a __VA_OPT__( __CONCATS10( __VA_ARGS__) )
#define __CONCATS12(a,...)  #a __VA_OPT__( __CONCATS11( __VA_ARGS__) )
#define __CONCATS13(a,...)  #a __VA_OPT__( __CONCATS12( __VA_ARGS__) )
#define __CONCATS14(a,...)  #a __VA_OPT__( __CONCATS13( __VA_ARGS__) )
#define __CONCATS15(a,...)  #a __VA_OPT__( __CONCATS14( __VA_ARGS__) )
#define __CONCATS16(a,...)  #a __VA_OPT__( __CONCATS15( __VA_ARGS__) )
#define __CONCATS17(a,...)  #a __VA_OPT__( __CONCATS16( __VA_ARGS__) )
#define __CONCATS18(a,...)  #a __VA_OPT__( __CONCATS17( __VA_ARGS__) )
#define __CONCATS19(a,...)  #a __VA_OPT__( __CONCATS18( __VA_ARGS__) )
#define __CONCATS20(a,...)  #a __VA_OPT__( __CONCATS19( __VA_ARGS__) )
#define __CONCATS21(a,...)  #a __VA_OPT__( __CONCATS20( __VA_ARGS__) )
#define __CONCATS22(a,...)  #a __VA_OPT__( __CONCATS21( __VA_ARGS__) )
#define __CONCATS23(a,...)  #a __VA_OPT__( __CONCATS22( __VA_ARGS__) )
#define __CONCATS24(a,...)  #a __VA_OPT__( __CONCATS23( __VA_ARGS__) )
#define __CONCATS25(a,...)  #a __VA_OPT__( __CONCATS24( __VA_ARGS__) )
#define __CONCATS26(a,...)  #a __VA_OPT__( __CONCATS25( __VA_ARGS__) )
#define __CONCATS27(a,...)  #a __VA_OPT__( __CONCATS26( __VA_ARGS__) )
#define __CONCATS28(a,...)  #a __VA_OPT__( __CONCATS27( __VA_ARGS__) )
#define __CONCATS29(a,...)  #a __VA_OPT__( __CONCATS28( __VA_ARGS__) )
#define __CONCATS30(a,...)  #a __VA_OPT__( __CONCATS29( __VA_ARGS__) )
#define __CONCATS31(a,...)  #a __VA_OPT__( __CONCATS30( __VA_ARGS__) )
#define __CONCATS32(a,...)  #a __VA_OPT__( __CONCATS31( __VA_ARGS__) )

//+doc CONCATS
// Stringify and concatenate all arguments
//+def CONCATS
#define CONCATS( a, ... ) #a __VA_OPT__( __CONCATS32( __VA_ARGS__ ) )


//#define CONCAT( ... ) __VA_OPT__( CONCAT1(__VA_ARGS__) )
//#define CONCAT@1(a,...) #a

//+doc call the macro F for each argument a,...
// separated with SEP
//+def FOREACH
#define FOREACH( _F,_SEP, ... ) __VA_OPT__( __FOREACH0( _F,_SEP, __VA_ARGS__) )
#define _FOREACH( _F,_SEP, ... ) __VA_OPT__( __FOREACH0( _F,_SEP, __VA_ARGS__) )

#define  __FOREACH0(F,sep,a,...) F(a) __VA_OPT__( __FOREACH1(  F,sep, __VA_ARGS__) )
//#define  __FOREACH@1(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH@2(  F,sep, __VA_ARGS__) )
#define  __FOREACH1(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH2(  F,sep, __VA_ARGS__) )
#define  __FOREACH2(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH3(  F,sep, __VA_ARGS__) )
#define  __FOREACH3(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH4(  F,sep, __VA_ARGS__) )
#define  __FOREACH4(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH5(  F,sep, __VA_ARGS__) )
#define  __FOREACH5(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH6(  F,sep, __VA_ARGS__) )
#define  __FOREACH6(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH7(  F,sep, __VA_ARGS__) )
#define  __FOREACH7(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH8(  F,sep, __VA_ARGS__) )
#define  __FOREACH8(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH9(  F,sep, __VA_ARGS__) )
#define  __FOREACH9(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH10(  F,sep, __VA_ARGS__) )
#define  __FOREACH10(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH11(  F,sep, __VA_ARGS__) )
#define  __FOREACH11(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH12(  F,sep, __VA_ARGS__) )
#define  __FOREACH12(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH13(  F,sep, __VA_ARGS__) )
#define  __FOREACH13(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH14(  F,sep, __VA_ARGS__) )
#define  __FOREACH14(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH15(  F,sep, __VA_ARGS__) )
#define  __FOREACH15(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH16(  F,sep, __VA_ARGS__) )
#define  __FOREACH16(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH17(  F,sep, __VA_ARGS__) )
#define  __FOREACH17(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH18(  F,sep, __VA_ARGS__) )
#define  __FOREACH18(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH19(  F,sep, __VA_ARGS__) )
#define  __FOREACH19(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH20(  F,sep, __VA_ARGS__) )
#define  __FOREACH20(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH21(  F,sep, __VA_ARGS__) )
#define  __FOREACH21(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH22(  F,sep, __VA_ARGS__) )
#define  __FOREACH22(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH23(  F,sep, __VA_ARGS__) )
#define  __FOREACH23(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH24(  F,sep, __VA_ARGS__) )
#define  __FOREACH24(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH25(  F,sep, __VA_ARGS__) )
#define  __FOREACH25(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH26(  F,sep, __VA_ARGS__) )
#define  __FOREACH26(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH27(  F,sep, __VA_ARGS__) )
#define  __FOREACH27(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH28(  F,sep, __VA_ARGS__) )
#define  __FOREACH28(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH29(  F,sep, __VA_ARGS__) )
#define  __FOREACH29(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH30(  F,sep, __VA_ARGS__) )
#define  __FOREACH30(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH31(  F,sep, __VA_ARGS__) )
#define  __FOREACH31(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH32(  F,sep, __VA_ARGS__) )
#define  __FOREACH32(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH33(  F,sep, __VA_ARGS__) )
#define  __FOREACH33(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH34(  F,sep, __VA_ARGS__) )
#define  __FOREACH34(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH35(  F,sep, __VA_ARGS__) )
#define  __FOREACH35(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH36(  F,sep, __VA_ARGS__) )
#define  __FOREACH36(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH37(  F,sep, __VA_ARGS__) )
#define  __FOREACH37(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH38(  F,sep, __VA_ARGS__) )
#define  __FOREACH38(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH39(  F,sep, __VA_ARGS__) )
#define  __FOREACH39(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH40(  F,sep, __VA_ARGS__) )
#define  __FOREACH40(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH41(  F,sep, __VA_ARGS__) )
#define  __FOREACH41(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH42(  F,sep, __VA_ARGS__) )
#define  __FOREACH42(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH43(  F,sep, __VA_ARGS__) )
#define  __FOREACH43(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH44(  F,sep, __VA_ARGS__) )
#define  __FOREACH44(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH45(  F,sep, __VA_ARGS__) )
#define  __FOREACH45(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH46(  F,sep, __VA_ARGS__) )
#define  __FOREACH46(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH47(  F,sep, __VA_ARGS__) )
#define  __FOREACH47(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH48(  F,sep, __VA_ARGS__) )
#define  __FOREACH48(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH49(  F,sep, __VA_ARGS__) )
#define  __FOREACH49(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH50(  F,sep, __VA_ARGS__) )
#define  __FOREACH50(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH51(  F,sep, __VA_ARGS__) )
#define  __FOREACH51(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH52(  F,sep, __VA_ARGS__) )
#define  __FOREACH52(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH53(  F,sep, __VA_ARGS__) )
#define  __FOREACH53(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH54(  F,sep, __VA_ARGS__) )
#define  __FOREACH54(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH55(  F,sep, __VA_ARGS__) )
#define  __FOREACH55(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH56(  F,sep, __VA_ARGS__) )
#define  __FOREACH56(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH57(  F,sep, __VA_ARGS__) )
#define  __FOREACH57(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH58(  F,sep, __VA_ARGS__) )
#define  __FOREACH58(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH59(  F,sep, __VA_ARGS__) )
#define  __FOREACH59(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH60(  F,sep, __VA_ARGS__) )
#define  __FOREACH60(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH61(  F,sep, __VA_ARGS__) )
#define  __FOREACH61(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH62(  F,sep, __VA_ARGS__) )
#define  __FOREACH62(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH63(  F,sep, __VA_ARGS__) )
#define  __FOREACH63(F,sep,a,...) sep F(a) __VA_OPT__( __FOREACH64(  F,sep, __VA_ARGS__) )
#define  __FOREACH64(F,sep,a,...) sep F(a) __VA_OPT__( __LOOP_ERROR: FOREACH )


//+doc call _F for each element _a, ... 
// separate the output of _F with ',' 
#define FOREACHK(_F,...) __VA_OPT__(__FOREACHK0( _F, __VA_ARGS__ ) )
#define FOREACH_K(_F,...) __VA_OPT__(__FOREACHK0( _F, __VA_ARGS__ ) )
#define _FOREACH_K(_F,...) __VA_OPT__(__FOREACHK0( _F, __VA_ARGS__ ) )

#define __FOREACHK0(_F,_a,...)  _F(_a) __VA_OPT__(,__FOREACHK1( _F, __VA_ARGS__ ) )
#define __FOREACHK1(_F,_a,...)  _F(_a) __VA_OPT__(,__FOREACHK2( _F, __VA_ARGS__ ) )
#define __FOREACHK2(_F,_a,...)  _F(_a) __VA_OPT__(,__FOREACHK3( _F, __VA_ARGS__ ) )
#define __FOREACHK3(_F,_a,...)  _F(_a) __VA_OPT__(,__FOREACHK4( _F, __VA_ARGS__ ) )
#define __FOREACHK4(_F,_a,...)  _F(_a) __VA_OPT__(,__FOREACHK5( _F, __VA_ARGS__ ) )
#define __FOREACHK5(_F,_a,...)  _F(_a) __VA_OPT__(,__FOREACHK6( _F, __VA_ARGS__ ) )
#define __FOREACHK6(_F,_a,...)  _F(_a) __VA_OPT__(,__FOREACHK7( _F, __VA_ARGS__ ) )
#define __FOREACHK7(_F,_a,...)  _F(_a) __VA_OPT__(,__FOREACHK8( _F, __VA_ARGS__ ) )
#define __FOREACHK8(_F,_a,...)  _F(_a) __VA_OPT__(,__FOREACHK9( _F, __VA_ARGS__ ) )
#define __FOREACHK9(_F,_a,...)  _F(_a) __VA_OPT__(,__FOREACHK10( _F, __VA_ARGS__ ) )
#define __FOREACHK10(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK11( _F, __VA_ARGS__ ) )
#define __FOREACHK11(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK12( _F, __VA_ARGS__ ) )
#define __FOREACHK12(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK13( _F, __VA_ARGS__ ) )
#define __FOREACHK13(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK14( _F, __VA_ARGS__ ) )
#define __FOREACHK14(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK15( _F, __VA_ARGS__ ) )
#define __FOREACHK15(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK16( _F, __VA_ARGS__ ) )
#define __FOREACHK16(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK17( _F, __VA_ARGS__ ) )
#define __FOREACHK17(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK18( _F, __VA_ARGS__ ) )
#define __FOREACHK18(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK19( _F, __VA_ARGS__ ) )
#define __FOREACHK19(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK20( _F, __VA_ARGS__ ) )
#define __FOREACHK20(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK21( _F, __VA_ARGS__ ) )
#define __FOREACHK21(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK22( _F, __VA_ARGS__ ) )
#define __FOREACHK22(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK23( _F, __VA_ARGS__ ) )
#define __FOREACHK23(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK24( _F, __VA_ARGS__ ) )
#define __FOREACHK24(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK25( _F, __VA_ARGS__ ) )
#define __FOREACHK25(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK26( _F, __VA_ARGS__ ) )
#define __FOREACHK26(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK27( _F, __VA_ARGS__ ) )
#define __FOREACHK27(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK28( _F, __VA_ARGS__ ) )
#define __FOREACHK28(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK29( _F, __VA_ARGS__ ) )
#define __FOREACHK29(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK30( _F, __VA_ARGS__ ) )
#define __FOREACHK30(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK31( _F, __VA_ARGS__ ) )
#define __FOREACHK31(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK32( _F, __VA_ARGS__ ) )
#define __FOREACHK32(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK33( _F, __VA_ARGS__ ) )
#define __FOREACHK33(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK34( _F, __VA_ARGS__ ) )
#define __FOREACHK34(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK35( _F, __VA_ARGS__ ) )
#define __FOREACHK35(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK36( _F, __VA_ARGS__ ) )
#define __FOREACHK36(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK37( _F, __VA_ARGS__ ) )
#define __FOREACHK37(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK38( _F, __VA_ARGS__ ) )
#define __FOREACHK38(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK39( _F, __VA_ARGS__ ) )
#define __FOREACHK39(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK40( _F, __VA_ARGS__ ) )
#define __FOREACHK40(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK41( _F, __VA_ARGS__ ) )
#define __FOREACHK41(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK42( _F, __VA_ARGS__ ) )
#define __FOREACHK42(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK43( _F, __VA_ARGS__ ) )
#define __FOREACHK43(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK44( _F, __VA_ARGS__ ) )
#define __FOREACHK44(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK45( _F, __VA_ARGS__ ) )
#define __FOREACHK45(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK46( _F, __VA_ARGS__ ) )
#define __FOREACHK46(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK47( _F, __VA_ARGS__ ) )
#define __FOREACHK47(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK48( _F, __VA_ARGS__ ) )
#define __FOREACHK48(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK49( _F, __VA_ARGS__ ) )
#define __FOREACHK49(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK50( _F, __VA_ARGS__ ) )
#define __FOREACHK50(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK51( _F, __VA_ARGS__ ) )
#define __FOREACHK51(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK52( _F, __VA_ARGS__ ) )
#define __FOREACHK52(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK53( _F, __VA_ARGS__ ) )
#define __FOREACHK53(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK54( _F, __VA_ARGS__ ) )
#define __FOREACHK54(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK55( _F, __VA_ARGS__ ) )
#define __FOREACHK55(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK56( _F, __VA_ARGS__ ) )
#define __FOREACHK56(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK57( _F, __VA_ARGS__ ) )
#define __FOREACHK57(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK58( _F, __VA_ARGS__ ) )
#define __FOREACHK58(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK59( _F, __VA_ARGS__ ) )
#define __FOREACHK59(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK60( _F, __VA_ARGS__ ) )
#define __FOREACHK60(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK61( _F, __VA_ARGS__ ) )
#define __FOREACHK61(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK62( _F, __VA_ARGS__ ) )
#define __FOREACHK62(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK63( _F, __VA_ARGS__ ) )
#define __FOREACHK63(_F,_a,...) _F(_a) __VA_OPT__(,__FOREACHK64( _F, __VA_ARGS__ ) )
#define __FOREACHK64(_F,_a,...) _F(_a) __VA_OPT__( ERROR: too many elements, FOREACHK at __FILE__, __LINE__ )


//+doc foreach, supply the iteration number as first argument to _macro
//+def
#define FOREACH_COUNTED(_macro,_sep, ... ) __VA_OPT__(__FOREACH_COUNTED1(_macro,_sep,__VA_ARGS__))
#define _FOREACH_COUNTED(_macro,_sep, ... ) __VA_OPT__(__FOREACH_COUNTED1(_macro,_sep,__VA_ARGS__))
//DEF __FOREACH_COUNTED@1(_macro,_sep,_a,...) _macro(@1,_a) __VA_OPT__(_sep __FOREACH_COUNTED@2(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED1(_macro,_sep,_a,...) _macro(1,_a) __VA_OPT__(_sep __FOREACH_COUNTED2(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED2(_macro,_sep,_a,...) _macro(2,_a) __VA_OPT__(_sep __FOREACH_COUNTED3(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED3(_macro,_sep,_a,...) _macro(3,_a) __VA_OPT__(_sep __FOREACH_COUNTED4(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED4(_macro,_sep,_a,...) _macro(4,_a) __VA_OPT__(_sep __FOREACH_COUNTED5(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED5(_macro,_sep,_a,...) _macro(5,_a) __VA_OPT__(_sep __FOREACH_COUNTED6(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED6(_macro,_sep,_a,...) _macro(6,_a) __VA_OPT__(_sep __FOREACH_COUNTED7(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED7(_macro,_sep,_a,...) _macro(7,_a) __VA_OPT__(_sep __FOREACH_COUNTED8(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED8(_macro,_sep,_a,...) _macro(8,_a) __VA_OPT__(_sep __FOREACH_COUNTED9(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED9(_macro,_sep,_a,...) _macro(9,_a) __VA_OPT__(_sep __FOREACH_COUNTED10(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED10(_macro,_sep,_a,...) _macro(10,_a) __VA_OPT__(_sep __FOREACH_COUNTED11(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED11(_macro,_sep,_a,...) _macro(11,_a) __VA_OPT__(_sep __FOREACH_COUNTED12(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED12(_macro,_sep,_a,...) _macro(12,_a) __VA_OPT__(_sep __FOREACH_COUNTED13(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED13(_macro,_sep,_a,...) _macro(13,_a) __VA_OPT__(_sep __FOREACH_COUNTED14(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED14(_macro,_sep,_a,...) _macro(14,_a) __VA_OPT__(_sep __FOREACH_COUNTED15(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED15(_macro,_sep,_a,...) _macro(15,_a) __VA_OPT__(_sep __FOREACH_COUNTED16(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED16(_macro,_sep,_a,...) _macro(16,_a) __VA_OPT__(_sep __FOREACH_COUNTED17(_macro,_sep,__VA_ARGS__))
#ifndef MLIB_MACROLOOPS_32
#define __FOREACH_COUNTED17(_macro,_sep,_a,...) __LOOP_ERROR
#else
#define __FOREACH_COUNTED17(_macro,_sep,_a,...) _macro(17,_a) __VA_OPT__(_sep __FOREACH_COUNTED18(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED18(_macro,_sep,_a,...) _macro(18,_a) __VA_OPT__(_sep __FOREACH_COUNTED19(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED19(_macro,_sep,_a,...) _macro(19,_a) __VA_OPT__(_sep __FOREACH_COUNTED20(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED20(_macro,_sep,_a,...) _macro(20,_a) __VA_OPT__(_sep __FOREACH_COUNTED21(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED21(_macro,_sep,_a,...) _macro(21,_a) __VA_OPT__(_sep __FOREACH_COUNTED22(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED22(_macro,_sep,_a,...) _macro(22,_a) __VA_OPT__(_sep __FOREACH_COUNTED23(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED23(_macro,_sep,_a,...) _macro(23,_a) __VA_OPT__(_sep __FOREACH_COUNTED24(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED24(_macro,_sep,_a,...) _macro(24,_a) __VA_OPT__(_sep __FOREACH_COUNTED25(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED25(_macro,_sep,_a,...) _macro(25,_a) __VA_OPT__(_sep __FOREACH_COUNTED26(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED26(_macro,_sep,_a,...) _macro(26,_a) __VA_OPT__(_sep __FOREACH_COUNTED27(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED27(_macro,_sep,_a,...) _macro(27,_a) __VA_OPT__(_sep __FOREACH_COUNTED28(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED28(_macro,_sep,_a,...) _macro(28,_a) __VA_OPT__(_sep __FOREACH_COUNTED29(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED29(_macro,_sep,_a,...) _macro(29,_a) __VA_OPT__(_sep __FOREACH_COUNTED30(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED30(_macro,_sep,_a,...) _macro(30,_a) __VA_OPT__(_sep __FOREACH_COUNTED31(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED31(_macro,_sep,_a,...) _macro(31,_a) __VA_OPT__(_sep __FOREACH_COUNTED32(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED32(_macro,_sep,_a,...) _macro(32,_a) __VA_OPT__(_sep __FOREACH_COUNTED33(_macro,_sep,__VA_ARGS__))
#define __FOREACH_COUNTED33(_macro,_sep,_a,...) __LOOP_ERROR : FOREACH_COUNTED
#endif

//+doc foreach, supply _arg as first argument to _macro, 
// each element of list as second, separate with , 
//+def
#define FOREACH_ARG_K(_macro,_arg,_sep, ... ) __VA_OPT__(__FOREACH_AK1(_macro,CAT(_arg),_sep,__VA_ARGS__))
//DEF __FOREACH_AK@1(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK@2(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK1(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK2(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK2(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK3(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK3(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK4(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK4(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK5(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK5(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK6(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK6(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK7(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK7(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK8(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK8(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK9(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK9(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK10(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK10(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK11(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK11(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK12(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK12(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK13(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK13(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK14(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK14(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK15(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK15(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK16(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK16(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK17(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK17(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK18(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK18(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK19(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK19(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK20(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK20(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK21(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK21(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK22(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK22(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK23(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK23(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK24(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK24(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK25(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK25(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK26(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK26(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK27(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK27(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK28(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK28(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK29(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK29(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK30(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK30(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK31(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK31(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK32(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK32(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(, _sep __FOREACH_AK33(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_AK33(_macro,_arg,_sep,_a,...) __LOOP_ERROR : _FOREACH_AK
//+doc foreach, supply _arg as first argument to _macro, 
// and each element of list as second
//+def
#define FOREACH_ARG(_macro,_arg,_sep, ... ) __VA_OPT__(__FOREACH_A1(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define _FOREACH_ARG(_macro,_arg,_sep, ... ) __VA_OPT__(__FOREACH_A1(_macro,CAT(_arg),_sep,__VA_ARGS__))
//DEF __FOREACH_A@1(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A@2(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A1(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A2(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A2(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A3(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A3(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A4(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A4(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A5(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A5(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A6(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A6(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A7(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A7(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A8(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A8(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A9(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A9(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A10(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A10(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A11(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A11(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A12(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A12(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A13(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A13(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A14(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A14(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A15(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A15(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A16(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A16(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A17(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A17(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A18(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A18(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A19(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A19(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A20(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A20(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A21(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A21(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A22(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A22(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A23(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A23(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A24(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A24(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A25(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A25(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A26(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A26(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A27(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A27(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A28(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A28(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A29(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A29(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A30(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A30(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A31(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A31(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A32(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A32(_macro,_arg,_sep,_a,...) _macro(CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_A33(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_A33(_macro,_arg,_sep,_a,...) __LOOP_ERROR : _FOREACH_A
//+doc foreach, supply the iteration number as first argument to _macro, _arg as second argument,
// and each element of list as third
//+def
#define FOREACH_COUNT_ARG(_macro,_arg,_sep, ... ) __VA_OPT__(__FOREACH_CA1(_macro,CAT(_arg),_sep,__VA_ARGS__))
//+def
#define FOREACH_CA(_macro,_arg,_sep, ... ) __VA_OPT__(__FOREACH_CA1(_macro,CAT(_arg),_sep,__VA_ARGS__))
//DEF __FOREACH_CA@1(_macro,_arg,_sep,_a,...) _macro(@1,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA@2(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA1(_macro,_arg,_sep,_a,...) _macro(1,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA2(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA2(_macro,_arg,_sep,_a,...) _macro(2,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA3(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA3(_macro,_arg,_sep,_a,...) _macro(3,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA4(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA4(_macro,_arg,_sep,_a,...) _macro(4,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA5(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA5(_macro,_arg,_sep,_a,...) _macro(5,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA6(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA6(_macro,_arg,_sep,_a,...) _macro(6,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA7(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA7(_macro,_arg,_sep,_a,...) _macro(7,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA8(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA8(_macro,_arg,_sep,_a,...) _macro(8,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA9(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA9(_macro,_arg,_sep,_a,...) _macro(9,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA10(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA10(_macro,_arg,_sep,_a,...) _macro(10,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA11(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA11(_macro,_arg,_sep,_a,...) _macro(11,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA12(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA12(_macro,_arg,_sep,_a,...) _macro(12,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA13(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA13(_macro,_arg,_sep,_a,...) _macro(13,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA14(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA14(_macro,_arg,_sep,_a,...) _macro(14,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA15(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA15(_macro,_arg,_sep,_a,...) _macro(15,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA16(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA16(_macro,_arg,_sep,_a,...) _macro(16,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA17(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA17(_macro,_arg,_sep,_a,...) _macro(17,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA18(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA18(_macro,_arg,_sep,_a,...) _macro(18,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA19(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA19(_macro,_arg,_sep,_a,...) _macro(19,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA20(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA20(_macro,_arg,_sep,_a,...) _macro(20,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA21(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA21(_macro,_arg,_sep,_a,...) _macro(21,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA22(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA22(_macro,_arg,_sep,_a,...) _macro(22,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA23(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA23(_macro,_arg,_sep,_a,...) _macro(23,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA24(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA24(_macro,_arg,_sep,_a,...) _macro(24,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA25(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA25(_macro,_arg,_sep,_a,...) _macro(25,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA26(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA26(_macro,_arg,_sep,_a,...) _macro(26,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA27(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA27(_macro,_arg,_sep,_a,...) _macro(27,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA28(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA28(_macro,_arg,_sep,_a,...) _macro(28,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA29(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA29(_macro,_arg,_sep,_a,...) _macro(29,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA30(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA30(_macro,_arg,_sep,_a,...) _macro(30,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA31(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA31(_macro,_arg,_sep,_a,...) _macro(31,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA32(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA32(_macro,_arg,_sep,_a,...) _macro(32,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA33(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA33(_macro,_arg,_sep,_a,...) _macro(33,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA34(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA34(_macro,_arg,_sep,_a,...) _macro(34,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA35(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA35(_macro,_arg,_sep,_a,...) _macro(35,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA36(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA36(_macro,_arg,_sep,_a,...) _macro(36,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA37(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA37(_macro,_arg,_sep,_a,...) _macro(37,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA38(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA38(_macro,_arg,_sep,_a,...) _macro(38,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA39(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA39(_macro,_arg,_sep,_a,...) _macro(39,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA40(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA40(_macro,_arg,_sep,_a,...) _macro(40,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA41(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA41(_macro,_arg,_sep,_a,...) _macro(41,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA42(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA42(_macro,_arg,_sep,_a,...) _macro(42,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA43(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA43(_macro,_arg,_sep,_a,...) _macro(43,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA44(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA44(_macro,_arg,_sep,_a,...) _macro(44,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA45(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA45(_macro,_arg,_sep,_a,...) _macro(45,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA46(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA46(_macro,_arg,_sep,_a,...) _macro(46,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA47(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA47(_macro,_arg,_sep,_a,...) _macro(47,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA48(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA48(_macro,_arg,_sep,_a,...) _macro(48,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA49(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA49(_macro,_arg,_sep,_a,...) _macro(49,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA50(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA50(_macro,_arg,_sep,_a,...) _macro(50,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA51(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA51(_macro,_arg,_sep,_a,...) _macro(51,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA52(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA52(_macro,_arg,_sep,_a,...) _macro(52,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA53(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA53(_macro,_arg,_sep,_a,...) _macro(53,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA54(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA54(_macro,_arg,_sep,_a,...) _macro(54,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA55(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA55(_macro,_arg,_sep,_a,...) _macro(55,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA56(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA56(_macro,_arg,_sep,_a,...) _macro(56,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA57(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA57(_macro,_arg,_sep,_a,...) _macro(57,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA58(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA58(_macro,_arg,_sep,_a,...) _macro(58,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA59(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA59(_macro,_arg,_sep,_a,...) _macro(59,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA60(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA60(_macro,_arg,_sep,_a,...) _macro(60,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA61(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA61(_macro,_arg,_sep,_a,...) _macro(61,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA62(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA62(_macro,_arg,_sep,_a,...) _macro(62,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA63(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA63(_macro,_arg,_sep,_a,...) _macro(63,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA64(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA64(_macro,_arg,_sep,_a,...) _macro(64,CAT(_arg),_a) __VA_OPT__(_sep __FOREACH_CA65(_macro,CAT(_arg),_sep,__VA_ARGS__))
#define __FOREACH_CA65(_macro,_arg,_sep,_a,...) __LOOP_ERROR : FOREACH_CA / FOREACH_COUNT_ARG

#define  __FOREACHREV0(F,sep,a,...) __VA_OPT__( ERROR: too many agruments at __LINE__ )
#define  __FOREACHREV1(F,sep,a,...) __VA_OPT__( __FOREACHREV0(  F,sep, __VA_ARGS__) ) F(a)
#define  __FOREACHREV2(F,sep,a,...) __VA_OPT__( __FOREACHREV1(  F,sep, __VA_ARGS__) ) F(a) sep 
#define  __FOREACHREV3(F,sep,a,...) __VA_OPT__( __FOREACHREV2(  F,sep, __VA_ARGS__) ) F(a) sep
#define  __FOREACHREV4(F,sep,a,...) __VA_OPT__( __FOREACHREV3(  F,sep, __VA_ARGS__) ) F(a) sep
#define  __FOREACHREV5(F,sep,a,...) __VA_OPT__( __FOREACHREV4(  F,sep, __VA_ARGS__) ) F(a) sep
#define  __FOREACHREV6(F,sep,a,...) __VA_OPT__( __FOREACHREV5(  F,sep, __VA_ARGS__) ) F(a) sep
#define  __FOREACHREV7(F,sep,a,...) __VA_OPT__( __FOREACHREV6(  F,sep, __VA_ARGS__) ) F(a) sep
#define  __FOREACHREV8(F,sep,a,...) __VA_OPT__( __FOREACHREV7(  F,sep, __VA_ARGS__) ) F(a) sep
#define  __FOREACHREV9(F,sep,a,...) __VA_OPT__( __FOREACHREV8(  F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV10(F,sep,a,...) __VA_OPT__( __FOREACHREV9(  F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV11(F,sep,a,...) __VA_OPT__( __FOREACHREV10( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV12(F,sep,a,...) __VA_OPT__( __FOREACHREV11( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV13(F,sep,a,...) __VA_OPT__( __FOREACHREV12( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV14(F,sep,a,...) __VA_OPT__( __FOREACHREV13( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV15(F,sep,a,...) __VA_OPT__( __FOREACHREV14( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV16(F,sep,a,...) __VA_OPT__( __FOREACHREV15( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV17(F,sep,a,...) __VA_OPT__( __FOREACHREV16( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV18(F,sep,a,...) __VA_OPT__( __FOREACHREV17( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV19(F,sep,a,...) __VA_OPT__( __FOREACHREV18( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV20(F,sep,a,...) __VA_OPT__( __FOREACHREV19( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV21(F,sep,a,...) __VA_OPT__( __FOREACHREV20( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV22(F,sep,a,...) __VA_OPT__( __FOREACHREV21( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV23(F,sep,a,...) __VA_OPT__( __FOREACHREV22( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV24(F,sep,a,...) __VA_OPT__( __FOREACHREV23( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV25(F,sep,a,...) __VA_OPT__( __FOREACHREV24( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV26(F,sep,a,...) __VA_OPT__( __FOREACHREV25( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV27(F,sep,a,...) __VA_OPT__( __FOREACHREV26( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV28(F,sep,a,...) __VA_OPT__( __FOREACHREV27( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV29(F,sep,a,...) __VA_OPT__( __FOREACHREV28( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV30(F,sep,a,...) __VA_OPT__( __FOREACHREV29( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV31(F,sep,a,...) __VA_OPT__( __FOREACHREV30( F,sep, __VA_ARGS__) ) F(a) sep
#define __FOREACHREV32(F,sep,a,...) __VA_OPT__( __FOREACHREV31( F,sep, __VA_ARGS__) ) F(a) sep


#define FOREACHREV( F,sep, ... ) __VA_OPT__( __FOREACHREV32( F,sep, __VA_ARGS__) ) F(a) 
#define _FOREACHREV( F,sep, ... ) __VA_OPT__( __FOREACHREV32( F,sep, __VA_ARGS__) ) F(a) 

//+doc call F foreach pair of elements 
#define FOREACH_PAIR( F,sep,... ) __VA_OPT__( __FOREACH_PAIR32( F,sep, __VA_ARGS__) )
#define _FOREACH_PAIR( F,sep,... ) __VA_OPT__( __FOREACH_PAIR32( F,sep, __VA_ARGS__) )

#define  __FOREACH_PAIR0(F,sep,a,b,...) sep F(a,b) __VA_OPT__( ERROR: too many agruments at __LINE__ )
#define  __FOREACH_PAIR1(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR0(  F,sep, __VA_ARGS__) )
#define  __FOREACH_PAIR2(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR1(  F,sep, __VA_ARGS__) )
#define  __FOREACH_PAIR3(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR2(  F,sep, __VA_ARGS__) )
#define  __FOREACH_PAIR4(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR3(  F,sep, __VA_ARGS__) )
#define  __FOREACH_PAIR5(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR4(  F,sep, __VA_ARGS__) )
#define  __FOREACH_PAIR6(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR5(  F,sep, __VA_ARGS__) )
#define  __FOREACH_PAIR7(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR6(  F,sep, __VA_ARGS__) )
#define  __FOREACH_PAIR8(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR7(  F,sep, __VA_ARGS__) )
#define  __FOREACH_PAIR9(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR8(  F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR10(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR9(  F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR11(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR10( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR12(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR11( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR13(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR12( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR14(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR13( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR15(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR14( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR16(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR15( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR17(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR16( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR18(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR17( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR19(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR18( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR20(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR19( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR21(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR20( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR22(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR21( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR23(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR22( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR24(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR23( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR25(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR24( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR26(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR25( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR27(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR26( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR28(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR27( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR29(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR28( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR30(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR29( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR31(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR30( F,sep, __VA_ARGS__) )
#define __FOREACH_PAIR32(F,sep,a,b,...) sep F(a,b) __VA_OPT__( __FOREACH_PAIR31( F,sep, __VA_ARGS__) )


//+doc
#define FOREACH_PAIR_K(_F,...) __VA_OPT__(__FOREACH_PAIR_K1( _F, __VA_ARGS__ ))
#define _FOREACH_PAIR_K(_F,...) __VA_OPT__(__FOREACH_PAIR_K1( _F, __VA_ARGS__ ))

#define __FOREACH_PAIR_K1(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K2( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K2(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K3( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K3(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K4( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K4(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K5( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K5(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K6( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K6(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K7( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K7(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K8( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K8(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K9( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K9(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K10( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K10(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K11( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K11(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K12( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K12(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K13( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K13(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K14( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K14(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K15( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K15(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K16( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K16(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K17( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K17(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K18( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K18(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K19( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K19(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K20( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K20(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K21( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K21(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K22( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K22(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K23( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K23(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K24( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K24(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K25( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K25(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K26( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K26(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K27( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K27(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K28( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K28(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K29( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K29(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K30( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K30(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K31( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K31(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K32( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K32(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K33( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K33(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K34( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K34(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K35( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K35(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K36( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K36(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K37( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K37(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K38( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K38(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K39( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K39(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K40( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K40(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K41( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K41(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K42( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K42(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K43( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K43(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K44( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K44(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K45( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K45(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K46( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K46(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K47( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K47(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K48( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K48(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K49( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K49(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K50( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K50(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K51( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K51(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K52( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K52(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K53( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K53(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K54( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K54(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K55( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K55(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K56( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K56(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K57( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K57(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K58( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K58(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K59( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K59(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K60( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K60(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K61( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K61(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K62( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K62(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K63( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K63(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K64( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K64(_F,_a,_b,...) _F(_a,_b) __VA_OPT__(,__FOREACH_PAIR_K65( _F, __VA_ARGS__ ) )
#define __FOREACH_PAIR_K65(_F,...) __VA_OPT__( ERROR: too many elements for FOREACH_PAIR_K at __FILE__,__LINE__ )

//
//+doc
//+def
#define FOREACH_TRIPLE( F,sep, ... ) __VA_OPT__( __FOREACH_TRIPLE0( F,sep, __VA_ARGS__) )
#define _FOREACH_TRIPLE( F,sep, ... ) __VA_OPT__( __FOREACH_TRIPLE0( F,sep, __VA_ARGS__) )

#define  __FOREACH_TRIPLE0(F,sep,a,b,c,...) F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE1(  F,sep, __VA_ARGS__) )
//DEF __FOREACH_TRIPLE@1(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE@2(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE1(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE2(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE2(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE3(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE3(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE4(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE4(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE5(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE5(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE6(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE6(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE7(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE7(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE8(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE8(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE9(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE9(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE10(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE10(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE11(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE11(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE12(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE12(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE13(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE13(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE14(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE14(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE15(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE15(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE16(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE16(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE17(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE17(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE18(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE18(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE19(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE19(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE20(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE20(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE21(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE21(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE22(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE22(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE23(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE23(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE24(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE24(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE25(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE25(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE26(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE26(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE27(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE27(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE28(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE28(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE29(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE29(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE30(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE30(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE31(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE31(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE32(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE32(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE33(  F,sep, __VA_ARGS__) )
#ifndef MLIB_MACROLOOPS_64
#define __FOREACH_TRIPLE33(F,sep,a,b,c,...) __LOOP_ERROR
#else
#define __FOREACH_TRIPLE33(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE34(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE34(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE35(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE35(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE36(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE36(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE37(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE37(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE38(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE38(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE39(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE39(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE40(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE40(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE41(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE41(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE42(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE42(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE43(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE43(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE44(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE44(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE45(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE45(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE46(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE46(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE47(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE47(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE48(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE48(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE49(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE49(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE50(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE50(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE51(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE51(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE52(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE52(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE53(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE53(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE54(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE54(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE55(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE55(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE56(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE56(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE57(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE57(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE58(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE58(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE59(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE59(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE60(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE60(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE61(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE61(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE62(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE62(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE63(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE63(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE64(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE64(F,sep,a,b,c,...) sep F(a,b,c) __VA_OPT__( __FOREACH_TRIPLE65(  F,sep, __VA_ARGS__) )
#define __FOREACH_TRIPLE65(F,sep,a,b,c,...) __LOOP_ERROR : FOREACH_TRIPLE
#endif



//+def
#define FOREACH_FOUR_K( F, ... ) __VA_OPT__( __FOREACH_FOUR_K0( F, __VA_ARGS__ ) )

#define __FOREACH_FOUR_K0(F,a,b,c,d,...) F(a,b,c,d)  __VA_OPT__(,__FOREACH_FOUR_K1(F,__VA_ARGS__))
//DEF __FOREACH_FOUR_K@1(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K@2(F,__VA_ARGS__))
#define __FOREACH_FOUR_K1(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K2(F,__VA_ARGS__))
#define __FOREACH_FOUR_K2(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K3(F,__VA_ARGS__))
#define __FOREACH_FOUR_K3(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K4(F,__VA_ARGS__))
#define __FOREACH_FOUR_K4(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K5(F,__VA_ARGS__))
#define __FOREACH_FOUR_K5(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K6(F,__VA_ARGS__))
#define __FOREACH_FOUR_K6(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K7(F,__VA_ARGS__))
#define __FOREACH_FOUR_K7(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K8(F,__VA_ARGS__))
#define __FOREACH_FOUR_K8(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K9(F,__VA_ARGS__))
#define __FOREACH_FOUR_K9(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K10(F,__VA_ARGS__))
#define __FOREACH_FOUR_K10(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K11(F,__VA_ARGS__))
#define __FOREACH_FOUR_K11(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K12(F,__VA_ARGS__))
#define __FOREACH_FOUR_K12(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K13(F,__VA_ARGS__))
#define __FOREACH_FOUR_K13(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K14(F,__VA_ARGS__))
#define __FOREACH_FOUR_K14(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K15(F,__VA_ARGS__))
#define __FOREACH_FOUR_K15(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K16(F,__VA_ARGS__))
#define __FOREACH_FOUR_K16(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K17(F,__VA_ARGS__))
#define __FOREACH_FOUR_K17(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K18(F,__VA_ARGS__))
#define __FOREACH_FOUR_K18(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K19(F,__VA_ARGS__))
#define __FOREACH_FOUR_K19(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K20(F,__VA_ARGS__))
#define __FOREACH_FOUR_K20(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K21(F,__VA_ARGS__))
#define __FOREACH_FOUR_K21(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K22(F,__VA_ARGS__))
#define __FOREACH_FOUR_K22(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K23(F,__VA_ARGS__))
#define __FOREACH_FOUR_K23(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K24(F,__VA_ARGS__))
#define __FOREACH_FOUR_K24(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K25(F,__VA_ARGS__))
#define __FOREACH_FOUR_K25(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K26(F,__VA_ARGS__))
#define __FOREACH_FOUR_K26(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K27(F,__VA_ARGS__))
#define __FOREACH_FOUR_K27(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K28(F,__VA_ARGS__))
#define __FOREACH_FOUR_K28(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K29(F,__VA_ARGS__))
#define __FOREACH_FOUR_K29(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K30(F,__VA_ARGS__))
#define __FOREACH_FOUR_K30(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K31(F,__VA_ARGS__))
#define __FOREACH_FOUR_K31(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K32(F,__VA_ARGS__))
#define __FOREACH_FOUR_K32(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K33(F,__VA_ARGS__))
#ifndef MLIB_MACROLOOPS_64
#define __FOREACH_FOUR_K33(F,a,b,c,d,...) __LOOP_ERROR
#else
#define __FOREACH_FOUR_K33(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K34(F,__VA_ARGS__))
#define __FOREACH_FOUR_K34(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K35(F,__VA_ARGS__))
#define __FOREACH_FOUR_K35(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K36(F,__VA_ARGS__))
#define __FOREACH_FOUR_K36(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K37(F,__VA_ARGS__))
#define __FOREACH_FOUR_K37(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K38(F,__VA_ARGS__))
#define __FOREACH_FOUR_K38(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K39(F,__VA_ARGS__))
#define __FOREACH_FOUR_K39(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K40(F,__VA_ARGS__))
#define __FOREACH_FOUR_K40(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K41(F,__VA_ARGS__))
#define __FOREACH_FOUR_K41(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K42(F,__VA_ARGS__))
#define __FOREACH_FOUR_K42(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K43(F,__VA_ARGS__))
#define __FOREACH_FOUR_K43(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K44(F,__VA_ARGS__))
#define __FOREACH_FOUR_K44(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K45(F,__VA_ARGS__))
#define __FOREACH_FOUR_K45(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K46(F,__VA_ARGS__))
#define __FOREACH_FOUR_K46(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K47(F,__VA_ARGS__))
#define __FOREACH_FOUR_K47(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K48(F,__VA_ARGS__))
#define __FOREACH_FOUR_K48(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K49(F,__VA_ARGS__))
#define __FOREACH_FOUR_K49(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K50(F,__VA_ARGS__))
#define __FOREACH_FOUR_K50(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K51(F,__VA_ARGS__))
#define __FOREACH_FOUR_K51(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K52(F,__VA_ARGS__))
#define __FOREACH_FOUR_K52(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K53(F,__VA_ARGS__))
#define __FOREACH_FOUR_K53(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K54(F,__VA_ARGS__))
#define __FOREACH_FOUR_K54(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K55(F,__VA_ARGS__))
#define __FOREACH_FOUR_K55(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K56(F,__VA_ARGS__))
#define __FOREACH_FOUR_K56(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K57(F,__VA_ARGS__))
#define __FOREACH_FOUR_K57(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K58(F,__VA_ARGS__))
#define __FOREACH_FOUR_K58(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K59(F,__VA_ARGS__))
#define __FOREACH_FOUR_K59(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K60(F,__VA_ARGS__))
#define __FOREACH_FOUR_K60(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K61(F,__VA_ARGS__))
#define __FOREACH_FOUR_K61(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K62(F,__VA_ARGS__))
#define __FOREACH_FOUR_K62(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K63(F,__VA_ARGS__))
#define __FOREACH_FOUR_K63(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K64(F,__VA_ARGS__))
#define __FOREACH_FOUR_K64(F,a,b,c,d,...) F(a,b,c,d) __VA_OPT__(,__FOREACH_FOUR_K65(F,__VA_ARGS__))
#define __FOREACH_FOUR_K65(F,a,b,c,d,...) __LOOP_ERROR
#endif



//+def
#define FOREACH_FOUR( F, sep, ... ) __VA_OPT__( __FOREACH_FOUR0( F, sep, __VA_ARGS__ ) )

#define __FOREACH_FOUR0(F,sep,a,b,c,d,...) F(a,b,c,d)  __VA_OPT__(__FOREACH_FOUR1(F,sep,__VA_ARGS__))
//DEF __FOREACH_FOUR@1(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR@2(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR1(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR2(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR2(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR3(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR3(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR4(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR4(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR5(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR5(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR6(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR6(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR7(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR7(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR8(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR8(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR9(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR9(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR10(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR10(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR11(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR11(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR12(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR12(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR13(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR13(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR14(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR14(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR15(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR15(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR16(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR16(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR17(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR17(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR18(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR18(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR19(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR19(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR20(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR20(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR21(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR21(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR22(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR22(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR23(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR23(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR24(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR24(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR25(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR25(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR26(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR26(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR27(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR27(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR28(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR28(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR29(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR29(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR30(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR30(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR31(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR31(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR32(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR32(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR33(F,sep,__VA_ARGS__))
#ifndef MLIB_MACROLOOPS_64
#define __FOREACH_FOUR33(F,sep,a,b,c,d,...) __LOOP_ERROR
#else
#define __FOREACH_FOUR33(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR34(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR34(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR35(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR35(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR36(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR36(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR37(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR37(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR38(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR38(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR39(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR39(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR40(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR40(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR41(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR41(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR42(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR42(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR43(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR43(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR44(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR44(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR45(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR45(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR46(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR46(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR47(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR47(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR48(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR48(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR49(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR49(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR50(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR50(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR51(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR51(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR52(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR52(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR53(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR53(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR54(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR54(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR55(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR55(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR56(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR56(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR57(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR57(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR58(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR58(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR59(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR59(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR60(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR60(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR61(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR61(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR62(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR62(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR63(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR63(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR64(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR64(F,sep,a,b,c,d,...) sep F(a,b,c,d) __VA_OPT__(__FOREACH_FOUR65(F,sep,__VA_ARGS__))
#define __FOREACH_FOUR65(F,sep,a,b,c,d,...) __LOOP_ERROR
#endif



//+ doc LOOP _n times, call F(n) foreach iteration
//optional ... ,__VA_ARGS__, are the "separator". 
// (inserted only between the elements)
//+def
#define LOOP(_n,F,...) REV( LOOP_REV ##_n(F,__VA_ARGS__) )
//+def
#define LOOP_K(_n,F,...) REV_K( LOOP_REV ##_n(F,__VA_ARGS__) )

//+doc loop, reverse order
//+def
#define LOOP_REV(_n,F,...) JOIN(, LOOP_REV ## _n(F,__VA_ARGS__) )
//+def 
#define LOOP_REV_K(_n,F,...) CAT( LOOP_REV ## _n(F,__VA_ARGS__) )

#define LOOP_REV0(F,...)
#define LOOP_REV1(F,...) F(1)
//DEF LOOP_REV@2(F,...) F(@2) __VA_ARGS__ , LOOP_REV@1(F,__VA_ARGS__)
#define LOOP_REV2(F,...) F(2) __VA_ARGS__ , LOOP_REV1(F,__VA_ARGS__)
#define LOOP_REV3(F,...) F(3) __VA_ARGS__ , LOOP_REV2(F,__VA_ARGS__)
#define LOOP_REV4(F,...) F(4) __VA_ARGS__ , LOOP_REV3(F,__VA_ARGS__)
#define LOOP_REV5(F,...) F(5) __VA_ARGS__ , LOOP_REV4(F,__VA_ARGS__)
#define LOOP_REV6(F,...) F(6) __VA_ARGS__ , LOOP_REV5(F,__VA_ARGS__)
#define LOOP_REV7(F,...) F(7) __VA_ARGS__ , LOOP_REV6(F,__VA_ARGS__)
#define LOOP_REV8(F,...) F(8) __VA_ARGS__ , LOOP_REV7(F,__VA_ARGS__)
#define LOOP_REV9(F,...) F(9) __VA_ARGS__ , LOOP_REV8(F,__VA_ARGS__)
#define LOOP_REV10(F,...) F(10) __VA_ARGS__ , LOOP_REV9(F,__VA_ARGS__)
#define LOOP_REV11(F,...) F(11) __VA_ARGS__ , LOOP_REV10(F,__VA_ARGS__)
#define LOOP_REV12(F,...) F(12) __VA_ARGS__ , LOOP_REV11(F,__VA_ARGS__)
#define LOOP_REV13(F,...) F(13) __VA_ARGS__ , LOOP_REV12(F,__VA_ARGS__)
#define LOOP_REV14(F,...) F(14) __VA_ARGS__ , LOOP_REV13(F,__VA_ARGS__)
#define LOOP_REV15(F,...) F(15) __VA_ARGS__ , LOOP_REV14(F,__VA_ARGS__)
#define LOOP_REV16(F,...) F(16) __VA_ARGS__ , LOOP_REV15(F,__VA_ARGS__)
#define LOOP_REV17(F,...) F(17) __VA_ARGS__ , LOOP_REV16(F,__VA_ARGS__)
#define LOOP_REV18(F,...) F(18) __VA_ARGS__ , LOOP_REV17(F,__VA_ARGS__)
#define LOOP_REV19(F,...) F(19) __VA_ARGS__ , LOOP_REV18(F,__VA_ARGS__)
#define LOOP_REV20(F,...) F(20) __VA_ARGS__ , LOOP_REV19(F,__VA_ARGS__)
#define LOOP_REV21(F,...) F(21) __VA_ARGS__ , LOOP_REV20(F,__VA_ARGS__)
#define LOOP_REV22(F,...) F(22) __VA_ARGS__ , LOOP_REV21(F,__VA_ARGS__)
#define LOOP_REV23(F,...) F(23) __VA_ARGS__ , LOOP_REV22(F,__VA_ARGS__)
#define LOOP_REV24(F,...) F(24) __VA_ARGS__ , LOOP_REV23(F,__VA_ARGS__)
#define LOOP_REV25(F,...) F(25) __VA_ARGS__ , LOOP_REV24(F,__VA_ARGS__)
#define LOOP_REV26(F,...) F(26) __VA_ARGS__ , LOOP_REV25(F,__VA_ARGS__)
#define LOOP_REV27(F,...) F(27) __VA_ARGS__ , LOOP_REV26(F,__VA_ARGS__)
#define LOOP_REV28(F,...) F(28) __VA_ARGS__ , LOOP_REV27(F,__VA_ARGS__)
#define LOOP_REV29(F,...) F(29) __VA_ARGS__ , LOOP_REV28(F,__VA_ARGS__)
#define LOOP_REV30(F,...) F(30) __VA_ARGS__ , LOOP_REV29(F,__VA_ARGS__)
#define LOOP_REV31(F,...) F(31) __VA_ARGS__ , LOOP_REV30(F,__VA_ARGS__)
#define LOOP_REV32(F,...) F(32) __VA_ARGS__ , LOOP_REV31(F,__VA_ARGS__)
#define LOOP_REV33(F,...) F(33) __VA_ARGS__ , LOOP_REV32(F,__VA_ARGS__)
#define LOOP_REV34(F,...) F(34) __VA_ARGS__ , LOOP_REV33(F,__VA_ARGS__)
#define LOOP_REV35(F,...) F(35) __VA_ARGS__ , LOOP_REV34(F,__VA_ARGS__)
#define LOOP_REV36(F,...) F(36) __VA_ARGS__ , LOOP_REV35(F,__VA_ARGS__)
#define LOOP_REV37(F,...) F(37) __VA_ARGS__ , LOOP_REV36(F,__VA_ARGS__)
#define LOOP_REV38(F,...) F(38) __VA_ARGS__ , LOOP_REV37(F,__VA_ARGS__)
#define LOOP_REV39(F,...) F(39) __VA_ARGS__ , LOOP_REV38(F,__VA_ARGS__)
#define LOOP_REV40(F,...) F(40) __VA_ARGS__ , LOOP_REV39(F,__VA_ARGS__)
#define LOOP_REV41(F,...) F(41) __VA_ARGS__ , LOOP_REV40(F,__VA_ARGS__)
#define LOOP_REV42(F,...) F(42) __VA_ARGS__ , LOOP_REV41(F,__VA_ARGS__)
#define LOOP_REV43(F,...) F(43) __VA_ARGS__ , LOOP_REV42(F,__VA_ARGS__)
#define LOOP_REV44(F,...) F(44) __VA_ARGS__ , LOOP_REV43(F,__VA_ARGS__)
#define LOOP_REV45(F,...) F(45) __VA_ARGS__ , LOOP_REV44(F,__VA_ARGS__)
#define LOOP_REV46(F,...) F(46) __VA_ARGS__ , LOOP_REV45(F,__VA_ARGS__)
#define LOOP_REV47(F,...) F(47) __VA_ARGS__ , LOOP_REV46(F,__VA_ARGS__)
#define LOOP_REV48(F,...) F(48) __VA_ARGS__ , LOOP_REV47(F,__VA_ARGS__)
#define LOOP_REV49(F,...) F(49) __VA_ARGS__ , LOOP_REV48(F,__VA_ARGS__)
#define LOOP_REV50(F,...) F(50) __VA_ARGS__ , LOOP_REV49(F,__VA_ARGS__)
#define LOOP_REV51(F,...) F(51) __VA_ARGS__ , LOOP_REV50(F,__VA_ARGS__)
#define LOOP_REV52(F,...) F(52) __VA_ARGS__ , LOOP_REV51(F,__VA_ARGS__)
#define LOOP_REV53(F,...) F(53) __VA_ARGS__ , LOOP_REV52(F,__VA_ARGS__)
#define LOOP_REV54(F,...) F(54) __VA_ARGS__ , LOOP_REV53(F,__VA_ARGS__)
#define LOOP_REV55(F,...) F(55) __VA_ARGS__ , LOOP_REV54(F,__VA_ARGS__)
#define LOOP_REV56(F,...) F(56) __VA_ARGS__ , LOOP_REV55(F,__VA_ARGS__)
#define LOOP_REV57(F,...) F(57) __VA_ARGS__ , LOOP_REV56(F,__VA_ARGS__)
#define LOOP_REV58(F,...) F(58) __VA_ARGS__ , LOOP_REV57(F,__VA_ARGS__)
#define LOOP_REV59(F,...) F(59) __VA_ARGS__ , LOOP_REV58(F,__VA_ARGS__)
#define LOOP_REV60(F,...) F(60) __VA_ARGS__ , LOOP_REV59(F,__VA_ARGS__)
#define LOOP_REV61(F,...) F(61) __VA_ARGS__ , LOOP_REV60(F,__VA_ARGS__)
#define LOOP_REV62(F,...) F(62) __VA_ARGS__ , LOOP_REV61(F,__VA_ARGS__)
#define LOOP_REV63(F,...) F(63) __VA_ARGS__ , LOOP_REV62(F,__VA_ARGS__)
#define LOOP_REV64(F,...) F(64) __VA_ARGS__ , LOOP_REV63(F,__VA_ARGS__)
#define LOOP_REV65(F,...) F(65) __VA_ARGS__ , LOOP_REV64(F,__VA_ARGS__)
#define LOOP_REV66(F,...) __LOOP_ERROR


// the @ seems to be sort of a "star", ??? (in the first or second layer, as macro name.)
//#define LOOP_REV@(F,...) __LOOP_ERROR


//+doc concatenate all arguments. a,b,c -> abc
//+def
#define CONCAT(...) __VA_OPT__( __CONCAT1(__VA_ARGS__) )
#define _CONCAT(...) __VA_OPT__( __CONCAT1(__VA_ARGS__) )
//DEF __CONCAT@1(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT@2(__VA_ARGS__)))
#define __CONCAT1(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT2(__VA_ARGS__)))
#define __CONCAT2(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT3(__VA_ARGS__)))
#define __CONCAT3(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT4(__VA_ARGS__)))
#define __CONCAT4(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT5(__VA_ARGS__)))
#define __CONCAT5(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT6(__VA_ARGS__)))
#define __CONCAT6(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT7(__VA_ARGS__)))
#define __CONCAT7(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT8(__VA_ARGS__)))
#define __CONCAT8(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT9(__VA_ARGS__)))
#define __CONCAT9(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT10(__VA_ARGS__)))
#define __CONCAT10(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT11(__VA_ARGS__)))
#define __CONCAT11(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT12(__VA_ARGS__)))
#define __CONCAT12(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT13(__VA_ARGS__)))
#define __CONCAT13(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT14(__VA_ARGS__)))
#define __CONCAT14(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT15(__VA_ARGS__)))
#define __CONCAT15(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT16(__VA_ARGS__)))
#define __CONCAT16(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT17(__VA_ARGS__)))
#define __CONCAT17(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT18(__VA_ARGS__)))
#define __CONCAT18(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT19(__VA_ARGS__)))
#define __CONCAT19(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT20(__VA_ARGS__)))
#define __CONCAT20(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT21(__VA_ARGS__)))
#define __CONCAT21(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT22(__VA_ARGS__)))
#define __CONCAT22(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT23(__VA_ARGS__)))
#define __CONCAT23(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT24(__VA_ARGS__)))
#define __CONCAT24(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT25(__VA_ARGS__)))
#define __CONCAT25(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT26(__VA_ARGS__)))
#define __CONCAT26(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT27(__VA_ARGS__)))
#define __CONCAT27(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT28(__VA_ARGS__)))
#define __CONCAT28(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT29(__VA_ARGS__)))
#define __CONCAT29(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT30(__VA_ARGS__)))
#define __CONCAT30(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT31(__VA_ARGS__)))
#define __CONCAT31(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT32(__VA_ARGS__)))
#define __CONCAT32(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT33(__VA_ARGS__)))
#define __CONCAT33(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT34(__VA_ARGS__)))
#define __CONCAT34(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT35(__VA_ARGS__)))
#define __CONCAT35(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT36(__VA_ARGS__)))
#define __CONCAT36(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT37(__VA_ARGS__)))
#define __CONCAT37(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT38(__VA_ARGS__)))
#define __CONCAT38(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT39(__VA_ARGS__)))
#define __CONCAT39(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT40(__VA_ARGS__)))
#define __CONCAT40(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT41(__VA_ARGS__)))
#define __CONCAT41(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT42(__VA_ARGS__)))
#define __CONCAT42(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT43(__VA_ARGS__)))
#define __CONCAT43(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT44(__VA_ARGS__)))
#define __CONCAT44(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT45(__VA_ARGS__)))
#define __CONCAT45(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT46(__VA_ARGS__)))
#define __CONCAT46(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT47(__VA_ARGS__)))
#define __CONCAT47(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT48(__VA_ARGS__)))
#define __CONCAT48(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT49(__VA_ARGS__)))
#define __CONCAT49(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT50(__VA_ARGS__)))
#define __CONCAT50(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT51(__VA_ARGS__)))
#define __CONCAT51(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT52(__VA_ARGS__)))
#define __CONCAT52(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT53(__VA_ARGS__)))
#define __CONCAT53(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT54(__VA_ARGS__)))
#define __CONCAT54(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT55(__VA_ARGS__)))
#define __CONCAT55(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT56(__VA_ARGS__)))
#define __CONCAT56(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT57(__VA_ARGS__)))
#define __CONCAT57(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT58(__VA_ARGS__)))
#define __CONCAT58(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT59(__VA_ARGS__)))
#define __CONCAT59(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT60(__VA_ARGS__)))
#define __CONCAT60(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT61(__VA_ARGS__)))
#define __CONCAT61(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT62(__VA_ARGS__)))
#define __CONCAT62(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT63(__VA_ARGS__)))
#define __CONCAT63(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT64(__VA_ARGS__)))
#define __CONCAT64(a,...) __arg(a)__arg(__VA_OPT__(__CONCAT65(__VA_ARGS__)))
#define __CONCAT65(a,...) __LOOP_ERROR : _CONCAT

// F needs to be a macro or function, sep can be e.g. a operator,
// which concatenates the result of F(element)
//#define __OPT(x) 1<<x
//FOREACH( __OPT, |, z,y,v,b );
//#define OPT( ... ) FOREACH( __OPT, |, __VA_ARGS__ )

// OPT( a,b,c )
// 1<<a | 1<<b | 1<<c
//

//+doc combine all arguments to a,b,c .. (with commata)
// Such an argument gets to one single argument with macros
//+def CAT
#define CAT(...) __VA_OPT__(__CAT0(__VA_ARGS__))
#define _CAT(...) __VA_OPT__(__CAT0(__VA_ARGS__))

#define __arg(x) x

#define __CAT0(a,...)  __arg(a)__VA_OPT__( __CAT1(__VA_ARGS__) )
//DEF __CAT@1(a,...)   ,a __VA_OPT__( __CAT@2( __VA_ARGS__) )
#define __CAT1(a,...)   , __arg(a)__VA_OPT__( __CAT2( __VA_ARGS__) )
#define __CAT2(a,...)   , __arg(a)__VA_OPT__( __CAT3( __VA_ARGS__) )
#define __CAT3(a,...)   , __arg(a)__VA_OPT__( __CAT4( __VA_ARGS__) )
#define __CAT4(a,...)   , __arg(a)__VA_OPT__( __CAT5( __VA_ARGS__) )
#define __CAT5(a,...)   , __arg(a)__VA_OPT__( __CAT6( __VA_ARGS__) )
#define __CAT6(a,...)   , __arg(a)__VA_OPT__( __CAT7( __VA_ARGS__) )
#define __CAT7(a,...)   , __arg(a)__VA_OPT__( __CAT8( __VA_ARGS__) )
#define __CAT8(a,...)   , __arg(a)__VA_OPT__( __CAT9( __VA_ARGS__) )
#define __CAT9(a,...)   , __arg(a)__VA_OPT__( __CAT10( __VA_ARGS__) )
#define __CAT10(a,...)   , __arg(a)__VA_OPT__( __CAT11( __VA_ARGS__) )
#define __CAT11(a,...)   , __arg(a)__VA_OPT__( __CAT12( __VA_ARGS__) )
#define __CAT12(a,...)   , __arg(a)__VA_OPT__( __CAT13( __VA_ARGS__) )
#define __CAT13(a,...)   , __arg(a)__VA_OPT__( __CAT14( __VA_ARGS__) )
#define __CAT14(a,...)   , __arg(a)__VA_OPT__( __CAT15( __VA_ARGS__) )
#define __CAT15(a,...)   , __arg(a)__VA_OPT__( __CAT16( __VA_ARGS__) )
#define __CAT16(a,...)   , __arg(a)__VA_OPT__( __CAT17( __VA_ARGS__) )
#define __CAT17(a,...)   , __arg(a)__VA_OPT__( __CAT18( __VA_ARGS__) )
#define __CAT18(a,...)   , __arg(a)__VA_OPT__( __CAT19( __VA_ARGS__) )
#define __CAT19(a,...)   , __arg(a)__VA_OPT__( __CAT20( __VA_ARGS__) )
#define __CAT20(a,...)   , __arg(a)__VA_OPT__( __CAT21( __VA_ARGS__) )
#define __CAT21(a,...)   , __arg(a)__VA_OPT__( __CAT22( __VA_ARGS__) )
#define __CAT22(a,...)   , __arg(a)__VA_OPT__( __CAT23( __VA_ARGS__) )
#define __CAT23(a,...)   , __arg(a)__VA_OPT__( __CAT24( __VA_ARGS__) )
#define __CAT24(a,...)   , __arg(a)__VA_OPT__( __CAT25( __VA_ARGS__) )
#define __CAT25(a,...)   , __arg(a)__VA_OPT__( __CAT26( __VA_ARGS__) )
#define __CAT26(a,...)   , __arg(a)__VA_OPT__( __CAT27( __VA_ARGS__) )
#define __CAT27(a,...)   , __arg(a)__VA_OPT__( __CAT28( __VA_ARGS__) )
#define __CAT28(a,...)   , __arg(a)__VA_OPT__( __CAT29( __VA_ARGS__) )
#define __CAT29(a,...)   , __arg(a)__VA_OPT__( __CAT30( __VA_ARGS__) )
#define __CAT30(a,...)   , __arg(a)__VA_OPT__( __CAT31( __VA_ARGS__) )
#define __CAT31(a,...)   , __arg(a)__VA_OPT__( __CAT32( __VA_ARGS__) )
#define __CAT32(a,...)   , __arg(a)__VA_OPT__( __CAT33( __VA_ARGS__) )
#define __CAT33(a,...)   , __arg(a)__VA_OPT__( __CAT34( __VA_ARGS__) )
#define __CAT34(a,...)   , __arg(a)__VA_OPT__( __CAT35( __VA_ARGS__) )
#define __CAT35(a,...)   , __arg(a)__VA_OPT__( __CAT36( __VA_ARGS__) )
#define __CAT36(a,...)   , __arg(a)__VA_OPT__( __CAT37( __VA_ARGS__) )
#define __CAT37(a,...)   , __arg(a)__VA_OPT__( __CAT38( __VA_ARGS__) )
#define __CAT38(a,...)   , __arg(a)__VA_OPT__( __CAT39( __VA_ARGS__) )
#define __CAT39(a,...)   , __arg(a)__VA_OPT__( __CAT40( __VA_ARGS__) )
#define __CAT40(a,...)   , __arg(a)__VA_OPT__( __CAT41( __VA_ARGS__) )
#define __CAT41(a,...)   , __arg(a)__VA_OPT__( __CAT42( __VA_ARGS__) )
#define __CAT42(a,...)   , __arg(a)__VA_OPT__( __CAT43( __VA_ARGS__) )
#define __CAT43(a,...)   , __arg(a)__VA_OPT__( __CAT44( __VA_ARGS__) )
#define __CAT44(a,...)   , __arg(a)__VA_OPT__( __CAT45( __VA_ARGS__) )
#define __CAT45(a,...)   , __arg(a)__VA_OPT__( __CAT46( __VA_ARGS__) )
#define __CAT46(a,...)   , __arg(a)__VA_OPT__( __CAT47( __VA_ARGS__) )
#define __CAT47(a,...)   , __arg(a)__VA_OPT__( __CAT48( __VA_ARGS__) )
#define __CAT48(a,...)   , __arg(a)__VA_OPT__( __CAT49( __VA_ARGS__) )
#define __CAT49(a,...)   , __arg(a)__VA_OPT__( __CAT50( __VA_ARGS__) )
#define __CAT50(a,...)   , __arg(a)__VA_OPT__( __CAT51( __VA_ARGS__) )
#define __CAT51(a,...)   , __arg(a)__VA_OPT__( __CAT52( __VA_ARGS__) )
#define __CAT52(a,...)   , __arg(a)__VA_OPT__( __CAT53( __VA_ARGS__) )
#define __CAT53(a,...)   , __arg(a)__VA_OPT__( __CAT54( __VA_ARGS__) )
#define __CAT54(a,...)   , __arg(a)__VA_OPT__( __CAT55( __VA_ARGS__) )
#define __CAT55(a,...)   , __arg(a)__VA_OPT__( __CAT56( __VA_ARGS__) )
#define __CAT56(a,...)   , __arg(a)__VA_OPT__( __CAT57( __VA_ARGS__) )
#define __CAT57(a,...)   , __arg(a)__VA_OPT__( __CAT58( __VA_ARGS__) )
#define __CAT58(a,...)   , __arg(a)__VA_OPT__( __CAT59( __VA_ARGS__) )
#define __CAT59(a,...)   , __arg(a)__VA_OPT__( __CAT60( __VA_ARGS__) )
#define __CAT60(a,...)   , __arg(a)__VA_OPT__( __CAT61( __VA_ARGS__) )
#define __CAT61(a,...)   , __arg(a)__VA_OPT__( __CAT62( __VA_ARGS__) )
#define __CAT62(a,...)   , __arg(a)__VA_OPT__( __CAT63( __VA_ARGS__) )
#define __CAT63(a,...)   , __arg(a)__VA_OPT__( __CAT64( __VA_ARGS__) )
#define __CAT64(a,...)   , __arg(a)__VA_OPT__( __CAT65( __VA_ARGS__) )
#define __CAT65(a,...)__LOOP_ERROR


//+doc reverse a list, and join wit kommata
//+def
#define REV_K(...) __VA_OPT__(__REV_K1(__VA_ARGS__))
#define _REV_K(...) __VA_OPT__(__REV_K1(__VA_ARGS__))
//DEF __REV_K@1(_a,...) __VA_OPT__(__REV_K@2(__VA_ARGS__), ) _a
#define __REV_K1(_a,...) __VA_OPT__(__REV_K2(__VA_ARGS__), ) _a
#define __REV_K2(_a,...) __VA_OPT__(__REV_K3(__VA_ARGS__), ) _a
#define __REV_K3(_a,...) __VA_OPT__(__REV_K4(__VA_ARGS__), ) _a
#define __REV_K4(_a,...) __VA_OPT__(__REV_K5(__VA_ARGS__), ) _a
#define __REV_K5(_a,...) __VA_OPT__(__REV_K6(__VA_ARGS__), ) _a
#define __REV_K6(_a,...) __VA_OPT__(__REV_K7(__VA_ARGS__), ) _a
#define __REV_K7(_a,...) __VA_OPT__(__REV_K8(__VA_ARGS__), ) _a
#define __REV_K8(_a,...) __VA_OPT__(__REV_K9(__VA_ARGS__), ) _a
#define __REV_K9(_a,...) __VA_OPT__(__REV_K10(__VA_ARGS__), ) _a
#define __REV_K10(_a,...) __VA_OPT__(__REV_K11(__VA_ARGS__), ) _a
#define __REV_K11(_a,...) __VA_OPT__(__REV_K12(__VA_ARGS__), ) _a
#define __REV_K12(_a,...) __VA_OPT__(__REV_K13(__VA_ARGS__), ) _a
#define __REV_K13(_a,...) __VA_OPT__(__REV_K14(__VA_ARGS__), ) _a
#define __REV_K14(_a,...) __VA_OPT__(__REV_K15(__VA_ARGS__), ) _a
#define __REV_K15(_a,...) __VA_OPT__(__REV_K16(__VA_ARGS__), ) _a
#define __REV_K16(_a,...) __VA_OPT__(__REV_K17(__VA_ARGS__), ) _a
#define __REV_K17(_a,...) __VA_OPT__(__REV_K18(__VA_ARGS__), ) _a
#define __REV_K18(_a,...) __VA_OPT__(__REV_K19(__VA_ARGS__), ) _a
#define __REV_K19(_a,...) __VA_OPT__(__REV_K20(__VA_ARGS__), ) _a
#define __REV_K20(_a,...) __VA_OPT__(__REV_K21(__VA_ARGS__), ) _a
#define __REV_K21(_a,...) __VA_OPT__(__REV_K22(__VA_ARGS__), ) _a
#define __REV_K22(_a,...) __VA_OPT__(__REV_K23(__VA_ARGS__), ) _a
#define __REV_K23(_a,...) __VA_OPT__(__REV_K24(__VA_ARGS__), ) _a
#define __REV_K24(_a,...) __VA_OPT__(__REV_K25(__VA_ARGS__), ) _a
#define __REV_K25(_a,...) __VA_OPT__(__REV_K26(__VA_ARGS__), ) _a
#define __REV_K26(_a,...) __VA_OPT__(__REV_K27(__VA_ARGS__), ) _a
#define __REV_K27(_a,...) __VA_OPT__(__REV_K28(__VA_ARGS__), ) _a
#define __REV_K28(_a,...) __VA_OPT__(__REV_K29(__VA_ARGS__), ) _a
#define __REV_K29(_a,...) __VA_OPT__(__REV_K30(__VA_ARGS__), ) _a
#define __REV_K30(_a,...) __VA_OPT__(__REV_K31(__VA_ARGS__), ) _a
#define __REV_K31(_a,...) __VA_OPT__(__REV_K32(__VA_ARGS__), ) _a
#define __REV_K32(_a,...) __VA_OPT__(__REV_K33(__VA_ARGS__), ) _a
#define __REV_K33(_a,...) __VA_OPT__(__REV_K34(__VA_ARGS__), ) _a
#define __REV_K34(_a,...) __VA_OPT__(__REV_K35(__VA_ARGS__), ) _a
#define __REV_K35(_a,...) __VA_OPT__(__REV_K36(__VA_ARGS__), ) _a
#define __REV_K36(_a,...) __VA_OPT__(__REV_K37(__VA_ARGS__), ) _a
#define __REV_K37(_a,...) __VA_OPT__(__REV_K38(__VA_ARGS__), ) _a
#define __REV_K38(_a,...) __VA_OPT__(__REV_K39(__VA_ARGS__), ) _a
#define __REV_K39(_a,...) __VA_OPT__(__REV_K40(__VA_ARGS__), ) _a
#define __REV_K40(_a,...) __VA_OPT__(__REV_K41(__VA_ARGS__), ) _a
#define __REV_K41(_a,...) __VA_OPT__(__REV_K42(__VA_ARGS__), ) _a
#define __REV_K42(_a,...) __VA_OPT__(__REV_K43(__VA_ARGS__), ) _a
#define __REV_K43(_a,...) __VA_OPT__(__REV_K44(__VA_ARGS__), ) _a
#define __REV_K44(_a,...) __VA_OPT__(__REV_K45(__VA_ARGS__), ) _a
#define __REV_K45(_a,...) __VA_OPT__(__REV_K46(__VA_ARGS__), ) _a
#define __REV_K46(_a,...) __VA_OPT__(__REV_K47(__VA_ARGS__), ) _a
#define __REV_K47(_a,...) __VA_OPT__(__REV_K48(__VA_ARGS__), ) _a
#define __REV_K48(_a,...) __VA_OPT__(__REV_K49(__VA_ARGS__), ) _a
#define __REV_K49(_a,...) __VA_OPT__(__REV_K50(__VA_ARGS__), ) _a
#define __REV_K50(_a,...) __VA_OPT__(__REV_K51(__VA_ARGS__), ) _a
#define __REV_K51(_a,...) __VA_OPT__(__REV_K52(__VA_ARGS__), ) _a
#define __REV_K52(_a,...) __VA_OPT__(__REV_K53(__VA_ARGS__), ) _a
#define __REV_K53(_a,...) __VA_OPT__(__REV_K54(__VA_ARGS__), ) _a
#define __REV_K54(_a,...) __VA_OPT__(__REV_K55(__VA_ARGS__), ) _a
#define __REV_K55(_a,...) __VA_OPT__(__REV_K56(__VA_ARGS__), ) _a
#define __REV_K56(_a,...) __VA_OPT__(__REV_K57(__VA_ARGS__), ) _a
#define __REV_K57(_a,...) __VA_OPT__(__REV_K58(__VA_ARGS__), ) _a
#define __REV_K58(_a,...) __VA_OPT__(__REV_K59(__VA_ARGS__), ) _a
#define __REV_K59(_a,...) __VA_OPT__(__REV_K60(__VA_ARGS__), ) _a
#define __REV_K60(_a,...) __VA_OPT__(__REV_K61(__VA_ARGS__), ) _a
#define __REV_K61(_a,...) __VA_OPT__(__REV_K62(__VA_ARGS__), ) _a
#define __REV_K62(_a,...) __VA_OPT__(__REV_K63(__VA_ARGS__), ) _a
#define __REV_K63(_a,...) __VA_OPT__(__REV_K64(__VA_ARGS__), ) _a
#define __REV_K64(_a,...) __VA_OPT__(__REV_K65(__VA_ARGS__), ) _a
#define __REV_K65(_a,...) __LOOP_ERROR


//+doc reverse a list
//+def
#define REV(...) __VA_OPT__(__REV1(__VA_ARGS__))
#define _REV(...) __VA_OPT__(__REV1(__VA_ARGS__))
//DEF __REV@1(_a,...) __VA_OPT__(__REV@2(__VA_ARGS__)) _a
#define __REV1(_a,...) __VA_OPT__(__REV2(__VA_ARGS__)) _a
#define __REV2(_a,...) __VA_OPT__(__REV3(__VA_ARGS__)) _a
#define __REV3(_a,...) __VA_OPT__(__REV4(__VA_ARGS__)) _a
#define __REV4(_a,...) __VA_OPT__(__REV5(__VA_ARGS__)) _a
#define __REV5(_a,...) __VA_OPT__(__REV6(__VA_ARGS__)) _a
#define __REV6(_a,...) __VA_OPT__(__REV7(__VA_ARGS__)) _a
#define __REV7(_a,...) __VA_OPT__(__REV8(__VA_ARGS__)) _a
#define __REV8(_a,...) __VA_OPT__(__REV9(__VA_ARGS__)) _a
#define __REV9(_a,...) __VA_OPT__(__REV10(__VA_ARGS__)) _a
#define __REV10(_a,...) __VA_OPT__(__REV11(__VA_ARGS__)) _a
#define __REV11(_a,...) __VA_OPT__(__REV12(__VA_ARGS__)) _a
#define __REV12(_a,...) __VA_OPT__(__REV13(__VA_ARGS__)) _a
#define __REV13(_a,...) __VA_OPT__(__REV14(__VA_ARGS__)) _a
#define __REV14(_a,...) __VA_OPT__(__REV15(__VA_ARGS__)) _a
#define __REV15(_a,...) __VA_OPT__(__REV16(__VA_ARGS__)) _a
#define __REV16(_a,...) __VA_OPT__(__REV17(__VA_ARGS__)) _a
#define __REV17(_a,...) __VA_OPT__(__REV18(__VA_ARGS__)) _a
#define __REV18(_a,...) __VA_OPT__(__REV19(__VA_ARGS__)) _a
#define __REV19(_a,...) __VA_OPT__(__REV20(__VA_ARGS__)) _a
#define __REV20(_a,...) __VA_OPT__(__REV21(__VA_ARGS__)) _a
#define __REV21(_a,...) __VA_OPT__(__REV22(__VA_ARGS__)) _a
#define __REV22(_a,...) __VA_OPT__(__REV23(__VA_ARGS__)) _a
#define __REV23(_a,...) __VA_OPT__(__REV24(__VA_ARGS__)) _a
#define __REV24(_a,...) __VA_OPT__(__REV25(__VA_ARGS__)) _a
#define __REV25(_a,...) __VA_OPT__(__REV26(__VA_ARGS__)) _a
#define __REV26(_a,...) __VA_OPT__(__REV27(__VA_ARGS__)) _a
#define __REV27(_a,...) __VA_OPT__(__REV28(__VA_ARGS__)) _a
#define __REV28(_a,...) __VA_OPT__(__REV29(__VA_ARGS__)) _a
#define __REV29(_a,...) __VA_OPT__(__REV30(__VA_ARGS__)) _a
#define __REV30(_a,...) __VA_OPT__(__REV31(__VA_ARGS__)) _a
#define __REV31(_a,...) __VA_OPT__(__REV32(__VA_ARGS__)) _a
#define __REV32(_a,...) __VA_OPT__(__REV33(__VA_ARGS__)) _a
#define __REV33(_a,...) __VA_OPT__(__REV34(__VA_ARGS__)) _a
#define __REV34(_a,...) __VA_OPT__(__REV35(__VA_ARGS__)) _a
#define __REV35(_a,...) __VA_OPT__(__REV36(__VA_ARGS__)) _a
#define __REV36(_a,...) __VA_OPT__(__REV37(__VA_ARGS__)) _a
#define __REV37(_a,...) __VA_OPT__(__REV38(__VA_ARGS__)) _a
#define __REV38(_a,...) __VA_OPT__(__REV39(__VA_ARGS__)) _a
#define __REV39(_a,...) __VA_OPT__(__REV40(__VA_ARGS__)) _a
#define __REV40(_a,...) __VA_OPT__(__REV41(__VA_ARGS__)) _a
#define __REV41(_a,...) __VA_OPT__(__REV42(__VA_ARGS__)) _a
#define __REV42(_a,...) __VA_OPT__(__REV43(__VA_ARGS__)) _a
#define __REV43(_a,...) __VA_OPT__(__REV44(__VA_ARGS__)) _a
#define __REV44(_a,...) __VA_OPT__(__REV45(__VA_ARGS__)) _a
#define __REV45(_a,...) __VA_OPT__(__REV46(__VA_ARGS__)) _a
#define __REV46(_a,...) __VA_OPT__(__REV47(__VA_ARGS__)) _a
#define __REV47(_a,...) __VA_OPT__(__REV48(__VA_ARGS__)) _a
#define __REV48(_a,...) __VA_OPT__(__REV49(__VA_ARGS__)) _a
#define __REV49(_a,...) __VA_OPT__(__REV50(__VA_ARGS__)) _a
#define __REV50(_a,...) __VA_OPT__(__REV51(__VA_ARGS__)) _a
#define __REV51(_a,...) __VA_OPT__(__REV52(__VA_ARGS__)) _a
#define __REV52(_a,...) __VA_OPT__(__REV53(__VA_ARGS__)) _a
#define __REV53(_a,...) __VA_OPT__(__REV54(__VA_ARGS__)) _a
#define __REV54(_a,...) __VA_OPT__(__REV55(__VA_ARGS__)) _a
#define __REV55(_a,...) __VA_OPT__(__REV56(__VA_ARGS__)) _a
#define __REV56(_a,...) __VA_OPT__(__REV57(__VA_ARGS__)) _a
#define __REV57(_a,...) __VA_OPT__(__REV58(__VA_ARGS__)) _a
#define __REV58(_a,...) __VA_OPT__(__REV59(__VA_ARGS__)) _a
#define __REV59(_a,...) __VA_OPT__(__REV60(__VA_ARGS__)) _a
#define __REV60(_a,...) __VA_OPT__(__REV61(__VA_ARGS__)) _a
#define __REV61(_a,...) __VA_OPT__(__REV62(__VA_ARGS__)) _a
#define __REV62(_a,...) __VA_OPT__(__REV63(__VA_ARGS__)) _a
#define __REV63(_a,...) __VA_OPT__(__REV64(__VA_ARGS__)) _a
#define __REV64(_a,...) __VA_OPT__(__REV65(__VA_ARGS__)) _a
#define __REV65(_a,...) __LOOP_ERROR



#define __PREPEND0(p,a,...)   , p a __VA_OPT__( ERROR: too many agruments at __LINE__ )
#define __PREPEND1(p,a,...)   , p a __VA_OPT__( __PREPEND0(p, __VA_ARGS__) )
#define __PREPEND2(p,a,...)   , p a __VA_OPT__( __PREPEND1(p, __VA_ARGS__) )
#define __PREPEND3(p,a,...)   , p a __VA_OPT__( __PREPEND2(p, __VA_ARGS__) )
#define __PREPEND4(p,a,...)   , p a __VA_OPT__( __PREPEND3(p, __VA_ARGS__) )
#define __PREPEND5(p,a,...)   , p a __VA_OPT__( __PREPEND4(p, __VA_ARGS__) )
#define __PREPEND6(p,a,...)   , p a __VA_OPT__( __PREPEND5(p, __VA_ARGS__) )
#define __PREPEND7(p,a,...)   , p a __VA_OPT__( __PREPEND6(p, __VA_ARGS__) )
#define __PREPEND8(p,a,...)   , p a __VA_OPT__( __PREPEND7(p, __VA_ARGS__) )
#define __PREPEND9(p,a,...)   , p a __VA_OPT__( __PREPEND8(p, __VA_ARGS__) )
#define __PREPEND10(p,a,...)   , p a __VA_OPT__( __PREPEND9(p, __VA_ARGS__) )
#define __PREPEND11(p,a,...)   , p a __VA_OPT__( __PREPEND10(p, __VA_ARGS__) )
#define __PREPEND12(p,a,...)   , p a __VA_OPT__( __PREPEND11(p, __VA_ARGS__) )
#define __PREPEND13(p,a,...)   , p a __VA_OPT__( __PREPEND12(p, __VA_ARGS__) )
#define __PREPEND14(p,a,...)   , p a __VA_OPT__( __PREPEND13(p, __VA_ARGS__) )
#define __PREPEND15(p,a,...)   , p a __VA_OPT__( __PREPEND14(p, __VA_ARGS__) )
#define __PREPEND16(p,a,...)   , p a __VA_OPT__( __PREPEND15(p, __VA_ARGS__) )
#define __PREPEND17(p,a,...)   , p a __VA_OPT__( __PREPEND16(p, __VA_ARGS__) )
#define __PREPEND18(p,a,...)   , p a __VA_OPT__( __PREPEND17(p, __VA_ARGS__) )
#define __PREPEND19(p,a,...)   , p a __VA_OPT__( __PREPEND18(p, __VA_ARGS__) )
#define __PREPEND20(p,a,...)   , p a __VA_OPT__( __PREPEND19(p, __VA_ARGS__) )
#define __PREPEND21(p,a,...)   , p a __VA_OPT__( __PREPEND20(p, __VA_ARGS__) )
#define __PREPEND22(p,a,...)   , p a __VA_OPT__( __PREPEND21(p, __VA_ARGS__) )
#define __PREPEND23(p,a,...)   , p a __VA_OPT__( __PREPEND22(p, __VA_ARGS__) )
#define __PREPEND24(p,a,...)   , p a __VA_OPT__( __PREPEND23(p, __VA_ARGS__) )
#define __PREPEND25(p,a,...)   , p a __VA_OPT__( __PREPEND24(p, __VA_ARGS__) )
#define __PREPEND26(p,a,...)   , p a __VA_OPT__( __PREPEND25(p, __VA_ARGS__) )
#define __PREPEND27(p,a,...)   , p a __VA_OPT__( __PREPEND26(p, __VA_ARGS__) )
#define __PREPEND28(p,a,...)   , p a __VA_OPT__( __PREPEND27(p, __VA_ARGS__) )
#define __PREPEND29(p,a,...)   , p a __VA_OPT__( __PREPEND28(p, __VA_ARGS__) )
#define __PREPEND30(p,a,...)   , p a __VA_OPT__( __PREPEND29(p, __VA_ARGS__) )
#define __PREPEND31(p,a,...)   , p a __VA_OPT__( __PREPEND30(p, __VA_ARGS__) )
#define __PREPEND32(p,a,...)   p a __VA_OPT__( __PREPEND31(p, __VA_ARGS__) )

//+doc combine all arguments to prepend a, prepend b, prepend c .. (with commata)
//+def PREPEND
#define PREPEND(_prepend,...)  __VA_OPT__( FOREACH_ARG_K(__PREPEND, _prepend,, __VA_ARGS__) )
#define __PREPEND(_prepend,_arg)  _prepend _arg

//#define PREPEND_K(_prepend,...)  __VA_OPT__( _FOREACH_ARG_K(__PREPEND_K, _prepend, __VA_ARGS__) )
//#define __PREPEND_K(_prepend,_arg)  _prepend _arg


//+doc prepend _prepend (without space), join with commata
#define PREP(_prepend,...) __VA_OPT__(__PREP1( _prepend,__VA_ARGS__))
#define _PREP(_prepend,...) __VA_OPT__(__PREP1( _prepend,__VA_ARGS__))

//#define __PREP@1(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP@2(__VA_ARGS__))
#define __PREP1(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP2(_prepend,__VA_ARGS__))
#define __PREP2(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP3(_prepend,__VA_ARGS__))
#define __PREP3(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP4(_prepend,__VA_ARGS__))
#define __PREP4(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP5(_prepend,__VA_ARGS__))
#define __PREP5(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP6(_prepend,__VA_ARGS__))
#define __PREP6(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP7(_prepend,__VA_ARGS__))
#define __PREP7(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP8(_prepend,__VA_ARGS__))
#define __PREP8(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP9(_prepend,__VA_ARGS__))
#define __PREP9(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP10(_prepend,__VA_ARGS__))
#define __PREP10(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP11(_prepend,__VA_ARGS__))
#define __PREP11(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP12(_prepend,__VA_ARGS__))
#define __PREP12(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP13(_prepend,__VA_ARGS__))
#define __PREP13(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP14(_prepend,__VA_ARGS__))
#define __PREP14(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP15(_prepend,__VA_ARGS__))
#define __PREP15(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP16(_prepend,__VA_ARGS__))
#define __PREP16(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP17(_prepend,__VA_ARGS__))
#define __PREP17(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP18(_prepend,__VA_ARGS__))
#define __PREP18(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP19(_prepend,__VA_ARGS__))
#define __PREP19(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP20(_prepend,__VA_ARGS__))
#define __PREP20(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP21(_prepend,__VA_ARGS__))
#define __PREP21(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP22(_prepend,__VA_ARGS__))
#define __PREP22(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP23(_prepend,__VA_ARGS__))
#define __PREP23(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP24(_prepend,__VA_ARGS__))
#define __PREP24(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP25(_prepend,__VA_ARGS__))
#define __PREP25(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP26(_prepend,__VA_ARGS__))
#define __PREP26(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP27(_prepend,__VA_ARGS__))
#define __PREP27(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP28(_prepend,__VA_ARGS__))
#define __PREP28(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP29(_prepend,__VA_ARGS__))
#define __PREP29(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP30(_prepend,__VA_ARGS__))
#define __PREP30(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP31(_prepend,__VA_ARGS__))
#define __PREP31(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP32(_prepend,__VA_ARGS__))
#define __PREP32(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP33(_prepend,__VA_ARGS__))
#define __PREP33(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP34(_prepend,__VA_ARGS__))
#define __PREP34(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP35(_prepend,__VA_ARGS__))
#define __PREP35(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP36(_prepend,__VA_ARGS__))
#define __PREP36(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP37(_prepend,__VA_ARGS__))
#define __PREP37(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP38(_prepend,__VA_ARGS__))
#define __PREP38(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP39(_prepend,__VA_ARGS__))
#define __PREP39(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP40(_prepend,__VA_ARGS__))
#define __PREP40(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP41(_prepend,__VA_ARGS__))
#define __PREP41(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP42(_prepend,__VA_ARGS__))
#define __PREP42(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP43(_prepend,__VA_ARGS__))
#define __PREP43(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP44(_prepend,__VA_ARGS__))
#define __PREP44(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP45(_prepend,__VA_ARGS__))
#define __PREP45(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP46(_prepend,__VA_ARGS__))
#define __PREP46(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP47(_prepend,__VA_ARGS__))
#define __PREP47(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP48(_prepend,__VA_ARGS__))
#define __PREP48(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP49(_prepend,__VA_ARGS__))
#define __PREP49(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP50(_prepend,__VA_ARGS__))
#define __PREP50(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP51(_prepend,__VA_ARGS__))
#define __PREP51(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP52(_prepend,__VA_ARGS__))
#define __PREP52(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP53(_prepend,__VA_ARGS__))
#define __PREP53(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP54(_prepend,__VA_ARGS__))
#define __PREP54(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP55(_prepend,__VA_ARGS__))
#define __PREP55(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP56(_prepend,__VA_ARGS__))
#define __PREP56(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP57(_prepend,__VA_ARGS__))
#define __PREP57(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP58(_prepend,__VA_ARGS__))
#define __PREP58(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP59(_prepend,__VA_ARGS__))
#define __PREP59(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP60(_prepend,__VA_ARGS__))
#define __PREP60(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP61(_prepend,__VA_ARGS__))
#define __PREP61(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP62(_prepend,__VA_ARGS__))
#define __PREP62(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP63(_prepend,__VA_ARGS__))
#define __PREP63(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP64(_prepend,__VA_ARGS__))
#define __PREP64(_prepend,_x,...) _prepend ## _x __VA_OPT__(,__PREP65(_prepend,__VA_ARGS__))


#if 0
#define __COUNT1(a,...) + 1 __VA_OPT__( __COUNT2(__VA_ARGS__ ) )
#define __COUNT2(a,...) + 1 __VA_OPT__( __COUNT3(__VA_ARGS__ ) )
#define __COUNT3(a,...) + 1 __VA_OPT__( __COUNT4(__VA_ARGS__ ) )
#define __COUNT4(a,...) + 1 __VA_OPT__( __COUNT5(__VA_ARGS__ ) )
#define __COUNT5(a,...) + 1 __VA_OPT__( __COUNT6(__VA_ARGS__ ) )
#define __COUNT6(a,...) + 1 __VA_OPT__( __COUNT7(__VA_ARGS__ ) )
#define __COUNT7(a,...) + 1 __VA_OPT__( __COUNT8(__VA_ARGS__ ) )
#define __COUNT8(a,...) + 1 __VA_OPT__( __COUNT9(__VA_ARGS__ ) )
#define __COUNT9(a,...) + 1 __VA_OPT__( __COUNT10(__VA_ARGS__ ) )
#define __COUNT10(a,...) + 1 __VA_OPT__( __COUNT11(__VA_ARGS__ ) )
#define __COUNT11(a,...) + 1 __VA_OPT__( __COUNT12(__VA_ARGS__ ) )
#define __COUNT12(a,...) + 1 __VA_OPT__( __COUNT13(__VA_ARGS__ ) )
#define __COUNT13(a,...) + 1 __VA_OPT__( __COUNT14(__VA_ARGS__ ) )
#define __COUNT14(a,...) + 1 __VA_OPT__( __COUNT15(__VA_ARGS__ ) )
#define __COUNT15(a,...) + 1 __VA_OPT__( __COUNT16(__VA_ARGS__ ) )
#define __COUNT16(a,...) + 1 __VA_OPT__( __COUNT17(__VA_ARGS__ ) )
#define __COUNT17(a,...) + 1 __VA_OPT__( __COUNT18(__VA_ARGS__ ) )
#define __COUNT18(a,...) + 1 __VA_OPT__( __COUNT19(__VA_ARGS__ ) )
#define __COUNT19(a,...) + 1 __VA_OPT__( __COUNT20(__VA_ARGS__ ) )
#define __COUNT20(a,...) + 1 __VA_OPT__( __COUNT21(__VA_ARGS__ ) )
#define __COUNT21(a,...) + 1 __VA_OPT__( __COUNT22(__VA_ARGS__ ) )
#define __COUNT22(a,...) + 1 __VA_OPT__( __COUNT23(__VA_ARGS__ ) )
#define __COUNT23(a,...) + 1 __VA_OPT__( __COUNT24(__VA_ARGS__ ) )
#define __COUNT24(a,...) + 1 __VA_OPT__( __COUNT25(__VA_ARGS__ ) )
#define __COUNT25(a,...) + 1 __VA_OPT__( __COUNT26(__VA_ARGS__ ) )
#define __COUNT26(a,...) + 1 __VA_OPT__( __COUNT27(__VA_ARGS__ ) )
#define __COUNT27(a,...) + 1 __VA_OPT__( __COUNT28(__VA_ARGS__ ) )
#define __COUNT28(a,...) + 1 __VA_OPT__( __COUNT29(__VA_ARGS__ ) )
#define __COUNT29(a,...) + 1 __VA_OPT__( __COUNT30(__VA_ARGS__ ) )
#define __COUNT30(a,...) + 1 __VA_OPT__( __COUNT31(__VA_ARGS__ ) )
#define __COUNT31(a,...) + 1 __VA_OPT__( __COUNT32(__VA_ARGS__ ) )
#define __COUNT32(a,...) + 1 __VA_OPT__( __LOOP_ERROR : COUNT )

//+doc count the number of arguments (max 32)
// COUNT(...) is replaced with ( 0+1+1... )
// 0 to 32 arguments allowed
//+def COUNT
#define COUNT(...) ( 0 __VA_OPT__( __COUNT1(__VA_ARGS__) ) )
#define _COUNT(...) ( 0 __VA_OPT__( __COUNT1(__VA_ARGS__) ) )

#else
// doesnt work that good. (elements like "str" don't work..)
//+doc count the number of arguments (max 64)
// 0 to 64 arguments allowed
//+def COUNT
#define COUNT(_a,...) __COUNT0(_a,__VA_ARGS__)  
#define _COUNT(_a,...) __COUNT0(_a,__VA_ARGS__) 
#define __COUNT0(_a,...) IF_OPT(_a,__COUNT1(__VA_ARGS__) ) IFNOT_OPT( _a, 0 )
//DEF COUNT@1(_a,...) IF_OPT(_a,__COUNT@2(__VA_ARGS__) ) IFNOT_OPT( _a, @1 )
#define __COUNT1(_a,...) IF_OPT(_a,__COUNT2(__VA_ARGS__) ) IFNOT_OPT( _a, 1 )
#define __COUNT2(_a,...) IF_OPT(_a,__COUNT3(__VA_ARGS__) ) IFNOT_OPT( _a, 2 )
#define __COUNT3(_a,...) IF_OPT(_a,__COUNT4(__VA_ARGS__) ) IFNOT_OPT( _a, 3 )
#define __COUNT4(_a,...) IF_OPT(_a,__COUNT5(__VA_ARGS__) ) IFNOT_OPT( _a, 4 )
#define __COUNT5(_a,...) IF_OPT(_a,__COUNT6(__VA_ARGS__) ) IFNOT_OPT( _a, 5 )
#define __COUNT6(_a,...) IF_OPT(_a,__COUNT7(__VA_ARGS__) ) IFNOT_OPT( _a, 6 )
#define __COUNT7(_a,...) IF_OPT(_a,__COUNT8(__VA_ARGS__) ) IFNOT_OPT( _a, 7 )
#define __COUNT8(_a,...) IF_OPT(_a,__COUNT9(__VA_ARGS__) ) IFNOT_OPT( _a, 8 )
#define __COUNT9(_a,...) IF_OPT(_a,__COUNT10(__VA_ARGS__) ) IFNOT_OPT( _a, 9 )
#define __COUNT10(_a,...) IF_OPT(_a,__COUNT11(__VA_ARGS__) ) IFNOT_OPT( _a, 10 )
#define __COUNT11(_a,...) IF_OPT(_a,__COUNT12(__VA_ARGS__) ) IFNOT_OPT( _a, 11 )
#define __COUNT12(_a,...) IF_OPT(_a,__COUNT13(__VA_ARGS__) ) IFNOT_OPT( _a, 12 )
#define __COUNT13(_a,...) IF_OPT(_a,__COUNT14(__VA_ARGS__) ) IFNOT_OPT( _a, 13 )
#define __COUNT14(_a,...) IF_OPT(_a,__COUNT15(__VA_ARGS__) ) IFNOT_OPT( _a, 14 )
#define __COUNT15(_a,...) IF_OPT(_a,__COUNT16(__VA_ARGS__) ) IFNOT_OPT( _a, 15 )
#define __COUNT16(_a,...) IF_OPT(_a,__COUNT17(__VA_ARGS__) ) IFNOT_OPT( _a, 16 )
#define __COUNT17(_a,...) IF_OPT(_a,__COUNT18(__VA_ARGS__) ) IFNOT_OPT( _a, 17 )
#define __COUNT18(_a,...) IF_OPT(_a,__COUNT19(__VA_ARGS__) ) IFNOT_OPT( _a, 18 )
#define __COUNT19(_a,...) IF_OPT(_a,__COUNT20(__VA_ARGS__) ) IFNOT_OPT( _a, 19 )
#define __COUNT20(_a,...) IF_OPT(_a,__COUNT21(__VA_ARGS__) ) IFNOT_OPT( _a, 20 )
#define __COUNT21(_a,...) IF_OPT(_a,__COUNT22(__VA_ARGS__) ) IFNOT_OPT( _a, 21 )
#define __COUNT22(_a,...) IF_OPT(_a,__COUNT23(__VA_ARGS__) ) IFNOT_OPT( _a, 22 )
#define __COUNT23(_a,...) IF_OPT(_a,__COUNT24(__VA_ARGS__) ) IFNOT_OPT( _a, 23 )
#define __COUNT24(_a,...) IF_OPT(_a,__COUNT25(__VA_ARGS__) ) IFNOT_OPT( _a, 24 )
#define __COUNT25(_a,...) IF_OPT(_a,__COUNT26(__VA_ARGS__) ) IFNOT_OPT( _a, 25 )
#define __COUNT26(_a,...) IF_OPT(_a,__COUNT27(__VA_ARGS__) ) IFNOT_OPT( _a, 26 )
#define __COUNT27(_a,...) IF_OPT(_a,__COUNT28(__VA_ARGS__) ) IFNOT_OPT( _a, 27 )
#define __COUNT28(_a,...) IF_OPT(_a,__COUNT29(__VA_ARGS__) ) IFNOT_OPT( _a, 28 )
#define __COUNT29(_a,...) IF_OPT(_a,__COUNT30(__VA_ARGS__) ) IFNOT_OPT( _a, 29 )
#define __COUNT30(_a,...) IF_OPT(_a,__COUNT31(__VA_ARGS__) ) IFNOT_OPT( _a, 30 )
#define __COUNT31(_a,...) IF_OPT(_a,__COUNT32(__VA_ARGS__) ) IFNOT_OPT( _a, 31 )
#define __COUNT32(_a,...) IF_OPT(_a,__COUNT33(__VA_ARGS__) ) IFNOT_OPT( _a, 32 )
#define __COUNT33(_a,...) IF_OPT(_a,__COUNT34(__VA_ARGS__) ) IFNOT_OPT( _a, 33 )
#define __COUNT34(_a,...) IF_OPT(_a,__COUNT35(__VA_ARGS__) ) IFNOT_OPT( _a, 34 )
#define __COUNT35(_a,...) IF_OPT(_a,__COUNT36(__VA_ARGS__) ) IFNOT_OPT( _a, 35 )
#define __COUNT36(_a,...) IF_OPT(_a,__COUNT37(__VA_ARGS__) ) IFNOT_OPT( _a, 36 )
#define __COUNT37(_a,...) IF_OPT(_a,__COUNT38(__VA_ARGS__) ) IFNOT_OPT( _a, 37 )
#define __COUNT38(_a,...) IF_OPT(_a,__COUNT39(__VA_ARGS__) ) IFNOT_OPT( _a, 38 )
#define __COUNT39(_a,...) IF_OPT(_a,__COUNT40(__VA_ARGS__) ) IFNOT_OPT( _a, 39 )
#define __COUNT40(_a,...) IF_OPT(_a,__COUNT41(__VA_ARGS__) ) IFNOT_OPT( _a, 40 )
#define __COUNT41(_a,...) IF_OPT(_a,__COUNT42(__VA_ARGS__) ) IFNOT_OPT( _a, 41 )
#define __COUNT42(_a,...) IF_OPT(_a,__COUNT43(__VA_ARGS__) ) IFNOT_OPT( _a, 42 )
#define __COUNT43(_a,...) IF_OPT(_a,__COUNT44(__VA_ARGS__) ) IFNOT_OPT( _a, 43 )
#define __COUNT44(_a,...) IF_OPT(_a,__COUNT45(__VA_ARGS__) ) IFNOT_OPT( _a, 44 )
#define __COUNT45(_a,...) IF_OPT(_a,__COUNT46(__VA_ARGS__) ) IFNOT_OPT( _a, 45 )
#define __COUNT46(_a,...) IF_OPT(_a,__COUNT47(__VA_ARGS__) ) IFNOT_OPT( _a, 46 )
#define __COUNT47(_a,...) IF_OPT(_a,__COUNT48(__VA_ARGS__) ) IFNOT_OPT( _a, 47 )
#define __COUNT48(_a,...) IF_OPT(_a,__COUNT49(__VA_ARGS__) ) IFNOT_OPT( _a, 48 )
#define __COUNT49(_a,...) IF_OPT(_a,__COUNT50(__VA_ARGS__) ) IFNOT_OPT( _a, 49 )
#define __COUNT50(_a,...) IF_OPT(_a,__COUNT51(__VA_ARGS__) ) IFNOT_OPT( _a, 50 )
#define __COUNT51(_a,...) IF_OPT(_a,__COUNT52(__VA_ARGS__) ) IFNOT_OPT( _a, 51 )
#define __COUNT52(_a,...) IF_OPT(_a,__COUNT53(__VA_ARGS__) ) IFNOT_OPT( _a, 52 )
#define __COUNT53(_a,...) IF_OPT(_a,__COUNT54(__VA_ARGS__) ) IFNOT_OPT( _a, 53 )
#define __COUNT54(_a,...) IF_OPT(_a,__COUNT55(__VA_ARGS__) ) IFNOT_OPT( _a, 54 )
#define __COUNT55(_a,...) IF_OPT(_a,__COUNT56(__VA_ARGS__) ) IFNOT_OPT( _a, 55 )
#define __COUNT56(_a,...) IF_OPT(_a,__COUNT57(__VA_ARGS__) ) IFNOT_OPT( _a, 56 )
#define __COUNT57(_a,...) IF_OPT(_a,__COUNT58(__VA_ARGS__) ) IFNOT_OPT( _a, 57 )
#define __COUNT58(_a,...) IF_OPT(_a,__COUNT59(__VA_ARGS__) ) IFNOT_OPT( _a, 58 )
#define __COUNT59(_a,...) IF_OPT(_a,__COUNT60(__VA_ARGS__) ) IFNOT_OPT( _a, 59 )
#define __COUNT60(_a,...) IF_OPT(_a,__COUNT61(__VA_ARGS__) ) IFNOT_OPT( _a, 60 )
#define __COUNT61(_a,...) IF_OPT(_a,__COUNT62(__VA_ARGS__) ) IFNOT_OPT( _a, 61 )
#define __COUNT62(_a,...) IF_OPT(_a,__COUNT63(__VA_ARGS__) ) IFNOT_OPT( _a, 62 )
#define __COUNT63(_a,...) IF_OPT(_a,__COUNT64(__VA_ARGS__) ) IFNOT_OPT( _a, 63 )
#define __COUNT64(_a,...) IF_OPT(_a,__COUNT65(__VA_ARGS__) ) IFNOT_OPT( _a, 64 )
#define __COUNT65(_a,...) __LOOP_ERROR : COUNT
#endif

#ifndef _RANDSEQ_  // should be filled by make
//#warning _RANDSEQ_ is not defined. Using predefined order
#define _RANDSEQ_ 5,10,28,3,4,25,17,6,20,2,13,22,21,26,12,11,23,24,29,27,19,31,18,8,9,16,30,15,32,1,7
//#define _RANDSEQ_ 5,9,10,1,11,17,3
#endif

//+doc Randomize order of the array, append a semikolon
// max elements is 64
//+def
#define RANDOMIZE(...) FOREACH_CA(_RANDOMIZE,CAT(__VA_ARGS__),,_RANDSEQ_)
//+doc callen by RANDOMIZE foreach element.
// Could be overwritten
//+def
#define _RANDOMIZE(n,args,r) SK(FETCH(r,args))


//+doc append a semikolon, if _opt is not empty
//+def
#define SK(_opt) __SK(_opt)
#define _SK(_opt) __SK(_opt)
#define __SK(...) __VA_OPT__(__VA_ARGS__;)

//#define _RANDOMIZE(n,args,r) IF_OPT(FETCH(r,args),FETCH(r,args);)


//+doc Fetch element number _x from an array
#define FETCH(_x,...) __FETCH##_x(__VA_ARGS__)
#define _FETCH(_x,...) __FETCH##_x(__VA_ARGS__)

#define __FETCH1(_a,...) _a
//DEF __FETCH@2(_a,...) __VA_OPT__(__FETCH@1(__VA_ARGS__))
#define __FETCH2(_a,...) __VA_OPT__(__FETCH1(__VA_ARGS__))
#define __FETCH3(_a,...) __VA_OPT__(__FETCH2(__VA_ARGS__))
#define __FETCH4(_a,...) __VA_OPT__(__FETCH3(__VA_ARGS__))
#define __FETCH5(_a,...) __VA_OPT__(__FETCH4(__VA_ARGS__))
#define __FETCH6(_a,...) __VA_OPT__(__FETCH5(__VA_ARGS__))
#define __FETCH7(_a,...) __VA_OPT__(__FETCH6(__VA_ARGS__))
#define __FETCH8(_a,...) __VA_OPT__(__FETCH7(__VA_ARGS__))
#define __FETCH9(_a,...) __VA_OPT__(__FETCH8(__VA_ARGS__))
#define __FETCH10(_a,...) __VA_OPT__(__FETCH9(__VA_ARGS__))
#define __FETCH11(_a,...) __VA_OPT__(__FETCH10(__VA_ARGS__))
#define __FETCH12(_a,...) __VA_OPT__(__FETCH11(__VA_ARGS__))
#define __FETCH13(_a,...) __VA_OPT__(__FETCH12(__VA_ARGS__))
#define __FETCH14(_a,...) __VA_OPT__(__FETCH13(__VA_ARGS__))
#define __FETCH15(_a,...) __VA_OPT__(__FETCH14(__VA_ARGS__))
#define __FETCH16(_a,...) __VA_OPT__(__FETCH15(__VA_ARGS__))
#define __FETCH17(_a,...) __VA_OPT__(__FETCH16(__VA_ARGS__))
#define __FETCH18(_a,...) __VA_OPT__(__FETCH17(__VA_ARGS__))
#define __FETCH19(_a,...) __VA_OPT__(__FETCH18(__VA_ARGS__))
#define __FETCH20(_a,...) __VA_OPT__(__FETCH19(__VA_ARGS__))
#define __FETCH21(_a,...) __VA_OPT__(__FETCH20(__VA_ARGS__))
#define __FETCH22(_a,...) __VA_OPT__(__FETCH21(__VA_ARGS__))
#define __FETCH23(_a,...) __VA_OPT__(__FETCH22(__VA_ARGS__))
#define __FETCH24(_a,...) __VA_OPT__(__FETCH23(__VA_ARGS__))
#define __FETCH25(_a,...) __VA_OPT__(__FETCH24(__VA_ARGS__))
#define __FETCH26(_a,...) __VA_OPT__(__FETCH25(__VA_ARGS__))
#define __FETCH27(_a,...) __VA_OPT__(__FETCH26(__VA_ARGS__))
#define __FETCH28(_a,...) __VA_OPT__(__FETCH27(__VA_ARGS__))
#define __FETCH29(_a,...) __VA_OPT__(__FETCH28(__VA_ARGS__))
#define __FETCH30(_a,...) __VA_OPT__(__FETCH29(__VA_ARGS__))
#define __FETCH31(_a,...) __VA_OPT__(__FETCH30(__VA_ARGS__))
#define __FETCH32(_a,...) __VA_OPT__(__FETCH31(__VA_ARGS__))
#define __FETCH33(_a,...) __VA_OPT__(__FETCH32(__VA_ARGS__))
#define __FETCH34(_a,...) __VA_OPT__(__FETCH33(__VA_ARGS__))
#define __FETCH35(_a,...) __VA_OPT__(__FETCH34(__VA_ARGS__))
#define __FETCH36(_a,...) __VA_OPT__(__FETCH35(__VA_ARGS__))
#define __FETCH37(_a,...) __VA_OPT__(__FETCH36(__VA_ARGS__))
#define __FETCH38(_a,...) __VA_OPT__(__FETCH37(__VA_ARGS__))
#define __FETCH39(_a,...) __VA_OPT__(__FETCH38(__VA_ARGS__))
#define __FETCH40(_a,...) __VA_OPT__(__FETCH39(__VA_ARGS__))
#define __FETCH41(_a,...) __VA_OPT__(__FETCH40(__VA_ARGS__))
#define __FETCH42(_a,...) __VA_OPT__(__FETCH41(__VA_ARGS__))
#define __FETCH43(_a,...) __VA_OPT__(__FETCH42(__VA_ARGS__))
#define __FETCH44(_a,...) __VA_OPT__(__FETCH43(__VA_ARGS__))
#define __FETCH45(_a,...) __VA_OPT__(__FETCH44(__VA_ARGS__))
#define __FETCH46(_a,...) __VA_OPT__(__FETCH45(__VA_ARGS__))
#define __FETCH47(_a,...) __VA_OPT__(__FETCH46(__VA_ARGS__))
#define __FETCH48(_a,...) __VA_OPT__(__FETCH47(__VA_ARGS__))
#define __FETCH49(_a,...) __VA_OPT__(__FETCH48(__VA_ARGS__))
#define __FETCH50(_a,...) __VA_OPT__(__FETCH49(__VA_ARGS__))
#define __FETCH51(_a,...) __VA_OPT__(__FETCH50(__VA_ARGS__))
#define __FETCH52(_a,...) __VA_OPT__(__FETCH51(__VA_ARGS__))
#define __FETCH53(_a,...) __VA_OPT__(__FETCH52(__VA_ARGS__))
#define __FETCH54(_a,...) __VA_OPT__(__FETCH53(__VA_ARGS__))
#define __FETCH55(_a,...) __VA_OPT__(__FETCH54(__VA_ARGS__))
#define __FETCH56(_a,...) __VA_OPT__(__FETCH55(__VA_ARGS__))
#define __FETCH57(_a,...) __VA_OPT__(__FETCH56(__VA_ARGS__))
#define __FETCH58(_a,...) __VA_OPT__(__FETCH57(__VA_ARGS__))
#define __FETCH59(_a,...) __VA_OPT__(__FETCH58(__VA_ARGS__))
#define __FETCH60(_a,...) __VA_OPT__(__FETCH59(__VA_ARGS__))
#define __FETCH61(_a,...) __VA_OPT__(__FETCH60(__VA_ARGS__))
#define __FETCH62(_a,...) __VA_OPT__(__FETCH61(__VA_ARGS__))
#define __FETCH63(_a,...) __VA_OPT__(__FETCH62(__VA_ARGS__))
#define __FETCH64(_a,...) __VA_OPT__(__FETCH63(__VA_ARGS__))
#define __FETCH65(_a,...) __VA_OPT__(__FETCH64(__VA_ARGS__))



#define __DEL0(...) __VA_ARGS__
#define __DEL1(a,...) __VA_ARGS__
//DEF __DEL@2(a,...) __DEL@1(__VA_ARGS__)
#define __DEL2(a,...) __DEL1(__VA_ARGS__)
#define __DEL3(a,...) __DEL2(__VA_ARGS__)
#define __DEL4(a,...) __DEL3(__VA_ARGS__)
#define __DEL5(a,...) __DEL4(__VA_ARGS__)
#define __DEL6(a,...) __DEL5(__VA_ARGS__)
#define __DEL7(a,...) __DEL6(__VA_ARGS__)
#define __DEL8(a,...) __DEL7(__VA_ARGS__)
#define __DEL9(a,...) __DEL8(__VA_ARGS__)
#define __DEL10(a,...) __DEL9(__VA_ARGS__)
#define __DEL11(a,...) __DEL10(__VA_ARGS__)
#define __DEL12(a,...) __DEL11(__VA_ARGS__)
#define __DEL13(a,...) __DEL12(__VA_ARGS__)
#define __DEL14(a,...) __DEL13(__VA_ARGS__)
#define __DEL15(a,...) __DEL14(__VA_ARGS__)
#define __DEL16(a,...) __DEL15(__VA_ARGS__)
#define __DEL17(a,...) __DEL16(__VA_ARGS__)
#define __DEL18(a,...) __DEL17(__VA_ARGS__)
#define __DEL19(a,...) __DEL18(__VA_ARGS__)
#define __DEL20(a,...) __DEL19(__VA_ARGS__)
#define __DEL21(a,...) __DEL20(__VA_ARGS__)
#define __DEL22(a,...) __DEL21(__VA_ARGS__)
#define __DEL23(a,...) __DEL22(__VA_ARGS__)
#define __DEL24(a,...) __DEL23(__VA_ARGS__)
#define __DEL25(a,...) __DEL24(__VA_ARGS__)
#define __DEL26(a,...) __DEL25(__VA_ARGS__)
#define __DEL27(a,...) __DEL26(__VA_ARGS__)
#define __DEL28(a,...) __DEL27(__VA_ARGS__)
#define __DEL29(a,...) __DEL28(__VA_ARGS__)
#define __DEL30(a,...) __DEL29(__VA_ARGS__)
#define __DEL31(a,...) __DEL30(__VA_ARGS__)
#define __DEL32(a,...) __DEL31(__VA_ARGS__)
#define __DEL33(a,...) __DEL32(__VA_ARGS__)

//+doc delete num elements (from the start)
#define DEL(num,...) __DEL##num(__VA_ARGS__)
#define _DEL(num,...) __DEL##num(__VA_ARGS__)


//#define _OPTARG(a,b) ( ((b+0) ? -INT_MAX : a) == -INT_MAX ? b : a )
//#define _SHIFT(a,...) a

//#define _POP1(a,...) ((__VA_OPT__(__VA_ARGS__)+0) ? :a-333)
//#define _POP(a,...) ((__VA_OPT__(_POP1(__VA_ARGS__))+333) ? :a)
//#define POP(...) _POP(-333,__VA_ARGS__)

//POP(1,2,3);

//+doc optional arguments
#define OPTARG(default,...) _OPTARG(default,_SHIFT(__VA_ARGS__))

//#define mo(a,...) fmo(a,OPTARG(13,__VA_ARGS__))
//mo(1)

//#define A(...) __VA_ARGS__
//CAT(DEL(1,a,b,c));

// without meddle macro FF, a,b,c get's to some sort of schroedingers argument.
// Supplied once to fz, but expanding to several arguments.
//#define fz(a) Macro: a
//#define FF(...) FOREACH( fz,+,__VA_ARGS__);
//FF(DEL(1,a,b,c))

//COUNT(a)
//COUNT(1,2,3,4,5,6,7,8,9,10)
//COUNT(a,b)

//PREPEND( (const void**), a, b,c);

//JOIN(|, CAT(a,b,c) );
#if 0

#define _V(x,y) x - y
#define _V2(x,y) 2+x - y
FOREACH_PAIR( _V, +, 1,2,3,4 )
FOREACH_PAIR( _V, +, 5,6,7,8 )

#define PAIR( ... ) FOREACH_PAIR(_V,+,__VA_ARGS__)  ,    FOREACH_PAIR(_V2,+,__VA_ARGS__) 

//	PAIR(1,2)

#define bARG0(F,...) __VA_OPT__( F(__VA_ARGS__) )
#define bARG1(F,x,...)  bARG0(F,x)
#define bARG2(F,x,...)  __VA_OPT__( bARG1(F,__VA_ARGS__) )
#define bARG3(F,x,...)  __VA_OPT__( bARG2(F,__VA_ARGS__) )

#endif
#if 0

#define __DECLARE_OPT( opt ) opt,
#define DECLARE_OPTIONS( tool, ... ) enum tool##_OPTIONS_enum { FOREACH( __DECLARE_OPT,,__VA_ARGS__ ) tool##optsend=255 };\
	static const char* tool##_OPTIONS_short = CONCAT( a, __VA_ARGS__ )

DECLARE_OPTIONS(uniq,w,f,s,i,d,c,u,h  ,r,g,n,G );


#define ARG1(a,...) a
#define O1(a,...) __VA__OPT__( param 1 : ARG1(__VA_ARGS__) )


#define OOO(...) FOREACH(
#endif
#if 0
#define DECLARE_OPTIONS( tool, ... ) enum tool##_OPTIONS_enum { __VA_ARGS__ };\
	static const char* tool##_OPTIONS_short = CONCAT( __VA_ARGS__ );\
	static inline int __tool##_getopt( enum tool##_OPTIONS_enum o){ return(o);}


DECLARE_OPTIONS(uniq,w,f,s,i,d,c,u,h  ,r,g,n,G );

DECLARE_INT_OPTIONS( uniq, a,i,write ) -> enum toolname_opts_int{ ...


#define __OPT_SHORT( x, _shortopt ) _shortopt
#define __OPT_LONG( longopt, shortopt ) { longopt, shortopt },

typedef { 
	const char* longopt;
	const enum toolname_OPTIONS_enum shortopt;
} options_long;

#define DECLARE_OPTIONS_LONG( tool, longopt, shortopt, ... ) \
	static const options_long[] = { FOREACH_PAIR( __OPT_LONG,, longopt, shortopt, __VA_ARGS__ ) {0,0} };

	//const char* tool##_options_long = JOIN( "\255", longopt, shortopt, __VA_ARGS__ ) 


DECLARE_OPTIONS_LONG( toolname, "write", w, "read", r );
#endif


//JOIN( +, 3, 4 );


//#define __IMPORT(f) (const void**)&f ,
//#define KOMMA() ,

//	const void **import[] = { FOREACH( __IMPORT, ,exported_f1, exported_f2 ) 0 };

	//asm( PREPENDS( "\n push %",rdi,rsi ) );
	
#endif

#if (_RAND_ & 1)
#define RANDOMIZE1(a,b) a;b;
#else 
#define RANDOMIZE1(a,b) b;a;
#endif

#if (_RAND_ & 2)
#define RANDOMIZE2(a,b) a;b; 
#else 
#define RANDOMIZE2(a,b) b;a;
#endif


#if (_RAND_ & 4)
#define _RANDOMIZE1(a,b) a b
#else 
#define _RANDOMIZE1(a,b) b a
#endif

#if (_RAND_ & 8)
#define _RANDOMIZE2(a,b) a b
#else 
#define _RANDOMIZE2(a,b) b a
#endif


#if (_RAND_ & 16)
#if (_RAND_ & 32)
#define RANDOMIZE4(a,b,c,d) _RANDOMIZE1( RANDOMIZE1(a,b), RANDOMIZE2(c,d) )
#else //8
#define RANDOMIZE4(a,b,c,d) _RANDOMIZE2( RANDOMIZE1(a,c), RANDOMIZE2(b,d) )
#endif //8
#else 
#define RANDOMIZE4(a,b,c,d) _RANDOMIZE1( RANDOMIZE1(a,d), RANDOMIZE2(b,c) )
#endif


#if (_RAND_ & 64)
#if (_RAND_ & 128)
#define RANDOMIZE4b(a,b,c,d) _RANDOMIZE1( RANDOMIZE1(a,b), RANDOMIZE2(c,d) )
#else //8
#define RANDOMIZE4b(a,b,c,d) _RANDOMIZE2( RANDOMIZE1(a,c), RANDOMIZE2(b,d) )
#endif //8
#else 
#define RANDOMIZE4b(a,b,c,d) _RANDOMIZE1( RANDOMIZE1(a,d), RANDOMIZE2(b,c) )
#endif



// not perfect. but mixing with 2^10 - something. - so. might be ugly to crack
// I'm tedious of typing. 
#if (_RAND_ & 256)
#if (_RAND_ & 512)
#define RANDOMIZE8(a,b,c,d,e,f,g,h) _RANDOMIZE1( RANDOMIZE4(a,b,c,d), RANDOMIZE4b(e,f,g,h) )
#else
#define RANDOMIZE8(a,b,c,d,e,f,g,h) _RANDOMIZE1( RANDOMIZE4(a,b,g,h), RANDOMIZE4b(c,d,e,f) )
#endif //512
#else //256
#if (_RAND_ & 512)
#define RANDOMIZE8(a,b,c,d,e,f,g,h) _RANDOMIZE1( RANDOMIZE4(a,c,d,e), RANDOMIZE4b(b,f,g,h) )
#else
#define RANDOMIZE8(a,b,c,d,e,f,g,h) _RANDOMIZE1( RANDOMIZE4(a,d,g,f), RANDOMIZE4b(b,c,e,h) )
#endif //512
#endif

#if (_RAND_ & 1024)
#if (_RAND_ & 2048)
#define RANDOMIZE8b(a,b,c,d,e,f,g,h) _RANDOMIZE1( RANDOMIZE4(a,b,c,d), RANDOMIZE4b(e,f,g,h) )
#else
#define RANDOMIZE8b(a,b,c,d,e,f,g,h) _RANDOMIZE1( RANDOMIZE4(a,b,g,h), RANDOMIZE4b(c,d,e,f) )
#endif //2048
#else 
#if (_RAND_ & 2048)
#define RANDOMIZE8b(a,b,c,d,e,f,g,h) _RANDOMIZE1( RANDOMIZE4(a,c,d,e), RANDOMIZE4b(b,f,g,h) )
#else
#define RANDOMIZE8b(a,b,c,d,e,f,g,h) _RANDOMIZE1( RANDOMIZE4(a,d,g,f), RANDOMIZE4b(b,c,e,h) )
#endif //2048
#endif


// Could mix more. 
#if (_RAND_&4096)
#if (_RAND_&8192)
#define RANDOMIZE16(a,b,c,d,e,f,g,h,...) RANDOMIZE8(a,b,c,d,e,f,g,h) RANDOMIZE8b(__VA_ARGS__)
#else
#define RANDOMIZE16(a,b,c,d,e,f,g,h,...) RANDOMIZE8(__VA_ARGS__) RANDOMIZE8b(a,b,c,d,e,f,g,h) 
#endif //8192
#else //4096
#if (_RAND_&8192)
#define RANDOMIZE16(a,b,c,d,e,f,g,h,i,j,k,l,...) RANDOMIZE8(e,f,g,h,i,j,k,l) RANDOMIZE8b(a,b,c,d,__VA_ARGS__)
#else
#define RANDOMIZE16(a,b,c,d,e,f,g,h,i,j,k,l,...) RANDOMIZE8(e,f,g,h,__VA_ARGS__) RANDOMIZE8b(a,b,c,d,i,j,k,l) 
#endif //8192
#endif

//+doc RANDOMIZE
// Randomize the order of the arguments,
// an ; is appended to each argument.
// RANDOMIZE2, RANDOMIZE4, RANDOMIZE4b, RANDOMIZE8, RANDOMIZE8b, RANDOMIZE16
//+def RANDOMIZE
//RANDOMIZE8(1,2,3,4,5,6,7,8)

//RANDOMIZE16(0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f)


#if (_RAND_&1)
#define _Ra1(x) x
#define _Rb1(x) 
#else
#define _Ra1(x) 
#define _Rb1(x) x
#endif


#if (_RAND_&2)
#define _Ra2(x) x
#define _Rb2(x) 
#else
#define _Ra2(x) 
#define _Rb2(x) x
#endif

#if (_RAND_&4)
#define _Ra4(x) x
#define _Rb4(x) 
#else
#define _Ra4(x) 
#define _Rb4(x) x
#endif


#if (_RAND_&8)
#define _Ra8(x) x
#define _Rb8(x) 
#else
#define _Ra8(x) 
#define _Rb8(x) x
#endif


#define __Ra1(x) _Ra2(_Ra1(x))
#define __Rb1(x) _Ra2(_Rb1(x))
#define __Rc1(x) _Rb2(_Rb1(x))
#define __Rd1(x) _Rb2(_Ra1(x))


#define __Ra2(x) _Ra4(_Ra8(x))
#define __Rb2(x) _Ra4(_Rb8(x))
#define __Rc2(x) _Rb4(_Rb8(x))
#define __Rd2(x) _Rb4(_Ra8(x))


#define __Ra3(x) _Ra4(_Ra2(x))
#define __Rb3(x) _Ra4(_Rb2(x))
#define __Rc3(x) _Rb4(_Rb2(x))
#define __Rd3(x) _Rb4(_Ra2(x))

#define __Ra4(x) _Ra4(_Ra1(x))
#define __Rb4(x) _Ra4(_Rb1(x))
#define __Rc4(x) _Rb4(_Rb1(x))
#define __Rd4(x) _Rb4(_Ra1(x))


#define __Ra5(x) _Ra8(_Ra2(x))
#define __Rb5(x) _Ra8(_Rb2(x))
#define __Rc5(x) _Rb8(_Rb2(x))
#define __Rd5(x) _Rb8(_Ra2(x))

#define __Ra6(x) _Ra8(_Ra1(x))
#define __Rb6(x) _Ra8(_Rb1(x))
#define __Rc6(x) _Rb8(_Rb1(x))
#define __Rd6(x) _Rb8(_Ra1(x))

//+doc R4(x,y,xxx)
// Only one of a,b,c or d (y) for each number (x) xxx will be emitted
// number can be between 1 and 6
//+def R4
#define R4(x,y,a) __R##y##x(a)

/*
R4(2,a,13a)
R4(2,b,13b)
R4(2,c,13c)
R4(2,d,13d)
*/


#if (_RAND_&(1<<16))
#define _rra1(x) x;
#define _rrb1(x) 
#else
#define _rra1(x) 
#define _rrb1(x) x;
#endif


#if (_RAND_&(1<<17))
#define _rra2(x) x;
#define _rrb2(x) 
#else
#define _rra2(x) 
#define _rrb2(x) x;
#endif



#if (_RAND_&(1<<18))
#define _rra3(x) x;
#define _rrb3(x) 
#else
#define _rra3(x) 
#define _rrb3(x) x;
#endif



#if (_RAND_&(1<<19))
#define _rra4(x) x;
#define _rrb4(x) 
#else
#define _rra4(x) 
#define _rrb4(x) x;
#endif



#if (_RAND_&(1<<20))
#define _rra5(x) x;
#define _rrb5(x) 
#else
#define _rra5(x) 
#define _rrb5(x) x;
#endif



#if (_RAND_&(1<<21))
#define _rra6(x) x;
#define _rrb6(x) 
#else
#define _rra6(x) 
#define _rrb6(x) x;
#endif



#if (_RAND_&(1<<22))
#define _rra7(x) x;
#define _rrb7(x) 
#else
#define _rra7(x) 
#define _rrb7(x) x;
#endif

#if (_RAND_&(1<<23))
#define _rra8(x) x;
#define _rrb8(x) 
#else
#define _rra8(x) 
#define _rrb8(x) x;
#endif

//+doc R2(x,y,a)
// Only one time of R2(1,a,xxx) and R2(1,b,xxx) 
// xxx wil be pasted
// number can be 1 to 8
//+def R2
#define R2(x,y,a) _rr##y##x(a)


//R2(1,a,RANDOMIZE2(a1,a2))
//R2(1,b,RANDOMIZE2(a1,a2))

#if 0

#define OPTS v,h,n

#define O(x) ({ enum { OPTS }; x })
O(n);

#define STR JOINS("",OPTS)

STR

#endif
//#define M(x) (ulong) IF_OPT(x,x) IFNOT_OPT(x,0)
//FOREACH_K(M,1,2,,3,4);
//(ulong)1 ,(ulong)2 ,(ulong)0, (ulong)3 ,(ulong)4 ;



