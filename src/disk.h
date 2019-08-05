#ifndef DISK_H
#define DISK_H

#define BLOCK_SIZE 512

void create_disk(const char * path, int size);
void close_disk();
void write_block(char * buffer, int block);
void read_block(char * buffer, int block);


#endif DISK_H