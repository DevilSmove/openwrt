#ifndef TEST_COMMON
#define TEST_COMMON
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>
#include <sys/stat.h>
#ifdef __APPLE__
typedef int64_t loff_t;
typedef unsigned int uint;
#ifndef EUCLEAN
#define EUCLEAN 117
#endif
#endif
static inline int is_power_of_2(unsigned long n){return n && !(n&(n-1));}
#define SIZE (128u*1024*1024)
#define BLOCK 131072u
#define PAGE 2048u
static unsigned char *flash, bad[1024];
static unsigned reads,writes,erases;
static int erase_fail=-1;
static uint32_t crc32_le(uint32_t c,const void *data,size_t n){const unsigned char *p=data;while(n--){c^=*p++;for(int i=0;i<8;i++)c=(c>>1)^((c&1)?0xedb88320:0);}return c;}
static int read_page_common(uint64_t a,void *d,void *o){assert(a+PAGE<=SIZE);reads++;if(d)memcpy(d,flash+a,PAGE);if(o)memset(o,255,64);return 0;}
static int write_page_common(uint64_t a,const void *d,const void *o){assert(a+PAGE<=SIZE);writes++;if(d)for(size_t i=0;i<PAGE;i++)flash[a+i]&=((const unsigned char*)d)[i];return 0;}
static int erase_common(uint64_t a){assert(a%BLOCK==0&&a+BLOCK<=SIZE);erases++;if((int)(a/BLOCK)==erase_fail)return -EIO;memset(flash+a,255,BLOCK);return 0;}
static void load_flash(const char *p){struct stat st;assert(!stat(p,&st)&&S_ISREG(st.st_mode)&&st.st_size>=SIZE);flash=malloc(SIZE);assert(flash);FILE*f=fopen(p,"rb");assert(f);assert(fread(flash,1,SIZE,f)==SIZE);size_t got=fread(bad,1,1024,f);assert(got==0||got==1024);fclose(f);}
static void save_flash(const char *p){if(!strcmp(p,"-"))return;FILE*f=fopen(p,"wb");assert(f);assert(fwrite(flash,1,SIZE,f)==SIZE);assert(fwrite(bad,1,1024,f)==1024);fclose(f);}
#endif
