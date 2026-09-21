/*
+doc macro implementation of shellsort.
 This spares a lot of overhead, on the cost of having
 the implementation with every sort. (if needed several times)
 usage:
 int array[1024]; // to sort

 SHELLSORT(array,1024);
 
 optionally, cmp and swap can be supplied:

 SHELLSORT(array,1024, *a>*b );

 SHELLSORT(array,1024, , {*a^=*b;*b^=*a;*a^=*b;} );

 SHELLSORT(array,1024, *a<*b , {*a^=*b;*b^=*a;*a^=*b;} );


 !! strcmp et al need to return true or false(!)

 SHELLSORT(array,1024, strcmp(*a,*b) >=0  );


 Arguments: 
 	array of elements, (pointer)
 	number of elements,
 optional: 
 	compare a <=> b, 
 	swap a <=> b
*/
//+def SHELLSORT
# define SHELLSORT(_base,_count,...) { \
	int _gap, _i, _j; \
	for (_gap = 0; ++_gap < _count;) \
		_gap <<= 2; \
	while ((_gap >>= 2)) { \
		for (_i = _gap; _i < _count; _i++ ) { \
			for (_j = _i - _gap; ;_j -= _gap) { \
				typeof(*_base) *a = _j + _base; \
				typeof(*_base) *b = a + _gap; \
				if ( _VA_DEFAULT(1,*a<=*b,__VA_ARGS__) ) \
					break; \
				_VA_DEFAULT(2,typeof(*_base) _tmp=*a; *a=*b; *b=_tmp;,__VA_ARGS__); \
				if (_j < _gap) \
					break; \
	}	}	}	}

// (misc)
// changed the division by 3 to a bitshift
// uses typeof now.
// made sort and swap routines optional.
