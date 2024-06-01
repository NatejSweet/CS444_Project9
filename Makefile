CC = gcc
CFLAGS = -Wall -Wextra -DCTEST_ENABLE
LIB = libvvsfs.a
SRCS = image.c block.c inode.c free.c pack.c dir.c ls.c
OBJS = $(SRCS:.c=.o)
EXEC = testfs

.PHONY: all clean pristine test

all: $(EXEC)

test: $(EXEC)
	./$(EXEC)

$(EXEC): testfs.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $^

$(LIB): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

testfs.o: testfs.c image.h block.h pack.h inode.h free.h dir.h ls.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) testfs.o $(EXEC) $(LIB)

pristine:
	rm -f $(OBJS) testfs.o $(EXEC) $(LIB)