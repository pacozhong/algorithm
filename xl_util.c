
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "xl_util.h"

int xl_sysbit(){
	char *p;
	if(sizeof(p) == 4)return XL_SHORT_BIT;
	if(sizeof(p) == 8) return XL_LONG_BIT;
}

int xl_endian(){
	int16_t a = 0x1234;
	char *b = (char*)(&a);
	if(*b == 0x12) return XL_MSB;
    else return XL_LSB;	
}

char* xl_print_addr(const void *p, char *ret){
	char *retp = ret;
	int8_t base[] = "0123456789ABCDEF";
	int length = xl_sysbit() / 8;
	int endian = xl_endian();
	char *tmp = (char*)(&p);
	if(endian == XL_LSB)
		tmp += length;
	if(ret != NULL) {
		*ret = '0';
		ret ++;
		*ret = 'x';
		ret ++;
	}else 
		printf("0x");
	while(length -- > 0){
		if(ret != NULL){
			*ret = base[((*tmp) >> 4) & 0x0F];
			ret ++;
			*ret = base[(*tmp) & 0x0F];
			ret ++;	
		}else 
			printf("%c%c", base[((*tmp) >> 4) & 0x0F], base[(*tmp) & 0x0F]);
		if(endian == XL_MSB)
			tmp ++;
		else tmp --;
	}
	if(ret != NULL) {
		*ret = '\0';
		return retp;
	}else return NULL;
}
