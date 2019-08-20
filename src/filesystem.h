#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "disk.h"
#include <fuse.h>

void pl_init (struct fuse_conn_info *conn);
int pl_getattr (const char * path, struct stat * stat_buff);
int pl_rmdir (const char * path);
int pl_read (const char *, char *, size_t, off_t, struct fuse_file_info *);
int pl_write (const char *, const char *, size_t, off_t, struct fuse_file_info *);
int pl_readdir (const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *);
int pl_mkdir (const char *, mode_t);
int pl_rename (const char * path, const char * new_name);

int get_free_block();
void set_busy_block(int block);
void set_free_block(int block);
void load_directory();
struct File_Entry * get_entry(const char * path);
void get_file_size(struct File_Entry * entry, int * size, int * blocks);

#endif