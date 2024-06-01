#include "image.h"
#include "block.h"
#include "ctest.h"
#include "free.h"
#include "inode.h"
#include "dir.h"
#include "ls.c"
#include <stdlib.h>
#include <string.h>

void test_image_fd_init(void)
{
    CTEST_ASSERT(image_fd == -1, "Test image_fd is -1 before use");
}

void test_image_fd_gets_set(void)
{
    CTEST_ASSERT(image_fd != -1, "Test image_fd gets set once used");
}
void test_image_open(void)
{
    CTEST_ASSERT(image_open("sample.txt", 1) == 4, "Test image_open");
}

void test_image_close(void)
{
    CTEST_ASSERT(image_close() == 0, "Test image_close");
}

void test_bread(void)
{
    unsigned char blockMap[4096] = {1, 1, 1, 1, 1, 1};
    CTEST_ASSERT(bread(2, blockMap) == blockMap, "Test bread");
}
void test_bwrite(void)
{
    unsigned char tmp[4096] = {1, 1, 1, 1, 1, 0};
    bwrite(3, tmp);
    CTEST_ASSERT(bread(3, tmp) == tmp, "Test bwrite(write, then assert read)");
}

void test_set_free(void)
{
    unsigned char block[4096] = {0, 0, 0, 0, 0, 0};
    set_free(block, 1, 1);
    CTEST_ASSERT(block[0] == 1, "Test set_free");
}

void test_find_free(void)
{
    unsigned char block[4096] = {1, 0, 0, 0, 0, 0};
    CTEST_ASSERT(find_free(block) == 1, "Test find_free");
}

void test_ialloc(void)
{
    unsigned char inode_map[4096] = {0, 0, 0, 0, 0, 0};
    struct inode *in = ialloc(inode_map);
    CTEST_ASSERT(in->size == 0, "Test ialloc");
}

void test_alloc(void)
{
    unsigned char data_block[4096] = {0, 0, 0, 0, 0, 0};
    CTEST_ASSERT(alloc(data_block) == 0, "Test alloc");
}

void test_incore_find_free(void)
{
    struct inode *node = incore_find_free();
    CTEST_ASSERT(node != NULL && node->ref_count==0, "Test incore_find_free");
}
void test_incore_find(void)
{
    struct inode *node = incore_find(0);
    CTEST_ASSERT(node != NULL && node->ref_count!=0, "Test incore_find");
}

void test_incore_free_all(void)
{
    struct inode *node = incore_find(0);
    node->ref_count = 1;
    incore_free_all();
    CTEST_ASSERT(node->ref_count == 0, "Test incore_free_all");
}

void test_read_inode(void)
{
    struct inode in;
    unsigned int inode_num = 0;
    read_inode(&in, inode_num);
    CTEST_ASSERT(in.ref_count==1 && in.inode_num==inode_num, "Test read_inode");//ref count is set to 1 by read_inode, and the inode_num is set to the inode_num passed in
}

void test_write_inode(void)
{
    struct inode in;
    struct inode out;
    write_inode(&in);
    CTEST_ASSERT(in.size!=out.size, "Test write_inode");
    read_inode(&out, in.inode_num);
    CTEST_ASSERT(in.size==out.size, "Test write_inode");
}

void test_iget(void)
{
    incore_free_all();//reset incore from other tests to use inode 0

    CTEST_ASSERT(iget(0)->ref_count==1, "Test iget");//first find, sets ref_count to 1
    CTEST_ASSERT(iget(0)->ref_count==2, "Test iget");//second find, increments ref_count

}

void test_iput(void)
{ 
    struct inode in;
    in.ref_count = 0;
    CTEST_ASSERT(iput(&in)==NULL, "Test iput");//ref_count is 0, so should return NULL
    in.ref_count = 1;
    CTEST_ASSERT(iput(&in)==&in, "Test iput");//ref_count is 1, so should return in
    struct inode out;
    read_inode(&out, 0);
}

void test_directory_open_and_close(void)
{
    struct directory *dir = directory_open(0);
    CTEST_ASSERT(dir->inode->inode_num==0, "Test directory_open sets directory inode to passed inode_num");
    CTEST_ASSERT(dir->offset==0, "Test directory_open sets directory offset to 0");
    directory_close(dir);
    //I dont know how to test free memory, has undefined behaviors
}

void test_mkfs()
{
    mkfs();
    struct inode *in = iget(0);
    CTEST_ASSERT(in->size==2*DIR_ENTRY_SIZE, "Test mkfs");
    CTEST_ASSERT(in->flags==2, "Test mkfs");
    CTEST_ASSERT(in->block_ptr[0]==0, "Test mkfs");
}



void test_directory_get(void)
{
    mkfs();
    struct directory *dir = directory_open(0);
    struct directory_entry ent;
    int result = directory_get(dir, &ent);
    CTEST_ASSERT(ent.inode_num==0, "Test directory_get");
    CTEST_ASSERT(strcmp(ent.name, ".")==0, "Test directory_get");
    CTEST_ASSERT(result==0, "Test directory_get returns 0 if successful");
    directory_close(dir);

}

void test_ls(void)
{
    // mkfs();// would need to be called if tested individually
    ls();
    CTEST_ASSERT(1==1, "Test ls");
}

#ifdef CTEST_ENABLE
int main() 
{
    CTEST_VERBOSE(1);
    test_image_fd_init();
    image_open("sample.txt", 0);
    test_image_fd_gets_set();
    test_image_open();
    test_image_close();
    test_bread();
    test_bwrite();
    test_set_free();
    test_find_free();
    test_ialloc();
    test_alloc();
    test_incore_find_free();
    test_incore_find();
    test_incore_free_all();
    test_read_inode();
    test_write_inode();
    test_iget();
    test_iput();
    test_directory_open_and_close();
    test_mkfs();
    test_directory_get();
    test_ls();
    CTEST_RESULTS();
    CTEST_EXIT();
    return 0;
}

#else
int main()
{
    // unsigned char superblock[4096];
    // unsigned char inode_map[4096];
    // unsigned char block_map[4096];
    // unsigned char inode0[4096];
    // unsigned char inode1[4096];
    // unsigned char inode2[4096];
    // unsigned char inode3[4096];
    // unsigned char file_data_block[4096];
    image_open("sample.txt", 0);
    unsigned char tmp[4096];
    bread(0, tmp);
    tmp[0] = 1;
    bwrite(0, tmp);
    bread(0, tmp);
    return 0;
}
#endif