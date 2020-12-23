#include <os_internal.h>
#include "controller.h"

s32 __osPfsSelectBank(OSPfs *pfs, u8 bank)
{
    u8 temp[BLOCKSIZE];
    int i;
    s32 ret;
    ret = 0;
    for (i = 0; i < ARRLEN(temp); i++)
    {
        temp[i] = bank;
    }
    ret = __osContRamWrite(pfs->queue, pfs->channel, 1024, (u8*)temp, FALSE);
    return ret;
}
