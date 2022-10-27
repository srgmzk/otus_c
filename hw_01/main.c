/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  программа определяет является ли заданный файл rarjpeg-ом и выводит
 *					список файлов в архиве  
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
#include <linux/limits.h>

#define ZIP_MAGIC_NUM 0x04034b50

struct FileHeader 
{
    uint32_t signature;
    uint16_t versionToExtract;
    uint16_t generalPurposeBitFlag;
    uint16_t compressionMethod;
    uint16_t modificationTime;
    uint16_t modificationDate;
    uint32_t crc32;
    uint32_t compressedSize;
    uint32_t uncompressedSize;
    uint16_t filenameLength;
    uint16_t extraFieldLength;
    char filename[PATH_MAX];
    uint8_t *extraField;
} __attribute__((packed));


int main(int argc, char **argv) {

	if (argc < 2) {
		puts("Usage: out <file_name>");
		exit(0);
	}

	struct FileHeader fh;
	long i = 0;
	char buffer[PATH_MAX] = "\0";

	FILE *f = fopen(argv[1], "rb");

	if (!f) {
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}

	if (fseek(f, 0, SEEK_SET) == -1) {
		perror("fseek");
		exit(EXIT_FAILURE);
	}
	
	while ( fread(&fh, sizeof(struct FileHeader), 1, f) ) {
		if (fh.signature == ZIP_MAGIC_NUM) {
			snprintf(buffer, fh.filenameLength + 1, "%s", (char *)&fh.filename);
			puts(buffer);
		}
		if (fseek(f, ++i, SEEK_SET) == -1) {
			perror("fseek");
			break;
		}
	}
	if (buffer[0] == '\0')
		printf("File is not zip archive\n");
	fclose(f);
	return EXIT_SUCCESS;
}
