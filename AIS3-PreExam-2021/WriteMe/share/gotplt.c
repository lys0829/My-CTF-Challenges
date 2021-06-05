#include <stdlib.h>
#include <stdio.h>
int main()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    void *systemgot = 0x404028;
    void *scanfgot = 0x404040;
    //void *systemgot = (void *)((long long)(*(int *)(systemptr+2))+(long long)(systemptr+6));
    *(long long *)systemgot = (long long)0x0;

    printf("Address: ");
    void *addr;
    long long v;
    scanf("%ld",&addr);
    printf("Value: ");
    scanf("%ld",&v);
    *(long long *)addr = (long long)v;
    *(long long *)scanfgot = (long long)0x0;
    printf("OK! Shell for you :)\n");
    system("/bin/sh");
    return 0;
}
