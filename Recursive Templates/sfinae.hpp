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

template<bool is_present, typename T, typename U>
using conditional_t = typename conditional<is_present, T, U>::type;

template<bool, typename T = void>
struct enable_if {};
 
template<class T>
struct enable_if<true, T> { 
    using type = T; 
};

template<bool is_present, typename T = void>
using enable_if_t = typename enable_if<is_present, T>::type;

template<class T, class U>
struct is_same {
    static constexpr bool value = false;
};
 
template<class T>
struct is_same<T, T> {
    static constexpr bool value = true;
};

#define MAKE_SFINAE(name, expression)                                               \
template<typename T>                                                                \
struct name {                                                                       \
private:                                                                            \
    static void detect(...);                                                        \
                                                                                    \
    template<typename U>                                                            \
    static enable_if_t<(expression), U> detect(const U&);                           \
                                                                                    \
public:                                                                             \
    static constexpr bool ok = is_same<void, decltype(detect(T()))>::value;         \
};                      

#endif