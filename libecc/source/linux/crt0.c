#include <stdlib.h>             // exit()
#include <string.h>             // memset()

extern char **environ;
extern int main(int argc,char **argv,char **envp);

extern char _end;
extern char _bss;

void _start(int arguments)
{
    int *p = &arguments-1;
    int argc = *p;
    char **argv = (char **)(p + 1);
    environ = argv + argc + 1;
    memset(&_bss, 0, &_end - &_bss);
    int status = main(argc, argv, environ);
    exit(status);
}