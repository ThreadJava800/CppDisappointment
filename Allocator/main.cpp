#include <cstdio>
#include <string>

#include "allocator.hpp"

struct Test {
    const char*  str1;
    int          val1;
    double       val2;

    ~Test() {
        printf("Called dtor: %s\n", str1);
    }
};

using int_traits = allocator_traits<allocator<int>>;

int main() {
    allocator<long> long_alloc;
    long* long_ptr = long_alloc.allocate(1);
    printf("LONG PTR: %p\n", (void*)long_ptr);
    long_alloc.deallocate(long_ptr, 1);
    
    allocator<int> int_alloc;
    int* int_ptr = int_traits::allocate(int_alloc, 1);
    int_traits::construct(int_alloc, int_ptr, 42);
    printf("CONSTRUCTED INT: %d\n", *int_ptr);
    int_traits::deallocate(int_alloc, int_ptr, 1);
 
    allocator<Test> struct_alloc;
    Test* struct_ptr = struct_alloc.allocate(3);
    struct_alloc.construct(struct_ptr, "Test1", 1, 1.0);
    struct_alloc.construct(struct_ptr + 1, "Test2", 3, -1.0);
    struct_alloc.construct(struct_ptr + 2, "Test3", 4, 16.0);

    for (int i = 0; i < 3; i++) {
        printf("STRUCT DATA: %s %d %lf\n", struct_ptr[i].str1, struct_ptr[i].val1, struct_ptr[i].val2);
    }

    struct_alloc.destroy(struct_ptr);
    struct_alloc.destroy(struct_ptr + 1);
    struct_alloc.destroy(struct_ptr + 2);

    struct_alloc.deallocate(struct_ptr, 3);

    return 0;
}