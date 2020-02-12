#include <stdio.h>
#include <assert.h>
#include <stdint.h>

static uint16_t WORD_VALUE_BASE  = 0x100;
static uint16_t WORD_BIT_MORE    = 0x8000;
static uint16_t WORD_VALUE_RANGE = 0x7f00;

uint32_t decode(uint16_t *data)
{
    uint32_t retval = 0;
    do
    {
        assert((*data & ~WORD_BIT_MORE) >= WORD_VALUE_BASE);
        assert((*data & ~WORD_BIT_MORE) < WORD_VALUE_BASE + WORD_VALUE_RANGE);
        uint32_t temp = (*data & ~WORD_BIT_MORE) - WORD_VALUE_BASE;

        retval *= WORD_VALUE_RANGE;
        retval += temp;

    } while (*data++ & WORD_BIT_MORE);

    return retval;
}

int main(int argc, char const *argv[])
{
    uint16_t vals[] = {0x8102, 0x5AD7};
    uint32_t val = decode(vals);
    printf("0x%X (%u)\n", val, val);

    return 0;
}

// 00781490