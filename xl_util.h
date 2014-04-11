#ifndef _XL_UTIL_H_ 
#define _XL_UTIL_H_

#define XL_LONG_BIT 64
#define XL_SHORT_BIT 32

#define XL_MSB 1 //big endian
#define XL_LSB 2 //little endian

int xl_sysbit();

int xl_endian();

char* xl_print_addr(const void *p, char *ret);

#endif

