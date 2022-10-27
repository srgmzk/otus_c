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
		puts("Program using: out <file_name>");
		exit(0);
	}

	ssize_t ret;
	struct FileHeader fh;
	int i = 0;

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}

	off_t curr = lseek(fd, 0, SEEK_END);
	off_t first = lseek(fd, 0, SEEK_SET);
	char buffer[PATH_MAX] = "\0";

	while(curr != first )
	{
		ret = read(fd, &fh, sizeof(struct FileHeader));
		if (ret == -1) {
			if (errno == EINTR)
				continue;
			perror("read");
			exit(EXIT_FAILURE);
		}
		if (fh.signature == ZIP_MAGIC_NUM) 
		{
			snprintf(buffer, fh.filenameLength + 1, "%s", (char *)&fh.filename);
			puts(buffer);
		}
		first = lseek(fd, ++i, SEEK_SET);
	}
	if (buffer[0] == '\0')
		printf("File is not zip archive\n");
	close(fd);
	return EXIT_SUCCESS;
}
