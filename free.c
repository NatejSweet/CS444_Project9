#include "free.h"

int find_low_clear_bit(unsigned char x)
{
    for (int i = 0; i < 8; i++)
        if (!(x & (1 << i)))
            return i;

    return -1;
}

void set_free(unsigned char *block, int num, int set)
{
    int byte_num = num / 8;
    int bit_num = num % 8;
    if (set == 1)
    {
        bit_num = find_low_clear_bit(block[byte_num]);
        block[byte_num] |= (1 << bit_num);
    }
    else
    {
        block[byte_num] &= ~(1 << bit_num);
    }
}

int find_free(unsigned char *block)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        if (block[i] == 0)
        {
            return i;
        }
    }
    return -1;
}