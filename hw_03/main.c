/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  hahsh table with char keys
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
#include <locale.h>

#define LONGEST_WORD 256
static uint32_t TABLE_SIZE=2047;

int KH = 1;

#define SYM_FLTR(sym)	(((sym)!='"') && !(ispunct((sym))) && !(iscntrl((sym))))
#define HASH_T_S ((TABLE_SIZE)*(KH))
#define GET_H_INDEX(x) ((x) % (TABLE_SIZE))
#define GET_HASH(k,h) \
	int __i = 0; \
		while (((k)[__i])) { \
		((h) *= 31 << __i); \
		((h) += ((k)[__i++])); \
	}

struct t_hdata {
	char key[LONGEST_WORD];
	uint16_t count;
	uint8_t used;
} t_hdata;

struct t_htbl {
	struct t_hdata *data;
	size_t size;
} t_htbl;

struct t_htbl *probing(struct t_htbl* htbl, char *key);
struct t_htbl *expand(struct t_htbl *old_table); 
struct t_hdata *get_by_hash(struct t_htbl* htbl, char *key); 
void print_h(struct t_htbl *htbl, uint32_t total); 
void free_h(struct t_htbl *htbl); 

int main(int argc, char **argv) {
	if (argc < 2) {
		puts("Usage: <in_file> [table size]");
		exit(0);
	}

	if (argv[2])
		TABLE_SIZE = atoi(argv[2]);

	assert(TABLE_SIZE);

	FILE *f = fopen(argv[1], "r"); 

	if (!f) {
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}

	struct t_htbl *htable = NULL;
	struct t_htbl *htbl;
	char nword[LONGEST_WORD] = "\0";
	uint32_t total = 0;
	uint16_t ns = 0;
	const char dilsym[] = "-\", ,!?,.:;\\"; 	

	htbl = (struct t_htbl *)calloc(1, sizeof(struct t_htbl));
	assert(htbl); 
	htbl->data = (struct t_hdata *)calloc(TABLE_SIZE + 1, sizeof(struct t_hdata));	
	assert(htbl->data); 

	while (fscanf(f, "%s", nword)!=EOF) {
		ns = 0;
		htable = htbl;
		nword[ns=strcspn(nword,dilsym)]='\0';		
		total++;

		if (ns)	
			htbl = probing(htable, nword); 
		
		if ( htable->size > (size_t)((HASH_T_S)/2))  
			htbl = expand(htbl);
	}

	print_h(htbl,total);
	free_h(htbl);
	fclose(f);

	exit(EXIT_SUCCESS);
}

void free_h(struct t_htbl *htbl) {
	for (int i=0; i < KH; i++) 
		free(htbl[i].data);
	free(htbl);
}

struct t_htbl *expand(struct t_htbl *old_table) {
		/* expand table	*/
		KH +=1;
		struct t_htbl *htable = realloc(old_table, (KH)*(sizeof(struct t_htbl)));
		assert(htable); 
		htable[KH - 1].data = calloc(TABLE_SIZE + 1, (sizeof(struct t_hdata)));
		assert(htable[KH - 1].data);
		return htable;
}
struct t_htbl *probing(struct t_htbl* htbl, char *key) {

	struct t_hdata *hdata;
	
	uint32_t k = 0;
	uint32_t __table = 0;
	uint32_t __index = 0;	
	uint32_t __hash = 0;

	size_t klen = strlen(key); 

	GET_HASH(key, __hash)
	__index = GET_H_INDEX(__hash);	
	
	while(1) {
		hdata = &(htbl[__table].data[(__index)]);

		if (!hdata->used) {
			memcpy(hdata->key, key, klen);
			hdata->count = 1;
			hdata->used = 1;
			htbl->size++;
			break;
		}

		if ((memcmp(hdata->key, key, klen) == 0) &&
				(strlen(hdata->key) == klen)) {
			hdata->count++;
			break;
		}
		if (k > TABLE_SIZE - 1) {
			__table++;
			k = 0;
			__index = GET_H_INDEX(__hash);
		} else { 
			__index = (__index + k++) % TABLE_SIZE;
		}
	}
	return htbl;
}

struct t_hdata *get_by_key(struct t_htbl* htbl, char *key) {

	struct t_hdata *hdata;
	
	uint32_t k = 0;
	uint32_t __table = 0;
	uint32_t __index = 0;
	uint32_t __hash = 0;
	size_t klen = strlen(key); 

	GET_HASH(key, __hash)
	__index = GET_H_INDEX(__hash);  	

	while(1) {
		hdata = &(htbl[__table].data[(__index)]);
		if (!hdata->used) 
			return NULL;
		if ((memcmp(hdata->key, key, klen) == 0) &&
				strlen(hdata->key) == klen) {
			break;
		}
		if (k > TABLE_SIZE - 1) {
			__table++;
			k = 0;
			__index = GET_H_INDEX(__hash);
		} else { 
			__index = (__index + k++) % TABLE_SIZE;
		}
	}
	return hdata;
}

void print_h(struct t_htbl *htbl, uint32_t total) {
 	uint32_t j = 0, u = 0, sum = 0, i = 0;
	struct t_hdata *hdata;
	int k = 0;
	for ( k=0; k < KH; k++) {
		i = 0;
		while(i < TABLE_SIZE) {
			hdata = &htbl[k].data[i];
			if (hdata->used) {
				printf("%u. T%d[%u]: %s [%d]\n", u, KH, i, hdata->key, hdata->count);
				sum += hdata->count;
				u++;
			}			
			else j++;
			i++;
		}
	}

	printf("\n----------------------------\n\
	total items: %u\n\
	hashed items: %u \n\
	used keys: %u\n\
	unused keys: %u\n",total,u,j,sum);

	#if 0
	printf("\n----------------------------\n\tTEST\n----------------------------\n");
	uint32_t max = u;
	char *test_array[max];
	u = 0;
	for ( k=0; k < KH; k++) {
		i = 0;
		while(i < TABLE_SIZE) {
			hdata = &htbl[k].data[i];
			if (*hdata->key) {
				test_array[u] = malloc(strlen(hdata->key)*sizeof(char));	
				memcpy(test_array[u], hdata->key, strlen(hdata->key));
				u++;
			}			
			else j++;
			i++;
		}
	}
	char *p = NULL;
	for (i = 0; i < max; i++) {
		p = test_array[i];
		for (uint32_t j = i+1; j < max; j++)
			if ((strncmp(p, test_array[j], strlen(test_array[j])) == 0)
				&& (strlen(p) == strlen(test_array[j]))) {
				printf("Double key fault %i: %s <> %s\n",i, test_array[j],p);	
				exit(0);
			}
	}
	printf("> Double key test: OK\n");
# endif
#if 1
	char key1[] ="shook";
	char key2[] ="рассмеялся";
	struct t_hdata *hd = get_by_key(htbl, key1);

	(hd) ? printf("> key: '%s', count: %u\n", hd->key, hd->count):
			printf("> key: '%s' is not present.\n", key1);

	hd = get_by_key(htbl, key2);

	(hd) ? printf("> key: '%s', count: %u\n", hd->key, hd->count):
			printf("> key: '%s' is not present.\n", key2);

	#endif


}
