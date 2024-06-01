#include "dir.h"
#include "block.h"
#include "inode.h"
#include "pack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>



void mkfs(){
    //get new inode
    unsigned char inode_map[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0};
    unsigned char data_block[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0};
    struct inode *root = ialloc(inode_map);
    root->inode_num = ROOT_INODE_NUM;
    int data_block_num = alloc(data_block);
    root->block_ptr[0] = data_block_num;
    root->size = 2*DIR_ENTRY_SIZE;
    root->flags = 2;
    unsigned char dir_data[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0};
    // . directory
    write_u16(dir_data, root->inode_num);
    strcpy((char *)(dir_data + 2), ".");
    // .. directory
    write_u16(dir_data + DIR_ENTRY_SIZE, root->inode_num);
    strcpy((char *)(dir_data + DIR_ENTRY_SIZE + 2), "..");
    //write out
    bwrite(data_block_num, dir_data);
    iput(root);
}


struct directory *directory_open(int inode_num){
    struct inode *inode = iget(inode_num);
    if (inode == NULL)
    {
        return NULL;
    }
    struct directory *dir = malloc(sizeof(struct directory));
    dir->inode = inode;
    dir->offset = 0;
    return dir;
    
}


int directory_get(struct directory *dir, struct directory_entry *ent){
    if (dir->offset >= dir->inode->size)
    {
        return -1;
    }
    int data_block_index =dir->offset / BLOCK_SIZE;
    int data_block_offset = dir->offset % BLOCK_SIZE;
    int data_block_num = dir->inode->block_ptr[data_block_index];
    unsigned char block[BLOCK_SIZE];
    bread(data_block_num, block);
    ent->inode_num = read_u16(block + data_block_offset);
    strcpy(ent->name, (char *)(block + data_block_offset + 2));
    dir->offset += DIR_ENTRY_SIZE;
    return 0;

}

void directory_close(struct directory *dir){
    iput(dir->inode);
    free(dir);
}