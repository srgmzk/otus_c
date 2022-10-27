/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  27.10.2022 10:37:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

unsigned koi8_table[] = { [0 ... 191] = 0, 
	0x8ed1, 0xb0d0, 0xb1d0, 0x86d1, 0xb4d0, 0xb5d0, 0x84d1, 0xb3d0, 0x85d1, 0xb8d0, 0xb9d0, 0xbad0,	0xbbd0, 0xbcd0, 0xbdd0,	0xbed0, 
	0xbfd0, 0x8fd1, 0x80d1, 0x81d1, 0x82d1, 0x83d1, 0xb6d0, 0xb2d0, 0x8cd1, 0x8bd1, 0xb7d0, 0x88d1, 0x8dd1, 0x89d1, 0x87d1, 0x8ad1, 
	0xaed0,	0x90d0, 0x91d0, 0xa6d0, 0x94d0, 0x95d0, 0xa4d0, 0x93d0, 0xa5d0, 0x98d0, 0x99d0, 0x9ad0, 0x9bd0,	0x9cd0, 0x9dd0, 0x9ed0, 
	0x9fd0, 0xafd0, 0xa0d0, 0xa1d0, 0xa2d0, 0xa3d0, 0x96d0, 0x92d0, 0xacd0,	0xabd0, 0x97d0, 0xa8d0, 0xadd0, 0xa9d0, 0xa7d0, 0xaad0  
};

unsigned win1251_table[] = { [0 ... 191] = 0, 
	0x90d0, 0x91d0, 0x92d0, 0x93d0, 0x94d0, 0x95d0, 0x96d0, 0x97d0, 0x98d0, 0x99d0, 0x9ad0, 0x9bd0, 0x9cd0, 0x9dd0, 0x9ed0, 0x9fd0,
	0xa0d0, 0xa1d0, 0xa2d0, 0xa3d0, 0xa4d0, 0xa5d0, 0xa6d0, 0xa7d0, 0xa8d0, 0xa9d0, 0xaad0, 0xabd0, 0xacd0, 0xadd0, 0xaed0, 0xafd0,
	0xb0d0, 0xb1d0, 0xb2d0, 0xb3d0, 0xb4d0, 0xb5d0, 0xb6d0, 0xb7d0, 0xb8d0, 0xb9d0, 0xbad0, 0xbbd0, 0xbcd0, 0xbdd0, 0xbed0, 0xbfd0,
	0x80d1, 0x81d1, 0x82d1, 0x83d1, 0x84d1, 0x85d1, 0x86d1, 0x87d1, 0x88d1, 0x89d1, 0x8ad1, 0x8bd1, 0x8cd1, 0x8dd1, 0x8ed1, 0x8fd1
};

unsigned iso8859_5_table[] = { [0 ... 175] = 0, 
	0x90d0, 0x91d0, 0x92d0, 0x93d0, 0x94d0, 0x95d0, 0x96d0, 0x97d0, 0x98d0, 0x99d0, 0x9ad0, 0x9bd0, 0x9cd0, 0x9dd0, 0x9ed0, 0x9fd0,
	0xa0d0, 0xa1d0, 0xa2d0, 0xa3d0, 0xa4d0, 0xa5d0, 0xa6d0, 0xa7d0, 0xa8d0, 0xa9d0, 0xaad0, 0xabd0, 0xacd0, 0xadd0, 0xaed0, 0xafd0,
	0xb0d0, 0xb1d0, 0xb2d0, 0xb3d0, 0xb4d0, 0xb5d0, 0xb6d0, 0xb7d0, 0xb8d0, 0xb9d0, 0xbad0, 0xbbd0, 0xbcd0, 0xbdd0, 0xbed0, 0xbfd0,
	0x80d1, 0x81d1, 0x82d1, 0x83d1, 0x84d1, 0x85d1, 0x86d1, 0x87d1, 0x88d1, 0x89d1, 0x8ad1, 0x8bd1, 0x8cd1, 0x8dd1, 0x8ed1, 0x8fd1
};


int main(int argc, char **argv) {
		
	FILE *f1 = fopen("iso-8859-5.txt", "r"); 
	FILE *f2 = fopen("utf8.txt", "w+"); 
	unsigned sym;
#if 0
		fread(&sym,1,1, f1);
		printf("%x ", sym);
		printf("%x", koi8_table[sym + 1]);
		if (sym < 192)	
			fwrite(&sym,1,1,f2); 
		else
	//	putchar(koi8_table[sym]); 
			fwrite(&koi8_table[sym + 1],1,2,f2); 
#endif
	#if 1
	while ( !feof(f1) ) {
		fread(&sym,1,1, f1);
		if (sym < 176)	
			fwrite(&sym,1,1,f2); 
		else
			fwrite(&iso8859_5_table[sym],1,2,f2); 
	}
	#endif
	fclose(f1);
	fclose(f2);
	
	exit(EXIT_SUCCESS);
}
