#include <sys/elf_note.h>

/** The initial entry point.
 */
asm(
"       .text\n"
"       .globl  _start\n"
"       .align  2\n"
"_start:\n"
"       sub     lr, lr, lr\n"           // Clear the link register.
"       ldr     r0, [sp]\n"             // Get argc...
"       add     r1, sp, #4\n"           // ... and argv ...
"       add     r2, r0, #1\n"           // ... and argc + 1 ...
"       add     r2, r1, r2, LSL #2\n"   // ... and compute environ.
"       bl      _estart\n"              // Let's go!
"       b       .\n"                    // Never gets here.
"       .size   _start, .-_start\n"
);
