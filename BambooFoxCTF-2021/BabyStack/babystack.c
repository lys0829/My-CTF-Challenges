#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
void func(){
    int len,idx;
    char buf[56];
    puts("str1: ");
    idx = read(0,buf,16);
    len = strlen(buf);
    puts(buf);
    puts("str2: ");
    read(0,buf+idx,56-len);
    puts(buf+idx);
}

void token(char *name){
    //char err[8] = "TokenErr";
    char token[16];
    char *TOKEN = token+8;
    TOKEN[0] = 'd';
    TOKEN[1] = 'e';
    TOKEN[2] = 'a';
    TOKEN[3] = 'd';
    TOKEN[4] = 'b';
    TOKEN[5] = 'e';
    TOKEN[6] = 'e';
    TOKEN[7] = 'f';
    puts("Hello, please give me your token: ");
    read(0,token,16);
    if( memcmp(TOKEN,token,8)!=0 ){
        puts("Token Error");
        exit(0);
    }
}

void name(){
    char name[16];
    puts("Name: ");
    read(0,name,16);
    token(name);
}

int main(){
    setvbuf(stdin,0,_IONBF,0);
    setvbuf(stdout,0,_IONBF,0);
    char buf[64];
    for(int i=0;i<64;i++)buf[i]=0;
    name();
    func();
    func();
    read(0,buf,24);
    puts(buf);
    asm volatile ("mov $1,%rdi");
    asm volatile ("mov $3,%rax");
    asm volatile ("syscall");
    //close(1);
    //fflush(stdout);
    return 0;
}