#include <stdio.h>
#include <assert.h>

int foo (int a, int b)
{
    // https://gcc.gnu.org/onlinedocs/gcc/Nested-Functions.html
    // nested function, function inside function
    int square (int z) { return z * z; }

    return square (a) + square (b);
}

int main(int argc, char* argv[]) {

    describe("test ptr_array", {
            
        it("test bound\n", {

            assert(1 == 2);
                
        });

    });
   
    return 0;
}


