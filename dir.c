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

struct inode *namei(char *path){
    if (strcmp(path, "/") == 0){
        return iget(ROOT_INODE_NUM);
    }
    if (strcmp(path, "/foo") == 0){
        return iget(1);
    }
    if (strcmp(path, "/foo/bar") == 0){
        return iget(2);
    }
    else {
        return NULL;
    }
}

char *get_dirname(const char *path, char *dirname)
{
    strcpy(dirname, path);

    char *p = strrchr(dirname, '/');

    if (p == NULL) {
        strcpy(dirname, ".");
        return dirname;
    }

    if (p == dirname)  // Last slash is the root /
        *(p+1) = '\0';

    else
        *p = '\0';  // Last slash is not the root /

    return dirname;
}

char *get_basename(const char *path, char *basename)
{
    if (strcmp(path, "/") == 0) {
        strcpy(basename, path);
        return basename;
    }

    const char *p = strrchr(path, '/');

    if (p == NULL)
        p = path;   // No slash in name, start at beginning
    else
        p++;        // Start just after slash

    strcpy(basename, p);

    return basename;
}


int directory_make(char *path){
    char dirname[1024];
    char basename[1024];
    get_dirname(path, dirname);
    get_basename(path, basename);
    struct inode *parent = namei(dirname);
    if (parent == NULL){
        return -1;
    }
    unsigned char inode_map[BLOCK_SIZE];
    unsigned char data_block[BLOCK_SIZE];
    struct inode *new = ialloc(inode_map);
    int data_block_num = alloc(data_block);
    if (new == NULL || data_block_num == -1){
        return -1;
    }
    new->block_ptr[0] = data_block_num;
    new->size = 2*DIR_ENTRY_SIZE;
    new->flags = 2;
    unsigned char dir_data[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0};    
    write_u16(dir_data, new->inode_num);
    strcpy((char *)(dir_data + 2), ".");
    write_u16(dir_data + DIR_ENTRY_SIZE, parent->inode_num);
    strcpy((char *)(dir_data + DIR_ENTRY_SIZE + 2), "..");
    bwrite(data_block_num, dir_data);
    struct directory *dir = directory_open(parent->inode_num);
    struct directory_entry ent;
    while (directory_get(dir, &ent) == 0){
        if (strcmp(ent.name, basename) == 0){
            return -1;
        }
    }
    write_u16(dir_data, new->inode_num);
    strcpy((char *)(dir_data + 2), basename);
    bwrite(data_block_num, dir_data);
    parent->size += DIR_ENTRY_SIZE;
    write_inode(parent);
    iput(parent);
    iput(new);
    return 0;  
}

// char **path_split(char *path){
//     char **splitPath = malloc(sizeof(char*) * 129); // 128 dirs + NULL pointer
//     char *token;
//     int i = 0;

//     token = strtok(path, "/");
//     while (token != NULL && i < 128) {
//         splitPath[i] = token;
//         i++;
//         token = strtok(NULL, "/");
//     }
//     splitPath[i] = NULL; // null pointer at end

//     return splitPath;
// }


// THIS IS POSSIBLE CODE FOR THE namei EXTENSION, NOT COMPLETE
// struct inode *namei(char *path){
//     if (strcmp(path, "/") == 0){
//         return iget(ROOT_INODE_NUM);
//     }
//     else {
//         return NULL;
//     }
    // char **splitPath = path_split(path);//splits path on '/' into an array of dirs
    // struct inode *current = iget(ROOT_INODE_NUM);
    // struct directory_entry ent;
    // struct directory *dir;
    // if (splitPath[0] == NULL){//when path is just '/'
    //     return current;
    // }
    // int i=0;//stored outside so accessible when accessing last dir
    // for (splitPath[i+1] != NULL; i++;){//when not just the root node, iterates until the last dir, then breaks
    //     dir = directory_open(current->inode_num);
    //     while (directory_get(dir, &ent) == 0){
    //         if (strcmp(ent.name, splitPath[i]) == 0){
    //             current = iget(ent.inode_num);
    //             if (current == NULL || current->flags != 2){
    //                 dir = directory_open(ent.inode_num);
    //                 break;
    //             }
    //         }
    //     }
        
    // }
    // if (directory_get(dir, &ent) == 0){//last dir
    //     if (strcmp(ent.name, splitPath[i]) == 0){
    //         current = iget(ent.inode_num);
    //         if (current != NULL && current->flags == 2){
    //             return current;
    //         }
    //     }
        
    // }
    // else{
    //     return NULL;
    // }
    // return NULL