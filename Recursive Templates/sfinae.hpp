#ifndef _SFINAE_H_
#define _SFINAE_H_

template <bool, typename T, typename U>
struct conditional;

template <typename T, typename U>
struct conditional<false, T, U> { 
    using type = U;
};

template <typename T, typename U>
struct conditional<true, T, U> { 
    using type = T; 
};


#endif