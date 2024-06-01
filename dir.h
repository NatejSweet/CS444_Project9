#ifndef DIR_H
#define DIR_H
#define ROOT_INODE_NUM 0
#define BLOCK_SIZE 4096
#define DIR_ENTRY_SIZE 32

struct directory {
    struct inode *inode;
    unsigned int offset;
};

struct directory_entry {
    unsigned int inode_num;
    char name[16];
};

void mkfs();
struct directory *directory_open(int inode_num);
int directory_get(struct directory *dir, struct directory_entry *ent);
void directory_close(struct directory *dir);
#endif