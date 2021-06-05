#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>

int syscall_black_list[] = {};

void make_a_syscall()
{
    unsigned long long rax, rdi, rsi, rdx;
    scanf("%llu %llu %llu %llu", &rax, &rdi, &rsi, &rdx);
    syscall(rax, rdi, rsi, rdx);
}

int main()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    puts("You can call a system call, then I will open the flag for you.");
    puts("Input: [rax] [rdi] [rsi] [rdx]");
    close(1);
    make_a_syscall();
    int fd = open("flag", O_RDONLY);
    char flag[0x100];
    size_t flag_len = read(fd, flag, 0xff);
    write(1, flag, flag_len);
    return 0;
}