#include "util_func.hpp"

// #ifndef _GLIBCXX_IOSTREAM
//     #include<iostream>
// #endif
// #ifndef _GLIBCXX_ALGORITHM
//     #include<algorithm>
// #endif

//LEARNT+QUESTIONS BELOW//
//ERROR - [in getTerminalDimen()] Replace intTuple with simpleTuple<int,int> gives error : conversion from ‘simpleTuple<int, int>’ to non-scalar type ‘intTuple {aka homoTuple<int>}’ requested
/*LEARNT - ASCII value of integers 0-9 are actually 48-57*/
    /*Learnt - In case of only 1 argument passed to std::string::erase()...
                1. If we provide an index number, ALL including and after it are erased
                2. If we provide an iterator,    ONLY that position is removed*/

    //LEARNT - algorithm -> find_if - Keep finding, until the predicate is true
    //LEARNT - Using .base() on a reverse_iterator returns us the normal_iterator, BUT CAUTION- This isn't advisable, and might cause SEGFAULT, (for eg. accessing .rbegin().base())

/*LEARNT - stringify() can turn any name into text, BUT remember, it ignores trailing and front spaces, for eg. in stringify( Hello World ), first and last space ignored*(Verified)/
*/

/*LEARNT - The component type of STL containers like vectors must be 'asignable'.
			References are NOT assignable, ie. we can only initialise them once, and,
			you can't make them reference something else later (you can't assign it to reference any other vairable, and even after equating 2 references, only the values of variable they reference is changed, not the addresses they both refer to)
			Due to same reason, other non-assignable types are also not allowed as components of containers, for eg. vector<const int> not allowed
*/