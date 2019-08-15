#ifndef DISK_H
#define DISK_H

#define BLOCK_SIZE 4096

#include <cstdio>
#include <stdint.h>
#include <fuse.h>

void create_disk(const char * path);
FILE * open_disk(const char * path);
void close_disk();
bool write_block(char * buffer, int block);
bool read_block(char * buffer, int block);


#endif