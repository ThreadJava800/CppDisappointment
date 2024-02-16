#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

template<typename... Other>
int add(Other... args);

template<typename T, typename... Other>
int add(T cur_val, Other... to_add) {
    return cur_val + add(to_add...);
}

template<> 
int add() {
    return 0;
}

class Matrix {

};

#endif