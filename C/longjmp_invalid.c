/* How NOT to use the longjmp function. This code tries to jump to a function
   that *has already returned*, and this can cause all sorts of errors, since the
   function will try to unwind the stack to a frame that is no longer in the stack.
   
   This code ilustrates *undefined behavior*.
*/


#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;

static void
function_that_will_setjmp(void) {
    switch (setjmp(env)) {
        case 0:
            printf("Jump location set.\n");
            break;

        case 1:
            printf("You may or may not see this.\n");
            break;
    }
}

static void
longjmp_caller(void) {
    printf("Jumping in...\n");
    longjmp(env, 1);
}

int
main(void) {
    function_that_will_setjmp();
    longjmp_caller(); /* BOOM? */

    return 0;
}
