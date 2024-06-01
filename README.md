# CS444_Project7
# Test File System

Collaborators: 
    - Tristan Hook

## Building
`make` to build. An executable called `testfs` will be produced.
`make clean` to remove .o build products
`make pristine` to clean up all build products
`make test` to compile and run testfs

## Files
`testfs.c`: 
`block.c`: 
`image.c`: 
`block.h`: 
`image.h`: 
`ctest.h`: 

## Data
reads and writes data to disk
blocks are 4096 bits

## Functions

- `main`
    - `test_image_fd_init`: tests that before any open, image_fd is -1
    - `test_image_fd_gets_set`: tests that after an open, image_fd is no longer -1
    - `test_image_open`: tests that the image opens and updates image_fd
        - `image_open`: opens a file, sets image_fd to the return of open, returns image_fd so it can be tested
    - `test_image_close`: tests that the image is closed correctly
        - `image_close`: closes the file, returning 0 on success
    - `test_bread`: check the blockmap against a new one to test that read works
        - `bread`: reads from the passed block
    - `test_bwrite`: 
        - `bwrite`: writes to the passed block
        - `bread`: reads from the passed block
    - `test_set_free`: 
        - `set_free`: sets a bit in the block to free
    - `test_find_free`: 
        - `find_free`: finds a free bit in the block
    - `test_ialloc`: 
        - `ialloc`: allocates an inode
    - `test_alloc`: 
        - `alloc`: allocates a block
    - `test_incore_find_free`: 
        - `incore_find_free`: finds a free inode in the in-core table
    - `test_incore_find`: 
        - `incore_find`: finds an inode in the in-core table
    - `test_incore_free_all`: 
        - `incore_free_all`: frees all inodes in the in-core table
    - `test_read_inode`: 
        - `read_inode`: reads an inode from the disk
    - `test_write_inode`: 
        - `write_inode`: writes an inode to the disk
        - `read_inode`: reads an inode from the disk
    - `test_iget`: 
        - `incore_free_all` used as a helper function here to reset incore
        - `iget`: gets an inode from the in-core table, loading it from the disk if necessary
    - `test_iput`: 
        - `iput`: puts an inode back to the in-core table, writing it to the disk if necessary
        - `read_inode`: reads an inode from the disk
    - `test_directory_open_and_close`:
        - `directory_open`: opens the directory in the passed inode number
        - `directory_close`: frees the passed directory and writes the inode of the directory
    - `test_mkfs`:
        - `mkfs`: sets up the root directory of a file system
        - `iget`: finds inode for the passed inode number
    - `test_directory_get`:
        - `mkfs`: sets up the root directory of a file system
        - `directory_open`: opens the directory in the passed inode number
        - `directory_get`: gets an entry from the passed directory
        - `directory_close`: frees the passed directory and writes the inode of the directory
    - `test_ls`:
        - 'ls': iteratively calls and prints `directory_get` until it has printed all entries

## Notes