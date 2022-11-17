/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  utf-8 converter form cp-1251, koi8-r, iso-8859-5
 *
 *        Version:  1.0
 *        Created:  
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sergei.mos@gmail.com 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <wchar.h>
#include <locale.h>

#define LONGEST_WORD 64

#define HASH_T 512 //2^15
//#define HASH_T 2048 //2^15
//#define HASH_T 16000 //2^15
uint32_t KH = 1;
#define HASH_T_S ((HASH_T)*(KH))

int TOTAL;
 
struct t_hdata {
	wchar_t key[LONGEST_WORD];
	uint16_t count;
} t_hdata;

struct t_htbl {
	struct t_hdata *data;
	size_t size;
} t_htbl;

struct t_htbl *htblInit(); 
struct t_htbl *probing(struct t_htbl* htbl, uint32_t *index, wchar_t *nword);
struct t_htbl *expand(struct t_htbl* htbl);
void print_h(struct t_htbl *htbl, uint32_t total); 

int main(int argc, char **argv) {
	setlocale(LC_ALL, "");
	if (argc < 2) {
		puts("Usage: <in_file>");
		exit(0);
	}

	FILE *f = fopen(argv[1], "r"); 
//	uint16_t k = 1;

	if (!f) {
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}
	
	struct t_htbl *htbl = htblInit();
	//print_h(htbl,0);
	//exit(0);
	//htbl->size = 0;

	struct t_htbl *htable;

	wchar_t word[LONGEST_WORD] = {0};
	wchar_t nword[LONGEST_WORD] = {0};
	uint32_t total = 0;
	uint32_t h1;
	uint32_t index = 0;

	while (fscanf(f, "%ls", word) != EOF) { 
		uint8_t s = 0, ns=0;

//		printf("%s fscanf:  %ls %u %u\n",__func__, word, index, HASH_T_S);
		h1 = 0; 
		while (word[s]!=L'\0') {
			//if (isalpha(word[s])) {
			if (((word[s] > 64 && word[s] < 91) || ((word[s] > 95) && (word[s] < 123))) || 
				((word[s] > 0x40f) && (word[s] < 0x450 ))) {
				h1 *= 31 << ns;
				h1 += (word[s]);
				nword[ns] = word[s];		
				ns++;
			} 
			s++;
			
		}
		nword[ns] = L'\0';


		if (ns > 0) {
			index = (h1) % (HASH_T_S);
//			printf("%s: %ls %u %u\n",__func__, nword, index, HASH_T_S);
			htable = probing(htbl, &index, nword); 
			total++;	
		//	print_h(htable,total);			
			
			htbl = htable;

		}
#if 1
//		printf("%s %p: %p\n",__func__, htbl, htable);

		if ( htable->size > (size_t)((HASH_T_S)/2)) { 
//			printf("before -------------- %s %p: htbl->size %u\n",__func__, htbl, htbl->size);
		//	printf("------------------ EXPAND in ------------------\n");
			htable = expand(htbl);
		//	printf("------------------ EXPAND out ------------------\n");
		//	printf("after -------------- %s %p: htable->size %u\n",__func__, htable, htable->size);

//			printf("%s hdata : %p\n",__func__, &htable->data[0]);
			if (htable != NULL) {
				htbl->data = htable->data;
				htbl = htable;
			}
		}
#endif
	}

	print_h(htbl,total);

	free(htbl->data);
	exit(EXIT_SUCCESS);
}

struct t_htbl *probing(struct t_htbl* htbl, uint32_t *index, wchar_t *nword) {

	struct t_hdata *hdata;
	int k = 0;

//	printf("%s in: %ls index: %u hash_t %u\n",__func__, nword, *index, HASH_T_S);
	while(1) {
		hdata = (htbl->data + (*index));
		if (*hdata->key!=L'\0') {
//			printf("%s : hdata->key: %ls, count %u\n",__func__, hdata->key, hdata->count);
			if ((wcsncmp(hdata->key, nword, wcslen(hdata->key)) == 0) &&
					wcslen(hdata->key) == wcslen(nword))
			 {
				hdata->count++;
				break;
			} else {
				//*index = nword[k % wcslen(nword)] + k++;
				*index = (nword[0] + k++) % HASH_T_S;
//				printf("%s index %u\n", __func__, *index);
//				if (k > wcslen(nword)-1)
//					k = 0;
			}
		} else {
//			printf("else %s index: %d, nword %ls\n",__func__, *index, nword);
			wcsncpy(hdata->key, nword, wcslen(nword));

//			hdata->key[wcslen(nword)] = L'\0';
			hdata->count=1;
			htbl->size++;
//			print_h(htbl, 0);
			break;
		}
	}
//	printf("%s out: htbl->data[%u].key = %ls\n",__func__, *index, htbl->data[*index].key);
	return htbl;
}

struct t_htbl *htblInit() {
	struct t_htbl *htbl = (struct t_htbl *)calloc(1, sizeof(struct t_htbl));
	htbl->data = (struct t_hdata *)calloc(HASH_T_S+1, sizeof(struct t_hdata));
	return htbl;
}


struct t_htbl *expand(struct t_htbl *htbl) {

	uint32_t h1 =0;
	wchar_t *w;
	size_t j=0;
	struct t_htbl *htable;

//	printf("%s in : htbl->size  %u HASH_T %u KH %u\n",__func__, htbl->size, HASH_T_S, KH);

	KH++;
	struct t_htbl *new_htbl = htblInit();

	htable =(struct t_htbl *)new_htbl;
	struct t_hdata *hdata = NULL;

	uint32_t i;
	while (new_htbl->size < (htbl->size)) {
		i = 0;
//		printf("%s : htbl->size %u new_htbl->size %u j %u HASH_T %u KH %u\n",__func__, 
//			htbl->size, new_htbl->size, j, HASH_T_S, KH);
		hdata = (struct t_hdata *)(htbl->data + j);
//		printf("curr: %s : j = %d htbl->key %ls, count %u\n",__func__, j, hdata->key, hdata->count);
	
		if (*hdata->key!=L'\0') {
			h1 = 0;
			w = hdata->key;
			while(*w != L'\0') { 
				h1 *= 31 << i;
				h1 += *w++;
				i++;
			}
			h1 = (h1) % HASH_T_S;	
//			printf("expand curr: %s : h1: %d, j %d, hdata->key  %ls\n",__func__, h1, j, hdata->key);
			htable = (struct t_htbl *)probing(new_htbl,&h1,hdata->key);
			new_htbl->data[h1].count = hdata->count;
			
		} 
		j++;
	} 

//	printf("%s out: %d new_htbl->key  %ls\n",__func__, j, new_htbl->data[h1].key);
	free(htbl->data);
//	free(htbl);

	return new_htbl;
}

void print_h(struct t_htbl *htbl, uint32_t total) {
 	int j = 0, u = 0, sum = 0;
	uint32_t i = 0;
	struct t_hdata *hdata;
	while(i < HASH_T_S) {
		hdata = &htbl->data[i];
		//if (hdata->count > 0) {
		if (*hdata->key) {
			printf("%u: %ls:%u\n",i, hdata->key, hdata->count);
			sum += hdata->count;
			u++;
		}			
		else j++;
		i++;
	}

	printf("\n-------------\ntotal: %u\nused %u\nunused: %u \ncounts sum: %u \ntable size: %u\n"
						,total,u,j,sum, i);

}
