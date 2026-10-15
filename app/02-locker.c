// F2 · Step 4 (short version): the locker
// Compile: cc -Wall app/02-locker.c -o app/locker
// Run:     ./app/locker

#include <stdio.h>;
#include <stdlib.h>;

// option 1: put the item on the desk (stack)
// when the function ends, the desk is cleared.
int *on_desk(void)
{
    int item = 1234; // put the item on this function's desk
    int *note = &item; // write down where it is on the desk
    return note;
}