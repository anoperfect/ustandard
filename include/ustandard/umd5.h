#ifndef __UMD5_H__
#define __UMD5_H__

#include <stdio.h>
__BEGIN_DECLS


#define UMD5_LEN_VALUE          16
#define UMD5_LEN_STRING         32
typedef unsigned int UINT4;

/* MD5 context. */
typedef struct {
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
}md5ctx_t;

void umd5_init(md5ctx_t*);
void umd5_update(md5ctx_t *, const unsigned char *, unsigned int);
void umd5_final(unsigned char [16], md5ctx_t*);

void umd5_calc(unsigned char value[16], const unsigned char* content, unsigned int len);
int  umd5_path(unsigned char value[16], const char* path);

void umd5_to_checksum(char str[32], const unsigned char value[16]);

__END_DECLS

#endif//__UMD5_H__
